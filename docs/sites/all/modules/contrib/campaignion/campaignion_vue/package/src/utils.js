/**
 * Comply to RFC 3986 when encoding URI components.
 * Encode also !, ', (, ) and *.
 * @param {string} str - The URI component to encode.
 * @return {string} The encoded URI component.
 */
export function fixedEncodeURIComponent (str) {
  return encodeURIComponent(str).replace(/[!'()*]/g, c => '%' + c.charCodeAt(0).toString(16))
}

/**
 * Prepare a url for appending a GET param key-value pair.
 * @param {string} url - The url to prepare.
 * @return {string} The url with either a ? or a & at the end.
 */
export function paramReadyUrl (url) {
  if (!url.match(/\?[^=]+=[^&]*/)) {
    // there’s no parameter. replace trailing ? or / or /? with ?
    return url.replace(/[/?]$|(?:\/)\?$/, '') + '?'
  } else {
    // parameter present in the string. ensure trailing &
    return url.replace(/[&]$/, '') + '&'
  }
}

/**
 * Escape characters that have a meaning in regular expressions.
 * @param {string} str - The string to process.
 * @return {string} The string with escaped special characters.
 */
export function escapeRegExp (str) {
  return str.replace(/[\-\[\]\/\{\}\(\)\*\+\?\.\\\^\$\|]/g, '\\$&') // eslint-disable-line no-useless-escape
}

export default {
  fixedEncodeURIComponent,
  paramReadyUrl,
  escapeRegExp
}
