<?php

/**
 * Implements hook_entity_info_alter().
 */
function dvg_global_entity_info_alter(&$entity_info) {
  $entity_info['node']['view modes']['frontpage'] = array(
    'label' => t('Frontpage'),
    'custom settings' => FALSE,
  );

  $entity_info['node']['view modes']['search_results'] = array(
    'label' => t('Search Results'),
    'custom settings' => FALSE,
  );

  // Make Voc translatable on Admin pages
  if (isset($entity_info['taxonomy_vocabulary'])) {
    $entity_info['taxonomy_vocabulary']['label callback'] = '_dvg_global_taxonomy_vocabulary_label';
  }
}

/**
 * Implements hook_node_view().
 */
function dvg_global_node_view($node, $view_mode, $langcode) {
  if ((!empty($node->field_title_related_pages) || !empty($node->field_related_pages)) && !empty($node->content['#groups']['group_main_section'])) {
    // Check if the related-pages-enabled class has already been added.
    $section_classes = $node->content['#groups']['group_main_section']->format_settings['instance_settings']['classes'];
    $class = strpos($section_classes, 'related-pages-enabled') ? '' : ' related-pages-enabled';
    $node->content['#groups']['group_main_section']->format_settings['instance_settings']['classes'] .= $class;
  }

  // Warn viewer about unpublished content.
  if ($view_mode == 'full' && $node->status == NODE_NOT_PUBLISHED) {
    drupal_set_message(t('%title is NOT published.', array('%title' => $node->title)), 'warning');
  }
}

/**
 * Implements hook_preprocess_node().
 */
function dvg_global_preprocess_node(&$variables) {
  if (!empty($variables['content']['field_title_related_pages'][0])) {
    $variables['content']['field_title_related_pages'][0] += array(
      '#prefix' => '<h2 class="title">',
      '#suffix' => '</h2>',
    );
  }
}

/**
 * Implements hook_node_access().
 */
function dvg_global_node_access($node, $op, $account) {
  if ($op == 'delete') {
    // Disable delete option for non-root users for specified error pages.
    $site_403 = variable_get('site_403', NULL);
    $site_404 = variable_get('site_404', NULL);

    $path_403 = drupal_get_normal_path($site_403);
    $path_404 = drupal_get_normal_path($site_404);

    $node_path = 'node/' . $node->nid;

    if ($node_path == $path_403 || $node_path == $path_404) {
      return NODE_ACCESS_DENY;
    }
  }

  return NODE_ACCESS_IGNORE;
}

/**
 * Implements hook_node_presave().
 */
function dvg_global_node_presave($node) {
  $node->path['pathauto'] = TRUE;
}
