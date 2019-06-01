<?php

/**
 * @file
 * Implements core hooks for wetkit_layouts.
 */

/**
 * Implements hook_panels_layouts().
 */
function wetkit_layouts_sutro_panels_layouts() {
  $items['sutro'] = array(
    'title' => t('Sutro'),
    'icon' => 'sutro.png',
    'category' => t('Layouts BackEnd'),
    'theme' => 'sutro',
    'css' => 'sutro.css',
    'regions' => array(
      'header' => t('Header'),
      'column1' => t('First Column'),
      'column2' => t('Second Column'),
      'footer' => t('Footer'),
    ),
  );

  return $items;
}

/**
 * Preprocess variables for sutro.tpl.php.
 */
function template_preprocess_sutro(&$vars) {
  $container = theme_get_setting('bootstrap_fluid_container');
  $vars['container_class'] = !empty($container) ? 'container-fluid' : 'container';
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
