<?php
/**
 * @file
 * Administrative pages for Entity Queue.
 */

/**
 * Add all appropriate includes to forms so that caching the form
 * still loads the files that we need.
 */
function _entityqueue_subqueue_add_form_files($form, &$form_state) {
  ctools_form_include($form_state, 'export');
  ctools_form_include($form_state, 'export-ui');

  // Also make sure the plugin .inc and .class.php files are loaded.
  form_load_include($form_state, 'inc', 'entityqueue', '/includes/entityqueue.admin');
  form_load_include($form_state, 'inc', 'entityqueue', '/plugins/ctools/export_ui/entityqueue_export_ui');
  form_load_include($form_state, 'php', 'entityqueue', '/plugins/ctools/export_ui/entityqueue_export_ui.class');
}

/**
 * Form callback; Displays the subqueue edit form.
 */
function entityqueue_subqueue_edit_form($form, &$form_state, EntityQueue $queue, EntitySubqueue $subqueue) {
  // When called using #ajax via ajax_form_callback(), 'export' may
  // not be included so include it here.
  _entityqueue_subqueue_add_form_files($form, $form_state);

  $handler = entityqueue_get_handler($queue);
  $form = $handler->subqueueForm($subqueue, $form_state);
  $form_state['entityqueue_queue'] = $queue;
  $form_state['entityqueue_subqueue'] = $subqueue;

  field_attach_form('entityqueue_subqueue', $subqueue, $form, $form_state);

  // Since the form has ajax buttons, the $wrapper_id will change each time
  // one of those buttons is clicked. Therefore the whole form has to be
  // replaced, otherwise the buttons will have the old $wrapper_id and will only
  // work on the first click.
  $field_name = _entityqueue_get_target_field_name($queue->target_type);
  if (isset($form_state['form_wrapper_id'])) {
    $wrapper_id = $form_state['form_wrapper_id'];
  }
  else {
    $wrapper_id = drupal_html_id($field_name . '-wrapper');
  }
  $form_state['form_wrapper_id'] = $wrapper_id;
  $form_state['field_name'] = $field_name;

  $form['#prefix'] = '<div id="' . $wrapper_id . '">';
  $form['#suffix'] = '</div>';

  // Entity type (bundle) is needed by entity_form_submit_build_entity().
  $form['queue'] = array(
    '#type' => 'value',
    '#default_value' => $queue->name,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 40,
  );

  $form['actions']['reverse'] = array(
    '#type' => 'button',
    '#value' => t('Reverse'),
    '#weight' => 41,
    '#validate' => array('entityqueue_subqueue_reverse_validate'),
    '#ajax' => array(
      'callback' => 'entityqueue_subqueue_ajax_callback',
      'wrapper' => $wrapper_id,
    ),
  );
  $form['actions']['shuffle'] = array(
    '#type' => 'button',
    '#value' => t('Shuffle'),
    '#weight' => 42,
    '#validate' => array('entityqueue_subqueue_shuffle_validate'),
    '#ajax' => array(
      'callback' => 'entityqueue_subqueue_ajax_callback',
      'wrapper' => $wrapper_id,
    ),
  );
  $form['actions']['clear'] = array(
    '#type' => 'button',
    '#value' => t('Clear'),
    '#weight' => 43,
    '#validate' => array('entityqueue_subqueue_clear_validate'),
    '#ajax' => array(
      'callback' => 'entityqueue_subqueue_ajax_callback',
      'wrapper' => $wrapper_id,
    ),
  );

  $form['#validate'][] = 'entityqueue_subqueue_edit_form_validate';
  $form['#submit'][] = 'entityqueue_subqueue_edit_form_submit';

  return $form;
}

/**
 * Validation callback for the subqueue edit form.
 */
function entityqueue_subqueue_edit_form_validate($form, &$form_state) {
  $queue = $form_state['entityqueue_queue'];
  $subqueue = $form_state['entityqueue_subqueue'];

  // Use load multiple to ensure we load fresh without a cache.
  $original_subqueues = entityqueue_subqueue_load_multiple(array($subqueue->name), array(), TRUE);
  $original_subqueue = reset($original_subqueues);

  // Compare the subqueue that was in form state with the freshly loaded copy,
  // to ensure the queue hasn't been modified in a different user session.
  if ($original_subqueue) {
    $field_name = _entityqueue_get_target_field_name($queue->target_type);
    $lang = $form[$field_name]['#language'];
    $form_items = $subqueue->{$field_name} ? $subqueue->{$field_name}[$lang] : array();
    $original_items = $original_subqueue->{$field_name} ? $original_subqueue->{$field_name}[$lang] : array();
    if ($form_items !== $original_items) {
      form_error($form['queue'], t('This queue has been modified in another window or by another user and cannot be saved. Load this page again and remake your changes.'));
    }
  }

  entity_form_field_validate('entityqueue_subqueue', $form, $form_state);
}

/**
 * Validation callback to reverse items in the subqueue.
 */
function entityqueue_subqueue_reverse_validate($form, &$form_state) {
  $queue = $form_state['entityqueue_queue'];
  $field_name = _entityqueue_get_target_field_name($queue->target_type);
  $lang = $form[$field_name]['#language'];
  foreach(array('input', 'values') as $state) {
    if (isset($form_state[$state][$field_name][$lang])) {
      $field_values = $form_state[$state][$field_name][$lang];
      foreach ($field_values as $key => $value) {
        if (!is_numeric($key) || empty($value['target_id']) || $value['target_id'] == '_none') {
          unset($field_values[$key]);
        }
      }
      $field_values = array_reverse($field_values);
      // Set weights according to their new order.
      foreach ($field_values as $key => $value) {
        if (is_numeric($key)) {
          $field_values[$key]['_weight'] = $key;
        }
      }
      $form_state[$state][$field_name][$lang] = $field_values;
    }
  }
}

/**
 * Validation callback to shuffle items in the subqueue.
 */
function entityqueue_subqueue_shuffle_validate($form, &$form_state) {
  $queue = $form_state['entityqueue_queue'];
  $field_name = _entityqueue_get_target_field_name($queue->target_type);
  $lang = $form[$field_name]['#language'];
  foreach(array('input', 'values') as $state) {
    if (isset($form_state[$state][$field_name][$lang])) {
      $field_values = $form_state[$state][$field_name][$lang];
      foreach ($field_values as $key => $value) {
        if (!is_numeric($key) || empty($value['target_id']) || $value['target_id'] == '_none') {
          unset($field_values[$key]);
        }
      }
      shuffle($field_values);
      // Set weights according to their new order.
      foreach ($field_values as $key => $value) {
        if (is_numeric($key)) {
          $field_values[$key]['_weight'] = $key;
        }
      }
      $form_state[$state][$field_name][$lang] = $field_values;
    }
  }
}

/**
 * Validation callback to clear items in the subqueue.
 */
function entityqueue_subqueue_clear_validate($form, &$form_state) {
  $queue = $form_state['entityqueue_queue'];
  $field_name = _entityqueue_get_target_field_name($queue->target_type);
  $lang = $form[$field_name]['#language'];
  foreach(array('input', 'values') as $state) {
    $form_state[$state][$field_name][$lang] = array();
  }
}

/**
 * Submit callback for the subqueue edit form.
 */
function entityqueue_subqueue_edit_form_submit($form, &$form_state) {
  $queue = $form_state['entityqueue_queue'];
  $subqueue = $form_state['entityqueue_subqueue'];

  entity_form_submit_build_entity('entityqueue_subqueue', $subqueue, $form, $form_state);
  $subqueue->save();

  drupal_set_message(t('The %queue queue has been saved.', array('%queue' => $subqueue->label())), 'status');
}

/**
 * Form callback.
 */
function entityqueue_subqueue_delete_form($form, &$form_state, $queue, $subqueue) {
  $handler = entityqueue_get_handler($queue);
  // If they can't delete this subqueue, return access denied.
  if (!$handler->canDeleteSubqueue($subqueue)) {
    drupal_set_message(t('The %queue: %subqueue subqueue cannot be deleted.', array(
      '%queue' => $queue->label,
      '%subqueue' => $subqueue->label,
    )), 'warning');
    drupal_access_denied();
    drupal_exit();
  }

  $form['#queue'] = $queue;
  $form['#subqueue'] = $subqueue;
  $form['subqueue_id'] = array(
    '#type' => 'value',
    '#value' => $subqueue->subqueue_id,
  );
  return confirm_form($form, t('Are you sure you want to delete %queue: %subqueue?', array(
    '%queue' => $queue->label,
    '%subqueue' => $subqueue->label,
  )), 'admin/structure/entityqueue/list/' . $queue->name . '/subqueues', NULL, t('Delete'));
}

/**
 * Form submit handler.
 * @see entityqueue_subqueue_delete_form()
 */
function entityqueue_subqueue_delete_form_submit($form, &$form_state) {
  $queue = $form['#queue'];
  $subqueue = $form['#subqueue'];
  $handler = entityqueue_get_handler($queue);

  if ($handler->canDeleteSubqueue($subqueue)) {
    entity_delete('entityqueue_subqueue', $subqueue->subqueue_id);
  }
  $form_state['redirect'] = 'admin/structure/entityqueue/list/' . $queue->name . '/subqueues';
}
