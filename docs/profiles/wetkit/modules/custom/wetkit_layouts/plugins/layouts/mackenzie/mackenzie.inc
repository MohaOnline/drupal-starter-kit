<?php

/**
 * @file
 * Implements core hooks for wetkit_layouts.
 */

/**
 * Implements hook_panels_layouts().
 */
function wetkit_layouts_mackenzie_panels_layouts() {
  $items['mackenzie'] = array(
    'title'    => t('Mackenzie'),
    'category' => t('Layouts'),
    'icon'     => 'mackenzie.png',
    'theme'    => 'mackenzie',
    'admin css' => 'mackenzie.admin.css',
    'css' => 'mackenzie.css',
    'theme arguments' => array('id', 'content'),
    'regions' => array(
      'mackenzie_header'  => t('Header'),
      'mackenzie_banner_top'  => t('Banner'),
      'mackenzie_sidebar_first'  => t('Sidebar'),
      'mackenzie_sidebar_second'  => t('Sidebar'),
      'mackenzie_sidebar_third'  => t('Sidebar'),
      'mackenzie_section_first'  => t('Section'),
      'mackenzie_section_second' => t('Section'),
      'mackenzie_section_third'  => t('Section'),
      'mackenzie_section_fourth' => t('Section'),
      'mackenzie_section_fifth' => t('Section'),
      'mackenzie_section_sixth' => t('Section'),
      'mackenzie_banner_bottom'  => t('Banner'),
      'mackenzie_footer'  => t('Footer'),
    ),
    'bodyid' => 'wb-body',
    'type' => 'three',
    'options' => array(),
    'styles' => array(),
  );

  return $items;
}

/**
 * Preprocess variables for payette.tpl.php.
 */
function template_preprocess_mackenzie(&$vars) {
  $container = theme_get_setting('bootstrap_fluid_container');
  $vars['container_class'] = !empty($container) ? 'container-fluid' : 'container';
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
