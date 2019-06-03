<?php
/**
 * @file
 * dvg_ct_live_blog.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function dvg_ct_live_blog_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_basic_info|node|live_blog|form';
  $field_group->group_name = 'group_basic_info';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'live_blog';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_live_blog';
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
  $field_groups['group_basic_info|node|live_blog|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_content|node|live_blog|form';
  $field_group->group_name = 'group_content';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'live_blog';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_live_blog';
  $field_group->data = array(
    'label' => 'Content',
    'weight' => '8',
    'children' => array(
      0 => 'field_body',
      1 => 'field_blog',
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
  $field_groups['group_content|node|live_blog|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_higlight|node|live_blog|form';
  $field_group->group_name = 'group_higlight';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'live_blog';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_live_blog';
  $field_group->data = array(
    'label' => 'Highlight',
    'weight' => '10',
    'children' => array(
      0 => 'field_highlight',
      1 => 'field_highlight_image',
      2 => 'field_highlight_text',
      3 => 'field_highlight_more_label',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-higlight field-group-htab',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_higlight|node|live_blog|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_live_blog|node|live_blog|form';
  $field_group->group_name = 'group_live_blog';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'live_blog';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Live blog',
    'weight' => '0',
    'children' => array(
      0 => 'group_basic_info',
      1 => 'group_content',
      2 => 'group_search_and_follow',
      3 => 'group_higlight',
    ),
    'format_type' => 'htabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => 'group-live-blog field-group-htabs',
      ),
    ),
  );
  $field_groups['group_live_blog|node|live_blog|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_search_and_follow|node|live_blog|form';
  $field_group->group_name = 'group_search_and_follow';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'live_blog';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_live_blog';
  $field_group->data = array(
    'label' => 'Search and Follow',
    'weight' => '9',
    'children' => array(
      0 => 'field_search_result',
      1 => 'field_alternate_keywords',
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
  $field_groups['group_search_and_follow|node|live_blog|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Basic info');
  t('Content');
  t('Highlight');
  t('Live blog');
  t('Search and Follow');
  return $field_groups;
}
