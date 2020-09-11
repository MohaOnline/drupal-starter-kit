import * as fragment from './fragment'

/**
 * Listener to check the location hash string for events to trigger.
 *
 * Binds to `load` on `window`.
 */
export class FragmentListener {
  constructor (tracker, prefixes = [], debug = false) {
    this.debug = debug
    this.tracker = tracker
    this.prefixes = prefixes

    this.printDebug('init')

    if (typeof this.tracker === 'undefined') {
      this.printDebug('No TrackerManager found. Doing nothing.')
    }
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
      console.debug('[campaignion_tracking]', '(listener)', ...args)
    }
  }

  /**
   * Setup the listener.
   *
   * This should not be done when initializing, as we might want this to be
   * deferred.
   *
   * TODO: remove option
   */
  setup () {
    window.addEventListener('load', (e) => {
      const hash = window.location.hash.substr(1)
      const items = fragment.consumeLocationHashForPrefixes(this.prefixes, hash)
      if (items.locationHash !== hash) {
        if (items.locationHash.length) {
          window.location.hash = '#' + items.locationHash
        }
        else {
          // use replaceState so we get rid of the superfluouse '#' when setting
          // window.location.hash to ''
          window.history.replaceState(
            '',
            window.document.title,
            window.location.pathname + window.location.search
          )
        }

        /**
         * Publish event for all items, channel name is `code`.
         * All other information is in the item object inside items.
         *
         * They are sent all at once because information might depend on each
         * other. E.g. the `ds` tracking events needs and `sid` from webform.
         */
        this.tracker.publish('code', items)
      }
    })
  }
}
