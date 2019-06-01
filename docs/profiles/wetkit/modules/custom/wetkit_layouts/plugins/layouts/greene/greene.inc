<?php

/**
 * @file
 * Implements core hooks for wetkit_layouts.
 */

/**
 * Implements hook_panels_layouts().
 */
function wetkit_layouts_greene_panels_layouts() {
  $items['greene'] = array(
    'title' => t('Greene'),
    'category' => t('Layouts'),
    'icon' => 'greene.png',
    'theme' => 'greene',
    'admin css' => 'greene.admin.css',
    'css' => 'greene.css',
    'theme arguments' => array('id', 'content'),
    'regions' => array(
      'greene_top'    => t('Top (conditional)'),
      'greene_first'  => t('Left'),
      'greene_second' => t('Right'),
      'greene_bottom' => t('Bottom (conditional)'),
    ),
    'bodyid' => 'wb-body',
    'type' => 'two',
    'options' => array(),
    'styles' => array(),
  );

  return $items;
}

/**
 * Preprocess variables for greene.tpl.php.
 */
function template_preprocess_greene(&$vars) {
  $container = theme_get_setting('bootstrap_fluid_container');
  $vars['container_class'] = !empty($container) ? 'container-fluid' : 'container';
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
