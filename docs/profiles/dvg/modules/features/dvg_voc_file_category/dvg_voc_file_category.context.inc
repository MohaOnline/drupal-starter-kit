<?php
/**
 * @file
 * dvg_voc_file_category.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function dvg_voc_file_category_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'file-list-by-category';
  $context->description = '';
  $context->tag = 'views';
  $context->conditions = array(
    'callback' => array(
      'values' => array(
        'dvg_file_category_type' => 'dvg_file_category_type',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-file_list_by_category-block' => array(
          'module' => 'views',
          'delta' => 'file_list_by_category-block',
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
  $export['file-list-by-category'] = $context;

  return $export;
}
