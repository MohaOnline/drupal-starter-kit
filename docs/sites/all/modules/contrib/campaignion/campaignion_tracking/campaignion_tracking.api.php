<?php

/**
 * @file
 * Document hooks invoked by the campaignion_tracking module.
 */

/**
 * Define snippets to load.
 *
 * @return array
 *   Snippets keyed by an arbitrary key (use namespacing if applicable). Each
 *   snippet can either be a string (containing the JS code) or an associative
 *   array with the following keys:
 *   - js: The JS code.
 *   - event: Event that the snippet waits for (default: None).
 */
function hook_campaignion_tracking_snippets() {
  $exports['snippet'] = <<<SNIPPET
  (function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){
  (i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),
  m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)
  })(window,document,'script','https://www.google-analytics.com/analytics.js','ga');
  ga('create', 'UA-XXXXXX-YY', 'auto');
  ga('send', 'pageview');
SNIPPET;
  $export['extended_snippet'] = [
    'js' => '/* JS code */',
    'event' => 'examples-allowed',
  ];
  return $exports;
}

/**
 * Alter the snippets defined in hook_campaignion_tracking_snippets().
 *
 * @param array $snippets
 *   The snippets defined in the earlier hook invocation normalized into their
 *   explicit array form.
 */
function hook_campaignion_tracking_snippets_alter(array &$snippets) {
  unset($snippets['snippet']);
}

/**
 * Define events that should be made available as promises.
 *
 * @return array
 *   Array of JS event names. For simplicity events should also get their values
 *   as keys.
 */
function hook_campaignion_tracking_events() {
  $events['send-me-ads'] = 'send-me-ads';
  return $events;
}

/**
 * Alter the list of events that are available as promises.
 *
 * @param array $events
 *   The array of events as defined by hook_campaignion_tracking_events().
 */
function hook_campaignion_tracking_events_alter(array &$events) {
  unset($events['tracking-allowed']);
}
