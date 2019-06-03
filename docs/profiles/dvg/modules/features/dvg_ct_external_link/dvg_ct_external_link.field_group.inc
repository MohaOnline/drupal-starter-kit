<?php
/**
 * @file
 * dvg_ct_external_link.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function dvg_ct_external_link_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_basic_info|node|external_link|form';
  $field_group->group_name = 'group_basic_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'external_link';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_external_link';
  $field_group->data = array(
    'label' => 'Basic info',
    'weight' => '7',
    'children' => array(
      0 => 'field_menu_description',
      1 => 'title',
      2 => 'field_menu_image',
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
  $field_groups['group_basic_info|node|external_link|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_content|node|external_link|form';
  $field_group->group_name = 'group_content';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'external_link';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_external_link';
  $field_group->data = array(
    'label' => 'Content',
    'weight' => '8',
    'children' => array(
      0 => 'field_external_url',
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
  $field_groups['group_content|node|external_link|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_external_link|node|external_link|form';
  $field_group->group_name = 'group_external_link';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'external_link';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'External link',
    'weight' => '0',
    'children' => array(
      0 => 'group_basic_info',
      1 => 'group_content',
      2 => 'group_search_and_follow',
    ),
    'format_type' => 'htabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => '',
      ),
    ),
  );
  $field_groups['group_external_link|node|external_link|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_search_and_follow|node|external_link|form';
  $field_group->group_name = 'group_search_and_follow';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'external_link';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_external_link';
  $field_group->data = array(
    'label' => 'Search and Follow',
    'weight' => '9',
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
  $field_groups['group_search_and_follow|node|external_link|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Basic info');
  t('Content');
  t('External link');
  t('Search and Follow');
  return $field_groups;
}
