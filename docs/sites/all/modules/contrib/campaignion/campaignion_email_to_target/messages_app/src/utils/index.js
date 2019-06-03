import defaults from './defaults'

/**
 * Deep-copy an object.
 * @param {Object} obj - The object to clone.
 * @return {Object} The cloned object.
 */
export function clone (obj) {
  return JSON.parse(JSON.stringify(obj))
}

/**
 * Check if a message is empty.
 * @param {Object} message - The message object to validate.
 * @return {boolean} true only if all of the message fields are empty or contain only whitespace characters.
 */
export function isEmptyMessage (message) {
  return !((message.subject && message.subject.trim()) || (message.header && message.header.trim()) || (message.body && message.body.trim()) || (message.footer && message.footer.trim()))
}

/**
 * Validate the url property’s value.
 * Valid values are absolute or relative urls or expressions starting with `node/`.
 * @param {string} destination - The expression to validate.
 * @return {boolean} Is it valid?
 */
export function validateDestination (destination) {
  return !!(destination.length &&
    (destination.match(/^(www\.|http:\/\/|https:\/\/|\/)/) ||
    destination.match(/^node\//)) &&
    destination.indexOf(' ') === -1)
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

export default {
  clone,
  isEmptyMessage,
  validateDestination,
  defaults
}
