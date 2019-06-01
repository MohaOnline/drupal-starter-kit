<?php

/**
 * @file
 * Implements core hooks for wetkit_layouts.
 */

/**
 * Implements hook_panels_layouts().
 */
function wetkit_layouts_dallaire_panels_layouts() {
  $items['dallaire'] = array(
    'title'    => t('Dallaire'),
    'category' => t('Layouts Sidebar'),
    'icon'     => 'dallaire.png',
    'theme'    => 'dallaire',
    'admin css' => 'dallaire.admin.css',
    'css' => 'dallaire.css',
    'theme arguments' => array('id', 'content'),
    'regions' => array(
      'dallaire_top'    => t('Top (conditional)'),
      'dallaire_first'  => t('Content 1'),
      'dallaire_second' => t('Content 2'),
      'dallaire_third'  => t('Sidebar'),
      'dallaire_bottom' => t('Bottom (conditional)'),
    ),
    'bodyid' => 'wb-body-sec',
    'type' => 'three',
    'main' => TRUE,
    'options' => array(),
    'styles' => array(),
  );

  return $items;
}

/**
 * Preprocess variables for dallaire.tpl.php.
 */
function template_preprocess_dallaire(&$vars) {
  $container = theme_get_setting('bootstrap_fluid_container');
  $vars['container_class'] = !empty($container) ? 'container-fluid' : 'container';
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
