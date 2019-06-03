<?php

/**
 * @file
 * campaignion_test.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function campaignion_test_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_overlay_options|node|thank_you_page|form';
  $field_group->group_name = 'group_overlay_options';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'thank_you_page';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Overlay options',
    'weight' => '6',
    'children' => array(
      0 => 'group_overlay',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Overlay options',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-overlay-options field-group-fieldset field-group-heading',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_overlay_options|node|thank_you_page|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_overlay|node|thank_you_page|form';
  $field_group->group_name = 'group_overlay';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'thank_you_page';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_overlay_options';
  $field_group->data = array(
    'label' => 'Overlay',
    'weight' => '7',
    'children' => array(
      0 => 'campaignion_overlay_options',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Overlay',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-overlay field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $field_groups['group_overlay|node|thank_you_page|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Overlay');
  t('Overlay options');

  return $field_groups;
}
