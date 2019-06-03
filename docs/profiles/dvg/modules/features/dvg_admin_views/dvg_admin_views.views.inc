<?php
/**
 * @file
 * dvg_admin_views.views.inc
 */

/**
 * Implements hook_views_data()
 */
function dvg_admin_views_views_data() {
  // Basic table information.
  $data['menu_node'] = array(
    'table' => array(
      'group' => t('Content'),
      'join' => array(
        'node' => array(
          'left_field' => 'nid',
          'field' => 'nid',
        ),
      ),
    )
  );

  // Our fields
  $data['menu_node']['nid'] = array(
    'title' => t('Nid'),
    'help' => t('The nid that has a relationship.'),
  );
  $data['menu_node']['menu_link_id'] = array(
    'title' => t('Menu Link ID'),
    'help' => t('The menu link that has a relationship.'),
  );

  // Adds our field in the "Fields" section of Views
  $data['menu_node']['nid']['field'] = array(
    'handler' => 'views_handler_field_numeric',
    'click sortable' => TRUE,
  );

  // Adds our field in the "Filters" section of Views
  $data['menu_node']['nid']['filter'] = array(
    'handler' => 'views_handler_filter_menu',
  );

  // Adds our field in the "Sort" section of Views
  $data['menu_node']['nid']['sort'] = array(
    'handler' => 'views_handler_sort',
  );

  return $data;
}
