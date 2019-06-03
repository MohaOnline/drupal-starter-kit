<?php
/**
 * @file
 * dvg_digid.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function dvg_digid_autologout_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'user-menu-autologout';
  $context->description = '';
  $context->tag = 'Users';
  $context->conditions = array(
    'user' => array(
      'values' => array(
        'digid' => 'digid',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'dvg_digid_autologout-info' => array(
          'module' => 'dvg_digid_autologout',
          'delta' => 'info',
          'region' => 'top',
          'weight' => '-15',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Users');
  $export['user-menu-autologout'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'user-menu';
  $context->description = '';
  $context->tag = 'Users';
  $context->conditions = array(
    'user' => array(
      'values' => array(
        'digid' => 'digid',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'system-user-menu' => array(
          'module' => 'system',
          'delta' => 'user-menu',
          'region' => 'top',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Users');
  $export['user-menu'] = $context;

  return $export;
}
