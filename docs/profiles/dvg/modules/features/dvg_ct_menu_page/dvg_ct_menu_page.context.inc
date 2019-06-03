<?php
/**
 * @file
 * dvg_ct_menu_page.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function dvg_ct_menu_page_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'submenu';
  $context->description = '';
  $context->tag = 'menu_page';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'menu_page' => 'menu_page',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu_block-_active' => array(
          'module' => 'menu_block',
          'delta' => '_active',
          'region' => 'below_content',
          'weight' => '-26',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('menu_page');
  $export['submenu'] = $context;

  return $export;
}
