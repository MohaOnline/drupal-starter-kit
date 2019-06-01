<?php
/**
 * @file
 * wetkit_wysiwyg.features.filter.inc
 */

/**
 * Implements hook_filter_default_formats().
 */
function wetkit_wysiwyg_filter_default_formats() {
  $formats = array();

  // Exported format: HTML.
  $formats['wetkit_html_text'] = array(
    'format' => 'wetkit_html_text',
    'name' => 'HTML',
    'cache' => 1,
    'status' => 1,
    'weight' => -9,
    'filters' => array(
      'media_filter' => array(
        'weight' => -50,
        'status' => 1,
        'settings' => array(),
      ),
      'picture' => array(
        'weight' => -49,
        'status' => 1,
        'settings' => array(),
      ),
      'uuid_link_filter' => array(
        'weight' => -48,
        'status' => 1,
        'settings' => array(),
      ),
      'filter_htmlcorrector' => array(
        'weight' => -47,
        'status' => 1,
        'settings' => array(),
      ),
      'filter_url' => array(
        'weight' => -46,
        'status' => 1,
        'settings' => array(
          'filter_url_length' => 72,
        ),
      ),
    ),
  );

  // Exported format: WYSIWYG CKEDITOR.
  $formats['wetkit_wysiwyg_text'] = array(
    'format' => 'wetkit_wysiwyg_text',
    'name' => 'WYSIWYG CKEDITOR',
    'cache' => 1,
    'status' => 1,
    'weight' => -10,
    'filters' => array(
      'media_filter' => array(
        'weight' => -50,
        'status' => 1,
        'settings' => array(),
      ),
      'picture' => array(
        'weight' => -49,
        'status' => 1,
        'settings' => array(),
      ),
      'uuid_link_filter' => array(
        'weight' => -48,
        'status' => 1,
        'settings' => array(),
      ),
      'filter_htmlcorrector' => array(
        'weight' => -47,
        'status' => 1,
        'settings' => array(),
      ),
      'filter_url' => array(
        'weight' => -46,
        'status' => 1,
        'settings' => array(
          'filter_url_length' => 72,
        ),
      ),
    ),
  );

  return $formats;
}
