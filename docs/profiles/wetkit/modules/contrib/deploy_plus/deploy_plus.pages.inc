<?php
/**
 * @file
 * Deploy Plus page functions.
 */

/**
 * Deployment plan review callback.
 */
function deploy_plus_review_plan($plan) {
  $variables = array(
    'prefix' => array(
      'title' => check_plain($plan->title),
      'edit' => "admin/structure/deploy/plans/list/{$plan->name}/edit",
      'dependency_plugin' => NULL,
      'endpoints' => array(),
    ),
    'info' => array(),
    'actions' => array(),
  );

  if ($plan->dependency_plugin) {
    $plugins = entity_dependency_get_all_ctools_plugins();
    $variables['prefix']['dependency_plugin'] = $plugins[$plan->dependency_plugin];
  }

  foreach ($plan->endpoints as $name) {
    $endpoint = deploy_endpoint_load($name);
    if ($endpoint) {
      $variables['prefix']['endpoints'][] = $endpoint;
    }
  }

  // Get the entity keys from the aggregator.
  $entity_keys = $plan->getEntities();
  foreach ($entity_keys as $entity_key) {
    // Get the entity info and all entities of this type.
    $entity_info = entity_get_info($entity_key['type']);

    $entity = entity_load_single($entity_key['type'], $entity_key['id']);
    if (empty($entity_info['entity keys']['revision'])) {
      $entity = entity_load_single($entity_key['type'], $entity_key['id']);
    }
    else {
      $entity = entity_revision_load($entity_key['type'], $entity_key['revision_id']);
    }
    $title = "{$entity_key['type']}:{$entity_key['id']}";
    $label = entity_label($entity_key['type'], $entity);
    if ($label) {
      $title = $label;
    }
    // @see deploy_plus.helper.inc
    $title = deploy_plus_build_entity_title($entity_key['type'], $entity);

    // Construct a usable array for the theme function.
    $variables['info'][] = array(
      'title' => $title,
      'language' => entity_language($entity_key['type'], $entity),
      'type' => $entity_info['label'],
      'entity_type' => deploy_plus_entity_type_label($entity_key['type'], $entity),
      'dependencies' => deploy_plus_get_entity_dependencies($entity, $entity_key['type']),
      'operations' => array(
        array(
          'title' => t('Remove from plan'),
          'href' => "admin/structure/deploy/{$plan->name}/{$entity_key['id']}/{$entity_key['type']}/delete",
        ),
      ),
    );
  }

  $variables['actions'] = array(
    array(
      'title' => t('Go Back'),
      'href' => isset($_GET['return']) ? $_GET['return'] : 'admin/structure/deploy/overview',
    ),
    array(
      'title' => t('Deploy'),
      'href' => DEPLOY_PLUS_PLANS_PATH . "/{$plan->name}/deploy",
      'query' => array(
        'cancel_path' => $_GET['q'],
      ),
    ),
  );

  return theme('deploy_plus_overview_plan_page', $variables);
}

/**
 * Form constructor for the deleting entity from plan page.
 */
function deploy_plus_delete_from_plan_form($form, &$form_state, $deploy_plan, $entity_id, $type) {
  $form = array();

  $form['deploy_plan'] = array(
    '#type' => 'value',
    '#value' => $deploy_plan,
  );

  $entity = entity_load_single($type, $entity_id);
  $wentity = entity_metadata_wrapper($type, $entity);
  $entity_info = entity_get_info($type);

  $form['entity'] = array(
    '#type' => 'value',
    '#value' => $entity,
  );

  $form['item'] = array(
    '#type' => 'value',
    '#value' => $entity_info['label'] . ' "' . $wentity->label() . '"',
  );

  $form['type'] = array(
    '#type' => 'value',
    '#value' => $type,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to delete !type "!item" from deployment plan?', array(
      '!type' => $entity_info['label'],
      '!item' => $wentity->label(),
    )),
    "admin/structure/deploy/{$deploy_plan->name}/review",
    '',
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submit handler for deleting entity from plan form.
 */
function deploy_plus_delete_from_plan_form_submit(&$form, &$form_state) {
  $values = $form_state['values'];

  deploy_manager_delete_from_plan(
    $values['deploy_plan']->name,
    $values['type'],
    $values['entity']
  );

  drupal_set_message(
    t('!item was deleted from !plan.', array(
      '!item' => $values['item'],
      '!plan' => $values['deploy_plan']->title,
    ))
  );

  $form_state['redirect'] = "admin/structure/deploy/{$values['deploy_plan']->name}/review";
}

/**
 * Page callback for the overview page.
 */
function deploy_plus_overview_page() {
  $plans = deploy_plan_load_all_enabled();
  $blocks = array();

  // Iterate over all plans.
  foreach ($plans as $plan) {
    $info = array();

    // Get the entity keys from the aggregator.
    $entity_keys = $plan->getEntities();
    foreach ($entity_keys as $entity_key) {
      // Get the entity info and all entities of this type.
      $entity_info = entity_get_info($entity_key['type']);

      if (!empty($entity_info['entity keys']['revision']) && !empty($entity_key['revision_id'])) {
        $entity = entity_revision_load($entity_key['type'], $entity_key['revision_id']);
      }
      else {
        $entity = entity_load_single($entity_key['type'], $entity_key['id']);
      }

      $title = "{$entity_key['type']}:{$entity_key['id']}";
      $label = entity_label($entity_key['type'], $entity);
      if ($label) {
        $title = $label;
      }

      if ($entity_info['entity keys']['revision'] && !empty($entity_key['revision_id'])) {
        $title = t('@title (rev:@rev_id)', array('@title' => $title, '@rev_id' => $entity_key['revision_id']));
      }

      $uri = entity_uri($entity_key['type'], $entity);
      if ($uri) {
        $title = l($title, $uri['path'], $uri['options']);
      }
      // Construct a usable array for the theme function.
      $info[] = array(
        'title' => $title,
        'type' => $entity_info['label'],
      );
    }

    // Construct a usable array for the theme function.
    $blocks[] = array(
      'plan_name' => check_plain($plan->name),
      'plan_title' => check_plain($plan->title),
      'plan_description' => check_plain($plan->description),
      'content' => theme('deploy_ui_overview_plan_content', array('info' => $info)),
      'fetch_only' => $plan->fetch_only,
      'status' => deploy_plan_get_status($plan->name),
    );
  }
  return theme('deploy_plus_overview', array('blocks' => $blocks));
}
