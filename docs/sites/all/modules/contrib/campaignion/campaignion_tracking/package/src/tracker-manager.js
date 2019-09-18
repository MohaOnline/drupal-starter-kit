/**
 * TrackerManager provides a PubSub interface for tracking events.
 */
export class TrackerManager {
  /**
   * Constructor.
   *
   * @param {Boolean} debug set to true for debugging
   */
  constructor (debug = false) {
    this.debug = debug
    this.topics = {}

    this.printDebug('init')
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
      console.debug('[campaignion_tracking]', ...args)
    }
  }

  /**
   * Subscribe a handler function to a topic.
   *
   * The handler function get one argument when called: the `data` object.
   *
   * @param {string} topic The name of a topic to subscribe to
   * @param {function} handler The handler function to get called on publish
   * @returns {object} A Subscription object allowing to `remove()` the subscription
   */
  subscribe (topic, handler) {
    this.printDebug('subscribe', handler)

    if (!this.topics[topic]) {
      this.topics[topic] = []
    }
    let subscribers = this.topics[topic]
    let subscriberCount = subscribers.push(handler)
    let subscriberIndex = subscriberCount - 1

    /**
     * Subscription object.
     *
     * This objects captures the `subscriberIndex` which acts like an
     * internal id for the subscribers.
     */
    let subscription = {
      remove: () => {
        delete this.topics[subscriberIndex]
      }
    }

    return subscription
  }

  /**
   * Publish data to a topic.
   *
   * @param {string} topic The name of the topic to publish to
   * @param {object} data Any data to be provided to the subscribers
   */
  publish (topic, data = {}) {
    this.printDebug('publish', data)

    if (!this.topics[topic]) {
      return
    }

    for (let subscriber of this.topics[topic]) {
      subscriber(data)
    }
  }

  /**
   * Save an object to the sessionStorage.
   *
   * TODO: test if storage is available
   */
  saveToStorage (prefix = 'campaignion_tracking:', key = 'default', data) {
    let storageKey = prefix + key
    window.sessionStorage.setItem(storageKey, JSON.stringify(data))
  }

  /**
   * Load an object from the sessionStorage.
   *
   * TODO: test if storage is available
   */
  loadFromStorage (prefix = 'campaignion_tracking:', key = 'default') {
    let storageKey = prefix + key
    return JSON.parse(window.sessionStorage.getItem(storageKey))
  }

  /**
   * Remove an object from the sessionStorage.
   *
   * TODO: test if storage is available
   */
  removeFromStorage (prefix = 'campaignion_tracking:', key = 'default') {
    let storageKey = prefix + key
    window.sessionStorage.removeItem(storageKey)
  }
}
