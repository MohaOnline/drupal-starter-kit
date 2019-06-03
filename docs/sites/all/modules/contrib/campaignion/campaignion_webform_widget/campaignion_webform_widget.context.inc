<?php
/**
 * @file
 * campaignion_webform_widget.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function campaignion_webform_widget_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'current_webform_embedded';
  $context->description = 'Context for an embedded webform.';
  $context->tag = 'Campaignion';
  $context->conditions = array(
    'context_get' => array(
      'values' => array(
        'embed=1' => 'embed=1',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'cck_blocks-pgbar_default' => array(
          'module' => 'cck_blocks',
          'delta' => 'pgbar_default',
          'region' => 'widget',
          'weight' => '-19',
          'title' => '<none>',
        ),
        'webform_block-current' => array(
          'module' => 'webform_block',
          'delta' => 'current',
          'region' => 'widget',
          'weight' => '-18',
          'title' => '<none>',
        ),
      ),
    ),
    'theme_html' => array(
      'class' => 'embedded-webform no-aetoolbar',
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Campaignion');
  t('Context for an embedded webform.');
  $export['current_webform_embedded'] = $context;

  return $export;
}
