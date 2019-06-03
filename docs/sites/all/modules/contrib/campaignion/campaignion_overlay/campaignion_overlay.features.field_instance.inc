<?php

/**
 * @file
 * campaignion_overlay.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function campaignion_overlay_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance:
  // 'field_collection_item-campaignion_overlay_options-campaignion_overlay_content'.
  $field_instances['field_collection_item-campaignion_overlay_options-campaignion_overlay_content'] = array(
    'bundle' => 'campaignion_overlay_options',
    'custom_add_another' => '',
    'custom_remove' => '',
    'default_value' => array(
      0 => array(
        'target_id' => NULL,
      ),
    ),
    'deleted' => 0,
    'description' => 'The content displayed in the overlay.',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 2,
      ),
    ),
    'entity_type' => 'field_collection_item',
    'field_name' => 'campaignion_overlay_content',
    'label' => 'Content',
    'required' => 0,
    'settings' => array(
      'custom_add_another' => '',
      'custom_remove' => '',
      'select2_integration' => array(
        'styling' => array(
          'dropdownAutoWidth' => 0,
          'placeholder' => '',
        ),
        'use_select2' => 1,
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'use_select2' => FALSE,
      ),
      'type' => 'options_select',
      'weight' => 3,
    ),
  );

  // Exported field_instance:
  // 'field_collection_item-campaignion_overlay_options-campaignion_overlay_enabled'.
  $field_instances['field_collection_item-campaignion_overlay_options-campaignion_overlay_enabled'] = array(
    'bundle' => 'campaignion_overlay_options',
    'custom_add_another' => '',
    'custom_remove' => '',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => 'Enable or disable display of the overlay.',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'field_collection_item',
    'field_name' => 'campaignion_overlay_enabled',
    'label' => 'Display an overlay',
    'required' => 0,
    'settings' => array(
      'custom_add_another' => '',
      'custom_remove' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'display_label' => 1,
      ),
      'type' => 'options_onoff',
      'weight' => 1,
    ),
  );

  // Exported field_instance:
  // 'field_collection_item-campaignion_overlay_options-campaignion_overlay_introduction'.
  $field_instances['field_collection_item-campaignion_overlay_options-campaignion_overlay_introduction'] = array(
    'bundle' => 'campaignion_overlay_options',
    'custom_add_another' => '',
    'custom_remove' => '',
    'default_value' => array(
      0 => array(
        'value' => '',
      ),
    ),
    'deleted' => 0,
    'description' => 'A short introduction to the content displayed in the overlay.',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'field_collection_item',
    'field_name' => 'campaignion_overlay_introduction',
    'label' => 'Introduction',
    'required' => 0,
    'settings' => array(
      'custom_add_another' => '',
      'custom_remove' => '',
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
      'weight' => 2,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('A short introduction to the content displayed in the overlay.');
  t('Content');
  t('Display an overlay');
  t('Enable or disable display of the overlay.');
  t('Introduction');
  t('The content displayed in the overlay.');

  return $field_instances;
}
