<?php

/**
 * @file
 * Plugin to provide access control based upon AMP request.
 */

$plugin = array(
  'title' => t("AMP Request"),
  'description' => t('Control access for AMP-enabled requests.'),
  'callback' => 'amp_request_ctools_access_check',
  'summary' => 'amp_request_ctools_access_summary',
);

/**
 * Check for access.
 */
function amp_request_ctools_access_check($conf, $context) {
  return amp_is_amp_request();
}

/**
 * Provide a summary description based upon the checked node_types.
 */
function amp_request_ctools_access_summary($conf, $context) {
  return t('Is AMP Request');
}
