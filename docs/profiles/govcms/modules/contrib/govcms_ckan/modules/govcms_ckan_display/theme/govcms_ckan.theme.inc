<?php
/**
 * @file
 * Theme functionality for govcms_ckan_display.
 */

/**
 * Implements hook_preprocess_ckan_empty_visualisation().
 */
function template_preprocess_ckan_empty_visualisation(&$variables) {
  // Set default empty from variable and fall back to default text.
  if (empty($variables['empty_text'])) {
    $default_text = t('Unable to load the data for this visualisation');
    $variables['empty_text'] = variable_get('govcms_ckan_display_empty_visualisation_text', $default_text);
  }
}

/**
 * Implements hook_preprocess_ckan_display_table_wrapper().
 *
 * Provides both wrappers and optional titles for rendered ckan tables.
 */
function template_preprocess_ckan_display_table_wrapper(&$variables) {
  // If tables aren't an array we abort. Possibly due to parsing error.
  if (!is_array($variables['tables'])) {
    return;
  }

  // It is assumed that the key for each table is the title.
  foreach ($variables['tables'] as $title => $table) {
    // Wrapper container.
    $wrapper = array(
      '#type' => 'container',
      '#attributes' => array('class' => array('ckan-display-table')),
    );

    // If we are showing the title for each table.
    if ($variables['show_titles']) {
      $wrapper['title'] = array(
        '#theme' => 'html_tag',
        '#tag' => 'h3',
        '#value' => filter_xss($title),
      );
    }

    // Add the table.
    $wrapper['table'] = $table;

    // Add to content.
    $variables['content'][] = $wrapper;
  }

  // Indicate if there is titles with a wrapper class.
  $variables['classes_array'][] = ($variables['show_titles'] ? 'with' : 'without') . '-titles';
}
