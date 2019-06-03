<?php
/**
 * @file
 * dvg_search_view.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function dvg_search_view_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'search';
  $context->description = '';
  $context->tag = 'views';
  $context->conditions = array(
    'callback' => array(
      'values' => array(
        'functional_content_nid__search__block' => 'functional_content_nid__search__block',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-search-block' => array(
          'module' => 'views',
          'delta' => 'search-block',
          'region' => 'below_content',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('views');
  $export['search'] = $context;

  return $export;
}
