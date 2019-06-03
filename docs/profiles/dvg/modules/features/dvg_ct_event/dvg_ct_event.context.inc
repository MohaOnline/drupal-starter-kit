<?php
/**
 * @file
 * dvg_ct_event.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function dvg_ct_event_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'event_node';
  $context->description = '';
  $context->tag = 'page';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'event' => 'event',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'callback' => array(
      'dvg_ct_event_event_node_breadcrumb' => 'dvg_ct_event_event_node_breadcrumb',
      'dvg_ct_news_news_node_breadcrumb' => 0,
      'dvg_ct_plan_plan_node_breadcrumb' => 0,
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('page');
  $export['event_node'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'events';
  $context->description = '';
  $context->tag = 'views';
  $context->conditions = array(
    'callback' => array(
      'values' => array(
        'functional_content_nid__events__block_events' => 'functional_content_nid__events__block_events',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-events-block_events' => array(
          'module' => 'views',
          'delta' => 'events-block_events',
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
  $export['events'] = $context;

  return $export;
}
