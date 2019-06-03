<?php
/**
 * @file
 * dvg_global.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function dvg_global_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'frontpage';
  $context->description = '';
  $context->tag = 'Global';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '<front>' => '<front>',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu_block-menu-tasks' => array(
          'module' => 'menu_block',
          'delta' => 'menu-tasks',
          'region' => 'content',
          'weight' => '-10',
        ),
        'dvg_global-dvg_global__front_about' => array(
          'module' => 'dvg_global',
          'delta' => 'dvg_global__front_about',
          'region' => 'content',
          'weight' => '-7',
        ),
        'menu_block-menu-about' => array(
          'module' => 'menu_block',
          'delta' => 'menu-about',
          'region' => 'content',
          'weight' => '-6',
        ),
        'dvg_global-dvg_global__front_organization' => array(
          'module' => 'dvg_global',
          'delta' => 'dvg_global__front_organization',
          'region' => 'content',
          'weight' => '-5',
        ),
        'menu_block-menu-organization' => array(
          'module' => 'menu_block',
          'delta' => 'menu-organization',
          'region' => 'content',
          'weight' => '-4',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Global');
  $export['frontpage'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'global';
  $context->description = '';
  $context->tag = 'Global';
  $context->conditions = array(
    'sitewide' => array(
      'values' => array(
        1 => 1,
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views--exp-search-page' => array(
          'module' => 'views',
          'delta' => '-exp-search-page',
          'region' => 'header',
          'weight' => '-10',
        ),
        'menu-menu-social-media-menu' => array(
          'module' => 'menu',
          'delta' => 'menu-social-media-menu',
          'region' => 'footer_top',
          'weight' => '-10',
        ),
        'menu-menu-footer-menu' => array(
          'module' => 'menu',
          'delta' => 'menu-footer-menu',
          'region' => 'footer_top',
          'weight' => '-9',
        ),
        'dvg_global-dvg_global__footer_text' => array(
          'module' => 'dvg_global',
          'delta' => 'dvg_global__footer_text',
          'region' => 'footer_bottom',
          'weight' => '-10',
        ),
        'system-help' => array(
          'module' => 'system',
          'delta' => 'help',
          'region' => 'content',
          'weight' => -10,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Global');
  $export['global'] = $context;

  return $export;
}
