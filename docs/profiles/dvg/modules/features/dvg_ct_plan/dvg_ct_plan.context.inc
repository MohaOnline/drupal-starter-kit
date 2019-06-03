<?php
/**
 * @file
 * dvg_ct_plan.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function dvg_ct_plan_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'plan';
  $context->description = '';
  $context->tag = 'views';
  $context->conditions = array(
    'callback' => array(
      'values' => array(
        'functional_content_nid__spatial_plan__block' => 'functional_content_nid__spatial_plan__block',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-spatial_plan-block' => array(
          'module' => 'views',
          'delta' => 'spatial_plan-block',
          'region' => 'below_content',
          'weight' => '-9',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('views');
  $export['plan'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'plan_node';
  $context->description = '';
  $context->tag = 'page';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'plan' => 'plan',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'callback' => array(
      'dvg_ct_plan_plan_node_breadcrumb' => 'dvg_ct_plan_plan_node_breadcrumb',
      'dvg_ct_news_news_node_breadcrumb' => 0,
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('page');
  $export['plan_node'] = $context;

  return $export;
}
