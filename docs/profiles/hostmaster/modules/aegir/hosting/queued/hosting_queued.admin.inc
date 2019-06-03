<?php
/**
 * @file
 * Configuration forms for the queue daemon.
 */

/**
 * Configuration form for the queue daemon.
 */
function hosting_queued_settings_form($form, &$form_state) {

  $form['description'] = array(
    '#type' => 'markup',
    '#value' => t('Note that the settings on this form will only apply to the daemon once it has been restarted, which by default happens as least once an hour.'),
    '#weight' => -100,
  );

  $semaphore_status = db_select('semaphore', 's')
    ->fields('s', array('name'))
    ->condition('s.name', 'hosting_queue_tasks_running')
    ->execute()
    ->fetchField()
  ;

  $processing_tasks = db_select('hosting_task', 't')
    ->fields('t', array('nid'))
    ->condition('task_status', HOSTING_TASK_PROCESSING)
    ->execute()
    ->fetchField()
  ;

  $form['hosting_queue_tasks_running'] = array(
    '#type' => 'item',
    '#title' => t('Tasks Running'),
    '#markup' => $processing_tasks? t('Yes'): t('No'),
  );

  if (!$processing_tasks && $semaphore_status) {
    $form['hosting_queue_tasks_running']['#description'] = t('Warning: The task queue is locked but there are no tasks running. !link to remove the lock.', array(
      '!link' => l(t('Click Here'), 'admin/hosting/queued/semaphore')
    ));
  }

  $last_seen = variable_get('hosting_queued_process_started', NULL);
  $form['hosting_queued_process_started'] = array(
    '#type' => 'item',
    '#title' => t('Runner status'),
    '#markup' => !empty($last_seen) ? t('Last started: @interval ago.', array('@interval' => format_interval(REQUEST_TIME - $last_seen))) : t('Never started.'),
  );

  $form['hosting_queued_post_task_delay'] = array(
    '#type' => 'select',
    '#title' => t('Post task delay'),
    '#description' => t('Tasks are executed as fast as possible, so you may wish to add a delay after the execution of each task. After this delay, new tasks will still start executing almost instantly.'),
    '#default_value' => variable_get('hosting_queued_post_task_delay', 0),
    '#options' => array(
      0 => t('No delay'),
    ) + drupal_map_assoc(range(1, 60), '_hosting_queued_settings_form_delay_callback'),
  );

  $form['hosting_queued_process_lifetime'] = array(
    '#type' => 'select',
    '#title' => t('Process lifetime timeout'),
    '#description' => t('Because of memory leaks and bugs in PHP, the daemon automatically stops after this delay, and is restarted. If you are running a lot of tasks, you may want to lower this so it gets restarted more often. In empirical tests, it was found that around 100KB are leaked for every task fired.'),
    '#default_value' => variable_get('hosting_queued_process_lifetime', 3600),
    '#options' => drupal_map_assoc(array(
      60,
      60 * 5,
      60 * 10,
      60 * 15,
      60 * 20,
      60 * 25,
      60 * 30,
      60 * 35,
      60 * 40,
      60 * 45,
      60 * 50,
      60 * 55,
      60 * 60,
    ), 'format_interval'),
  );
  
  $form['hosting_queued_paused'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pause the Hosting Queue'),
    '#description' => t('Temporarily pause the hosting queue from running tasks.'),
    '#default_value' => variable_get('hosting_queued_paused', 0),
  );

  return system_settings_form($form);
  // @ignore security_fapi_markup
}

function _hosting_queued_settings_form_delay_callback($value) {
  return format_plural($value, '1 second', '@count seconds');
}

/**
 * Form for removing the hosting queue lock.
 *
 * @return mixed
 */
function hosting_queued_remove_semaphore_form() {

  drupal_set_title(t('Unlock Hosting Queue'));

  $processing_tasks = db_select('hosting_task', 't')
    ->fields('t', array('nid'))
    ->condition('task_status', HOSTING_TASK_PROCESSING)
    ->execute()
    ->fetchField()
  ;
  $semaphore_status = db_select('semaphore', 's')
    ->fields('s', array('name'))
    ->condition('s.name', 'hosting_queue_tasks_running')
    ->execute()
    ->fetchField()
  ;


  $form['hosting_queue_tasks_running'] = array(
    '#type' => 'item',
    '#title' => t('Tasks Running'),
    '#markup' => $processing_tasks? t('Yes'): t('No'),
  );

  $form['hosting_queue_tasks_locked'] = array(
    '#type' => 'item',
    '#title' => t('Task Queue Locked'),
    '#markup' => $semaphore_status? t('Yes'): t('No'),
  );

  $disabled = TRUE;
  if ($semaphore_status && !$processing_tasks) {
    $form['hosting_queue_tasks_locked']['#description'] = t('The task queue is locked but there are no tasks running. You should unlock the queue.');
    $button_class = 'btn-success';
    $disabled = FALSE;
  }
  else {
    $form['hosting_queue_tasks_locked']['#description'] = t('The task queue is not locked.');
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Unlock Hosting Queue'),
    '#disabled' => $disabled,
    '#attributes' => array(
      'class' => array($button_class)
    )
  );

  return $form;
}

/**
 * submit function for the form for removing the hosting queue lock.

 */
function hosting_queued_remove_semaphore_form_submit() {
  global $locks;
  unset($locks['hosting_queue_tasks_running']);
  $success = db_delete('semaphore')
    ->condition('name', 'hosting_queue_tasks_running')
    ->execute();

  if ($success) {
    drupal_set_message(t('Hosting Queue unlocked.'));
  }
  else {
    drupal_set_message(t('Hosting Queue was not unlocked. Something went wrong.'), 'error');

  }
}