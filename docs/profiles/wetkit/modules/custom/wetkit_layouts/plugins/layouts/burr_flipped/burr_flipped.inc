<?php

/**
 * @file
 * Implements core hooks for wetkit_layouts.
 */

/**
 * Implements hook_panels_layouts().
 */
function wetkit_layouts_burr_flipped_panels_layouts() {
  $items['burr_flipped'] = array(
    'title' => t('Burr Flipped'),
    'icon' => 'burr-flipped.png',
    'category' => t('Layouts BackEnd'),
    'theme' => 'burr_flipped',
    'css' => 'burr-flipped.css',
    'regions' => array(
      'sidebar' => t('Sidebar'),
      'contentmain' => t('Content'),
    ),
  );

  return $items;
}

/**
 * Preprocess variables for burr-flipped.tpl.php.
 */
function template_preprocess_burr_flipped(&$vars) {
  $container = theme_get_setting('bootstrap_fluid_container');
  $vars['container_class'] = !empty($container) ? 'container-fluid' : 'container';
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
