<?php
/**
 * @file
 * dvg_ct_guide.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function dvg_ct_guide_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_basicinfo|node|guide|form';
  $field_group->group_name = 'group_basicinfo';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'guide';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_guide';
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
  $field_groups['group_basicinfo|node|guide|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_content|node|guide|form';
  $field_group->group_name = 'group_content';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'guide';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_guide';
  $field_group->data = array(
    'label' => 'Content',
    'weight' => '2',
    'children' => array(
      0 => 'field_guide_pages',
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
  $field_groups['group_content|node|guide|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_guide|node|guide|form';
  $field_group->group_name = 'group_guide';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'guide';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Guide',
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
        'classes' => 'group-guide field-group-htabs',
      ),
    ),
  );
  $field_groups['group_guide|node|guide|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_related_pages|node|guide|form';
  $field_group->group_name = 'group_related_pages';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'guide';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_search_and_follow';
  $field_group->data = array(
    'label' => 'Related pages',
    'weight' => '9',
    'children' => array(
      0 => 'field_title_related_pages',
      1 => 'field_related_pages',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Related pages',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-related-pages field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_related_pages|node|guide|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_search_and_follow|node|guide|form';
  $field_group->group_name = 'group_search_and_follow';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'guide';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_guide';
  $field_group->data = array(
    'label' => 'Search and Follow',
    'weight' => '3',
    'children' => array(
      0 => 'field_search_result',
      1 => 'field_alternate_keywords',
      2 => 'field_boost_keywords',
      3 => 'group_related_pages',
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
  $field_groups['group_search_and_follow|node|guide|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Basic info');
  t('Content');
  t('Guide');
  t('Related pages');
  t('Search and Follow');
  return $field_groups;
}
