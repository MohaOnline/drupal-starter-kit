/**
 * Wrap an array with a Proxy to log `push`es
 *
 * For use with an array in `window` that is used as an queue, like
 * `window.dataLayer` for Google Tag Manager, or `window.ga` for Google
 * Analytics or `window.fbq` for Facebook Pixels.
 *
 * NB: only logs `push()` calls so far
 *
 * @param {Array} queueArray the array to wrap a proxy around
 * @param {String} queueArrayName the name of the array (for debugging output)
 */
export const debugProxy = (queueArray, queueArrayName = '') => {
  const proxy = new Proxy(queueArray, {
    get (target, prop) {
      const val = target[prop]
      if (typeof val === 'function') {
        if (['push'].includes(prop)) {
          return function (el) {
            console.debug('queue push', queueArrayName, target, arguments)
            return Array.prototype[prop].apply(target, arguments)
          }
        }
        return val.bind(target)
      }
      return val
    }
  })
  return proxy
}

/**
 * Replace a queue object by name with a debugging Proxy.
 *
 * NB: does not work in IE11 (no support for `Proxy`)
 *
 * @param {String} queueArrayName name of the queue object (lives in `window`)
 */
export const setupDebugProxy = (queueArrayName) => {
  if (!('Proxy' in window)) {
    console.warn("Your browser doesn't support Proxies.")
    return false
  }
  const queueArray = queueArrayName in window ? window[queueArrayName] : []
  window[queueArrayName] = debugProxy(queueArray, queueArrayName)
  return true
}
