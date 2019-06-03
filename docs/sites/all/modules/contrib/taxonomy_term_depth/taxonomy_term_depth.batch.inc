<?php
/**
 * @file
 */

/**
 * Require constants here again
 */
require_once __DIR__ . '/constants.inc';

function taxonomy_term_depth_batch_depth_update_form($form, &$form_state) {
  $form = array();

  $form['actions']['rebuild all'] = array(
    '#identity' => 'btn_rebuild_all',
    '#value' => t('Rebuild all terms'),
    '#type' => 'submit',
  );

  return $form;
}

function taxonomy_term_depth_batch_depth_update_form_submit($form, &$form_state) {
  $identity = isset($form_state['triggering_element']['#identity']) ? $form_state['triggering_element']['#identity'] : 'unknown';
  $options = array();
  switch ($identity) {
    case 'btn_rebuild_all':
      batch_set(array(
        'operations' => array(
          array(
            'taxonomy_term_depth_batch_callbacks_update_term_depth',
            array($options),
          ),
        ),
        'title' => t('Updating depths for all terms'),
        'file' => TAXONOMY_TERM_DEPTH_ROOT_REL . '/taxonomy_term_depth.batch.inc',
      ));
      break;
    default:
      drupal_set_message(t('Wrong operation selected'));
  }
}

function taxonomy_term_depth_batch_callbacks_update_term_depth($options, &$context) {
  $sandbox = &$context['sandbox'];
  // Build query
  $query = db_select('taxonomy_term_data', 'ttd');
  $query
    ->fields('ttd', array('tid'));

  $query
    ->groupBy('ttd.tid');

  // Count ALL elements and save the value for further usage
  if (!isset($sandbox['count'])) {
    // Clear all depths first
    db_update('taxonomy_term_data')
      ->fields(array(
          'depth' => NULL,
        )
      )
      ->execute();

    $sandbox['count'] = $query->countQuery()->execute()->fetchField();
  }

  $sandbox += array(
    'position' => 0,
    'bunch' => 20,
  );

  //$sandbox['position'] += $sandbox['bunch'];

  $query
    ->range($sandbox['position'], $sandbox['bunch']);

  foreach ($query->execute() as $row) {
    // Forcely rebuild data in database
    taxonomy_term_depth_get_by_tid($row->tid, TRUE);
    $sandbox['position']++;
  }

  $context['finished'] = $sandbox['count'] ? $sandbox['position'] / $sandbox['count'] : 1;
  $context['finished'] = $context['finished'] > 1 ? 1 : $context['finished'];
}
