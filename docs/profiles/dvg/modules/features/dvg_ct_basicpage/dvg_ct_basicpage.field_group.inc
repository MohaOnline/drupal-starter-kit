<?php
/**
 * @file
 * dvg_ct_basicpage.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function dvg_ct_basicpage_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_basic_page|node|page|form';
  $field_group->group_name = 'group_basic_page';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'page';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Basic Page',
    'weight' => '0',
    'children' => array(
      0 => 'group_basicinfo',
      1 => 'group_content',
      2 => 'group_highlight',
      3 => 'group_search_and_follow',
    ),
    'format_type' => 'htabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => 'group-basic-page field-group-htabs',
      ),
    ),
  );
  $field_groups['group_basic_page|node|page|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_basicinfo|node|page|form';
  $field_group->group_name = 'group_basicinfo';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'page';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_basic_page';
  $field_group->data = array(
    'label' => 'Basic info',
    'weight' => '34',
    'children' => array(
      0 => 'field_image',
      1 => 'field_menu_description',
      2 => 'field_menu_image',
      3 => 'title',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'label' => 'Basic info',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-basicinfo field-group-htab',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_basicinfo|node|page|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_content|node|page|form';
  $field_group->group_name = 'group_content';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'page';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_basic_page';
  $field_group->data = array(
    'label' => 'Content',
    'weight' => '35',
    'children' => array(
      0 => 'body',
      1 => 'field_introduction',
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
  $field_groups['group_content|node|page|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_highlight|node|page|form';
  $field_group->group_name = 'group_highlight';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'page';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_basic_page';
  $field_group->data = array(
    'label' => 'Highlight',
    'weight' => '37',
    'children' => array(
      0 => 'field_highlight',
      1 => 'field_highlight_image',
      2 => 'field_highlight_more_label',
      3 => 'field_highlight_text',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-highlight field-group-htab',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_highlight|node|page|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_main_section|node|page|full';
  $field_group->group_name = 'group_main_section';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'page';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Main section',
    'weight' => '0',
    'children' => array(
      0 => 'body',
      1 => 'field_introduction',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Main section',
      'instance_settings' => array(
        'classes' => 'main-section',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'none',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_main_section|node|page|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_related_pages|node|page|form';
  $field_group->group_name = 'group_related_pages';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'page';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_search_and_follow';
  $field_group->data = array(
    'label' => 'Related pages',
    'weight' => '18',
    'children' => array(
      0 => 'field_related_pages',
      1 => 'field_title_related_pages',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Related pages',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_related_pages|node|page|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_related_pages|node|page|full';
  $field_group->group_name = 'group_related_pages';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'page';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => '  Related pages section',
    'weight' => '2',
    'children' => array(
      0 => 'field_related_pages',
      1 => 'field_title_related_pages',
      2 => 'related_guides',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => '  Related pages section',
      'instance_settings' => array(
        'classes' => 'related-pages-section',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h2',
        'effect' => 'none',
        'speed' => 'none',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_related_pages|node|page|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_search_and_follow|node|page|form';
  $field_group->group_name = 'group_search_and_follow';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'page';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_basic_page';
  $field_group->data = array(
    'label' => 'Search and Follow',
    'weight' => '36',
    'children' => array(
      0 => 'field_alternate_keywords',
      1 => 'field_boost_keywords',
      2 => 'field_search_result',
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
  $field_groups['group_search_and_follow|node|page|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Basic Info');
  t('Basic Page');
  t('Basic info');
  t('Content');
  t('Highlight');
  t('Main section');
  t('Related pages');
  t('  Related pages section');
  t('Search and Follow');
  return $field_groups;
}
