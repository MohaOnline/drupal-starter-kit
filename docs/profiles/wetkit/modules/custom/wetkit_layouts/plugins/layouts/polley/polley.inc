<?php

/**
 * @file
 * Implements core hooks for wetkit_layouts.
 */

/**
 * Implements hook_panels_layouts().
 */
function wetkit_layouts_polley_panels_layouts() {
  $items['polley'] = array(
    'title' => t('Polley'),
    'category' => t('Layouts'),
    'icon' => 'polley.png',
    'theme' => 'polley',
    'admin css' => 'polley.admin.css',
    'css' => 'polley.css',
    'theme arguments' => array('id', 'content'),
    'regions' => array(
      'polley_top'    => t('Top (conditional)'),
      'polley_top_banner' => t('Top Banner'),
      'polley_content_listings'  => t('Content Listings'),
      'polley_spotlight'  => t('Spotlight'),
      'polley_bottom_banner' => t('Bottom Banner'),
      'polley_bottom' => t('Bottom (conditional)'),
    ),
    'bodyid' => 'wb-body',
    'type' => 'one',
    'options' => array(),
    'styles' => array(),
  );

  return $items;
}

/**
 * Preprocess variables for polley.tpl.php.
 */
function template_preprocess_polley(&$vars) {
  $container = theme_get_setting('bootstrap_fluid_container');
  $vars['container_class'] = !empty($container) ? 'container-fluid' : 'container';
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
