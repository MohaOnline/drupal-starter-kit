<?php
/**
 * @file
 * dvg_ct_crisis_block.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function dvg_ct_crisis_block_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_basic_info|node|crisis_block|form';
  $field_group->group_name = 'group_basic_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'crisis_block';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_crisis_block';
  $field_group->data = array(
    'label' => 'Basic info',
    'weight' => '7',
    'children' => array(
      0 => 'title',
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
  $export['group_basic_info|node|crisis_block|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_content|node|crisis_block|form';
  $field_group->group_name = 'group_content';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'crisis_block';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_crisis_block';
  $field_group->data = array(
    'label' => 'Content',
    'weight' => '8',
    'children' => array(
      0 => 'field_body',
      1 => 'field_link',
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
  $export['group_content|node|crisis_block|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_crisis_block|node|crisis_block|form';
  $field_group->group_name = 'group_crisis_block';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'crisis_block';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Crisis block',
    'weight' => '0',
    'children' => array(
      0 => 'group_basic_info',
      1 => 'group_content',
    ),
    'format_type' => 'htabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => 'group-crisis-block field-group-htabs',
      ),
    ),
  );
  $export['group_crisis_block|node|crisis_block|form'] = $field_group;

  return $export;
}
