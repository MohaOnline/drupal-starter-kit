/**
 * Machine readable name.
 *
 * Use for referencing tracker specifics like storage keys, channel names, ...
 */
export const name = 'gtm'

/**
 * Mapping of codes to eventNames.
 *
 * This also is a whitelist of which events should *also* be recognized as
 * codes.
 */
export const codes = {
  ds: 'donationSuccess',
  s: 'submission',
}

/**
 * Compare two donation product objects.
 *
 * Keys considered: 'name', 'price', 'id', 'quantity'
 *
 * Comparing with JSON.stringify will not work robustly as the order is not
 * guaranteed.
 *
 * @param {object} product1 first product
 * @param {object} product2 second product
 * @returns {Boolean}
 */
export const productIsEqual = (product1, product2) => {
  for (const key of ['name', 'price', 'id', 'quantity']) {
    // If at least one product has the property and their values differ
    // they are NOT equal. They are otherwise.
    if ((Object.prototype.hasOwnProperty.call(product1, key) || Object.prototype.hasOwnProperty.call(product2, key)) &&
        product1[key] !== product2[key]
    ) {
      return false
    }
  }
  return true
}

/**
 * Tracker for Google Tag Manager.
 *
 * Implements behaviour to dispatch events to GTM.
 */
export class GTMTracker {
  /**
   * Constructor.
   *
   * @param {TrackerManager} tracker the shared tracker manager object
   * @param {object} dataLayer the GTM datalayer to use (default `window.dataLayer`)
   * @param {Boolean} debug set to true for debugging
   */
  constructor (tracker, dataLayer = window.dataLayer, debug = false) {
    this.debug = debug
    this.dataLayer = dataLayer
    this.tracker = tracker

    // load from session, set defaults
    const defaultContext = {
      node: {},
      donation: { currencyCode: null, product: null },
      webform: { sid: null }
    }
    this._context = this.loadFromStorage('context') || defaultContext

    this.printDebug('init')

    if (typeof this.tracker === 'undefined') {
      this.printDebug('No TrackerManager found. Doing nothing.')
      return
    }

    if (typeof this.dataLayer === 'undefined') {
      this.printDebug('No datalayer found. Doing nothing.')
    }

    /**
     * Callback for subscribed events.
     *
     * This handles incoming data/events.
     *
     * TODO: validate event
     * TODO: sanitize data
     *
     * @param {object} e Tracking event
     */
    this._dispatch = e => {
      this.printDebug('campaignion_tracking_gtm', 'handle_form', e)

      this.printDebug('campaignion_tracking_gtm', 'handle_event', e.name, e.data, e.context)

      // dispatch to my handlers
      this.dispatch(e.name, e.data, e.context)
    }

    /**
     * Subscribe to messages of the `webform` tracking channel.
     */
    this.webformSubscription = this.tracker.subscribe('webform', this._dispatch)

    /**
     * Subscribe to messages of the `donation` tracking channel.
     */
    this.donationSubscription = this.tracker.subscribe('donation', this._dispatch)
  }

  /**
   * Utility function to print to `console.debug`.
   *
   * Print only if debug is set to a truthy value.
   *
   * @param  {...any} args arguments to print
   */
  printDebug (...args) {
    if (this.debug) {
      console.debug('[campaignion_tracking]', '(gtm)', ...args)
    }
  }

  saveToStorage (key = 'default', data) {
    this.tracker.saveToStorage('campaignion_tracking:gtm:', key, data)
  }

  loadFromStorage (key = 'default') {
    return this.tracker.loadFromStorage('campaignion_tracking:gtm:', key)
  }

  removeFromStorage (key = 'default') {
    return this.tracker.removeFromStorage('campaignion_tracking:gtm:', key)
  }

  /**
   * Dispatch to my handlers.
   *
   * The handler are named according to the `eventName`, prefixed with
   * `handle_`. The handlers are responsible for the actual sending of valid
   * events to GTM.
   *
   * @param {String} eventName the name of the event
   * @param {object} eventData data of the event
   * @param {object} context context of the event, e.g. site title, ...
   */
  dispatch (eventName = '', eventData = {}, context = {}) {
    if (typeof this['handle_' + eventName] === 'function') {
      this['handle_' + eventName](eventName, eventData, context)
    }
    else {
      this.printDebug('no handler for event name:', eventName)
    }
  }

  /**
   * Hook to allow users to manipulate GTM data.
   *
   * Maybe some projects/sites want different values for some GTM fields.
   * Provide a way to allow for that customizations.
   *
   * @param {String} eventName the campaignion_tracking event name
   * @param {object} gtmData the data which will get sent to GTM data layer
   * @param {object} context the tracking context
   */
  callChangeHook (eventName, gtmData, context) {
    // maybe also inject tracker
    if (typeof window.campaignion_tracking_change_msg === 'function') {
      gtmData = window.campaignion_tracking_change_msg('gtm', eventName, gtmData, context)
    }
    return gtmData
  }

  /**
   * Maintains a context between event handling.
   *
   * You can provide context with a first event and call this method during
   * handling.
   * When another event arrives it can also enrich this context and read former
   * context data as well.
   *
   * This context is saved in the browser storage to persist between page
   * loads. It is reset on donationSuccess.
   *
   * @param {context} context Tracking context data.
   */
  updateContext (context = {}) {
    // Check if context exists
    if (context.donation) {
      Object.assign(this._context.donation, context.donation)
    }
    if (context.node) {
      Object.assign(this._context.node, context.node)
    }
    if (context.webform) {
      Object.assign(this._context.webform, context.webform)
    }
    this.saveToStorage('context', this._context)
  }

  /**
   * Handle "submission".
   *
   * Event data: { nid, sid, title }
   *
   * @param {String} eventName the event name
   * @param {object} eventData data of the event
   * @param {object} context context of the event
   */
  handle_submission (eventName, eventData, context) { // eslint-disable-line camelcase
    this.printDebug('(handle)', eventName, eventData, context)
    this.updateContext(context)

    let data = {
      event: 'submission',
      webform: {
        nid: eventData.nid || null,
        sid: eventData.sid || null,
        title: eventData.title || null,
      }
    }
    // Allow others to modify the data being sent to GTM.
    data = this.callChangeHook(eventName, data, this._context)
    this.dataLayer.push(data)
  }

  /**
   * Handle "setDonationProduct".
   *
   * Remove a (donation) product when one was added before. Thus we have a
   * "cart" with only 1 slot for 1 donation.
   *
   * Event data: { product, currencyCode }
   *
   * @param {String} eventName the event name
   * @param {object} eventData data of the event
   * @param {object} context context of the event
   */
  handle_setDonationProduct (eventName, eventData, context) { // eslint-disable-line camelcase
    this.printDebug('(handle)', eventName, eventData, context)
    this.updateContext(context)
    if (eventData.currencyCode) {
      this._context.donation.currencyCode = eventData.currencyCode
    }
    const currencyCode = this._context.donation.currencyCode || null
    const currentProduct = this._context.donation.product || {}
    const newProduct = eventData.product || {}

    const addData = {
      event: 'addToCart',
      ecommerce: {
        currencyCode: currencyCode,
        add: {
          products: [newProduct]
        }
      }
    }
    if (currencyCode) {
      addData.ecommerce.currencyCode = currencyCode
    }
    const removeData = {
      event: 'removeFromCart',
      ecommerce: {
        remove: {
          products: [currentProduct]
        }
      }
    }
    // Only push a remove if we can assume we have pushed a valid product before.
    const pushRemove = Object.prototype.hasOwnProperty.call(currentProduct, 'price')
    let data = {
      addData: addData,
      removeData: removeData,
      pushRemove: pushRemove
    }
    // Allow others to modify the data being sent to GTM.
    data = this.callChangeHook(eventName, data, this._context)

    const changedNewProduct = data.addData.ecommerce.add.products[0]
    const changedCurrentProduct = data.removeData.ecommerce.remove.products[0]

    // Only change something or send a GTM event if the donation products differ.
    // Compare *after* any changes.
    if (productIsEqual(changedCurrentProduct, changedNewProduct)) {
      this.printDebug('(handle)', eventName, 'same product', eventData, context)
      return
    }

    this._context.donation.product = changedNewProduct
    this.saveToStorage('context', this._context)

    if (data.pushRemove) {
      this.dataLayer.push(data.removeData)
    }
    this.dataLayer.push(data.addData)
  }

  /**
   * Handle "checkoutBegin".
   *
   * Event data: { product, currencyCode }
   *
   * @param {String} eventName the event name
   * @param {object} eventData data of the event
   * @param {object} context context of the event
   */
  handle_checkoutBegin (eventName, eventData, context) { // eslint-disable-line camelcase
    this.printDebug('(handle)', eventName, eventData, context)
    this.updateContext(context)
    const product = eventData.product || this._context.donation.product || {}
    const currencyCode = eventData.currencyCode || this._context.donation.currencyCode || null
    let data = {
      event: 'checkoutBegin',
      ecommerce: {
        checkout: {
          actionField: { step: 1 }, // begin == 1
          products: [product]
        }
      }
    }
    if (currencyCode) {
      data.ecommerce.currencyCode = currencyCode
    }
    // Allow others to modify the data being sent to GTM.
    data = this.callChangeHook(eventName, data, this._context)
    this.dataLayer.push(data)
  }

  /**
   * Handle "checkoutEnd".
   *
   * Event data: { product, currencyCode }
   *
   * @param {String} eventName the event name
   * @param {object} eventData data of the event
   * @param {object} context context of the event
   */
  handle_checkoutEnd (eventName, eventData, context) { // eslint-disable-line camelcase
    this.printDebug('(handle)', eventName, eventData, context)
    this.updateContext(context)
    const product = eventData.product || this._context.donation.product || {}
    const currencyCode = eventData.currencyCode || this._context.donation.currencyCode || null
    let data = {
      event: 'checkoutEnd',
      ecommerce: {
        checkout: {
          actionField: { step: 2 }, // end == 2
          products: [product]
        }
      }
    }
    if (currencyCode) {
      data.ecommerce.currencyCode = currencyCode
    }
    // Allow others to modify the data being sent to GTM.
    data = this.callChangeHook(eventName, data, this._context)
    this.dataLayer.push(data)
  }

  /**
   * Handle "donationSuccess".
   *
   * The event data needs to include an transaction id.
   * Falls back to use a random number.
   *
   * Event data: { tid, revenue, product, currencyCode }
   *
   * @param {String} eventName the event name
   * @param {object} eventData data of the event
   * @param {object} context context of the event
   */
  handle_donationSuccess (eventName, eventData, context) { // eslint-disable-line camelcase
    this.printDebug('(handle)', eventName, eventData, context)
    this.updateContext(context)
    const product = eventData.product || this._context.donation.product || {}
    const currencyCode = eventData.currencyCode || this._context.donation.currencyCode || null
    // Ensure a transaction id.
    const transactionID = eventData.tid || Math.floor(Math.random() * 2 ** 64)
    const sentTransactionIDs = this.loadFromStorage('sentTransactionIDs') || []
    // Do nothing if the transaction was already sent.
    if (sentTransactionIDs.indexOf(transactionID) >= 0) {
      this.printDebug('(handle)', 'already sent TID', eventName, eventData, context)
      return
    }
    let revenue = eventData.revenue || null
    if (revenue === null) {
      revenue = parseFloat(product.price || 0) * parseInt(product.quantity || 1)
    }
    let data = {
      event: 'purchase',
      ecommerce: {
        purchase: {
          actionField: {
            id: transactionID, // required
            revenue: String(revenue)
          },
          products: [product]
        }
      }
    }
    if (currencyCode) {
      data.ecommerce.currencyCode = currencyCode
    }
    // Allow others to modify the data being sent to GTM.
    data = this.callChangeHook(eventName, data, this._context)
    this.dataLayer.push(data)
    // Remember sent transactions ids.
    sentTransactionIDs.push(transactionID)
    this.saveToStorage('sentTransactionIDs', sentTransactionIDs)
    // We are finished with this donation: clean up.
    this.removeFromStorage('context')
  }
}
