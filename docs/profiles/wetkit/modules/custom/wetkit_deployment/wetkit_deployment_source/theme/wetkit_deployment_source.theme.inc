<?php
/**
 * @file
 * wetkit_deployment_source.theme.inc
 */

/**
 * Theme callback for the overview page.
 */
function theme_wetkit_deploy_ui_overview($variables) {
  drupal_add_css(drupal_get_path('module', 'deploy_ui') . '/css/deploy_ui.admin.css');

  $blocks = $variables['blocks'];
  $container = array();

  $i = 0;
  foreach ($blocks as $block) {
    $position = ++$i % 2 ? 'left' : 'right';
    $container[$position][] = $block;
  }

  $output = '<div class="admin clearfix">';
  foreach ($container as $position => $blocks) {
    foreach ($blocks as $block) {
      $status_info = deploy_status_info($block['status']);

      $output .= '<div class="clearfix">';
      $output .= '<div class="admin-panel deploy-panel">';
      $output .= '<h3>' . $block['plan_title'] . '</h3>';
      if (!$block['fetch_only'] && $status_info) {
        $output .= '<div class="messages ' . $status_info['class'] . '">';
        $output .= '<h4 class="element-invisible">' . t('Status message') . '</h4>';
        $output .= $status_info['title'];
        $output .= '</div>';
      }
      $output .= '<div class="description">';
      $output .= '<h4 class="element-invisible">' . t('Plan description') . '</h4>';
      $output .= $block['plan_description'];
      $output .= '</div>';
      if (!$block['fetch_only']) {
        $actions = array(
          array(
            'title' => t('Deploy'),
            'href' => 'admin/dashboard/deployment/deploy',
            'query' => array(
              'cancel_path' => $_GET['q'],
            ),
          ));
        $output .= theme('links', array('links' => $actions, 'attributes' => array('class' => 'action-links')));
      }
      else {
        $output .= '<div class="description">' . t('This plan is <em>fetch-only</em>, and can therefore not be deployed in push fashion.') . '</div>';
      }
      $output .= '<div class="body">';
      $output .= $block['content'];
      $output .= '</div>';
      $output .= '</div>';
      $output .= '</div>';
    }
  }
  $output .= '</div>';
  return $output;
}

/**
 * Theme callback for a plan's content.
 */
function theme_wetkit_deploy_ui_overview_plan_content($variables) {
  $info = $variables['info'];
  $header = array(t('Title'), t('Type'));
  $rows = array();

  foreach ($info as $values) {
    $rows[] = array($values['title'], $values['type']);
  }
  if (empty($rows)) {
    $rows = array(array(array(
      'colspan' => 2,
      'data' => t('Currently no entities in this staging area.'),
    )));
  }
  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Theme the queue display.
 */
function theme_wetkit_deployment_source_items($variables) {
  // Include the CTools functionality that we need.
  ctools_include('ajax');
  ctools_include('modal');

  // Add CTools' javascript to the page.
  ctools_modal_add_js();

  $items = $variables['items'];
  $header = array(
    t('ID'),
    t('Title'),
    t('Type'),
    t('Timestamp'),
    t('UUID'),
    t('Content'),
  );
  $rows = array();

  // Adjust entity display for table.
  foreach ($items as &$item) {
    $data = unserialize($item['data']);
    $entity_title = '';
    $id = '';
    $entity_type = 'node';
    if (isset($entity_type)) {
      $entity_type = $data->__metadata['type'];
    }
    if ($entity_type == 'bean') {
      $entity_title = $data->title;
      $id = $data->bid;
    }
    if ($entity_type == 'file') {
      $entity_title = $data->filename;
      $id = $data->fid;
    }
    if ($entity_type == 'node') {
      $entity_title = $data->title;
      $id = $data->nid;
    }
    if ($entity_type == 'taxonomy_term') {
      $entity_title = $data->name;
      $id = $data->tid;
    }
    if ($entity_type == 'user') {
      $entity_title = $data->name;
      $id = $data->uid;
    }
    $ajax_string = 'wetkit_deployment_source_ajax/nojs/' . $entity_type . '/' . $id;
    $rows[] = array(
      $item['item_id'],
      isset($entity_title) ? $entity_title : '',
      $data->__metadata['type'],
      date("Y-m-d H:i:s", $item['created']),
      $data->uuid,
      ctools_modal_text_button(t('Show Content'), $ajax_string, t('Show Content'), 'ajax-processed'),
    );
  }
  if (empty($rows)) {
    $rows = array(array(array(
      'colspan' => 6,
      'data' => t('Currently no entities in the queue.'),
    )));
  }
  return theme('table', array('header' => $header, 'rows' => $rows));
}
