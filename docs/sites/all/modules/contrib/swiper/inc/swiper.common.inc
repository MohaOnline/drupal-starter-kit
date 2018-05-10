<?php
/**
 * @file
 * Common functions for swiper module
 */

/**
 * Returns the related path where resides the given template.
 *
 * @param string $template_name
 *   The name of the template, like 'block--swiper'.
 *
 * @return string
 *   Returns the current template path in case of the file resides in the
 *   theme folder, or returns the swiper module theme path.
 */
function swiper_get_template_folder_path($template_name) {
  // By default, uses the template inside in the module folder.
  $path = drupal_get_path('module', 'swiper') . '/theme';

  // Uses the template that resides in theme folder, if exist.
  $theme_folder_files = drupal_system_listing('/\.tpl\.php$/', path_to_theme(), 'name', 0);
  $template = $template_name . '.tpl';
  if (isset($theme_folder_files[$template])) {
    $path = str_replace('/' . $template . '.php', '', $theme_folder_files[$template]->uri);
  }

  return $path;
}

/**
 * Returns the nodes of a given content type.
 *
 * @param string $content_type
 *   The content type name
 *
 * @return object
 *   Returns a object containing all the nodes of the given content type.
 */
function swiper_get_nodes_of_content_type($content_type = SWIPER_CONTENT_TYPE_NAME) {
  return node_load_multiple(array(), array('type' => $content_type));
}

/**
 * Returns the saved swiper contents from a node object of swiper type.
 *
 * @param object $node
 *   Drupal node object format
 *
 * @return array
 *   An array of the related swiper content
 */
function swiper_get_swiper_contents_from_node($node) {
  $items = field_get_items('node', $node, SWIPER_FIELD_CONTENT);

  $swiper_contents = array();
  foreach ($items as $delta => $item_value) {
    $field_value = field_view_value('node', $node, SWIPER_FIELD_CONTENT, $items[$delta]);
    $swiper_contents[] = $field_value['#markup'];
  }

  return $swiper_contents;
}

/**
 * Adds the required plugin scripts.
 *
 * @param object $node
 *   Drupal node object format
 */
function swiper_add_swiper_scripts($node) {
  $module_path = drupal_get_path('module', 'swiper');

  // Adds core Swiper plugin file.
  $latest_plugin = swiper_get_plugin_file('\.min\.js$');
  drupal_add_js($latest_plugin->uri, 'file');

  // Adds the plugin options.
  swiper_add_js_plugin_options($node);

  // Adds Swiper implementation file.
  drupal_add_js($module_path . SWIPER_ASSETS_SCRIPTS_PATH . SWIPER_JS_IMPLEMENTATION_FILE, 'file');
}

/**
 * Returns the details of a swiper plugin file by a given file name pattern.
 *
 * @param string $pattern
 *   A given file pattern name to check against the plugin repository.
 *
 * @return
 *   An object containing the file details, NULL otherwise
 */
function swiper_get_plugin_file($pattern) {
  $library_path = swiper_get_plugin_library_path();

  $plugin_files = drupal_system_listing("/$pattern/", $library_path . '/' . SWIPER_LIBRARY_DISTRIBUTION_FILES_FOLDER_NAME, 'name', 0);

  return end($plugin_files);
}

/**
 * Adds the required plugin styles.
 *
 * @param object $node
 *   Drupal node object format
 */
function swiper_add_swiper_styles($node) {
  $module_path = drupal_get_path('module', 'swiper');

  // Adds inline dimensios styles.
  swiper_add_swiper_container_dimensions_styles($node);

  // Adds core plugin styles.
  $plugin_css_file = swiper_get_plugin_file('\.css$');
  drupal_add_css($plugin_css_file->uri, 'file');

  // Adds module custom styles.
  drupal_add_css($module_path . SWIPER_ASSETS_STYLES_PATH . SWIPER_CSS_PLUGIN_FILE, 'file');
}

/**
 * Defines a inline CSS style to the default swiper container element.
 *
 * @param object $node
 *   Drupal node object format
 */
function swiper_add_swiper_container_dimensions_styles($node) {
  $container_dimensions = swiper_get_container_dimensions_by_node($node);
  if (!in_array(NULL, $container_dimensions)) {
    // Set slider and container dimension style inline.
    drupal_add_css(
      '.swiper-container-nid-' . $node->nid . ',
       .swiper-slide-nid-' . $node->nid . ',
       .pagination-nid-' . $node->nid . ' {
         max-width:  ' . $container_dimensions['width'] . ' !important;
       }',
      array('type' => 'inline')
    );
  }
}

/**
 * Returns the Swiper container dimensions.
 *
 * @param object $node
 *   Drupal node object format
 *
 * @return array
 *   Returns an array containing the dimensions of a swiper node content
 */
function swiper_get_container_dimensions_by_node($node) {
  $dimensions = array(
    'width' => swiper_get_field_value($node, SWIPER_FIELD_CONTAINER_WIDTH),
  );

  return $dimensions;
}

/**
 * Returns the value of a given field in a node.
 *
 * @param object $node
 *   Drupal node object format
 *
 * @param string $field_name
 *   The name of the related field
 *
 * @param bool $safe_value
 *   If you want to get the 'safe' value set TRUE
 *
 * @return String
 *   Return a string value in case with the field have a value, NULL otherwise
 */
function swiper_get_field_value($node, $field_name, $safe_value = FALSE) {
  $value = NULL;

  $field_item = field_get_items('node', $node, $field_name);
  if ($field_item) {
    $item = reset($field_item);
    $value = $safe_value ? $item['safe_value'] : $item['value'];
  }

  return $value;
}

/**
 * Returns an array with the default plugin options.
 *
 * @return array
 *   An array containing the plugin implementation options values
 */
function swiper_get_default_plugin_options($node) {
  $default_options = array(
    'pagination' => '.pagination-nid-' . $node->nid,
    'loop' => TRUE,
  );

  return $default_options;
}

/**
 * Implements plugin behavior in the page.
 *
 * @param object $node
 *   Drupal node object format
 */
function swiper_implements_plugin_behavior($node) {
  if (swiper_get_plugin_library_path()) {
    swiper_add_swiper_scripts($node);
    swiper_add_swiper_styles($node);
  }
}

/**
 * Adds a JS with the plugin options.
 *
 * @param object $node
 *   Drupal node object format
 */
function swiper_add_js_plugin_options($node) {
  $plugin_options = swiper_get_default_plugin_options($node);
  drupal_alter('swiper_options', $node, $plugin_options);

  drupal_add_js(array(
    'swiper' => array(
      'nid-' . $node->nid => array('options' => $plugin_options),
    ),
  ), array('type' => 'setting', 'scope' => JS_DEFAULT));
}

/**
 * Returns Plugin library path
 *
 * @return
 *   The path to the specified library or FALSE if the library wasn't found.
 */
function swiper_get_plugin_library_path() {
  return libraries_get_path(SWIPER_LIBRARY_FOLDER_NAME);
}

/**
 * Form constructor for new node form.
 */
function swiper_node_form_submit($form, &$form_state) {
  if (!empty($form['#new_node'])) {
    // Clear all error messages
    drupal_get_messages('status');

    // Set a message to user to inform that a block was created.
    drupal_set_message(t('A new block was created: %block_title%. Use this block to position your swiper gallery in the pages that you want.', array('%block_title%' => 'Swiper: ' . $form_state['values']['title'])));
  }
}

/**
 * Sets a warning message to the user in the forms,
 * in case that not have the library installed.
 */
function swiper_set_no_library_found_warning_message() {
  $repository_link = l(t('idangero.us repository'), SWIPER_PLUGIN_REPOSITORY_URL, array(
    'attributes' => array('target' => '_blank'),
  ));

  $message = <<<MSG
Swiper module is not configured correctly and your gallery will not work. <br/>
You need to add the plugin library folder in "%library_path%". Download the plugin library in !download_link. <br/>
Note that the plugin library folder must have the name "%folder_name%" (case sensitive).
MSG;

  $t_args = array(
    '%library_path%' => 'sites/all/libraries',
    '%folder_name%' => SWIPER_LIBRARY_FOLDER_NAME,
    '!download_link' => $repository_link,
  );


  drupal_set_message(t($message, $t_args), 'warning');
}

/**
 * Deletes a Drupal block created by the module.
 *
 * @param string $block_delta
 *   Block delta.
 */
function swiper_delete_block($block_delta) {
  db_delete('block')
  ->condition('delta', $block_delta)
  ->execute();
}
