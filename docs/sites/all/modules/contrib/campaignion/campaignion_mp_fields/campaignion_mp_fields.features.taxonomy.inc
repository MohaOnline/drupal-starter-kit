<?php
/**
 * @file
 * campaignion_mp_fields.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function campaignion_mp_fields_taxonomy_default_vocabularies() {
  return array(
    'mp_country' => array(
      'name' => 'Country',
      'machine_name' => 'mp_country',
      'description' => 'Devolved country',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'language' => 'und',
      'i18n_mode' => 0,
    ),
    'mp_party' => array(
      'name' => 'MP Party',
      'machine_name' => 'mp_party',
      'description' => 'Parties sitting in the house of commons.',
      'hierarchy' => 0,
      'module' => 'taxonomy',
      'weight' => 0,
      'language' => 'und',
      'i18n_mode' => 0,
    ),
  );
}
