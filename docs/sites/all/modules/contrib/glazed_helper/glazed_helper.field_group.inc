<?php
/**
 * @file
 * glazed_helper.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function glazed_helper_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_glazed_header_top|field_collection_item|field_glazed_content_design|form';
  $field_group->group_name = 'group_glazed_header_top';
  $field_group->entity_type = 'field_collection_item';
  $field_group->bundle = 'field_glazed_content_design';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_glazed_header';
  $field_group->data = array(
    'label' => 'Top Header',
    'weight' => '11',
    'children' => array(
      0 => 'field_glazed_header_position',
      1 => 'field_glazed_header_style',
      2 => 'field_header_height',
      3 => 'field_full_width_header',
      4 => 'field_header_layout',
      5 => 'group_navigation_colors',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Top Header',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-glazed-header-top field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $field_groups['group_glazed_header_top|field_collection_item|field_glazed_content_design|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_glazed_header|field_collection_item|field_glazed_content_design|form';
  $field_group->group_name = 'group_glazed_header';
  $field_group->entity_type = 'field_collection_item';
  $field_group->bundle = 'field_glazed_content_design';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Header',
    'weight' => '2',
    'children' => array(
      0 => 'field_glazed_header_mode',
      1 => 'field_menu_hover_effect',
      2 => 'group_glazed_header_top',
      3 => 'group_side_header',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Header',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-glazed-header field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $field_groups['group_glazed_header|field_collection_item|field_glazed_content_design|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_glazed_layout|field_collection_item|field_glazed_content_design|form';
  $field_group->group_name = 'group_glazed_layout';
  $field_group->entity_type = 'field_collection_item';
  $field_group->bundle = 'field_glazed_content_design';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Layout',
    'weight' => '3',
    'children' => array(
      0 => 'field_glazed_content_region',
      1 => 'field_glazed_hide_regions',
      2 => 'field_page_layout',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Layout',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-glazed-layout field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $field_groups['group_glazed_layout|field_collection_item|field_glazed_content_design|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_glazed|node|blog|form';
  $field_group->group_name = 'group_glazed';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'blog';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Glazed Page Settings',
    'weight' => '8',
    'children' => array(
      0 => 'field_glazed_content_design',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Glazed Page Settings',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-glazed field-group-tab',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_glazed|node|blog|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_glazed|node|cb_drag_drop_page|form';
  $field_group->group_name = 'group_glazed';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'cb_drag_drop_page';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Glazed Page Settings',
    'weight' => '8',
    'children' => array(
      0 => 'field_glazed_content_design',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Glazed Page Settings',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-glazed field-group-tab',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_glazed|node|cb_drag_drop_page|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_glazed|node|drag_drop_page|form';
  $field_group->group_name = 'group_glazed';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'drag_drop_page';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Glazed Page Settings',
    'weight' => '9',
    'children' => array(
      0 => 'field_glazed_content_design',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Glazed Page Settings',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-glazed field-group-tab',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_glazed|node|drag_drop_page|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_glazed|node|event|form';
  $field_group->group_name = 'group_glazed';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'event';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Glazed Page Settings',
    'weight' => '10',
    'children' => array(
      0 => 'field_glazed_content_design',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Glazed Page Settings',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_glazed|node|event|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_glazed|node|news|form';
  $field_group->group_name = 'group_glazed';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'news';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Glazed Page Settings',
    'weight' => '4',
    'children' => array(
      0 => 'field_glazed_content_design',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Glazed Page Settings',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-glazed field-group-tab',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_glazed|node|news|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_glazed|node|page|form';
  $field_group->group_name = 'group_glazed';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'page';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Glazed Page Settings',
    'weight' => '46',
    'children' => array(
      0 => 'field_glazed_content_design',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Glazed Page Settings',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-glazed field-group-tab',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_glazed|node|page|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_glazed|node|portfolio|form';
  $field_group->group_name = 'group_glazed';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'portfolio';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Glazed Page Settings',
    'weight' => '7',
    'children' => array(
      0 => 'field_glazed_content_design',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Glazed Page Settings',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-glazed field-group-tab',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_glazed|node|portfolio|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_glazed|node|webform|form';
  $field_group->group_name = 'group_glazed';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'webform';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Glazed Page Settings',
    'weight' => '45',
    'children' => array(
      0 => 'field_glazed_content_design',
    ),
    'format_type' => 'tab',
    'format_settings' => array(
      'label' => 'Glazed Page Settings',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-glazed field-group-tab',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_glazed|node|webform|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_navigation_colors|field_collection_item|field_glazed_content_design|form';
  $field_group->group_name = 'group_navigation_colors';
  $field_group->entity_type = 'field_collection_item';
  $field_group->bundle = 'field_glazed_content_design';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_glazed_header_top';
  $field_group->data = array(
    'label' => 'Custom Colors',
    'weight' => '43',
    'children' => array(
      0 => 'field_navbar_background',
      1 => 'field_dropdown_background',
      2 => 'field_dropdown_hover_background',
      3 => 'field_dropdown_hover_text_color',
      4 => 'field_dropdown_text_color',
      5 => 'field_header_background',
      6 => 'field_header_block_background',
      7 => 'field_header_block_text_color',
      8 => 'field_menu_background',
      9 => 'field_menu_hover_background',
      10 => 'field_menu_hover_text_color',
      11 => 'field_menu_text_color',
      12 => 'field_header_text',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Custom Colors',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-navigation-colors field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $field_groups['group_navigation_colors|field_collection_item|field_glazed_content_design|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_side_header|field_collection_item|field_glazed_content_design|form';
  $field_group->group_name = 'group_side_header';
  $field_group->entity_type = 'field_collection_item';
  $field_group->bundle = 'field_glazed_content_design';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_glazed_header';
  $field_group->data = array(
    'label' => 'Side Header',
    'weight' => '12',
    'children' => array(
      0 => 'field_side_header_width',
      1 => 'field_side_header_background',
      2 => 'field_side_header_text_color',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Side Header',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-side-header field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $field_groups['group_side_header|field_collection_item|field_glazed_content_design|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Custom Colors');
  t('Glazed Page Settings');
  t('Glazed Page Settings');
  t('Header');
  t('Layout');
  t('Side Header');
  t('Top Header');

  return $field_groups;
}
