<?php

/**
 * @file
 * Implements core hooks for wetkit_layouts.
 */

/**
 * Implements hook_panels_layouts().
 */
function wetkit_layouts_moscone_panels_layouts() {
  $items['moscone'] = array(
    'title' => t('Moscone'),
    'icon' => 'moscone.png',
    'category' => t('Layouts BackEnd'),
    'theme' => 'moscone',
    'css' => 'moscone.css',
    'regions' => array(
      'header' => t('Header'),
      'sidebar' => t('Content Sidebar'),
      'contentmain' => t('Content'),
      'footer' => t('Footer'),
    ),
  );

  return $items;
}

/**
 * Preprocess variables for moscone.tpl.php.
 */
function template_preprocess_moscone(&$vars) {
  $container = theme_get_setting('bootstrap_fluid_container');
  $vars['container_class'] = !empty($container) ? 'container-fluid' : 'container';
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
