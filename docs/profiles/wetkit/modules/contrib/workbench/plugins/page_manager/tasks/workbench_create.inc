<?php

/**
 * @file
 * Handle the 'admin/workbench/create' override task.
 */

/**
 * Specialized implementation of hook_page_manager_task_tasks(). See api-task.html for
 * more information.
 */
function workbench_workbench_create_page_manager_tasks() {
  return array(
    // This is a 'page' task and will fall under the page admin UI
    'task type' => 'page',

    'title' => t('My Workbench: Create Content'),

    'admin title' => t('The Workbench Create Content page'),
    'admin description' => t('When enabled, this overrides the default Workbench Create Content page for <em>admin/workbench/create</em>. If no variant is selected, the default Create Content page will be shown. See http://drupal.org/node/1226174 for more information.'),
    'admin path' => 'admin/workbench/create',

    // Menu hooks so that we can alter the admin/workbench/create menu entry to point to us.
    'hook menu alter' => 'workbench_workbench_create_menu_alter',

    // This is task uses 'context' handlers and must implement these to give the
    // handler data it needs.
    'handler type' => 'context',

    // Allow this to be enabled or disabled:
    'disabled' => variable_get('workbench_workbench_create_disabled', TRUE),
    'enable callback' => 'workbench_workbench_create_enable',
  );
}

/**
 * Callback defined by workbench_workbench_create_page_manager_tasks().
 *
 * Alter menu item so that admin/workbench/create comes here.
 */
function workbench_workbench_create_menu_alter(&$items, $task) {
  if (variable_get('workbench_workbench_create_disabled', TRUE)) {
    return;
  }

  // Override the node view handler for our purpose.
  $callback = $items['admin/workbench/create']['page callback'];
  if ($callback == 'node_page_view' || variable_get('page_manager_override_anyway', FALSE)) {
    $items['admin/workbench/create']['page callback'] = 'workbench_workbench_create_page';
    $items['admin/workbench/create']['file path'] = $task['path'];
    $items['admin/workbench/create']['file'] = $task['file'];
  }
  else {
    // automatically disable this task if it cannot be enabled.
    //variable_set('workbench_workbench_create_disabled', TRUE);
    if (!empty($GLOBALS['page_manager_enabling_workbench_create'])) {
      drupal_set_message(t('Page manager module is unable to enable Workbench Create Content because some other module already has overridden with %callback.', array('%callback' => $callback)), 'error');
    }
  }
}

/**
 * Entry point for our overridden Create Content page.
 *
 * This function asks its assigned handlers who, if anyone, would like
 * to run with it. If no one does, it passes through to the main Create Content page.
 */
function workbench_workbench_create_page() {
  // Load my task plugin
  $task = page_manager_get_task('workbench_create');

  // Load the node into a context.
  ctools_include('context');
  ctools_include('context-task-handler');
  $contexts = ctools_context_handler_get_task_contexts($task, '', array(), array());

  $output = ctools_context_handler_render($task, '', $contexts, array($node->nid));
  if ($output != FALSE) {
    return $output;
  }

  module_load_include('inc', 'workbench', 'workbench.pages');
  $function = 'workbench_create';
  foreach (module_implements('page_manager_override') as $module) {
    $call = $module . '_page_manager_override';
    if (($rc = $call('workbench_create')) && function_exists($rc)) {
      $function = $rc;
      break;
    }
  }

  // Otherwise, fall back.
  return $function($node);
}

/**
 * Callback to enable/disable the page from the UI.
 */
function workbench_workbench_create_enable($cache, $status) {
  variable_set('workbench_workbench_create_disabled', $status);

  // Set a global flag so that the menu routine knows it needs
  // to set a message if enabling cannot be done.
  if (!$status) {
    $GLOBALS['page_manager_enabling_workbench_create'] = TRUE;
  }
}
