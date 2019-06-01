<?php

/**
 * @file
 * Implements core hooks for wetkit_layouts.
 */

/**
 * Implements hook_panels_layouts().
 */
function wetkit_layouts_pearson_panels_layouts() {
  $items['pearson'] = array(
    'title' => t('Pearson'),
    'category' => t('Layouts Sidebar'),
    'icon' => 'pearson.png',
    'theme' => 'pearson',
    'admin css' => 'pearson.admin.css',
    'css' => 'pearson.css',
    'theme arguments' => array('id', 'content'),
    'regions' => array(
      'pearson_top'    => t('Top (conditional)'),
      'pearson_first'  => t('Content'),
      'pearson_second' => t('Sidebar'),
      'pearson_third' => t('Sidebar'),
      'pearson_bottom' => t('Bottom (conditional)'),
    ),
    'bodyid' => 'wb-body-sec',
    'type' => 'two',
    'main' => TRUE,
    'options' => array(),
    'styles' => array(),
  );

  return $items;
}

/**
 * Preprocess variables for pearson.tpl.php.
 */
function template_preprocess_pearson(&$vars) {
  $container = theme_get_setting('bootstrap_fluid_container');
  $wxt_active = variable_get('wetkit_wetboew_theme', 'theme-wet-boew');
  $vars['container_class'] = !empty($container) ? 'container-fluid' : 'container';
  $vars['sidebar_class'] = ($wxt_active != 'theme-gcweb' && $wxt_active != 'theme-gcweb-v5') ? 'visible-md visible-lg' : '';
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
