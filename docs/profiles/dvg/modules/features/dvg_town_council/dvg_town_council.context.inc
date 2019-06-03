<?php
/**
 * @file
 * dvg_town_council.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function dvg_town_council_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'dvg_town_council';
  $context->description = '';
  $context->tag = 'Global';
  $context->conditions = array(
    'callback' => array(
      'values' => array(
        'dvg_town_council' => 'dvg_town_council',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'dvg_town_council-dvg_town_council_block' => array(
          'module' => 'dvg_town_council',
          'delta' => 'dvg_town_council_block',
          'region' => 'below_content',
          'weight' => '-10',
        ),
        'menu_block-menu-town-council' => array(
          'module' => 'menu_block',
          'delta' => 'menu-town-council',
          'region' => 'below_content',
          'weight' => '-9',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Global');
  $export['dvg_town_council'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'dvg_town_council_view';
  $context->description = '';
  $context->tag = 'views';
  $context->conditions = array(
    'callback' => array(
      'values' => array(
        'functional_content_nid__town_council__block' => 'functional_content_nid__town_council__block',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-town_council-block' => array(
          'module' => 'views',
          'delta' => 'town_council-block',
          'region' => 'below_content',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('views');
  $export['dvg_town_council_view'] = $context;

  return $export;
}
