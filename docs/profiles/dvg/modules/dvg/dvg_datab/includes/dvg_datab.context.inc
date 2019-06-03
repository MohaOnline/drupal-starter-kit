<?php

/**
 * @file Default context for DVG Data B.
 */

/**
 * Implements hook_context_default_contexts().
 */
function dvg_datab_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'dvg_datab_doclist';
  $context->description = 'Data B. DocList.';
  $context->tag = 'dvg_datab';
  $context->conditions = array(
    'callback' => array(
      'values' => array(
        'dvg_datab_doclist' => 'dvg_datab_doclist',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'dvg_datab-dvg_datab__doclist' => array(
          'module' => 'dvg_datab',
          'delta' => 'dvg_datab__doclist',
          'region' => 'below_content',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Data B. DocList');
  t('dvg_datab');

  $export['dvg_datab_doclist'] = $context;

  return $export;
}
