<?php

/**
 * @file
 * The Module Permissions module theme file.
 *
 * Module Permissions allows a subset of modules to be enabled/disabled.
 */

/**
 * Managed modules list theme.
 */
function theme_module_permissions_ui_managed_modules_list($variables) {
  $form = $variables['form'];

  // Individual table headers.
  $rows = array();
  // Iterate through all the modules, which are children of this fieldset.
  foreach (element_children($form) as $key) {
    // Stick it into $module for easier accessing.
    $module = $form[$key];
    $row = array();
    unset($module['granted']['#title']);
    $row[] = array(
      'class' => array('checkbox'),
      'data' => drupal_render($module['granted']),
    );
    $label = '<label';
    if (isset($module['modules']['#id'])) {
      $label .= ' for="' . $module['granted']['#id'] . '"';
    }
    $row[] = $label . '><strong>' . drupal_render($module['name']) . '</strong></label>';
    $row[] = array(
      'data' => drupal_render($module['status']),
      'class' => array('status'),
    );
    $description = drupal_render($module['description']);
    $row[] = array('data' => $description, 'class' => array('description'));
    $row[] = array(
      'data' => drupal_render($module['dependency']),
      'class' => array('dependency'),
    );
    $rows[] = $row;
  }

  return theme('table', array('header' => $form['#header'], 'rows' => $rows));
}
