<?php
/**
 * @file
 * dvg_ct_task.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function dvg_ct_task_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_appointment|node|task|form';
  $field_group->group_name = 'group_appointment';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'task';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_calltoaction';
  $field_group->data = array(
    'label' => 'Appointment',
    'weight' => '-45',
    'children' => array(
      0 => 'field_btn_appointment',
      1 => 'field_confirm_appointment',
      2 => 'field_product_code_appointment',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Appointment',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-appointment field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_appointment|node|task|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_basicinfo|node|task|form';
  $field_group->group_name = 'group_basicinfo';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'task';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_task';
  $field_group->data = array(
    'label' => 'Basic info',
    'weight' => '35',
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
  $field_groups['group_basicinfo|node|task|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_buttons|node|task|full';
  $field_group->group_name = 'group_buttons';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'task';
  $field_group->mode = 'full';
  $field_group->parent_name = 'group_main_section';
  $field_group->data = array(
    'label' => 'Buttons',
    'weight' => '2',
    'children' => array(
      0 => 'field_btn_webform',
      1 => 'field_btn_appointment',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Buttons',
      'instance_settings' => array(
        'classes' => 'group-buttons field-group-div',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'none',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_buttons|node|task|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_calltoaction|node|task|form';
  $field_group->group_name = 'group_calltoaction';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'task';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_task';
  $field_group->data = array(
    'label' => 'Call to action',
    'weight' => '36',
    'children' => array(
      0 => 'field_referral_site',
      1 => 'field_callus',
      2 => 'group_appointment',
      3 => 'group_webform',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-calltoaction field-group-htab',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_calltoaction|node|task|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_content|node|task|form';
  $field_group->group_name = 'group_content';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'task';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_task';
  $field_group->data = array(
    'label' => 'Content',
    'weight' => '37',
    'children' => array(
      0 => 'field_callout',
      1 => 'field_introduction',
      2 => 'field_sections',
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
  $field_groups['group_content|node|task|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_highlight|node|task|form';
  $field_group->group_name = 'group_highlight';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'task';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_task';
  $field_group->data = array(
    'label' => 'Highlight',
    'weight' => '39',
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
  $field_groups['group_highlight|node|task|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_main_section|node|task|full';
  $field_group->group_name = 'group_main_section';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'task';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Main section',
    'weight' => '0',
    'children' => array(
      0 => 'field_callout',
      1 => 'field_callus',
      2 => 'field_referral_site',
      3 => 'field_introduction',
      4 => 'group_buttons',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Main section',
      'instance_settings' => array(
        'classes' => 'main-section',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h2',
        'effect' => 'none',
        'speed' => 'none',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_main_section|node|task|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_meta_data|node|task|form';
  $field_group->group_name = 'group_meta_data';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'task';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_task';
  $field_group->data = array(
    'label' => 'Meta data',
    'weight' => '40',
    'children' => array(
      0 => 'field_owms_audience',
      1 => 'field_owms_authority',
      2 => 'field_owms_uniform_product_name',
    ),
    'format_type' => 'htab',
    'format_settings' => array(
      'formatter' => 'closed',
      'instance_settings' => array(
        'description' => '',
        'classes' => 'group-meta-data field-group-htab',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_meta_data|node|task|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_related_pages|node|task|form';
  $field_group->group_name = 'group_related_pages';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'task';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_search';
  $field_group->data = array(
    'label' => 'Related pages',
    'weight' => '11',
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
  $field_groups['group_related_pages|node|task|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_related_pages|node|task|full';
  $field_group->group_name = 'group_related_pages';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'task';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Related pages section',
    'weight' => '4',
    'children' => array(
      0 => 'field_related_pages',
      1 => 'field_title_related_pages',
      2 => 'related_guides',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Related pages section',
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
  $field_groups['group_related_pages|node|task|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_search|node|task|form';
  $field_group->group_name = 'group_search';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'task';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_task';
  $field_group->data = array(
    'label' => 'Search and Follow',
    'weight' => '38',
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
        'classes' => 'group-search field-group-htab',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $field_groups['group_search|node|task|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_section_1|node|task|form';
  $field_group->group_name = 'group_section_1';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'task';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Section 1',
    'weight' => '6',
    'children' => array(),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Section 1',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $field_groups['group_section_1|node|task|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_section_1|node|task|full';
  $field_group->group_name = 'group_section_1';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'task';
  $field_group->mode = 'full';
  $field_group->parent_name = 'group_sections';
  $field_group->data = array(
    'label' => 'Section 1',
    'weight' => '1',
    'children' => array(),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Section 1',
      'instance_settings' => array(
        'classes' => 'group-section-1 task-section',
        'description' => '',
        'show_label' => '1',
        'label_element' => 'h2',
        'effect' => 'none',
        'speed' => 'none',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_section_1|node|task|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_section_2|node|task|form';
  $field_group->group_name = 'group_section_2';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'task';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Section 2',
    'weight' => '7',
    'children' => array(),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Section 2',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $field_groups['group_section_2|node|task|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_section_2|node|task|full';
  $field_group->group_name = 'group_section_2';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'task';
  $field_group->mode = 'full';
  $field_group->parent_name = 'group_sections';
  $field_group->data = array(
    'label' => 'Section 2',
    'weight' => '2',
    'children' => array(),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Section 2',
      'instance_settings' => array(
        'classes' => 'group-section-2 task-section',
        'description' => '',
        'show_label' => '1',
        'label_element' => 'h2',
        'effect' => 'none',
        'speed' => 'none',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_section_2|node|task|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_section_3|node|task|form';
  $field_group->group_name = 'group_section_3';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'task';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Section 3',
    'weight' => '8',
    'children' => array(),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Section 3',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $field_groups['group_section_3|node|task|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_section_3|node|task|full';
  $field_group->group_name = 'group_section_3';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'task';
  $field_group->mode = 'full';
  $field_group->parent_name = 'group_sections';
  $field_group->data = array(
    'label' => 'Section 3',
    'weight' => '3',
    'children' => array(),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Section 3',
      'instance_settings' => array(
        'classes' => 'group-section-3 task-section',
        'description' => '',
        'show_label' => '1',
        'label_element' => 'h2',
        'effect' => 'none',
        'speed' => 'none',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_section_3|node|task|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_sections|node|task|full';
  $field_group->group_name = 'group_sections';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'task';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Sections',
    'weight' => '2',
    'children' => array(
      0 => 'group_section_1',
      1 => 'group_section_2',
      2 => 'group_section_3',
    ),
    'format_type' => 'html-element',
    'format_settings' => array(
      'label' => 'Sections',
      'instance_settings' => array(
        'classes' => 'task-sections',
        'element' => 'div',
        'attributes' => '',
      ),
    ),
  );
  $field_groups['group_sections|node|task|full'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_task|node|task|form';
  $field_group->group_name = 'group_task';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'task';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Task',
    'weight' => '0',
    'children' => array(
      0 => 'group_basicinfo',
      1 => 'group_calltoaction',
      2 => 'group_content',
      3 => 'group_highlight',
      4 => 'group_meta_data',
      5 => 'group_search',
    ),
    'format_type' => 'htabs',
    'format_settings' => array(
      'formatter' => '',
      'instance_settings' => array(
        'classes' => 'group-task field-group-htabs',
      ),
    ),
  );
  $field_groups['group_task|node|task|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_webform|node|task|form';
  $field_group->group_name = 'group_webform';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'task';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_calltoaction';
  $field_group->data = array(
    'label' => 'Webform',
    'weight' => '-46',
    'children' => array(
      0 => 'field_btn_webform',
      1 => 'field_confirm_webform',
      2 => 'field_webform',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Webform',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-webform field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_webform|node|task|form'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Appointment');
  t('Basic info');
  t('Buttons');
  t('Call to action');
  t('Content');
  t('Highlight');
  t('Main section');
  t('Meta data');
  t('Related pages');
  t('Related pages section');
  t('Search and Follow');
  t('Section 1');
  t('Section 1');
  t('Section 2');
  t('Section 2');
  t('Section 3');
  t('Section 3');
  t('Sections');
  t('Task');
  t('Webform');
  return $field_groups;
}
