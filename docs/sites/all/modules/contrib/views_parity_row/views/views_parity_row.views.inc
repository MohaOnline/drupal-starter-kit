<?php

/**
 * Implements hook_views_plugins().
 */
function views_parity_row_views_plugins() {
  $plugins = array('row' => array());

  $plugins['row']['views_parity_row'] = array(
    'title' => t('Content (alternate)'),
    'help' => t('Display the content with standard node view and alternate view mode according to the row index.'),
    'handler' => 'views_parity_row_views_plugin_row_node_view',
    'path' => drupal_get_path('module', 'views_parity_row') . '/views',
    'base' => array('node'),
    'uses options' => TRUE,
    'uses fields' => FALSE,
    'type' => 'normal',
    'help topic' => 'style-node',
  );

  if (module_exists('entity')) {
    $entity_views_plugins = entity_views_plugins();

    $entity_views_plugins['row']['entity']['title'] .= t(' (alternate)');
    $entity_views_plugins['row']['entity']['help'] = t('Display the entity with standard entity view and alternate view mode according to the row index.');
    $entity_views_plugins['row']['entity']['handler'] = 'views_parity_row_views_plugin_row_entity_view';
    $entity_views_plugins['row']['entity']['path'] = drupal_get_path('module', 'views_parity_row') . '/views';

    $plugins['row']['views_parity_row_entity'] = $entity_views_plugins['row']['entity'];
  }

  return $plugins;
}

