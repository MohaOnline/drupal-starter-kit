<?php

/**
 * @file
 * Implements core hooks for wetkit_layouts.
 */

/**
 * Implements hook_panels_layouts().
 */
function wetkit_layouts_boxton_panels_layouts() {
  $items['boxton'] = array(
    'title' => t('Boxton'),
    'icon' => 'boxton.png',
    'category' => t('Layouts BackEnd'),
    'theme' => 'boxton',
    'regions' => array(
      'contentmain' => t('Content'),
    ),
  );

  return $items;
}

/**
 * Preprocess variables for boxton.tpl.php.
 */
function template_preprocess_boxton(&$vars) {
  $container = theme_get_setting('bootstrap_fluid_container');
  $vars['container_class'] = !empty($container) ? 'container-fluid' : 'container';
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
