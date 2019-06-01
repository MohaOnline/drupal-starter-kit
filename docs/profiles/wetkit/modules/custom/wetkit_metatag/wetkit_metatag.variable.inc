<?php
/**
 * @file
 * wetkit_metatag.variable.inc
 */

/**
 * Implements hook_variable_info().
 */
function wetkit_metatag_variable_info($options) {

  $variables['wetkit_metatag_creator'] = array(
    'type' => 'text',
    'title' => t('WetKit Metag: Creator', array(), $options),
    'description' => t('This text is  displayed in the dcterms.creator metatag area.'),
    'default' => '',
    'localize' => TRUE,
    'group' => 'wetkit_metatag',
  );

  return $variables;
}

/**
 * Implements hook_variable_group_info().
 */
function wetkit_metatag_variable_group_info() {
  $groups['wetkit_metatag'] = array(
    'title' => t('WetKit Metatag'),
    'description' => t('WetKit Metatag settings for various metatags used throughout the site.'),
    'access' => 'administer site configuration',
  );

  return $groups;
}
