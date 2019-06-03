import Vue from 'vue'

export const INVALID_CONTACT_STRING = 'has:error'
export const TOOLTIP_CLASS = 'v-tooltip'

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
 * Get the index of an element inside its container.
 * @param {HTMLElement} el The element in question.
 * @return {integer} The element’s index.
 */
export function elementIndex (el) {
  var i = 0
  while ((el = el.previousSibling) != null) {
    i++
  }
  return i
}

/**
 * Test a list of contacts against a set of regular expressions and check field maximum length.
 * If `index` is passed, validate only that row.
 * Add an `__error: INVALID_CONTACT_STRING` property to invalid contacts.
 * This property is added for all invalid contacts at once, but only removed on single rows, if
 * `index` is passed. This behavior fits the use case that data is fixed manually row by row and makes
 * the loop faster.
 * @param {Object[]} contacts - The array of contacts to validate.
 * @param {Object} validations - Validations for each column. Dictionary of regex strings, keyed by column name.
 * @param {Object} maxFieldLengths - Maximum characters for each column. Dictionary of integers, keyed by column name.
 * @param {integer} index - Optional. The row to validate.
 * @return {boolean} Are the contacts valid?
 */
export function validateContacts ({contacts, validations, maxFieldLengths, index}) {
  // If index is passed, validate only that row.
  var from, to
  if (typeof index !== 'undefined') {
    from = to = index
  } else {
    from = 0
    to = contacts.length - 1
  }

  // Gather all columns to check.
  const cols = {}
  for (let col in validations) {
    if (validations.hasOwnProperty(col)) {
      cols[col] = {regex: new RegExp(validations[col])}
    }
  }
  for (let col in maxFieldLengths) {
    if (maxFieldLengths.hasOwnProperty(col)) {
      // Don’t overwrite the object with a validation...
      if (cols[col]) {
        cols[col].maxlength = maxFieldLengths[col]
      } else {
        cols[col] = {maxlength: maxFieldLengths[col]}
      }
    }
  }

  // Loop through the columns to check.
  var valid = true
  for (let col in cols) {
    if (cols.hasOwnProperty(col)) {
      for (let i = from, j = to; i <= j; i++) {
        if (typeof contacts[i][col] !== 'undefined' && // If a field exists in that column and...
          ((typeof cols[col].maxlength !== 'undefined' && contacts[i][col].length > cols[col].maxlength) || // a max length is defined for that field and the value exeeds the max length, or...
          (cols[col].regex && cols[col].regex.test(contacts[i][col]) === false))) { // a regular expression is defined for that field the value doesn’t match it, then...
          valid = false
          Vue.set(contacts[i], '__error', INVALID_CONTACT_STRING)
        }
      }
    }
  }

  // If only one row was checked (after editing that row), remove error mark from this row.
  // While we need to find errors in a whole dataset (CSV upload), they are fixed only row by row.
  // So we can optimize performance by not removing errors in the loop.
  if (valid && typeof index !== 'undefined') {
    Vue.delete(contacts[index], '__error')
  }
  return valid
}
