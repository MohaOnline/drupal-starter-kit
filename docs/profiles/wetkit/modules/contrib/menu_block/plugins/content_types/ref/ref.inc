<?php

/**
 * @file
 * Provides ctools integration for Menu Block along with Block References types.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  // And this is just the administrative title.
  // All our callbacks are named according to the standard pattern.
  'title' => t('Block Reference'),
);

// Load the standard config form.
module_load_include('inc', 'menu_block', 'menu_block.admin');

/**
 * Return all field content types available.
 */
function menu_block_ref_content_type_content_types() {
  $types = array();

  $entities = entity_get_info();
  foreach ($entities as $entity_type => $entity_info) {
    if (!empty($entity_info['fieldable'])) {
      $category = t('Block Reference');
      $context = new ctools_context_required(t(ucfirst($entity_type)), $entity_type);
      $types[$entity_type . ':blockreference'] = array(
        'category' => $category,
        'title' => t('Block Reference for @ref', array('@ref' => $entity_type)),
        'description' => 'CTools Plugin which renders Menu Block based on Block Reference field.',
        'required context' => $context,
        'defaults' => menu_block_get_config(),
        'menu_title' => t('Block Reference for @ref', array('@ref' => $entity_type)),
      );
    }
  }

  return $types;
}

/**
 * Render the custom content type.
 */
function menu_block_ref_content_type_render($subtype, $conf, $panel_args, $context) {

  if (empty($context) || empty($context->data)) {
    return FALSE;
  }

  $entity = $context->data;
  $entity_type = $context->type[2];
  $ref_type = '';
  $menu_name = '';

  $fields_info = field_info_instances($entity_type, $entity->type);
  foreach ($fields_info as $field_name => $value) {
    $field_info = field_info_field($field_name);
    $type = $field_info['type'];
    if ($type == 'blockreference') {
      $moddelta = field_get_items($entity_type, $entity, $field_info['field_name']);
      if ($moddelta) {
        list($ref_type, $menu_name) = explode(':',$moddelta[0]['moddelta']);
      }
    }
  }

  if ($ref_type == 'menu') {
    $conf['menu_name'] = $menu_name;
  }
  else {
    return FALSE;
  }

  // Ensure the delta is unique.
  $ids = &drupal_static(__FUNCTION__, array());
  if (empty($ids[$conf['menu_name']])) {
    $ids[$conf['menu_name']] = 0;
  }
  $delta = ++$ids[$conf['menu_name']];
  $conf['delta'] = 'ctools-' . $conf['menu_name'] . '-' . $delta;

  // Build the content type block.
  $tree = menu_tree_build($conf);

  $block              = new stdClass();
  $block->module      = 'menu_block';

  // Add contextual links.
  menu_block_block_view_alter($tree, $block);
  $block->subtype     = isset($tree['content']['#config']['menu_name']) ? $tree['content']['#config']['menu_name'] : $conf['menu_name'];
  $block->title       = $tree['subject'];
  $block->title_array = $tree['subject_array'];
  $block->content     = $tree['content'];

  return $block;
}

/**
* Returns an edit form for custom type settings.
*/
function menu_block_ref_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  // Create a pseudo form state.
  $sub_form_state = array('values' => $conf);
  $form += menu_block_configure_form($form, $sub_form_state);

  // Hide the menu selector.
  $form['menu_name']['#type'] = 'hidden';

  // Hide the Parent item option for the special "active" menu.
  $form['parent']['#type'] = 'hidden';

  // Remove CSS class hooks for jQuery script on parent select.
  unset($form['parent']['#attributes']);

  // Only show advanced section.
  unset($form['display_options']);

  return $form;
}

/**
 * Submit callback for content type editing form.
 */
function menu_block_ref_content_type_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['subtype']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
  if (!empty($form_state['values']['parent'])) {
    list($form_state['conf']['menu_name'], $form_state['conf']['parent_mlid']) = explode(':', $form_state['values']['parent']);
  }
}

/**
* Returns the administrative title for a type.
*/
function menu_block_ref_content_type_admin_title($subtype, $conf, $context) {
  if (!empty($conf['admin_title'])) {
    $output = filter_xss_admin($conf['admin_title']);
  }
  else {
    $output = t('Block Reference');
  }
  return $output;
}

/**
 * Callback to provide administrative info (the preview in panels when building a panel).
 */
function menu_block_ref_content_type_admin_info($subtype, $conf, $context = NULL) {
  $output = new stdClass();
  $output->title = t('Block Reference');
  $output->content = t('Renders Menu Block based on a Block Reference field.');

  return $output;
}
