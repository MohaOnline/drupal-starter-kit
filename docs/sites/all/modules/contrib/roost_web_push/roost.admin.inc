<?php

/**
 * @file
 * Roost module admin page with Roost account stats.
 */

/**
 * Checks to see if Roost is logged in.
 */
function roost_admin_page() {
  $key = variable_get('roost_key', '');
  $secret = variable_get('roost_secret', '');
  if ($key && $secret) {
    return drupal_get_form('roost_plugin_form');
  }
  else {
    return drupal_get_form('roost_admin_form');
  }
}

/**
 * Implements hook_form().
 */
function roost_admin_form($node, &$form_state) {
  global $base_url;
  $favicon = $base_url . base_path() . drupal_get_path('module', 'roost') . '/images/roost_thumb.png';
  $type = theme_get_setting('favicon_mimetype');
  drupal_add_html_head_link(
    array(
      'rel' => 'shortcut icon',
      'href' => $favicon,
      'type' => $type)
    );

  module_load_include('inc', 'roost', 'roost.api');
  $values = isset($form_state['values']) ? $form_state['values'] : array('roost_user' => '');

  $form['#attached']['css'] = array(
    drupal_get_path('module', 'roost') . '/css/rooststyle.css',
  );

  $form['#attached']['js'] = array(
    drupal_get_path('module', 'roost') . '/js/roost.admin.js',
  );

  $form['login'] = array(
    '#prefix' => '<div class="rooster_box"><div id="roostLoginForm">',
    '#suffix' => '</div></div>',
  );

  $stage = isset($form_state['stage']) ? $form_state['stage'] : 1;

  if ($stage == 1) {
    $form['login']['roost_header'] = array(
      '#markup' => t('<div id="rooster"><div id="logo"></div><div class="rooster_text"><strong>Welcome! Login into your Roost account below.</strong><br />If you don\'t have an account,') . ' ' . l(t('sign up here.'), 'https://dashboard.goroost.com/signup?source=drupal&returnURL=' . $base_url . '/admin/config/administration/roost', array('attributes' => array('target' => '_blank'))) . '</div></div>',
    );
    $form['login']['roost_user'] = array(
      '#type' => 'textfield',
      '#title' => t('Username / Email'),
      '#default_value' => $values['roost_user'],
      '#size' => 35,
      '#maxlength' => 50,
      '#required' => TRUE,
      '#attributes' => array('placeholder' => t('Enter your Email')),
    );
    $form['login']['roost_pass'] = array(
      '#type' => 'password',
      '#title' => t('Password'),
      '#size' => 35,
      '#maxlength' => 50,
      '#required' => TRUE,
      '#attributes' => array('placeholder' => t('Enter your Password')),
    );
    $form['login']['forgot'] = array(
      '#markup' => l(t('forgot password?'), 'https://dashboard.goroost.com/', array('attributes' => array('target' => '_blank', 'id' => 'roostForgotPassword'))),
    );

    $form['login']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Login'),
    );
  }
  elseif ($stage == 2) {
    $form['login']['roost_header'] = array(
      '#markup' => '<div id="rooster"><div id="logo"></div><div class="rooster_text"><br />Select which site do you want to access, <br />or ' . l(t('sign in into another account.'), 'admin/config/administration/roost') . '</div></div>',
    );
    foreach ($form_state['apps'] as $key => $site) {
      $sites[$key] = $site->name;
      $web[$key] = $site->name . ':' . $site->key . ':' . $site->secret;
    }
    $form['login']['site'] = array(
      '#type' => 'select',
      '#title' => t('Choose Your Site'),
      '#options' => $sites,
      '#description' => t('To switch sites after you log in, you will need to log out.'),
    );

    $form['login']['web'] = array(
      '#value' => $web,
      '#type' => 'value',
    );

    $form['login']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Continue'),
    );
  }
  return $form;
}

/**
 * Implements hook_submit().
 */
function roost_admin_form_submit($form, &$form_state) {
  module_load_include('inc', 'roost', 'roost.api');
  $values = $form_state['values'];

  if (!isset($form_state['apps'])) {
    $login_into_roost = roost_login($values['roost_user'], $values['roost_pass']);
    $data = json_decode($login_into_roost->data);
    $site = 0;
  }
  else {
    $site = $values['site'];
    $web = $values['web'];
    $website = explode(':', $web[$site]);
    $data->apps[] = $form_state['apps'][$site];
    $data->success = TRUE;
  }

  if ($data->success === TRUE) {
    if (count($data->apps) == 1) {
      if (empty($website)) {
        variable_set('roost_key', $data->apps[$site]->key);
        variable_set('roost_name', $data->apps[$site]->name);
        variable_set('roost_secret', $data->apps[$site]->secret);
      }
      else {
        variable_set('roost_name', $website['0']);
        variable_set('roost_key', $website['1']);
        variable_set('roost_secret', $website['2']);
      }
      $status = t('<b>Welcome to Roost!</b> The plugin is up and running and visitors to your site using Safari on OS X Mavericks are currently being prompted to subscribe for push notifications. Once you have subscribers you\'ll be able see recent activity, all-time stats, and send manual push notifications. If you have questions or need support, just email us at <a href="mailto:support@goroost.com" target="_blank">support@goroost.com</a>.');
      drupal_set_message($status);
    }
    else {
      $form_state['rebuild'] = TRUE;
      $form_state['apps'] = $data->apps;
      $form_state['stage'] = 2;
    }
  }
  else {
    drupal_set_message(t('Please check your Email or Username and Password.'));
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Implements hook_form().
 */
function roost_plugin_form() {
  global $base_url;
  $favicon = $base_url . base_path() . drupal_get_path('module', 'roost') . '/images/roost_thumb.png';
  $path = drupal_get_path('module', 'roost');
  $type = theme_get_setting('favicon_mimetype');
  drupal_add_html_head_link(
    array(
      'rel' => 'shortcut icon',
      'href' => $favicon,
      'type' => $type,
    )
  );
  drupal_add_css($path . '/css/rooststyle.css');

  module_load_include('inc', 'roost', 'roost.api');

  $key = variable_get('roost_key', '');
  $secret = variable_get('roost_secret', '');

  $roost_stats = roost_get_stats($key, $secret);

  if (isset($roost_stats->data)) {
    $data = json_decode($roost_stats->data);
  }

  $form['user'] = array(
    '#prefix' => '<div id="roost_top_right">',
    '#markup' => '<div id="roost_user">' . check_plain(variable_get('roost_name', '')) . '</div>',
  );
  $form['signout'] = array(
    '#markup' => l(t('Sign out'), 'admin/config/administration/roost/signout'),
    '#suffix' => '</div>',
  );

  $form['all-time'] = array(
    '#prefix' => '<div class="" id="rooster_all_time">',
    '#type' => 'fieldset',
    '#title' => t('All-time stats'),
    '#suffix' => '</div>',
  );
  $form['all-time']['total'] = array(
    '#markup' => '<div><span class="roost-stat">' . number_format($data->registrations - $data->unsubscribes) . '</span>Total subscribers</div>',
  );
  $form['all-time']['notifications-sent'] = array(
    '#markup' => '<div><span class="roost-stat">' . number_format($data->notifications) . '</span>Total notifications sent</div>',
  );
  $form['all-time']['notifications-deliveries'] = array(
    '#markup' => '<div><span class="roost-stat">' . number_format($data->reads) . '</span>Total notifications read</div>',
  );

  $form['manual-push'] = array(
    '#prefix' => '<div class="" id="rooster_manual_push">',
    '#type' => 'fieldset',
    '#title' => t('Send a manual push notification'),
    '#suffix' => '</div>',
  );
  $form['manual-push']['manualtext'] = array(
    '#type' => 'textfield',
    '#title' => t('Message'),
    '#attributes' => array('placeholder' => t('Enter message')),
    '#description' => t('Enter the text for the message you would like to send your subscribers.'),
  );
  $form['manual-push']['manuallink'] = array(
    '#type' => 'textfield',
    '#title' => t('URL'),
    '#attributes' => array('placeholder' => t('Enter message URL (http://mysite.com)')),
    '#description' => t('Enter a website link (URL) that your subscribers will be sent to upon clicking the message.'),
  );
  $form['manual-push']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send notification'),
    '#validate' => array('roost_manual_push_notification_validate'),
    '#submit' => array('roost_manual_push_notification_submit'),
  );

//  $roost_remote_settings = roost_get_remote_settings($key, $secret);

//  if (!empty($roost_remote_settings->data)) {
//    $roost_remote_settings->data = json_decode($roost_remote_settings->data);
//  }

  $form['settings'] = array(
    '#prefix' => '<div class="" id="rooster_settings">',
    '#type' => 'fieldset',
    '#title' => t('Module Settings'),
    '#suffix' => '</div>',
  );
  $form['settings']['roost_auto_push'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto push'),
    '#default_value' => variable_get('roost_auto_push', FALSE),
    '#description' => t('Enabling this will automatically send a push notification to your subscribers every time you publish a new article or blog entry'),
  );
  $form['settings']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('roost_plugin_form_submit'),
  );
  return $form;
}

/**
 * Implements hook_submit().
 */
function roost_plugin_form_submit($form, $form_state) {
//  $roost_app_key = variable_get('roost_key');
//  $roost_app_secret = variable_get('roost_secret');

  $values = &$form_state['values'];
  variable_set('roost_auto_push', $values['roost_auto_push']);

//  $remote_content = array(
//  );
//
//  $remote_data = array(
//    'method' => 'PUT',
//    'remoteAction' => 'app',
//    'appkey' => $roost_app_key,
//    'appsecret' => $roost_app_secret,
//    'remoteContent' => json_encode($remote_content),
//  );
//  roost_remote_request($remote_data);
}

/**
 * Implements hook_validate().
 */
function roost_manual_push_notification_validate($form, $form_state) {
  if (!$form_state['values']['manualtext']) {
    form_set_error('manualtext', t('Your message can not be blank.'));
  }
  if (!$form_state['values']['manuallink']) {
    form_set_error('manuallink', t('Your link can not be blank.'));
  }
  else {
    $manual_link = $form_state['values']['manuallink'];
    if (strpos($manual_link, 'http') === FALSE) {
      $manual_link = 'http://' . $manual_link;
    }
    if (!valid_url($manual_link, TRUE)) {
      form_set_error('manuallink', t('Your link is invalid.'));
    }
  }
}

/**
 * Implements hook_submit().
 */
function roost_manual_push_notification_submit($form, $form_state) {
  $values = $form_state['values'];
  $manual_text = $values['manualtext'];
  $manual_link = $values['manuallink'];
  $roost_app_key = variable_get('roost_key', '');
  $roost_app_secret = variable_get('roost_secret', '');
  if ($manual_link && strpos($manual_link, 'http') === FALSE) {
    $manual_link = 'http://' . $manual_link;
  }
  $msg_status = roost_send_notification($manual_text, $manual_link, FALSE, $roost_app_key, $roost_app_secret);
  $msg_status->data = json_decode($msg_status->data);
  if ($msg_status->data->success === TRUE) {
    $status = t('Message Sent.');
  }
  else {
    $status = $msg_status->data->error;
  }
  drupal_set_message(check_plain($status));
}

/**
 * Removes account specific database values used by Roost module on Sign Out.
 */
function roost_admin_signout() {
  variable_del('roost_key');
  variable_del('roost_secret');
  variable_del('roost_name');
  drupal_goto('admin/config/administration/roost');
}
