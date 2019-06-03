<?php

/**
 * Form that helps people start importing remote sites.
 */
function hosting_remote_import_form($form, &$form_state) {
  $form = array();

  $servers = hosting_get_servers('remote_import');

  switch (count($servers)) {
    case 1:
      // If there is a single server set up for import, just redirect to it.
      drupal_set_message(t('Only one server configured for remote importing, skipping server selection.'));
      drupal_goto('node/' . reset(array_keys($servers)) . '/import-sites');
      break;
    case 0:
      // If there are no servers configured for remote importing, let people
      // know how to set this up.
      $form['help'] = array(
        '#type' => 'item',
        '#title' => t('Set up required'),
        '#value' => t('None of your servers are currently configured for remote importing. Doing so is easy though:<ol><li>Ensure that the remote server is added to Aegir just like any other server. You can <a href="@add_server">add a new server</a> if you need to.</li><li>Select the correct \'Remote import\' service type for your server on the add/edit server form.</li></ol>Once you\'ve completed those steps, come back to this page.', array('@add_server' => url('node/add/server'))),
      );
      break;
    default:
      // We have multiple servers.
      $form['server'] = array(
        '#type' => 'radios',
        '#title' => t('Server'),
        '#description' => t('Select the server you want to import a site from.'),
        '#options' => $servers,
        '#required' => TRUE,
      );

      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Next >'),
      );
      break;
  }

  return $form;
}

/**
 * Form submit callback.
 *
 * @see hosting_remote_import_form()
 */
function hosting_remote_import_form_submit($form, &$form_state) {
  drupal_goto( 'node/' . $form_state['values']['server'] . '/import-sites');
}
