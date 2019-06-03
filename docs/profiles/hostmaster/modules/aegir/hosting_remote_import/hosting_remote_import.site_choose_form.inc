<?php

/**
 * Form to select exactly which site to import.
 */
function hosting_remote_import_sites_choose_site_form($form, &$form_state, $server, $vid) {
  $form = array();
  $sites = array();
  $result = db_query('SELECT site, platform FROM {remote_import_sites} WHERE tid = :tid ORDER BY site ASC', array(':tid' => $vid));
  foreach ($result as $row) {
    $sites[$row->site] = $row->site . ' (' . $row->platform . ')';
  }

  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $server->nid,
  );

  $form['vid'] = array(
    '#type' => 'value',
    '#value' => $vid,
  );

  $form['site'] = array(
    '#type' => 'radios',
    '#title' => t('Select the site to import'),
    '#description' => t('These are all of the existing (enabled and disabled) sites on the remote server, choose one to import. You\'ll be given options such as the target platform on the next screen.'),
    '#options' => $sites,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Next >'),
  );

  return $form;
}

/**
 * Simple submit handler that does a redirect.
 *
 * @see hosting_remote_import_sites_choose_site_form()
 */
function hosting_remote_import_sites_choose_site_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  // Just build a redirect
  $form_state['redirect'] = 'node/' . $values['nid'] . '/import-sites/platform/' . $values['vid'] . '/' . $values['site'] . '/' . drupal_get_token($values['vid'] . '/' . $values['site']);
}
