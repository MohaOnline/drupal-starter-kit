<?php
/**
 * @file
 * dvg_ct_news.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function dvg_ct_news_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'news';
  $context->description = '';
  $context->tag = 'views';
  $context->conditions = array(
    'callback' => array(
      'values' => array(
        'functional_content_nid__news__block_news' => 'functional_content_nid__news__block_news',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-news-block_news' => array(
          'module' => 'views',
          'delta' => 'news-block_news',
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
  $export['news'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'news_node';
  $context->description = '';
  $context->tag = 'page';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'news' => 'news',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'callback' => array(
      'dvg_ct_news_news_node_breadcrumb' => 'dvg_ct_news_news_node_breadcrumb',
      'dvg_ct_plan_plan_node_breadcrumb' => 0,
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('page');
  $export['news_node'] = $context;

  return $export;
}
