import * as tm from './tracker-manager'
import * as listener from './listener'

/**
 * Mapping of codes to eventNames.
 *
 * This also is a whitelist of which events should *also* be recognized as
 * codes.
 */
export const trackingCodes = {
  ds: 'donationSuccess'
}

/**
 * Check if we want debugging output.
 *
 * Parse the value as int, so we can disable debugging by setting to "0".
 * `sessionStorage` only stores strings.
 */
// eslint-disable-next-line no-unneeded-ternary
var debug = parseInt(sessionStorage.getItem('campaignion_debug')) ? true : false

let printDebug = (...args) => {
  if (debug) {
    console.debug('[campaignion_tracking]', '(drupal)', ...args)
  }
}

// some out-of-the-box prefixes
// t: tracking, w: webform, d: donation
export const codePrefixes = ['t', 'w', 'd']

// common tracker manager, listener, gtm
export const tracker = new tm.TrackerManager(debug)
export const fragmentListener = new listener.FragmentListener(tracker, codePrefixes, debug)
fragmentListener.setup()

/**
 * Subscribe to messages of the `code` tracking channel.
 *
 * The tracker listens for specific parts in the URL fragment and
 * published messages on this channel with event codes to trigger.
 */
export let codeSubscription = tracker.subscribe('code', (e) => {
  printDebug('handle_code', e)

  // map event codes to event names
  let events = e.items.reduce((acc, item) => {
    if (item.prefix === 't') {
      item.codes.forEach((code) => {
        if (trackingCodes[code]) {
          acc.tracking.events.push(trackingCodes[code])
        }
      })
    }
    if (item.prefix === 'w') {
      if (item['id'] === 'sid') {
        acc.webform['sid'] = item.codes[0]
      }
    }
    if (item.prefix === 'd') {
      if (item['id'] === 'm') {
        acc.donation['method'] = item.codes[0]
      }
    }
    return acc
  }, { tracking: { events: [] }, webform: {}, donation: {} })

  printDebug('handle_events', events)

  let data = { tid: events.webform['sid'] || null }
  let context = {
    webform: { sid: events.webform['sid'] || null },
    donation: { 'paymethod': events.donation['method'] || 'unknown' }
  }

  if (events.tracking.events.includes('donationSuccess')) {
    tracker.publish('donation', { name: 'donationSuccess', data: data, context: context })
  }
})

// re-exports
export { tm, listener, debug }
export { fragment } from './fragment'
