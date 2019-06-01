<?php
/**
 * @file
 * queue.inc
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'single' => TRUE,
  'render last' => TRUE,
  'title' => t('Deploy'),
  'description' => t('Deploy Plus overview form.'),
  'render callback' => 'deploy_plus_deploy_content_type_render',
  'edit form' => 'deploy_plus_deploy_edit_form',
  'category' => t('Deployment'),
);

/**
 * Admin title callback for the content type.
 */
function deploy_plus_deploy_content_type_admin_title($subtype = NULL, $conf = NULL, $context = NULL) {
  return t('Deploy Plus');
}

/**
 * Admin info callback for the content type.
 */
function deploy_plus_deploy_content_type_admin_info($subtype = NULL, $conf = NULL, $context = NULL) {
  $block = new stdClass();
  $block->title = t('Deploy Plus');

  return $block;
}

/**
 * Run-time rendering of the body of the block.
 */
function deploy_plus_deploy_content_type_render($subtype, $conf, $panel_args, &$context) {
  global $user;
  $block = new stdClass();
  $block->title = t('Deploy Plus');

  $plans = deploy_plan_load_all_enabled();
  $blocks = array();

  // Iterate over all plans.
  foreach ($plans as $plan) {

    // Skip other custom deploy plans.
    if (variable_get('deploy_plus_user_context', FALSE)) {
      if (strpos($plan->name, 'destination_site_plan_') === 0) {
        if ($plan->name != 'destination_site_plan_' . $user->name) {
          unset($plans[$plan->name]);
          continue;
        }
      }
    }

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

  $block->content = theme('deploy_plus_overview', array('blocks' => $blocks));
  return $block;

}

/**
 * Edit callback edit form for the content type.
 */
function deploy_plus_deploy_edit_form($form, &$form_state) {

  return $form;
}
