import {clone} from '@/utils'

/**
 * Returns a random v4 UUID of the form `xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx`, where
 * each `x` is replaced with a random hexadecimal digit from 0 to f, and y is replaced
 * with a random hexadecimal digit from 8 to b.
 * Taken from https://gist.github.com/jed/982883
 * @param {integer} a - Placeholder.
 * @return {string} Random uuid.
 */
function uuid (a) { return a ? (a ^ Math.random() * 16 >> a / 4).toString(16) : ([1e7] + -1e3 + -4e3 + -8e3 + -1e11).replace(/[018]/g, uuid) }

/**
 * Generate a fresh dataset with the standard contact columns as attributes and a new uuid as a key.
 * @param {Object} state - vuex state.
 * @return {Object} The new dataset.
 */
export function emptyDataset (state) {
  const attributes = clone(state.standardColumns)
  // Prefix attribute keys.
  for (var i = 0, j = attributes.length; i < j; i++) {
    attributes[i].key = state.contactPrefix + attributes[i].key
  }

  return {
    attributes,
    title: '',
    description: '',
    is_custom: true,
    _new: true, // Vue app internal use only. Not passed to the server.
    key: uuid()
  }
}
