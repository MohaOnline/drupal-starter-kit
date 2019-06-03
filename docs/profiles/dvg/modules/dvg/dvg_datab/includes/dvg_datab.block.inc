<?php

/**
 * Implements hook_block_info().
 */
function dvg_datab_block_info() {
  $blocks['dvg_datab__doclist'] = array(
    'info' => t('Data B. Doclist'),
    'cache' => DRUPAL_CACHE_PER_PAGE | DRUPAL_CACHE_PER_ROLE,
  );

  return $blocks;
}

/**
 * Implements hook_block_view().
 */
function dvg_datab_block_view($delta = '') {
  $block = array();

  switch ($delta) {
    case 'dvg_datab__doclist':
      $block['subject'] = '<none>';
      $block['content'] = _dvg_datab_block_doclist();
      break;
  }

  return $block;
}

/**
 * Fetches the Data B doclist for the logged in DigiD user or a DigiD-login-button.
 */
function _dvg_datab_block_doclist() {
  if (user_is_anonymous()) {
    return dvg_authentication_authentication_select_block('dvg_datab');
  }

  return _dvg_datab_generate_doclist();
}
