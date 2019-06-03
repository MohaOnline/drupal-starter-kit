<?php
/**
 * @file
 * dvg_voc_owms_uniform_product_name.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function dvg_voc_owms_uniform_product_name_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance:
  // 'taxonomy_term-owms_uniform_product_name-field_resource_identifier'.
  $field_instances['taxonomy_term-owms_uniform_product_name-field_resource_identifier'] = array(
    'bundle' => 'owms_uniform_product_name',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'link',
        'settings' => array(),
        'type' => 'link_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'taxonomy_term',
    'field_name' => 'field_resource_identifier',
    'label' => 'Resource Identifier',
    'required' => 1,
    'settings' => array(
      'absolute_url' => 1,
      'attributes' => array(
        'class' => '',
        'configurable_title' => 0,
        'rel' => '',
        'target' => 'default',
        'title' => '',
      ),
      'display' => array(
        'url_cutoff' => 80,
      ),
      'enable_tokens' => 1,
      'linkit' => array(
        'button_text' => 'Search',
        'enable' => 0,
        'profile' => '',
      ),
      'rel_remove' => 'default',
      'title' => 'none',
      'title_label_use_field_label' => FALSE,
      'title_maxlength' => 128,
      'title_value' => '',
      'url' => 0,
      'user_register_form' => FALSE,
      'validate_url' => 1,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'link',
      'settings' => array(),
      'type' => 'link_field',
      'weight' => 2,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Resource Identifier', array(), array('context' => 'field_resource_identifier:owms_uniform_product_name:label'));
  return $field_instances;
}
