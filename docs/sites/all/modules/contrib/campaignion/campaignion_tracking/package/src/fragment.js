/**
 * Parse a string for some URL fragment parts.
 *
 * @param {String} locationHash The input string, typically a location.hash string.
 *
 * @returns {Object[]} A list of matched items.
 */
export function parseLocationHash (locationHash = '') {
  let parts = locationHash.split(';')
  // only consider "truthy" values
  let filtered = parts.filter(Boolean)
  let items = filtered.map(item => parsePart(item))
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
  let allItems = parseLocationHash(locationHash)
  let filtered = allItems.filter(item => prefixes.includes(item['prefix']))
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
  let allItems = parseLocationHash(locationHash)
  let initialValue = { items: [], rest: [] }
  let result = allItems.reduce((acc, item) => {
    if (prefixes.includes(item['prefix'])) {
      acc.items.push(item)
    } else {
      acc.rest.push(item['origPart'])
    }
    return acc
  }, initialValue)

  result['origLocationHash'] = locationHash
  // re-join not consumed parts
  result['locationHash'] = result.rest.join(';')

  return result
}

/**
 * Parse a part from the URL fragment.
 *
 * The optional "id" in the match item is the empty string if no id was given.
 *
 * TODO: optional multiple ids
 * TODO: whitespace trimming?
 *
 * @param {String} part A part from the URL fragment.
 *
 * @returns {Object} A match item with the keys "prefix", "id", "codes", "origParts".
 */
export function parsePart (part) {
  let _pos1 = part.indexOf('=')
  let value1 = part.substring(0, _pos1)
  let value2 = part.substring(_pos1 + 1)
  let id = ''
  let prefix = value1
  // if there is a ':' we extract the id
  let _pos2 = value1.indexOf(':')
  if (_pos2 >= 0) {
    prefix = value1.substring(0, _pos2)
    id = value1.substring(_pos2 + 1)
  }
  let codes = value2.split(',')

  return {
    prefix: prefix,
    id: id,
    codes: codes,
    origPart: part
  }
}
