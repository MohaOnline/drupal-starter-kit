<?php
/**
 * @file
 * dvg_ct_live_blog.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function dvg_ct_live_blog_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'live_blog_node';
  $context->description = '';
  $context->tag = 'page';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'live_blog' => 'live_blog',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-live_blog_items-block' => array(
          'module' => 'views',
          'delta' => 'live_blog_items-block',
          'region' => 'below_content',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('page');
  $export['live_blog_node'] = $context;

  return $export;
}
