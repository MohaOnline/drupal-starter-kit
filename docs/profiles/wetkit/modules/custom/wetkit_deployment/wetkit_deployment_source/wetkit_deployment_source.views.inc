<?php
/**
 * @file
 * wetkit_deployment_source.views.inc
 */

/**
 * Implements hook_views_data_alter().
 */
function wetkit_deployment_source_views_data_alter(&$data) {
  $data['bean']['changed']['field']['handler'] = 'views_handler_field_date';
  $data['bean']['changed']['filter']['handler'] = 'views_handler_filter_date';
  return $data;
}

/*
 * Implements hook_views_pre_view().
 */
function wetkit_deployment_source_views_pre_view($view, &$display_id, &$args) {
  // Perform logic specific deploy views.
  $views = array(
    'deployment_administration_files',
    'deployment_administration_field_collection',
    'deployment_administration_menu_links',
    'deployment_administration_taxonomy',
    'deployment_administration_users',
  );
  //shared_content_source_entity_is_shared($entity_type, $entity)
  if (in_array($view->name, $views)) {
    if ($display_id == 'panel_pane_1' ) {
      if (module_exists('shared_content_source') && !variable_get('shared_content_source_deployment_disabled', FALSE)) {
        $content = '<section class="alert alert-info mrgn-tp-md">';
        $content .= '<h3>' . t('Shared Content is enabled') . '</h3>';
        $content .= '<p>' .  t('With Shared Content enabled this entity will auto deploy for non-batch deployments.') . '</p>';
        $content .= '</section>';
        $view->add_item('panel_pane_1', 'header', 'views', 'area', array('content' => $content, 'format' => 'wetkit_wysiwyg_text'));
      }
    }
  }
}

/**
 * Implements hook_views_query_alter().
 */
function wetkit_deployment_source_views_query_alter(&$view, &$query) {
  if ($view->name == 'deployment_administration_menu_links') {
    $query->where[] = array(
      'conditions' => array(array(
        'field' => 'menu_links.language',
        'value' => array('***CURRENT_LANGUAGE***'),
        'operator' => 'in',
      )),
      'args' => array(),
      'type' => 'AND',
    );
  }
}
