<?php

/**
 * Implements hook_token_info().
 */
function smart_paging_token_info() {
  $info = array(
    'types' => array(
      'smart-paging' => array(
        'name' => t('Smart Paging'),
        'description' => t('Smart Paging Tokens.'),
      )
    )
  );

  // Create token [smart-paging:page-number]
  $info['tokens']['smart-paging']['page-number'] = array(
    'name' => t('Page number'),
    'description' => t('The page number of the current page when viewing paged lists.'),
  );

  return $info;
}

/**
 * Implements hook_tokens().
 */
function smart_paging_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  if ($type == 'smart-paging') {
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'page-number':
          $replacements[$original] = pager_find_page(1) + 1;
          break;
      }
    }
  }

  return $replacements;
}