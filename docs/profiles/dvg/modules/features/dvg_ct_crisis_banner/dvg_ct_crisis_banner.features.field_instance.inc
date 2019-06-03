<?php
/**
 * @file
 * dvg_ct_crisis_banner.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function dvg_ct_crisis_banner_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-crisis_banner-field_body'
  $field_instances['node-crisis_banner-field_body'] = array(
    'bundle' => 'crisis_banner',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_body',
    'label' => 'Body',
    'required' => 1,
    'settings' => array(
      'linkit' => array(
        'button_text' => 'Search',
        'enable' => 0,
        'profile' => '',
      ),
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'node-crisis_banner-field_code'
  $field_instances['node-crisis_banner-field_code'] = array(
    'bundle' => 'crisis_banner',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 2,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_code',
    'label' => 'Code',
    'required' => 0,
    'settings' => array(
      'linkit' => array(
        'button_text' => 'Search',
        'enable' => 0,
        'profile' => '',
      ),
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'node-crisis_banner-field_reference'
  $field_instances['node-crisis_banner-field_reference'] = array(
    'bundle' => 'crisis_banner',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Show the banner on these pages. Leave empty to show on every page.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_reference',
    'label' => 'Pages',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-crisis_banner-field_type_banner'
  $field_instances['node-crisis_banner-field_type_banner'] = array(
    'bundle' => 'crisis_banner',
    'default_value' => array(
      0 => array(
        'value' => 'notification',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 3,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_type_banner',
    'label' => 'Type',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 2,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Body', array(), array('context' => 'field_body:crisis_banner:label'));
  t('Code', array(), array('context' => 'field_code:crisis_banner:label'));
  t('Pages', array(), array('context' => 'field_reference:crisis_banner:label'));
  t('Show the banner on these pages. Leave empty to show on every page.', array(), array('context' => 'field_reference:crisis_banner:description'));
  t('Type', array(), array('context' => 'field_type_banner:crisis_banner:label'));
  t('Notification', array(), array('context' => 'field_type_banner:#allowed_values:notification'));
  t('Crisis', array(), array('context' => 'field_type_banner:#allowed_values:crisis'));
  return $field_instances;
}
