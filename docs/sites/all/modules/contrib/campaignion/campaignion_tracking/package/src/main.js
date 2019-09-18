import * as tm from './tracker-manager'
import * as listener from './listener'
import * as gtm from './gtm'
import * as dp from './debug'

/**
 * Check if we want debugging output.
 *
 * Parse the value as int, so we can disable debugging by setting to "0".
 * `sessionStorage` only stores strings.
 */
// eslint-disable-next-line no-unneeded-ternary
var debug = parseInt(sessionStorage.getItem('campaignion_debug')) ? true : false

// some out-of-the-box prefixes
export const codePrefixes = ['t', 'd', 'w']

// ensure window.dataLayer
window.dataLayer = window['dataLayer'] || []

// common tracker manager, listener, gtm
export const tracker = new tm.TrackerManager(debug)
export const gtmTracker = new gtm.GTMTracker(tracker, window.dataLayer, debug)
export const fragmentListener = new listener.FragmentListener(tracker, codePrefixes, debug)
fragmentListener.setup()
dp.setupDebugProxy('dataLayer')

// re-exports
export { tm, listener, debug, gtm, dp }
export { fragment } from './fragment'
