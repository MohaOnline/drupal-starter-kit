<?php
/**
 * @file
 * wetkit_bean.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function wetkit_bean_field_group_info() {
  $field_groups = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_bean_highlight|bean|highlight|default';
  $field_group->group_name = 'group_bean_highlight';
  $field_group->entity_type = 'bean';
  $field_group->bundle = 'highlight';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Bean Highlight Group',
    'weight' => '1',
    'children' => array(
      0 => 'field_bean_highlight_description',
      1 => 'field_bean_highlight_image',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Bean Highlight Group',
      'instance_settings' => array(
        'classes' => 'module',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h2',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_bean_highlight|bean|highlight|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_bean_highlight|bean|wetkit_highlight|default';
  $field_group->group_name = 'group_bean_highlight';
  $field_group->entity_type = 'bean';
  $field_group->bundle = 'wetkit_highlight';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Group Bean Highlight',
    'weight' => '1',
    'children' => array(
      0 => 'field_bean_highlight_description',
      1 => 'field_bean_highlight_link_text',
      2 => 'field_bean_highlight_link',
      3 => 'field_bean_highlight_image',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Group Bean Highlight',
      'instance_settings' => array(
        'classes' => 'module',
        'description' => '',
        'show_label' => '0',
        'label_element' => 'h2',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $field_groups['group_bean_highlight|bean|wetkit_highlight|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_bean_slide_caption|field_collection_item|field_bean_slide_collection|default';
  $field_group->group_name = 'group_bean_slide_caption';
  $field_group->entity_type = 'field_collection_item';
  $field_group->bundle = 'field_bean_slide_collection';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Caption',
    'weight' => '1',
    'children' => array(
      0 => 'field_slide_link',
      1 => 'field_slide_teaser',
    ),
    'format_type' => 'html-element',
    'format_settings' => array(
      'label' => 'Caption',
      'instance_settings' => array(
        'classes' => '',
        'element' => 'figcaption',
        'attributes' => '',
        'required_fields' => 1,
      ),
      'formatter' => '',
    ),
  );
  $field_groups['group_bean_slide_caption|field_collection_item|field_bean_slide_collection|default'] = $field_group;

  // Translatables
  // Included for use with string extractors like potx.
  t('Bean Highlight Group');
  t('Caption');
  t('Group Bean Highlight');

  return $field_groups;
}
