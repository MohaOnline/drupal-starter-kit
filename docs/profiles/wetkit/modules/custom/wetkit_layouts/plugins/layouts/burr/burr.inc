<?php

/**
 * @file
 * Implements core hooks for wetkit_layouts.
 */

/**
 * Implements hook_panels_layouts().
 */
function wetkit_layouts_burr_panels_layouts() {
  $items['burr'] = array(
    'title' => t('Burr'),
    'icon' => 'burr.png',
    'category' => t('Layouts BackEnd'),
    'theme' => 'burr',
    'css' => 'burr.css',
    'regions' => array(
      'sidebar' => t('Sidebar'),
      'contentmain' => t('Content'),
    ),
  );

  return $items;
}

/**
 * Preprocess variables for burr.tpl.php.
 */
function template_preprocess_burr(&$vars) {
  $container = theme_get_setting('bootstrap_fluid_container');
  $vars['container_class'] = !empty($container) ? 'container-fluid' : 'container';
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
