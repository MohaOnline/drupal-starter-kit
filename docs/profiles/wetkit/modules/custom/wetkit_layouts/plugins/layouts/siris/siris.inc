<?php

/**
 * @file
 * Implements core hooks for wetkit_layouts.
 */

/**
 * Implements hook_panels_layouts().
 */
function wetkit_layouts_siris_panels_layouts() {
  $items['siris'] = array(
    'title' => t('Siris'),
    'icon' => 'siris.png',
    'category' => t('Layouts BackEnd'),
    'theme' => 'siris',
    'css' => 'siris.css',
    'regions' => array(
      'header' => t('Header'),
      'banner' => t('Banner'),
      'column1' => t('First Column'),
      'column2' => t('Second Column'),
      'footer' => t('Footer'),
    ),
  );

  return $items;
}

/**
 * Preprocess variables for siris.tpl.php.
 */
function template_preprocess_siris(&$vars) {
  $container = theme_get_setting('bootstrap_fluid_container');
  $vars['container_class'] = !empty($container) ? 'container-fluid' : 'container';
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
