<?php

/**
 * @file
 * Information about configuration variables defined by this module.
 */

/**
 * Implements hook_variable_info().
 */
function campaignion_content_security_policy_variable_info($options) {
  $description_vars['!documentation'] = l(t('Content-Security-Policy documentation'), 'https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Content-Security-Policy/frame-ancestors');
  $v['campaignion_content_security_policy_trusted_frame_ancestors'] = [
    'title' => t('Content-Security-Policy trusted frame-ancestors'),
    'description' => t('Define trusted URLs which should be allowed to display pages from this sites in frames. One URL per line. See the !documentation for more info.', $description_vars),
    'type' => 'text',
    'default' => "'self'\n",
    'localize' => FALSE,
  ];
  return $v;
}
