<?php
/**
 * @file
 * Deploy Plus themes functions.
 */

/**
 * Theme callback for a plan's overview page.
 */
function theme_deploy_plus_overview_plan_page($variables) {
  $output = '';

  drupal_add_library('system', 'drupal.collapse');

  $header = array(t('Name'), t('Value'));
  $rows = array();

  // Title.
  $prefix = $variables['prefix'];
  $edit = l(t('edit'), $prefix['edit'], array(
    'query' => array(
      'destination' => current_path(),
    ),
  ));
  $output .= "<h2>{$prefix['title']}</h2>";

  // Configuration info.
  // Plan
  $rows[] = array(
    t('Plan'),
    "{$prefix['title']} <small><span class='admin-link'>{$edit}</span></small>",
  );

  // Dependency Plugin.
  $rows[] = array(
    t('Dependency Plugin'),
    $prefix['dependency_plugin'] ? $prefix['dependency_plugin'] : t('NONE'),
  );

  // Endpoints.
  $items = array();
  foreach ($prefix['endpoints'] as $endpoint) {
    $edit = '<small><span class="admin-link">';
    $edit .= l(
      t('edit'),
      "admin/structure/deploy/endpoints/list/{$endpoint->name}/edit",
      array(
        'query' => array(
          'destination' => current_path(),
        ),
      ));
    $edit .= '</spam></small>';
    $items[] = "{$endpoint->title}: <span class='form-required'>{$endpoint->service_config['url']}</span> {$edit}";
  }
  if (count($items) == 1) {
    $rows[] = array(t('Endpoint'), reset($items));
  }
  else {
    $rows[] = array(
      t('Endpoints'),
      theme('item_list', array('items' => $items)),
    );
  }

  $output .= theme('ctools_collapsible', array(
    'handle' => '<h4><a href="#">' . t('Plan configuration') . '</a></h4>',
    'content' => theme('table', array('header' => $header, 'rows' => $rows)),
  ));

  // Content table.
  foreach ($variables['info'] as &$info) {
    if (!empty($info['dependencies'])) {
      $dep = '';
      foreach ($info['dependencies'] as $title => $value) {
        $dep .= theme('ctools_collapsible', array(
          'handle' => "<h3>{$title}</h3>",
          'content' => theme('item_list', array('items' => $value)),
          'collapsed' => TRUE,
        ));
      }
      $info['dependencies'] = $dep;
    }
    if (!empty($info['operations'])) {
      $op = theme('links', array('links' => $info['operations']));
      $info['operations'] = $op;
    }
  }

  $output .= theme('deploy_plus_overview_plan_content_details', array(
    'info' => $variables['info'],
  ));

  // Action links.
  $output .= theme('links', array(
    'links' => $variables['actions'],
    'attributes' => array('class' => 'action-links'),
  ));

  return $output;
}

/**
 * Theme callback for a plan's content.
 *
 * @see theme_deploy_plus_overview_plan_content()
 */
function theme_deploy_plus_overview_plan_content_details($variables) {
  $output = '<h3>' . t('Content') . '</h3>';
  $info = $variables['info'];
  $header = array(
    t('Title'),
    t('Language'),
    array('data' => t('Type'), 'sort' => 'ASC'),
    t('Entity Type'),
    t('Dependencies'),
    t('Operations'),
  );
  $rows = array();

  foreach ($info as $values) {
    $rows[] = array(
      array('data' => $values['title'], 'width' => '27%'),
      array('data' => $values['language'], 'width' => '7%', 'align' => 'center'),
      array('data' => $values['type'], 'width' => '10%'),
      array('data' => $values['entity_type'], 'width' => '10%'),
      array('data' => $values['dependencies'], 'width' => '33%'),
      array('data' => $values['operations'], 'width' => '13%'),
    );
  }
  if (empty($rows)) {
    $rows = array(
      array(
        array(
          'colspan' => count($header),
          'data' => t('Currently no content in this plan.'),
        ),
      ),
    );
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  return $output;
}

/**
 * Theme callback for the overview page.
 */
function theme_deploy_plus_overview($variables) {
  drupal_add_css(drupal_get_path('module', 'deploy_ui') . '/css/deploy_ui.admin.css');

  $blocks = $variables['blocks'];
  $container = array();

  $i = 0;
  foreach ($blocks as $block) {
    $position = ++$i % 2 ? 'left' : 'right';
    $container[$position][] = $block;
  }

  $output = '<div class="wb-tabs admin clearfix">';
  $output .= '<div class="tabpanels">';
  foreach ($container as $position => $blocks) {
    foreach ($blocks as $block) {
      $status_info = deploy_status_info($block['status']);

      $output .= '<details class=" ' . $position . ' clearfix">';
      $output .= '<summary>' . $block['plan_title'] . '</summary>';
      if (!$block['fetch_only'] && $status_info) {
        $output .= '<div class="messages ' . $status_info['class'] . '">';
        $output .= '<h4 class="element-invisible">Status message</h4>';
        $output .= $status_info['title'];
        $output .= '</div>';
      }
      $output .= '<div class="description">';
      $output .= '<h4 class="element-invisible">Plan description</h4>';
      $output .= $block['plan_description'];
      $output .= '</div>';
      if (!$block['fetch_only']) {
        $actions = array(
          array(
            'title' => t('Deploy'),
            'href' => "admin/structure/deploy/plans/list/{$block['plan_name']}/deploy",
            'query' => array(
              'cancel_path' => $_GET['q'],
            ),
          ),
          array(
            'title' => t('Review'),
            'href' => "admin/structure/deploy/{$block['plan_name']}/review",
            'query' => array(
              'return' => $_GET['q'],
            ),
          ),
          array(
            'title' => t('Empty'),
            'href' => "admin/structure/deploy/{$block['plan_name']}/empty",
          ),
        );
        $output .= theme('links', array('links' => $actions, 'attributes' => array('class' => 'action-links')));
      }
      else {
        $output .= '<div class="description">' . t('This plan is <em>fetch-only</em>, and can therefore not be deployed in push fashion.') . '</div>';
      }
      $output .= '<div class="body">';
      $output .= $block['content'];
      $output .= '</div>';
      $output .= '</details>';
    }
  }
  $output .= '</div>';
  $output .= '</div>';
  return $output;
}

/**
 * Menu links table.
 *
 * @see theme_menu_overview_form()
 */
function theme_deploy_plus_menu_overview_form($variables) {
  $form = $variables['form'];

  drupal_add_tabledrag('menu-overview', 'match', 'parent', 'menu-plid', 'menu-plid', 'menu-mlid', TRUE, MENU_MAX_DEPTH - 1);
  drupal_add_tabledrag('menu-overview', 'order', 'sibling', 'menu-weight');

  $header = array(
    t('Menu link'),
    array(
      'data' => t('Deploy'),
      'class' => array('checkbox'),
    ),
    array(
      'data' => t('Enabled'),
      'class' => array('checkbox'),
    ),
    t('Weight'),
    array(
      'data' => t('Operations'),
      'colspan' => '3',
    ),
  );

  $rows = array();
  foreach (element_children($form) as $mlid) {
    if (isset($form[$mlid]['hidden'])) {
      $element = &$form[$mlid];
      // Build a list of operations.
      $operations = array();
      foreach (element_children($element['operations']) as $op) {
        $operations[] = array(
          'data' => drupal_render($element['operations'][$op]),
          'class' => array('menu-operations'),
        );
      }
      while (count($operations) < 3) {
        $operations[] = '';
      }

      // Add special classes to be used for tabledrag.js.
      $element['plid']['#attributes']['class'] = array('menu-plid');
      $element['mlid']['#attributes']['class'] = array('menu-mlid');
      $element['weight']['#attributes']['class'] = array('menu-weight');

      // Change the parent field to a hidden. This allows any value but hides
      // the field.
      $element['plid']['#type'] = 'hidden';

      $row = array();
      $row[] = theme('indentation', array('size' => $element['#item']['depth'] - 1)) . drupal_render($element['title']);
      $row[] = array(
        'data' => drupal_render($element['plan']),
        'class' => array('checkbox'),
      );
      $row[] = array(
        'data' => drupal_render($element['hidden']),
        'class' => array('checkbox', 'menu-enabled'),
      );
      $row[] = drupal_render($element['weight']) . drupal_render($element['plid']) . drupal_render($element['mlid']);
      $row = array_merge($row, $operations);

      $row = array_merge(array('data' => $row), $element['#attributes']);
      $row['class'][] = 'draggable';
      $rows[] = $row;
    }
  }
  $output = '';
  if (empty($rows)) {
    $rows[] = array(
      array(
        'data' => $form['#empty_text'],
        'colspan' => '7',
      ),
    );
  }
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'menu-overview'),
  ));
  $output .= drupal_render_children($form);
  return $output;
}
