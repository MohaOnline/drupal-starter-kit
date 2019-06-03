<?php
/**
 * @file
 * dvg_voc_owms_authority.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function dvg_voc_owms_authority_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_resource_identifier'.
  $field_bases['field_resource_identifier'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_resource_identifier',
    'indexes' => array(),
    'locked' => 0,
    'module' => 'link',
    'settings' => array(
      'attributes' => array(
        'class' => '',
        'rel' => '',
        'target' => 'default',
      ),
      'display' => array(
        'url_cutoff' => 80,
      ),
      'enable_tokens' => 1,
      'title' => 'optional',
      'title_maxlength' => 128,
      'title_value' => '',
      'url' => 0,
    ),
    'translatable' => 0,
    'type' => 'link_field',
  );

  // Exported field_base: 'field_scheme'.
  $field_bases['field_scheme'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_scheme',
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'overheid:Gemeente' => 'Gemeente',
        'overheid:Waterschap' => 'Waterschap',
        'overheid:Provincie' => 'Provincie',
        'overheid:Ministerie' => 'Ministerie',
        'overheid:AndereOrganisatie' => 'Andere organisatie',
        'overheid:GGD' => 'GGD',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  return $field_bases;
}
