<?php
/**
 * @file
 * dvg_domains.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function dvg_domains_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'dvg-domains-global';
  $context->description = '';
  $context->tag = 'DVG Domains Global';
  $context->conditions = array(
    'sitewide' => array(
      'values' => array(
        1 => 1,
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'dvg_domains-dvg_domains_social-media-menu' => array(
          'module' => 'dvg_domains',
          'delta' => 'dvg_domains_social-media-menu',
          'region' => 'footer_top',
          'weight' => '-10',
        ),
        'dvg_domains-dvg_domains_footer-menu' => array(
          'module' => 'dvg_domains',
          'delta' => 'dvg_domains_footer-menu',
          'region' => 'footer_top',
          'weight' => '-9',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('DVG Domains Global');
  $export['dvg-domains-global'] = $context;

  return $export;
}
