<?php

/**
 * @file
 * Implements core hooks for wetkit_layouts.
 */

/**
 * Implements hook_panels_layouts().
 */
function wetkit_layouts_mccrae_panels_layouts() {
  $items['mccrae'] = array(
    'title' => t('McCrae'),
    'category' => t('Layouts'),
    'icon' => 'mccrae.png',
    'admin css' => 'mccrae.admin.css',
    'css' => 'mccrae.css',
    'theme' => 'mccrae',
    'theme arguments' => array('id', 'content'),
    'regions' => array(
      'sidebar' => t('Sidebar'),
      'contentheader' => t('Content Header'),
      'contentcolumn1' => t('Content Column 1'),
      'contentcolumn2' => t('Content Column 2'),
    ),
    'bodyid' => 'wb-body',
    'type' => 'one',
    'options' => array(),
    'styles' => array(),
  );

  return $items;
}

/**
 * Preprocess variables for mccrae.tpl.php.
 */
function template_preprocess_mccrae(&$vars) {
  $container = theme_get_setting('bootstrap_fluid_container');
  $vars['container_class'] = !empty($container) ? 'container-fluid' : 'container';
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
