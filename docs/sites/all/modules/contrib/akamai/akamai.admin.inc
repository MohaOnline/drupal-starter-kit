<?php

/**
 * @file
 * Administrative pages for the Akamai module.
 */

use Akamai\Open\EdgeGrid\Client as EdgeGridClient;
use Drupal\akamai\CcuClientInterface;

/**
 * Form builder to manage Akamai settings.
 */
function akamai_settings_form() {
  $form = array();

  $form['disable_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Disable akamai cache clearing'),
    '#description' => t('Set this field to disable purge request submissions. Purge requests will still be queued if queueing is enabled. This can be useful when performing imports, migrations, or other batch processes.'),
  );

  $form['disable_fieldset']['akamai_disabled'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Disable cache purging'),
    '#default_value' => variable_get('akamai_disabled', FALSE),
  );

  $form['cron'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => t('Cron tasks'),
  );
  $form['cron']['akamai_queue_purge_requests'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable queueing of purge requests'),
    '#default_value' => variable_get('akamai_queue_purge_requests', FALSE),
    '#description'   => t('If enabled, purge requests will be queued and submitted when cron runs.'),
  );
  $form['cron']['akamai_queue_on_failure'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Queue failed purge requests'),
    '#default_value' => variable_get('akamai_queue_on_failure', TRUE),
    '#description'   => t('If a purge request fails, the paths will be added to the queue and re-submitted when cron runs.'),
  );
  $form['cron']['akamai_batch_size'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Batch size'),
    '#default_value' => variable_get('akamai_batch_size', 0),
    '#size'          => 7,
    '#maxlength'     => 6,
    '#description'   => t("The number of queued items to submit in each batch. Set to 0 for no limit, in which case the batch size will be limited by the CCU API request body limit of 50,000 bytes."),
  );
  $form['cron']['akamai_cron_queue_time_limit'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Cron time limit'),
    '#description'   => t("Items in the queue will stop being processed if this time limit is reached."),
    '#size'          => 5,
    '#maxlength'     => 3,
    '#field_suffix'  => t('seconds'),
    '#default_value' => variable_get('akamai_cron_queue_time_limit', AKAMAI_CRON_QUEUE_TIME_LIMIT_DEFAULT),
    '#required'      => TRUE,
  );
  $form['cron']['akamai_purge_log_duration_complete'] = array(
    '#type'          => 'textfield',
    '#title'         => t('How long should information about completed purge requests be kept?'),
    '#size'          => 5,
    '#maxlength'     => 4,
    '#field_suffix'  => t('hours'),
    '#default_value' => variable_get('akamai_purge_log_duration_complete', AKAMAI_PURGE_REQUEST_HOURS_TO_KEEP_DEFAULT),
    '#required'      => TRUE,
    '#description'   => t('Set to 0 to keep indefinitely.'),
  );
  $form['cron']['akamai_purge_log_duration_incomplete'] = array(
    '#type'          => 'textfield',
    '#title'         => t('How long should information about pending purge requests be kept?'),
    '#size'          => 5,
    '#maxlength'     => 4,
    '#field_suffix'  => t('hours'),
    '#default_value' => variable_get('akamai_purge_log_duration_incomplete', AKAMAI_PURGE_REQUEST_HOURS_TO_KEEP_DEFAULT),
    '#required'      => TRUE,
    '#description'   => t('Set to 0 to keep indefinitely.'),
  );

  $form['credentials'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => t('API Credentials'),
    '#description' => t("For instructions on obtaining credentials, see Akamai's <a href=\"!url\">Authorize Your Client</a> documentation.", array('!url' => 'https://developer.akamai.com/introduction/Prov_Creds.html')),
  );
  $form['credentials']['akamai_credential_storage'] = array(
    '#type'          => 'radios',
    '#title'         => t('Credential storage method'),
    '#default_value' => variable_get('akamai_credential_storage', 'database'),
    '#options'       => array(
      'database' => t('Database'),
      'file'     => t('.edgerc file'),
    ),
    '#required'      => TRUE,
    '#description' => t('Credentials may be stored in the database or in a file. See the README file for more information.'),
  );

  $database_field_states = array(
    'required' => array(
      ':input[name="akamai_credential_storage"]' => array('value' => 'database'),
    ),
    'visible' => array(
      ':input[name="akamai_credential_storage"]' => array('value' => 'database'),
    ),
    'optional' => array(
      ':input[name="akamai_credential_storage"]' => array('value' => 'file'),
    ),
    'invisible' => array(
      ':input[name="akamai_credential_storage"]' => array('value' => 'file'),
    ),
  );
  $file_field_states = array(
    'required' => array(
      ':input[name="akamai_credential_storage"]' => array('value' => 'file'),
    ),
    'visible' => array(
      ':input[name="akamai_credential_storage"]' => array('value' => 'file'),
    ),
    'optional' => array(
      ':input[name="akamai_credential_storage"]' => array('value' => 'database'),
    ),
    'invisible' => array(
      ':input[name="akamai_credential_storage"]' => array('value' => 'database'),
    ),
  );
  $form['credentials']['akamai_base_uri'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Base URL'),
    '#default_value' => variable_get('akamai_base_uri', ''),
    '#description'   => t('Should be in format of %url', array('%url' => 'https://akaa-xxxxxxxxxxxxxxxx-xxxxxxxxxxxxxxxx.luna.akamaiapis.net/')),
    '#states' => $database_field_states,
    '#element_validate' => array(
      'akamai_settings_form_validate_credential_field',
      'akamai_settings_form_validate_url',
    ),
    '#akamai_credential_type' => 'database',
  );
  $form['credentials']['akamai_access_token'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Access token'),
    '#default_value' => variable_get('akamai_access_token', ''),
    '#description'   => t('Should be in format of %token', array('%token' => 'akab-xxxxxxxxxxxxxxxx-xxxxxxxxxxxxxxxx')),
    '#states' => $database_field_states,
    '#element_validate' => array('akamai_settings_form_validate_credential_field'),
    '#akamai_credential_type' => 'database',
  );
  $form['credentials']['akamai_client_token'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Client token'),
    '#default_value' => variable_get('akamai_client_token', ''),
    '#description'   => t('Should be in format of %token', array('%token' => 'akab-xxxxxxxxxxxxxxxx-xxxxxxxxxxxxxxxx')),
    '#states' => $database_field_states,
    '#element_validate' => array('akamai_settings_form_validate_credential_field'),
    '#akamai_credential_type' => 'database',
  );
  $form['credentials']['akamai_client_secret'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Client secret'),
    '#default_value' => variable_get('akamai_client_secret', ''),
    '#states' => $database_field_states,
    '#element_validate' => array('akamai_settings_form_validate_credential_field'),
    '#akamai_credential_type' => 'database',
  );
  $form['credentials']['akamai_edgerc_path'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Path to .edgerc file'),
    '#default_value' => variable_get('akamai_edgerc_path', ''),
    '#states' => $file_field_states,
    '#element_validate' => array(
      'akamai_settings_form_validate_credential_field',
      'akamai_settings_form_validate_file_exists',
      'akamai_settings_form_validate_file_is_readable',
    ),
    '#akamai_credential_type' => 'file',
  );
  $form['credentials']['akamai_edgerc_section'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Section of .edgerc file to use for the CCU API'),
    '#default_value' => variable_get('akamai_edgerc_section', 'default'),
    '#states' => $file_field_states,
    '#element_validate' => array('akamai_settings_form_validate_credential_field'),
    '#akamai_credential_type' => 'file',
  );

  $default_hostname = parse_url($GLOBALS['base_url'], PHP_URL_HOST);
  $form['akamai_hostname'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Hostname'),
    '#default_value' => variable_get('akamai_hostname', $default_hostname),
    '#description'   => t('The hostname that contains objects (paths) you want to purge, e.g. "www.example.com"'),
    '#required'      => TRUE,
  );
  if (drupal_multilingual()) {
    $form['akamai_use_language_domain'] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Use language domain for hostname.'),
      '#default_value' => variable_get('akamai_use_language_domain', FALSE),
      '#description'   => t('For multilingual sites, you may need to purge from multiple hostnames. Checking this box will cause the language domains specified in the <a href="!url">language settings</a> to be used as the hostname when submitting purge requests.', array('!url' => url('admin/config/regional/language'))),
    );
  }

  $form['akamai_timeout'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Timeout length'),
    '#description'   => t("The timeout used by when sending the cache purge request to Akamai's servers. Most users will not need to change this value."),
    '#size'          => 5,
    '#maxlength'     => 3,
    '#default_value' => variable_get('akamai_timeout', EdgeGridClient::DEFAULT_REQUEST_TIMEOUT),
    '#required'      => TRUE,
  );

  $form['akamai_network'] = array(
    '#type'          => 'radios',
    '#title'         => t('Domain'),
    '#default_value' => variable_get('akamai_network', CcuClientInterface::NETWORK_PRODUCTION),
    '#options'       => array(
      CcuClientInterface::NETWORK_STAGING    => t('Staging'),
      CcuClientInterface::NETWORK_PRODUCTION => t('Production'),
    ),
    '#description'   => t('The Akamai network to use for cache purge requests.'),
    '#required'      => TRUE,
  );

  $form['akamai_action'] = array(
    '#type'          => 'radios',
    '#title'         => t('Purge action'),
    '#default_value' => variable_get('akamai_action', 'invalidate'),
    '#options'       => array(
      'remove'     => t('Remove'),
      'invalidate' => t('Invalidate'),
    ),
    '#description'   => t('The default purging action. The options are <em>remove</em> (which removes the item from the Akamai cache) and <em>invalidate</em> (which leaves the item in the cache, but invalidates it so that the origin will be hit on the next request.)'),
    '#required'      => TRUE,
  );

  return system_settings_form($form);
}

/**
 * Sets an error if required credential fields are not filled in.
 */
function akamai_settings_form_validate_credential_field(&$element, &$form_state, $form) {
  $storage_type = $element['#akamai_credential_type'];
  $is_required = ($form_state['values']['akamai_credential_storage'] == $storage_type);
  $is_empty = (is_string($element['#value']) && drupal_strlen(trim($element['#value'])) == 0);
  if ($is_required && $is_empty) {
    form_error($element, t('!name field is required.', array('!name' => $element['#title'])));
  }
  elseif (!$is_required) {
    // This storage method associated with this feild is not being used, so
    // unset the value of this field.
    form_set_value($element, NULL, $form_state);
  }
}

/**
 * Sets an error if the form element's value is not a valid absolute URL.
 */
function akamai_settings_form_validate_url($element, &$form_state, $form) {
  $url = $form_state['values'][$element['#name']];
  if (!empty($url) && !valid_url($element['#value'], TRUE)) {
    form_error($element, t('The !name field must be a valid, absolute URL.', array('!name' => $element['#title'])));
  }
}

/**
 * Sets an error if the form element's value is not a path to an existing file.
 */
function akamai_settings_form_validate_file_exists($element, &$form_state, $form) {
  $path = $form_state['values'][$element['#name']];
  if (!empty($path) && !file_exists($path)) {
    form_error($element, t('Could not find the .edgerc file at %path. Please verify that it exists.', array('%path' => $path)));
  }
}

/**
 * Sets an error if the form element's value is not a path to a readable file.
 */
function akamai_settings_form_validate_file_is_readable($element, &$form_state, $form) {
  $path = $form_state['values'][$element['#name']];
  if (!empty($path) && !is_readable($path)) {
    form_error($element, t('Could not read the file at %path. Please verify that it is readable.', array('%path' => $path)));
  }
}

/**
 * Form Validation handler for akamai_settings_form().
 */
function akamai_settings_form_validate(&$form, &$form_state) {
  // Validate .edgerc file.
  if ($form_state['values']['akamai_credential_storage'] == 'file') {
    $section = $form_state['values']['akamai_edgerc_section'];
    $path = $form_state['values']['akamai_edgerc_path'];
    try {
      EdgeGridClient::createFromEdgeRcFile($section, $path);
    }
    catch (Exception $e) {
      form_set_error('akamai_edgerc_section', t('Could not validate .edgerc file. Exception: %message', array('%message' => $e->getMessage())));
    }
  }

  // Check if timeout value is an integer.
  $filtered_akamai_timeout = filter_var(
    $form_state['values']['akamai_timeout'],
    FILTER_VALIDATE_INT,
    array('options' => array('min_range' => 1)));
  if (!$filtered_akamai_timeout) {
    form_set_error('akamai_timeout', 'The Timeout Length must be an integer greater than 0.');
  }
}

/**
 * Form builder for manually purging Akamai.
 *
 * @see akamai_manual_purge_form_submit()
 */
function akamai_manual_purge_form() {
  $form = array();

  $form['paths'] = array(
    '#type'        => 'textarea',
    '#title'       => t('Paths/URLs'),
    '#description' => t('Enter one URL per line. Wildcards are not currently supported by v2 or v3 of the CCU REST API. URL entries should be relative to the basepath. (e.g. node/1, content/pretty-title, sites/default/files/some/image.png)'),
  );

  $form['domain_override'] = array(
    '#type'          => 'select',
    '#title'         => t('Network'),
    '#default_value' => variable_get('akamai_network', CcuClientInterface::NETWORK_PRODUCTION),
    '#options'       => array(
      CcuClientInterface::NETWORK_STAGING    => t('Staging'),
      CcuClientInterface::NETWORK_PRODUCTION => t('Production'),
    ),
    '#description'   => t('The Akamai network to use for cache purging. Defaults to the Domain setting from the settings page.'),
  );

  $form['purge_action'] = array(
    '#type'        => 'radios',
    '#title'       => t('Purge action'),
    '#default_value' => variable_get('akamai_action', ''),
    '#options'     => array(
      'remove'     => t('Remove'),
      'invalidate' => t('Invalidate'),
    ),
    '#description' => t('<b>Remove:</b> Purge the content from Akamai edge server caches. The next time the edge server receives a request for the content, it will retrieve the current version from the origin server. If it cannot retrieve a current version, it will follow instructions in your edge server configuration.<br/><br/><b>Invalidate:</b> Mark the cached content as invalid. The next time the Akamai edge server receives a request for the content, it will send an HTTP conditional get (If-Modified-Since) request to the origin. If the content has changed, the origin server will return a full fresh copy; otherwise, the origin normally will respond that the content has not changed, and Akamai can serve the already-cached content.<br/><br/><b>Note that <em>Remove</em> can increase the load on the origin more than <em>Invalidate</em>.</b> With <em>Invalidate</em>, objects are not removed from cache and full objects are not retrieved from the origin unless they are newer than the cached versions.'),
  );

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Start Purging Content'),
  );

  return $form;
}

/**
 * Form submission handler for akamai_manual_purge_form().
 */
function akamai_manual_purge_form_submit($form, &$form_state) {
  // filter_xss() does not need to be used here as it turns the & symbol into
  // an HTML entity and v3 rejects it. We don't need HTML entities here as this
  // data won't be used in actual HTML, just API requests.
  $paths = explode("\n", $form_state['values']['paths']);

  if ($result = akamai_purge_paths($paths)) {
    $paths['items'] = $paths;
    $message = t("Akamai purge request successful. The estimated completion time is @estimated_seconds seconds and the list of URLs submitted are:", array('@estimated_seconds' => $result->estimatedSeconds)) . theme("item_list", $paths);
    $status = 'status';

    // @todo Overhaul the $akamai_response_data logic, it is currently expecting
    // v2 response and doesn't report correct information.
    if (is_object($result) && !empty($result->data)) {
      if ($akamai_response_data = json_decode($result->data)) {
        if (isset($akamai_response_data->httpStatus) && $akamai_response_data->httpStatus > 300) {
          $message = t("There was a problem with your purge request. The error message returned was '@msg'",
            array('@msg' => $akamai_response_data->details));
          $status = 'error';
        }
        else {
          $message = t("Purge request successful. Akamai reports an estimated time to completion of @time",
            array('@time' => format_interval($akamai_response_data->estimatedSeconds))) . theme("item_list", $paths);
        }
      }
      else {
        $message = t('There was a problem with your purge request. Please check the watchdog logs for details.');
        $status = 'error';

        watchdog('akamai', 'Unable to parse Akamai API response data: @json_data',
          array('@json_data' => '<pre>' . print_r($result->data, TRUE) . '</pre>'), WATCHDOG_ERROR);
      }
    }
  }
  else {
    $message = t('There was a problem with your purge request. Please check the watchdog logs for details.');
    $status = 'error';
  }

  drupal_set_message($message, $status);
}

/**
 * Displays information about submitted purge requests.
 */
function akamai_purge_request_list() {
  $header = [
    [
      'data' => t('ID'),
    ],
    [
      'data' => t('Hostname'),
      'field' => 'hostname',
    ],
    [
      'data' => t('Paths'),
    ],
    [
      'data' => t('Status'),
      'field' => 'status',
    ],
    [
      'data' => t('Submission time'),
      'field' => 'submission_time',
      'sort' => 'DESC',
    ],
    [
      'data' => t('Estimated completion time'),
      'field' => 'check_after',
    ],
    [
      'data' => t('Completed'),
      'field' => 'completion_time',
    ],
  ];

  $query = db_select('akamai_purge_requests', 'pr')->extend('PagerDefault')->extend('TableSort')
    ->fields('pr', [
      'purge_id',
      'estimated_seconds',
      'check_after',
      'status',
      'submission_time',
      'completion_time',
      'hostname',
      'paths',
    ])
    ->limit(50)
    ->orderByHeader($header)
    ->orderBy('submission_time', 'DESC');
  $requests = $query->execute()->fetchAll();

  $rows = [];

  foreach ($requests as $request) {
    $submission_interval = format_interval(REQUEST_TIME - $request->submission_time, 1);
    $submission_string = t('@interval ago', ['@interval' => $submission_interval]);

    if (empty($request->completion_time)) {
      $completed_string = '';
    }
    else {
      $completed_interval = format_interval(REQUEST_TIME - $request->completion_time, 1);
      $completed_string = t('@interval ago', ['@interval' => $completed_interval]);
    }
    $rows[] = [
      l($request->purge_id, 'admin/reports/akamai/' . $request->purge_id),
      check_plain($request->hostname),
      count(unserialize($request->paths)),
      check_plain($request->status),
      $submission_string,
      _akamai_get_estimated_completion_string($request),
      $completed_string,
    ];
  }

  $build = [];
  $build['table'] = [
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No purge request data to display.'),
  ];
  $build['pager'] = ['#theme' => 'pager'];
  return $build;
}

/**
 * Presents information about a purge request.
 */
function akamai_purge_request_detail($id) {
  $query = db_select('akamai_purge_requests', 'pr')
    ->fields('pr')
    ->condition('purge_id', $id);
  $request = $query->execute()->fetchObject();

  if (empty($request)) {
    drupal_not_found();
    return;
  }

  $rows = [
    [
      ['data' => t('Status'), 'header' => TRUE],
      check_plain($request->status),
    ],
    [
      ['data' => t('Hostname'), 'header' => TRUE],
      check_plain($request->hostname),
    ],
    [
      ['data' => t('Paths'), 'header' => TRUE],
      theme('item_list', ['items' => unserialize($request->paths)]),
    ],
    [
      ['data' => t('ID'), 'header' => TRUE],
      check_plain($request->purge_id),
    ],
    [
      ['data' => t('Support ID'), 'header' => TRUE],
      check_plain($request->support_id),
    ],
    [
      ['data' => t('Progress URI'), 'header' => TRUE],
      check_plain($request->progress_uri),
    ],
    [
      ['data' => t('Submission time'), 'header' => TRUE],
      format_date($request->submission_time, 'long'),
    ],
    [
      ['data' => t('Estimated completion time'), 'header' => TRUE],
      _akamai_get_estimated_completion_string($request),
    ],
    [
      ['data' => t('Last checked'), 'header' => TRUE],
      empty($request->last_checked) ? '' : t('@interval ago', ['@interval' => format_interval(REQUEST_TIME - $request->last_checked)]),
    ],
  ];

  if (!empty($request->completion_time)) {
    $rows[] = [
      ['data' => t('Completion time'), 'header' => TRUE],
      empty($request->completion_time) ? '' : format_date($request->completion_time, 'long'),
    ];
    $rows[] = [
      ['data' => t('Time to complete'), 'header' => TRUE],
      format_interval($request->completion_time - $request->submission_time),
    ];
  }

  $build['table'] = [
    '#theme' => 'table',
    '#rows' => $rows,
  ];
  return $build;
}

/**
 * Generates a user-friendly string for the estimated completion time.
 */
function _akamai_get_estimated_completion_string($request) {
  $interval = format_interval(abs($request->check_after - REQUEST_TIME));
  if (!empty($request->completion_time)) {
    $string = t('Confirmed complete.');
  }
  elseif (REQUEST_TIME > $request->check_after) {
    $string = t('@interval ago', ['@interval' => $interval]);
  }
  else {
    $string = t('@interval from now', ['@interval' => $interval]);
  }
  return $string;
}
