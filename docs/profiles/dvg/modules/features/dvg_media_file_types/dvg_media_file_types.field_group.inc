<?php
/**
 * @file
 * dvg_media_file_types.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function dvg_media_file_types_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_copyrights|file|dvg_image|form';
  $field_group->group_name = 'group_copyrights';
  $field_group->entity_type = 'file';
  $field_group->bundle = 'dvg_image';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Copyrights',
    'weight' => '3',
    'children' => array(
      0 => 'field_file_author',
      1 => 'field_file_license',
      2 => 'field_file_url',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Copyrights',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-copyrights field-group-fieldset',
        'description' => 'All images need to be displayed with a legal copyright notice. There are different types of copyright licenses. Please ask your web administrator if there is a missing license type.',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_copyrights|file|dvg_image|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_info|file|dvg_image|form';
  $field_group->group_name = 'group_info';
  $field_group->entity_type = 'file';
  $field_group->bundle = 'dvg_image';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Information',
    'weight' => '2',
    'children' => array(
      0 => 'field_file_image_alt_text',
      1 => 'field_file_image_title_text',
      2 => 'filename',
      3 => 'preview',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Information',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-info field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_info|file|dvg_image|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_info|file|image|form';
  $field_group->group_name = 'group_info';
  $field_group->entity_type = 'file';
  $field_group->bundle = 'image';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Information',
    'weight' => '2',
    'children' => array(
      0 => 'field_file_image_alt_text',
      1 => 'field_file_image_title_text',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Information',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => 'group-info field-group-fieldset',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_info|file|image|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_media_element_container|file|dvg_image|full';
  $field_group->group_name = 'group_media_element_container';
  $field_group->entity_type = 'file';
  $field_group->bundle = 'dvg_image';
  $field_group->mode = 'full';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'media_element_container',
    'weight' => '1',
    'children' => array(
      0 => 'field_file_license',
      1 => 'field_file_author',
      2 => 'field_file_url',
      3 => 'file',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'media_element_container',
      'instance_settings' => array(
        'classes' => 'media-element-container',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'none',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_media_element_container|file|dvg_image|full'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Copyrights');
  t('Information');
  t('Information');
  t('media_element_container');
  return $field_groups;
}
