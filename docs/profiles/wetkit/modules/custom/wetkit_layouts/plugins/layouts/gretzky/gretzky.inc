<?php

/**
 * @file
 * Implements core hooks for wetkit_layouts.
 */

/**
 * Implements hook_panels_layouts().
 */
function wetkit_layouts_gretzky_panels_layouts() {
  $items['gretzky'] = array(
    'title' => t('Gretzky'),
    'category' => t('Layouts'),
    'icon' => 'gretzky.png',
    'theme' => 'gretzky',
    'admin css' => 'gretzky.admin.css',
    'css' => 'gretzky.css',
    'theme arguments' => array('id', 'content'),
    'regions' => array(
      'gretzky_top'    => t('Top (conditional)'),
      'gretzky_first'  => t('Left'),
      'gretzky_second' => t('Right'),
      'gretzky_bottom' => t('Bottom (conditional)'),
    ),
    'bodyid' => 'wb-body',
    'type' => 'two',
    'options' => array(),
    'styles' => array(),
  );

  return $items;
}

/**
 * Preprocess variables for gretzky.tpl.php.
 */
function template_preprocess_gretzky(&$vars) {
  $container = theme_get_setting('bootstrap_fluid_container');
  $vars['container_class'] = !empty($container) ? 'container-fluid' : 'container';
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
