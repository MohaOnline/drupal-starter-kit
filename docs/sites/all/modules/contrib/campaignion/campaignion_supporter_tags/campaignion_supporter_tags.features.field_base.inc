<?php
/**
 * @file
 * campaignion_supporter_tags.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function campaignion_supporter_tags_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'supporter_tags'.
  $field_bases['supporter_tags'] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'supporter_tags',
    'global_block_settings' => 1,
    'indexes' => array(
      'tid' => array(
        0 => 'tid',
      ),
    ),
    'locked' => 0,
    'module' => 'taxonomy',
    'settings' => array(
      'allowed_values' => array(
        0 => array(
          'vocabulary' => 'supporter_tags',
          'parent' => 0,
        ),
      ),
      'options_list_callback' => 'i18n_taxonomy_allowed_values',
      'profile2_private' => FALSE,
    ),
    'translatable' => 0,
    'type' => 'taxonomy_term_reference',
  );

  return $field_bases;
}
