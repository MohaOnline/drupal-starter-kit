<?php

/**
 * @file
 * Contains the administration pages for Raven.
 */

/**
 * Returns supported PHP error levels and their labels.
 *
 * @return array
 *   An array containing supported PHP error levels and their labels.
 */
function raven_get_error_levels() {
  foreach (drupal_error_levels() as $level => $label) {
    $levels[$level] = $label[0];
  }
  return $levels;
}

/**
 * Adds Raven settings form to form.
 *
 * @param array $form
 *   The form definition.
 * @param array $form_state
 *   The form state.
 */
function raven_settings_form(array &$form, array &$form_state) {
  // Show message if library cannot be loaded.
  _raven_get_client();

  $form['raven'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sentry'),
    '#collapsible' => TRUE,
  );
  $form['raven']['raven_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Sentry PHP handlers'),
    '#description' => t('Check to send events in server-side PHP code to Sentry.'),
    '#default_value' => variable_get('raven_enabled', FALSE),
  );

  $form['raven']['php'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sentry PHP handler settings'),
    '#collapsible' => TRUE,
    '#collapsed' => !variable_get('raven_enabled', FALSE),
    '#states' => array(
      'collapsed' => array(
        ':input[name="raven_enabled"]' => array('unchecked' => TRUE),
      ),
    ),
  );

  $form['raven']['php']['raven_dsn'] = array(
    '#type' => 'textfield',
    '#title' => t('Sentry DSN'),
    '#description' => t('Sentry DSN credentials for current site.'),
    '#default_value' => variable_get('raven_dsn', ''),
  );

  $form['raven']['php']['connection'] = array(
    '#type' => 'fieldset',
    '#title' => t('Connection settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['raven']['php']['connection']['raven_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Timeout'),
    '#description' => t('Connection timeout in seconds.'),
    '#default_value' => variable_get('raven_timeout', 2),
    '#size' => 10,
  );

  $form['raven']['php']['connection']['raven_ssl'] = array(
    '#type' => 'radios',
    '#title' => t('SSL Verification'),
    '#default_value' => variable_get('raven_ssl', 'verify_ssl'),
    '#options' => array(
      'verify_ssl' => t('Verify SSL'),
      'ca_cert' => t('Verify against a CA certificate'),
      'no_verify_ssl' => t("Don't verify SSL (not recommended)"),
    ),
  );
  $form['raven']['php']['connection']['raven_ca_cert'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to CA certificate'),
    '#default_value' => variable_get('raven_ca_cert', ''),
    '#description' => t('Path to the CA certificate file of the Sentry server specified in the DSN.'),
    '#states' => array(
      'visible' => array(
        ':input[name=raven_ssl]' => array('value' => 'ca_cert'),
      ),
    ),
  );

  $form['raven']['php']['raven_exception_handler'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable exception handler'),
    '#description' => t('Check to handle uncaught exceptions.'),
    '#default_value' => variable_get('raven_exception_handler', TRUE),
  );

  $form['raven']['php']['raven_fatal_error_handler'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable fatal error handler'),
    '#description' => t('Check to handle fatal PHP errors.'),
    '#default_value' => variable_get('raven_fatal_error_handler', TRUE),
  );

  $form['raven']['php']['raven_fatal_error_handler_memory'] = array(
    '#type' => 'textfield',
    '#title' => t('Reserved memory'),
    '#description' => t('Reserved memory for fatal error handler (KB).'),
    '#default_value' => variable_get('raven_fatal_error_handler_memory', 2.5 * 1024),
    '#size' => 10,
  );

  $form['raven']['php']['error'] = array(
    '#type' => 'fieldset',
    '#title' => t('Deprecated: PHP error handler'),
  );

  $form['raven']['php']['error']['raven_error_handler'] = array(
    '#type' => 'checkbox',
    '#title' => t('Deprecated: Enable error handler'),
    '#description' => t('Check to handle PHP errors. It is recommended to leave this off and rely on Drupal watchdog to capture PHP errors.'),
    '#default_value' => variable_get('raven_error_handler', FALSE),
  );

  $form['raven']['php']['error']['raven_error_levels'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Error levels'),
    '#description' => t('Check error levels to handle.'),
    '#default_value' => variable_get('raven_error_levels', array()),
    '#options' => raven_get_error_levels(),
    '#states' => array(
      'invisible' => array(
        ':input[name="raven_error_handler"]' => array('unchecked' => TRUE),
      ),
    ),
  );

  $form['raven']['php']['watchdog'] = array(
    '#type' => 'fieldset',
    '#title' => t('Watchdog handler'),
  );

  $form['raven']['php']['watchdog']['raven_watchdog_handler'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable watchdog handler'),
    '#description' => t('Check to handle watchdog messages.'),
    '#default_value' => variable_get('raven_watchdog_handler', TRUE),
  );

  $_watchdog_levels = watchdog_severity_levels();
  $watchdog_levels = array();
  foreach ($_watchdog_levels as $level => $label) {
    $watchdog_levels[$level + 1] = $label;
  }
  $form['raven']['php']['watchdog']['raven_watchdog_levels'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Message levels'),
    '#description' => t('Check message levels to handle.'),
    '#default_value' => variable_get('raven_watchdog_levels', array()),
    '#options' => $watchdog_levels,
    '#states' => array(
      'invisible' => array(
        ':input[name="raven_watchdog_handler"]' => array('unchecked' => TRUE),
      ),
    ),
  );

  $form['raven']['php']['watchdog']['raven_ignored_types'] = [
    '#type' => 'textarea',
    '#title' => t('Ignored types'),
    '#description' => t('A list of log event types for which messages should not be sent to Sentry (one type per line). Commonly-configured types include <em>access denied</em> for 403 errors and <em>page not found</em> for 404 errors.'),
    '#default_value' => variable_get('raven_ignored_types', ''),
  ];

  $form['raven']['php']['raven_message_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Message limit'),
    '#description' => t('Log message maximum length in characters.'),
    '#default_value' => variable_get('raven_message_limit', 2048),
    '#size' => 10,
  );

  $form['raven']['php']['raven_stack'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable stacktraces'),
    '#description' => t('Enable it to add stacktraces to reports.'),
    '#default_value' => variable_get('raven_stack', TRUE),
  );

  $form['raven']['php']['raven_trace'] = array(
    '#type' => 'checkbox',
    '#title' => t('Reflection tracing in stacktraces'),
    '#description' => t('Check this to enable reflection tracing (function calling arguments) in stacktraces. Warning: This setting allows sensitive data to be logged by Sentry!'),
    '#default_value' => variable_get('raven_trace', FALSE),
  );

  $form['raven']['php']['raven_trace_user'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable reflection tracing for user watchdog messages'),
    '#description' => t('Check this to enable reflection tracing of user watchdog messages. This is disabled by default to avoid logging user passwords.'),
    '#default_value' => variable_get('raven_trace_user', FALSE),
  );

  $form['raven']['raven_js_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Sentry JavaScript handler'),
    '#description' => t('Check to send client-side JavaScript exceptions to Sentry (if user has the <a target="_blank" href="@url"><em>send JavaScript errors to Sentry</em> permission</a>).', array('@url' => url('admin/people/permissions', array('fragment' => 'module-raven')))),
    '#default_value' => variable_get('raven_js_enabled', FALSE),
  );

  $form['raven']['js'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sentry JavaScript handler settings'),
    '#collapsible' => TRUE,
    '#collapsed' => !variable_get('raven_js_enabled', FALSE),
    '#states' => array(
      'collapsed' => array(
        ':input[name="raven_js_enabled"]' => array('unchecked' => TRUE),
      ),
    ),
  );

  $version = json_decode(file_get_contents(drupal_get_path('module', 'raven') . '/package-lock.json'))->dependencies->{'raven-js'}->version;

  $form['raven']['js']['raven_public_dsn'] = array(
    '#type' => 'textfield',
    '#title' => t('Sentry Public DSN'),
    '#description' => t('Sentry public DSN credentials for current site.'),
    '#default_value' => variable_get('raven_public_dsn', ''),
  );

  $form['raven']['js']['raven_js_source'] = array(
    '#type' => 'radios',
    '#title' => t('Raven.js source'),
    '#options' => drupal_map_assoc(array('library', 'cdn')),
    '#default_value' => variable_get('raven_js_source', 'library'),
  );

  $form['raven']['js']['raven_js_source']['library'] = array(
    '#type' => 'radio',
    '#title' => t('Use local Raven.js library'),
    '#return_value' => 'library',
    '#description' => t('Version %version installed.', array('%version' => $version)),
  );

  $form['raven']['js']['raven_js_source']['cdn'] = array(
    '#type' => 'radio',
    '#title' => t('Use Raven.js CDN'),
    '#return_value' => 'cdn',
    '#description' => t('Define URL below.'),
  );

  $form['raven']['js']['raven_js_cdn_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Raven.js CDN URL'),
    '#description' => t('Check latest version at <a target="_blank" rel="noreferrer" href="@url">@url</a>.', array('@url' => 'https://docs.sentry.io/clients/javascript/')),
    '#default_value' => variable_get('raven_js_cdn_url', "https://cdn.ravenjs.com/$version/raven.min.js"),
  );

  $form['raven']['raven_environment'] = array(
    '#type' => 'textfield',
    '#title' => t('Environment'),
    '#description' => t('The environment in which this site is running.'),
    '#default_value' => variable_get('raven_environment', ''),
  );

  $form['raven']['raven_release'] = array(
    '#type' => 'textfield',
    '#title' => t('Release'),
    '#description' => t('The release this site is running (could be a version or commit hash).'),
    '#default_value' => variable_get('raven_release', ''),
  );
}
