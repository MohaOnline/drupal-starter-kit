<?php
/**
 * @file
 * campaignion_mp_fields.features.uuid_term.inc
 */

/**
 * Implements hook_uuid_features_default_terms().
 */
function campaignion_mp_fields_uuid_features_default_terms() {
  $terms = array();

  $terms[] = array(
    'name' => 'England',
    'description' => '',
    'format' => 'full_html_with_editor',
    'weight' => 0,
    'uuid' => '3e4dc5dd-0335-4e6a-bd68-585a87aa9a80',
    'language' => 'und',
    'i18n_tsid' => 0,
    'vocabulary_machine_name' => 'mp_country',
    'path' => array(
      'pathauto' => 1,
    ),
  );
  $terms[] = array(
    'name' => 'Northern Ireland',
    'description' => '',
    'format' => 'full_html_with_editor',
    'weight' => 0,
    'uuid' => '48bba52f-836f-4ccf-afac-9944e9eaf599',
    'language' => 'und',
    'i18n_tsid' => 0,
    'vocabulary_machine_name' => 'mp_country',
    'path' => array(
      'pathauto' => 1,
    ),
  );
  $terms[] = array(
    'name' => 'Scotland',
    'description' => '',
    'format' => 'full_html_with_editor',
    'weight' => 0,
    'uuid' => '578118e7-477e-44b7-85df-cd897437c91d',
    'language' => 'und',
    'i18n_tsid' => 0,
    'vocabulary_machine_name' => 'mp_country',
    'path' => array(
      'pathauto' => 1,
    ),
  );
  $terms[] = array(
    'name' => 'Wales',
    'description' => '',
    'format' => 'full_html_with_editor',
    'weight' => 0,
    'uuid' => '60c353e3-7079-40ff-9790-264bd1a19a1f',
    'language' => 'und',
    'i18n_tsid' => 0,
    'vocabulary_machine_name' => 'mp_country',
    'path' => array(
      'pathauto' => 1,
    ),
  );
  return $terms;
}
