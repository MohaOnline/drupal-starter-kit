/**
 * Parse a string for some URL fragment parts.
 *
 * @param {String} locationHash The input string, typically a location.hash string.
 *
 * @returns {Object[]} A list of matched items.
 */
export function parseLocationHash (locationHash = '') {
  const parts = locationHash.split(';')
  // only consider "truthy" values
  const filtered = parts.filter(Boolean)
  const items = filtered.reduce((acc, item) => {
    const _pos1 = item.indexOf(':')
    const prefix = item.substring(0, _pos1)
    // TODO sanitize prefix
    const partsString = item.substring(_pos1 + 1)
    return acc.concat(parsePartsWithPrefix(partsString, prefix))
  }, [])
  return items
}

/**
 * Parse and filter a string for some URL fragment parts.
 *
 * @param {String[]} prefixes List of strings to filter prefixes.
 * @param {String} locationHash The input string, typically a location.hash string.
 *
 * @returns {Object[]} A list of matched items.
 */
export function parseLocationHashForPrefixes (prefixes = [], locationHash = '') {
  const allItems = parseLocationHash(locationHash)
  const filtered = allItems.filter(item => prefixes.includes(item.prefix))
  return filtered
}

/**
 * Parse and filter a string for some URL fragment parts, returning the remainers.
 *
 * @param {String[]} prefixes List of strings to filter prefixes.
 * @param {String} locationHash The input string, typically a location.hash string.
 *
 * @returns {Object[]} A result objects with the found parts in "items", the
 * remaining string in "locationHash".
 */
export function consumeLocationHashForPrefixes (prefixes = [], locationHash = '') {
  const allItems = parseLocationHash(locationHash)
  const initialValue = { items: [], rest: [] }
  const result = allItems.reduce((acc, item) => {
    if (prefixes.includes(item.prefix)) {
      acc.items.push(item)
    }
    else {
      acc.rest.push(item.origPart)
    }
    return acc
  }, initialValue)

  result.origLocationHash = locationHash
  // re-join not consumed parts
  result.locationHash = result.rest.join(';')

  return result
}

/**
 * Parse a single key-value part from the URL fragment.
 *
 * @param {String} part A part from the URL fragment.
 *
 * @returns {Object} A match item with the keys "id", "codes", "origPart".
 */
export function parsePart (part) {
  const _pos1 = part.indexOf('=')
  const value1 = part.substring(0, _pos1)
  const value2 = decodeURIComponent(part.substring(_pos1 + 1))
  const codes = value2.split(',')
  return {
    id: value1,
    codes: codes,
    origPart: part
  }
}

/**
 * Parse a possible multiple parts from the URL fragment.
 *
 * Optionally tag all parts with a commong prefix.
 *
 * @param {String} part Parts from the URL fragment, delimited with `&`.
 * @param {String} prefix A prefix to tag the parts with
 *
 * @returns {Array} A list of match objects
 */
export function parsePartsWithPrefix (partsString, prefix = '') {
  const split = partsString.split('&')
  const parts = split.map(part => {
    const _p = parsePart(part)
    _p.prefix = prefix
    return _p
  })
  return parts
}
