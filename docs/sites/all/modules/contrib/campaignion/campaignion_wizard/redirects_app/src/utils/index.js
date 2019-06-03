/**
 * Deep-copy an object.
 * @param {Object} obj - The object to clone.
 * @return {Object} The cloned object.
 */
export function clone (obj) {
  return JSON.parse(JSON.stringify(obj))
}

/**
 * Dispatch a custom JavaScript event.
 * @param {HTMLElement} el DOM element to dispatch the event on.
 * @param {string} type Event name.
 */
export function dispatch (el, type) {
  const e = document.createEvent('Event')
  e.initEvent(type, true, true)
  el.dispatchEvent(e)
}

/**
 * Validate a destination property’s value.
 * Valid values are absolute or relative urls or expressions starting with `node/`.
 * @param {string} destination - The expression to validate.
 * @return {boolean} Is it valid?
 */
export function validateDestination (destination) {
  return (destination.length &&
    destination.match(/^(www\.|http:\/\/|https:\/\/|\/)/) &&
    destination.indexOf(' ') === -1) ||
    destination.match(/^node\//)
}

export default {
  clone,
  dispatch,
  validateDestination
}
