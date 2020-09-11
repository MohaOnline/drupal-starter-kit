<?php

/**
 * @file
 * campaignion_test.features.filter.inc
 */

/**
 * Implements hook_filter_default_formats().
 */
function campaignion_test_filter_default_formats() {
  $formats = array();

  // Exported format: Full HTML with editor.
  $formats['full_html_with_editor'] = array(
    'format' => 'full_html_with_editor',
    'name' => 'Full HTML with editor',
    'cache' => 1,
    'status' => 1,
    'weight' => -42,
    'filters' => array(
      'filter_htmlcorrector' => array(
        'weight' => -49,
        'status' => 1,
        'settings' => array(),
      ),
      'filter_url' => array(
        'weight' => -47,
        'status' => 1,
        'settings' => array(
          'filter_url_length' => 72,
        ),
      ),
    ),
  );

  return $formats;
}
