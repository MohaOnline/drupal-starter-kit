<?php

/**
 * @file
 * Administrative page callbacks for the finteza_analytics module.
 */

/**
 * Settings page layout.
 */
function finteza_analytics_admin_page() {
  $page['content']['#attached']['js'][] = array(
    'data' => drupal_get_path('module', 'finteza_analytics') . '/finteza_analytics.admin.js',
  );

  $page['content']['#attached']['css'][] = array(
    'data' => drupal_get_path('module', 'finteza_analytics') . '/finteza_analytics.admin.css',
  );

  $website_id = variable_get('finteza_analytics_website_id', '');
  $page['tracking_settings'] = drupal_get_form('finteza_analytics_tracking_settings');

  if (empty($website_id)) {
    $page['registration'] = drupal_get_form('finteza_analytics_registration');
  }

  $page['getting_started'] = drupal_get_form('finteza_analytics_getting_started');
  $page['how_to'] = drupal_get_form('finteza_analytics_how_to');
  $page['stats'] = drupal_get_form('finteza_analytics_stats');

  return $page;
}

/**
 * The module settings form.
 */
function finteza_analytics_tracking_settings($form, $form_state) {
  $website_id = variable_get('finteza_analytics_website_id', '');
  $website_id_description = "";

  if (empty($website_id)) {
    $website_id_description = t(
      "<a href='@registration_url' target='_blank'>Register</a> an account in Finteza",
      finteza_analytics_urls()
    );
  }

  $form['tracking_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tracking settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['tracking_settings']['finteza_analytics_website_id'] = array(
    '#default_value' => $website_id,
    '#title' => t('Website ID'),
    '#type' => 'textfield',
    '#description' => $website_id_description,
  );

  $tracking_settings = finteza_analytics_tracking_settings_get();
  $tracking_settings_items = array(
    "track_hash" => t('Track hash changes in the address bar'),
    "track_links" => t('Track outbound links'),
    "time_on_page" => t('Exact time on website'),
    "dont_track_admins" => t('Disable tracking of admin visits'),
  );
  $tracking_settings_values = array();

  foreach ($tracking_settings as $key => $value) {
    if (!!$value) {
      array_push($tracking_settings_values, $key);
    }
  }

  $form['tracking_settings']['finteza_analytics_tracking_settings'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Tracking settings'),
    '#default_value' => $tracking_settings_values,
    '#options' => $tracking_settings_items,
  );

  $form['tracking_settings']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save Changes'),
  );

  return $form;
}

/**
 * The module registration form.
 */
function finteza_analytics_registration($form, $form_state) {
  $form['registration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Registration'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $utc_offset = format_date(time(), 'custom', 'Z') / 60 * -1;

  $form['registration']['finteza_analytics_offset'] = array(
    '#default_value' => $utc_offset,
    '#type' => 'hidden',
  );

  $form['registration']['finteza_analytics_name'] = array(
    '#required' => TRUE,
    '#title' => t('Your full name'),
    '#type' => 'textfield',
  );

  $form['registration']['finteza_analytics_company'] = array(
    '#required' => FALSE,
    '#title' => t('Company'),
    '#type' => 'textfield',
  );

  $form['registration']['finteza_analytics_email'] = array(
    '#required' => TRUE,
    '#title' => t('Email'),
    '#type' => 'textfield',
    '#element_validate' => array('finteza_analytics_email_element_validate'),
  );

  $form['registration']['finteza_analytics_password'] = array(
    '#required' => TRUE,
    '#title' => t('Password'),
    '#type' => 'password',
  );

  $form['registration']['finteza_analytics_policy'] = array(
    '#type' => 'checkbox',
    "#title" => t(
      "I have read and understood <a href='@privacy_url' target='_blank'>privacy and data protection policy</a>",
      finteza_analytics_urls()
    ),
    '#required' => TRUE,
  );

  $form['registration']['finteza_analytics_agreement'] = array(
    '#type' => 'checkbox',
    "#title" => t(
      "I agree to <a href='@agreement_url' target='_blank'>subscription service agreement</a>",
      finteza_analytics_urls()
    ),
    '#required' => TRUE,
  );

  $form['registration']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Register'),
  );

  return $form;
}

/**
 * Form submit handler for tracking settings form.
 */
function finteza_analytics_tracking_settings_submit($form, &$form_state) {
  $website_id = $form_state['values']['finteza_analytics_website_id'];
  $tracking_settings = $form_state['values']['finteza_analytics_tracking_settings'];

  foreach ($tracking_settings as $key => $value) {
    $tracking_settings[$key] = !!$value;
  }

  variable_set('finteza_analytics_website_id', $website_id);
  variable_set('finteza_analytics_tracking_settings', $tracking_settings);

  drupal_set_message(t("Settings has been saved successfully."));
}

/**
 * Form submit handler for registration form.
 */
function finteza_analytics_registration_submit($form, &$form_state) {
  $values = $form_state['values'];
  $message = "";
  $message_type = "status";
  $registration = finteza_analytics_register_website(
    array(
      'website'    => url(NULL, array('absolute' => TRUE)),
      'email'      => $values['finteza_analytics_email'],
      'password'   => $values['finteza_analytics_password'],
      'fullname'   => $values['finteza_analytics_name'],
      'company'    => $values['finteza_analytics_company'],
      'utc_offset' => $values['finteza_analytics_offset'],
    )
  );

  if ($registration && $registration['status'] === 1) {
    variable_set('finteza_analytics_website_id', $registration['website']);
    $message = t("<b>Registration completed successfully.</b> Please activate your account using the link sent to your email.");
  }
  else {
    $error = $registration && isset($registration['error']) ? $registration['error'] : NULL;
    $message_type = "error";
    $form_state['rebuild'] = TRUE;

    switch ($error) {
      case 1:
        $message = t('An account with this email address already exists');
        break;

      case 2:
        $message = t('Invalid password');
        break;

      case 3:
        $message = t('Invalid email address');
        break;

      case 4:
        $message = t('A company with this name already exists');
        break;

      case 5:
        $message = t('Invalid website address');
        break;

      case 6:
        $message = t('Registration limit exceeded');
        break;

      default:
        $message = t('Registration error');
    }
  }
  drupal_set_message($message, $message_type);
}

/**
 * The module getting_started form.
 */
function finteza_analytics_getting_started($form, $form_state) {
  if (module_exists('domain_variable')) {
    return variable_module_form($form, $form_state, 'finteza_analytics');
  }

  $form['getting_started'] = array(
    '#type' => 'fieldset',
    '#title' => t('Getting Started'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $output = '';
  $output .= '<p>';
  $output .= t(
    "How to use the plugin:<br /><br />&nbsp;1. <a href='@registration_url' target='_blank'>Register</a> an account in Finteza<br />&nbsp;2. Save the generated website ID in the settings<br />&nbsp;3. Configure tracking of link click events<br />&nbsp;4. View your website visit statistics in the <a href='@dashboard_url' target='_blank'>Finteza dashboard</a>",
    finteza_analytics_urls()
  );
  $output .= '</p>';

  $form['getting_started']['tracking_title'] = array(
    '#markup' => $output,
  );
  return $form;
}

/**
 * The module how_to form.
 */
function finteza_analytics_how_to($form, $form_state) {
  if (module_exists('domain_variable')) {
    return variable_module_form($form, $form_state, 'finteza_analytics');
  }

  $form['how_to'] = array(
    '#type' => 'fieldset',
    '#title' => t('How to track clicks'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $output = '';
  $output .= '<p>';
  $output .= t(
    "To enable tracking of link click events in your website:<br />&nbsp;<br />&nbsp;1. Open a website page or message for editing<br />&nbsp;2. In the text editor, select the link element and click on the Finteza button<br />&nbsp;3. Enter the click event name to be used in statistics<br />&nbsp;4. View event statistics in the <a href='@dashboard_url' target='_blank'>Finteza dashboard</a><br /><p>For the event addition button to appear, you should install the <a href='@ckeditor_url' target='_blank'>CKEditor</a> module and enable plugin use in the editor profile.</p>",
    finteza_analytics_urls()
  );
  $output .= '</p>';

  $form['how_to']['tracking_title'] = array(
    '#markup' => $output,
  );
  return $form;
}

/**
 * The module stats form.
 */
function finteza_analytics_stats($form, $form_state) {
  if (module_exists('domain_variable')) {
    return variable_module_form($form, $form_state, 'finteza_analytics');
  }

  $form['stats'] = array(
    '#type' => 'fieldset',
    '#title' => t('Where to view statistics'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $output = '';
  $output .= '<p>';
  $output .= t(
    "Statistics on your website visits is collected in the <a href='@dashboard_url' target='_blank'>Finteza dashboard</a>. Log in using the email and password specified during registration. If you forgot the password, use the <a href='@password_recovery_url' target='_blank'>password recovery</a> page",
    finteza_analytics_urls()
  );
  $output .= '</p>';

  $form['stats']['tracking_title'] = array(
    '#markup' => $output,
  );
  return $form;
}

/**
 * Provides urls for translations.
 */
function finteza_analytics_urls() {
  $registration_url = '#finteza-analytics-registration';

  if (variable_get('finteza_analytics_website_id', '')) {
    $registration_url = FINTEZA_ANALYTICS_REGISTRATION_URL;
  }

  return array(
    '@registration_url' => $registration_url,
    '@dashboard_url' => FINTEZA_ANALYTICS_DASHBOARD_URL,
    '@password_recovery_url' => FINTEZA_ANALYTICS_PASSWORD_RECOVERY_URL,
    '@privacy_url' => FINTEZA_ANALYTICS_PRIVACY_URL,
    '@agreement_url' => FINTEZA_ANALYTICS_AGREEMENT_URL,
    '@ckeditor_url' => FINTEZA_ANALYTICS_CKEDITOR_URL,
  );
}
