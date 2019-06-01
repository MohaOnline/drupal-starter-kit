<?php

/**
 * @file
 * Implements core hooks for wetkit_layouts.
 */

/**
 * Implements hook_panels_layouts().
 */
function wetkit_layouts_phelan_panels_layouts() {
  $items['phelan'] = array(
    'title' => t('Phelan'),
    'icon' => 'phelan.png',
    'category' => t('Layouts BackEnd'),
    'theme' => 'phelan',
    'css' => 'phelan.css',
    'regions' => array(
      'column1' => t('First Column'),
      'column2' => t('Second Column'),
    ),
  );

  return $items;
}

/**
 * Preprocess variables for phelan.tpl.php.
 */
function template_preprocess_phelan(&$vars) {
  $container = theme_get_setting('bootstrap_fluid_container');
  $vars['container_class'] = !empty($container) ? 'container-fluid' : 'container';
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
