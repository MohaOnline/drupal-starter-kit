import axios from 'axios'

/**
 * Prepare a url for appending a GET param key-value pair.
 * @param {string} url - The url to prepare.
 * @return {string} The url with either a ? or a & at the end.
 */
function paramReadyUrl (url) {
  if (!url.match(/\?[^=]+=[^&]*/)) {
    // there’s no parameter. replace trailing ? or / or /? with ?
    return url.replace(/[/?]$|(?:\/)\?$/, '') + '?'
  } else {
    // parameter present in the string. ensure trailing &
    return url.replace(/[&]$/, '') + '&'
  }
}

export default {
  /**
   * Make a PUT request.
   * @param {Object} options - We like objects as arguments.
   * @param {string} options.url - The url.
   * @param {(Object|string)} options.data - The data.
   * @param {Object} options.headers - Headers to send along.
   * @return {Promise} A Promise resolving if the request succeeds.
   */
  postData: function ({url, data, headers}) {
    return axios.put(url, data)
  },

  /**
   * Make a GET request with a parameter.
   * @param {Object} options - We like objects as arguments.
   * @param {string} options.url - The url.
   * @param {string} options.queryParam - The parameter key.
   * @param {string} options.queryString - The parameter value.
   * @param {Object} options.headers - Headers to send along.
   * @return {Promise} A Promise resolving if the request succeeds.
   */
  getNodes: function ({url, queryParam, queryString, headers}) {
    return axios.get(paramReadyUrl(url) + queryParam + '=' + queryString)
  }
}
