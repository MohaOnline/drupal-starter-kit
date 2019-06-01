<?php

/**
 * @file
 * Implements core hooks for wetkit_layouts.
 */

/**
 * Implements hook_panels_layouts().
 */
function wetkit_layouts_vanier_panels_layouts() {
  $items['vanier'] = array(
    'title' => t('Vanier'),
    'category' => t('Layouts'),
    'icon' => 'vanier.png',
    'theme' => 'vanier',
    'admin css' => 'vanier.admin.css',
    'css' => 'vanier.css',
    'theme arguments' => array('id', 'content'),
    'regions' => array(
      'vanier_top'    => t('Top (conditional)'),
      'vanier_content_banner'  => t('Content Banner'),
      'vanier_content_listings'  => t('Content Listings'),
      'vanier_content_search'  => t('Content Search'),
      'vanier_content_datasets'  => t('Content Datasets'),
      'vanier_section_1' => t('Section 1'),
      'vanier_section_2' => t('Section 2'),
      'vanier_section_3' => t('Section 3'),
      'vanier_section_4' => t('Section 4'),
      'vanier_spotlight_1' => t('Spotlight 1'),
      'vanier_spotlight_2' => t('Spotlight 2'),
      'vanier_spotlight_3' => t('Spotlight 3'),
      'vanier_spotlight_4' => t('Spotlight 4'),
      'vanier_spotlight_5' => t('Spotlight 5'),
      'vanier_spotlight_6' => t('Spotlight 6'),
      'vanier_bottom' => t('Bottom (conditional)'),
    ),
    'bodyid' => 'wb-body',
    'type' => 'one',
    'options' => array(),
    'styles' => array(),
  );

  return $items;
}

/**
 * Preprocess variables for vanier.tpl.php.
 */
function template_preprocess_vanier(&$vars) {
  $container = theme_get_setting('bootstrap_fluid_container');
  $vars['container_class'] = !empty($container) ? 'container-fluid' : 'container';
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
