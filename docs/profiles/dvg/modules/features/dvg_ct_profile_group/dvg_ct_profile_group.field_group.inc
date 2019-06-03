<?php
/**
 * @file
 * dvg_ct_profile_group.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function dvg_ct_profile_group_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_basicinfo|node|profile_group|form';
  $field_group->group_name = 'group_basicinfo';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'profile_group';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_profilegroup';
  $field_group->data = array(
    'label' => 'Basic info',
    'weight' => '1',
    'children' => array(
      0 => 'field_menu_description',
      1 => 'field_menu_image',
      2 => 'title',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-basicinfo field-group-htab',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_basicinfo|node|profile_group|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_content|node|profile_group|form';
  $field_group->group_name = 'group_content';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'profile_group';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_profilegroup';
  $field_group->data = array(
    'label' => 'Content',
    'weight' => '2',
    'children' => array(
      0 => 'body',
      1 => 'field_introduction',
      2 => 'field_profile_group_description',
      3 => 'field_profile_group_photo',
      4 => 'field_profile_group_profiles',
      5 => 'field_profile_group_type',
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
  $field_groups['group_content|node|profile_group|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_profilegroup|node|profile_group|form';
  $field_group->group_name = 'group_profilegroup';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'profile_group';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Profilegroup',
    'weight' => '0',
    'children' => array(
      0 => 'group_basicinfo',
      1 => 'group_content',
      2 => 'group_search_and_follow',
    ),
    'format_type' => 'htabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => 'group-profilegroup field-group-htabs',
      ),
    ),
  );
  $field_groups['group_profilegroup|node|profile_group|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_search_and_follow|node|profile_group|form';
  $field_group->group_name = 'group_search_and_follow';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'profile_group';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_profilegroup';
  $field_group->data = array(
    'label' => 'Search and Follow',
    'weight' => '3',
    'children' => array(
      0 => 'field_alternate_keywords',
      1 => 'field_boost_keywords',
      2 => 'field_search_result',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-search-and-follow field-group-htab',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_search_and_follow|node|profile_group|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Basic info');
  t('Content');
  t('Profilegroup');
  t('Search and Follow');
  return $field_groups;
}
