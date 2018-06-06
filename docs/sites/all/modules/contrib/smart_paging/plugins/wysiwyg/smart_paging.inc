<?php

/**
 * @file
 * Smart Paging/Wysiwyg API integration.
 */

/**
 * Implements hook_wysiwyg_plugin().
 */
function smart_paging_smart_paging_plugin() {
  $plugins['smart_paging'] = array(
    'title'      => t('Smart Paging'),
    'vendor url' => 'http://drupal.org/project/smart_paging',
    'icon file'  => 'smart_paging.gif',
    'icon title' => t('Insert a page break'),
    'settings'   => array('smartPagingPagebreak' => variable_get('smart_paging_pagebreak', '<!--pagebreak-->')),
  );
  return $plugins;
}

