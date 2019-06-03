<?php
/**
 * @file
 * dvg_ct_crisis_banner.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function dvg_ct_crisis_banner_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_basic_info|node|crisis_banner|form';
  $field_group->group_name = 'group_basic_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'crisis_banner';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_crisis_banner';
  $field_group->data = array(
    'label' => 'Basic info',
    'weight' => '7',
    'children' => array(
      0 => 'field_type_banner',
      1 => 'title',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-basic-info field-group-htab',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $export['group_basic_info|node|crisis_banner|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_content|node|crisis_banner|form';
  $field_group->group_name = 'group_content';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'crisis_banner';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_crisis_banner';
  $field_group->data = array(
    'label' => 'Content',
    'weight' => '8',
    'children' => array(
      0 => 'field_body',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-content field-group-htab',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $export['group_content|node|crisis_banner|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_crisis_banner|node|crisis_banner|form';
  $field_group->group_name = 'group_crisis_banner';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'crisis_banner';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Crisis banner',
    'weight' => '0',
    'children' => array(
      0 => 'group_basic_info',
      1 => 'group_content',
      2 => 'group_visibility',
    ),
    'format_type' => 'htabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => 'group-crisis-banner field-group-htabs',
      ),
    ),
  );
  $export['group_crisis_banner|node|crisis_banner|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_visibility|node|crisis_banner|form';
  $field_group->group_name = 'group_visibility';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'crisis_banner';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_crisis_banner';
  $field_group->data = array(
    'label' => 'Visibility',
    'weight' => '9',
    'children' => array(
      0 => 'field_code',
      1 => 'field_reference',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-visibility field-group-htab',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $export['group_visibility|node|crisis_banner|form'] = $field_group;

  return $export;
}
