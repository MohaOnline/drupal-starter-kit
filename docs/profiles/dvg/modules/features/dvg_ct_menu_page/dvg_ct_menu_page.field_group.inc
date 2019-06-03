<?php
/**
 * @file
 * dvg_ct_menu_page.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function dvg_ct_menu_page_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_basicinfo|node|menu_page|form';
  $field_group->group_name = 'group_basicinfo';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'menu_page';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_menu_page';
  $field_group->data = array(
    'label' => 'Basic info',
    'weight' => '1',
    'children' => array(
      0 => 'field_menu_description',
      1 => 'title',
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
  $field_groups['group_basicinfo|node|menu_page|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_content|node|menu_page|form';
  $field_group->group_name = 'group_content';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'menu_page';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_menu_page';
  $field_group->data = array(
    'label' => 'Content',
    'weight' => '2',
    'children' => array(
      0 => 'body',
      1 => 'field_external_url',
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
  $field_groups['group_content|node|menu_page|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_menu_page|node|menu_page|form';
  $field_group->group_name = 'group_menu_page';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'menu_page';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Menu page',
    'weight' => '0',
    'children' => array(
      0 => 'group_search_and_follow',
      1 => 'group_basicinfo',
      2 => 'group_content',
    ),
    'format_type' => 'htabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => 'group-menu-page field-group-htabs',
      ),
    ),
  );
  $field_groups['group_menu_page|node|menu_page|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_search_and_follow|node|menu_page|form';
  $field_group->group_name = 'group_search_and_follow';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'menu_page';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_menu_page';
  $field_group->data = array(
    'label' => 'Search and Follow',
    'weight' => '3',
    'children' => array(
      0 => 'field_alternate_keywords',
      1 => 'field_search_result',
      2 => 'field_boost_keywords',
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
  $field_groups['group_search_and_follow|node|menu_page|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Basic info');
  t('Content');
  t('Menu page');
  t('Search and Follow');
  return $field_groups;
}
