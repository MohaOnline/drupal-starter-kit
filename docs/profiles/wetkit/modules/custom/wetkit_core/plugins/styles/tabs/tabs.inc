<?php
/**
 * @file
 * Definition of the 'Tabs' panel style.
 */

// Plugin definition.
$plugin = array(
  'title' => t('jQuery Tabs'),
  'description' => t('Show panel panes in a region as tabs.'),
  'render region' => 'wetkit_core_tabs_style_render_region',
  'settings form' => 'wetkit_core_tabs_region_settings_form',
);

/**
 * Render callback.
 *
 * @ingroup themeable
 */
function theme_wetkit_core_tabs_style_render_region($vars) {
  $display = $vars['display'];
  $region_id = $vars['region_id'];
  $owner_id = $vars['owner_id'];
  $panes = $vars['panes'];

  $tab_id = 'tabs-' . $owner_id . '-' . $region_id;

  $element = array(
    '#prefix' => '<div id="' . $tab_id . '">',
    '#suffix' => '</div>',
    '#attached' => array(
      'library' => array(array('system', 'ui.tabs')),
      'js' => array(
        drupal_get_path('module', 'wetkit_core') . '/js/panels_tabs.js' => array('type' => 'file'),
      ),
    ),
  );

  if (isset($vars['settings']['title']) && !empty($vars['settings']['title'])) {
    $element['region_title'] = array(
      '#prefix' => '<h2>',
      '#suffix' => '</h2>',
      '#markup' => $vars['settings']['title'],
    );
  }

  $settings = array();
  $settings['panelsTabs']['tabsID'][] = $tab_id;

  $element['#attached']['js'][] = array(
    'type' => 'setting',
    'data' => $settings,
  );

  // Get the pane titles.
  $items = array();
  $delta = 1;
  foreach ($display->panels[$region_id] as $pane_id) {
    // Make sure the pane exists.
    if (!empty($panes[$pane_id])) {
      $title = wetkit_core_pane_titles($display->did, $pane_id);
      $title = $title ? $title : t('Tab @delta', array('@delta' => $delta));
      if (isset($display->content[$pane_id]->configuration['override_title']) && $display->content[$pane_id]->configuration['override_title']) {
        $title = ($display->content[$pane_id]->configuration['override_title_text'] != '') ? $display->content[$pane_id]->configuration['override_title_text'] : $title;
      }
      $items[] = '<a href="#' . $tab_id . '-' . $delta . '">' . $title . '</a>';
      ++$delta;
    }
  }

  $element['tabs_title'] = array('#theme' => 'item_list', '#items' => $items);

  $items = array();
  $delta = 1;
  foreach ($panes as $pane_id => $item) {
    $element['tabs_content'][$pane_id] = array(
      '#prefix' => '<div id="' . $tab_id . '-' . $delta . '">',
      '#suffix' => '</div>',
      '#markup' => $item,
    );
    ++$delta;
  }

  return drupal_render($element);
}

/**
 * Region settings form callback.
 */
function wetkit_core_tabs_region_settings_form($style_settings) {
  $settings_default_value = isset($style_settings['title']) ? $style_settings['title'] : '';
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $settings_default_value,
    '#description' => t('The title is filtered by filter_xss, so some simple HTML is allowed.'),
  );
  return $form;
}
