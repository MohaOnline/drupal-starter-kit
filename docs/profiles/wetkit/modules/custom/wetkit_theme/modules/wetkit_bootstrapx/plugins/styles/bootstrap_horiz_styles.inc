<?php

$plugin = array(
  'title' => t('Tabbed Interface'),
  'description' => t('The title of every pane in this region will become a horizontal tab. Uses Bootstrap 3'),
  'render region' => 'wetkit_bootstrapx_bootstrap_horiz_styles_render_region',
  'settings form' => 'wetkit_bootstrapx_bootstrap_horiz_styles_settings_form',
  'weight' => -50,
);

/**
 * Render callback
 */
function theme_wetkit_bootstrapx_bootstrap_horiz_styles_render_region($vars) {

  // Include some basic information.
  $display = $vars['display'];
  $panes = $vars['panes'];
  $did = $vars['display']->did;
  $content = array();
  $output = '';

  // Title handling.
   if (isset($vars['settings']['title']) && !empty($vars['settings']['title'])) {
    $header['region_header'] = array(
      '#prefix' => '<h2>',
      '#suffix' => '</h2>',
      '#markup' => $vars['settings']['title'],
    );
  }
  $title = drupal_render($header);
  $output .= isset($title) ? $title : '';

  // Start the Region Wrapper.
  $output .= '<div class="bootstrap-panels-horizontal-tabs wb-tabs margin-top-large">';

  // Start Panes output.
  $output .= '<div class="tabpanels">';
  $i = 0;

  foreach ($panes as $pane_id => $pane) {
    $pane_title = drupal_static($pane_id . '_title');
    if (empty($pane_title)) {
      $pane_title = t('This pane needs a title.');
    }
    $content[$pane_title] = array('#markup' => '<details id="details-pane-' . $pane_id . '" class="tab-pane">' . '<summary>' . $pane_title . '</summary>' . $pane . '</details>');
  }

  if (isset($vars['settings']['sort']) && !empty($vars['settings']['sort'])) {
    ksort($content);
  }
  $output .= drupal_render($content);

  // End Panes output.
  $output .= '</div>';

  // End Region Wrapper.
  $output .= '</div>';

  return $output;
}

/**
 * Settings form
 */
function wetkit_bootstrapx_bootstrap_horiz_styles_settings_form($style_settings) {

  // Region Title
  $settings_default_value = isset($style_settings['title']) ? $style_settings['title'] : '';
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $settings_default_value,
    '#description' => t('The title is filtered by filter_xss, so some simple HTML is allowed.'),
  );
  $settings_sort_value = isset($style_settings['sort']) ? $style_settings['sort'] : TRUE;
  $form['sort'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sort by title'),
    '#default_value' => $settings_sort_value,
    '#description' => t('Whether the panes should be sorted by title'),
  );
  $form['help_text'] = array(
    '#markup' => t('The title of each pane will be become a tab which will toggle display of that pane.
        Ensure that you give every pane a title, otherwise you will see a message reminding you to do so.'),
  );

  return $form;
}
