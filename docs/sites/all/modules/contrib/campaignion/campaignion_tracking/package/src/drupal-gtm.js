import * as gtm from './gtm'

/**
 * Check if we want debugging output.
 *
 * Parse the value as int, so we can disable debugging by setting to "0".
 * `sessionStorage` only stores strings.
 */
// eslint-disable-next-line no-unneeded-ternary
var debug = parseInt(sessionStorage.getItem('campaignion_debug')) ? true : false

// ensure window.dataLayer
window.dataLayer = window.dataLayer || []

// common tracker manager, listener, gtm
let tracker = window['campaignion_tracking']['tracker']
let gtmTracker = null
if (typeof tracker === 'undefined') {
  console.log('No Tracker found')
} else {
  gtmTracker = new gtm.GTMTracker(tracker, window.dataLayer, debug)
}

// re-exports
export { gtm, gtmTracker }
