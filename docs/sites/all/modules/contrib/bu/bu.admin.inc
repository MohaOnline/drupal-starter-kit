<?php

/**
* @file
* The module settings form.
*/

function bu_admin_settings() {
  $form = array();

  $form['bu_browsers']['info'] = array(
    '#markup' => t('These are the settings which determine how the browser-update.org update notification displays. You can test this notification by appending #test-bu to the end of any URL. For example, !homepage-link.', array(
      '!homepage-link' => l(t('click here to test the notification on the homepage'), '<front>', array('fragment' => 'test-bu')),
    )),
  );
  $form['bu_browsers']['bu_browsers_ie'] = array(
    '#type' => 'select',
    '#title' => t('Internet Explorer'),
    '#options' => array(
      '-0.01' => t('Every outdated version'),
      '15' => t('<= 15'),
      '14' => t('<= 14'),
      '13' => t('<= 13'),
      '12' => t('<= 12'),
      '11' => t('<= 11'),
      '10' => t('<= 10'),
      '-6' => t('more than 6 versions behind'),
      '-5' => t('more than 5 versions behind'),
      '-4' => t('more than 4 versions behind'),
      '-3' => t('more than 3 versions behind'),
      '-2' => t('more than 2 versions behind'),
      '-1' => t('more than 1 versions behind'),
    ),
    '#default_value' => variable_get('bu_browsers_ie', '-6'),
  );
  $form['bu_browsers']['bu_browsers_firefox'] = array(
    '#type' => 'select',
    '#title' => t('Firefox'),
    '#options' => array(
      '-0.01' => t('Every outdated version'),
      '57' => t('<= 57'),
      '56' => t('<= 56'),
      '55' => t('<= 55'),
      '54' => t('<= 54'),
      '53' => t('<= 53'),
      '52' => t('<= 52'),
      '-6' => t('more than 6 versions behind'),
      '-5' => t('more than 5 versions behind'),
      '-4' => t('more than 4 versions behind'),
      '-3' => t('more than 3 versions behind'),
      '-2' => t('more than 2 versions behind'),
      '-1' => t('more than 1 versions behind'),
    ),
    '#default_value' => variable_get('bu_browsers_firefox', '-4'),
  );
  $form['bu_browsers']['bu_browsers_opera'] = array(
    '#type' => 'select',
    '#title' => t('Opera'),
    '#options' => array(
      '-0.01' => t('Every outdated version'),
      '50' => t('<= 50'),
      '49' => t('<= 49'),
      '48' => t('<= 48'),
      '47' => t('<= 47'),
      '46' => t('<= 46'),
      '45' => t('<= 45'),
      '-6' => t('more than 6 versions behind'),
      '-5' => t('more than 5 versions behind'),
      '-4' => t('more than 4 versions behind'),
      '-3' => t('more than 3 versions behind'),
      '-2' => t('more than 2 versions behind'),
      '-1' => t('more than 1 versions behind'),
    ),
    '#default_value' => variable_get('bu_browsers_opera', '-4'),
  );
  $form['bu_browsers']['bu_browsers_safari'] = array(
    '#type' => 'select',
    '#title' => t('Safari'),
    '#options' => array(
      '-0.01' => t('Every outdated version'),
      '10' => t('<= 10'),
      '9' => t('<= 9'),
      '8' => t('<= 8'),
      '7' => t('<= 7'),
      '6' => t('<= 6'),
      '5' => t('<= 5'),
      '-6' => t('more than 6 versions behind'),
      '-5' => t('more than 5 versions behind'),
      '-4' => t('more than 4 versions behind'),
      '-3' => t('more than 3 versions behind'),
      '-2' => t('more than 2 versions behind'),
      '-1' => t('more than 1 versions behind'),
    ),
    '#default_value' => variable_get('bu_browsers_safari', '-2'),
  );
  $form['bu_browsers']['bu_browsers_chrome'] = array(
    '#type' => 'select',
    '#title' => t('Chrome'),
    '#options' => array(
      '-0.01' => t('Every outdated version'),
      '63' => t('<= 63'),
      '62' => t('<= 62'),
      '61' => t('<= 61'),
      '60' => t('<= 60'),
      '59' => t('<= 59'),
      '58' => t('<= 58'),
      '-6' => t('more than 6 versions behind'),
      '-5' => t('more than 5 versions behind'),
      '-4' => t('more than 4 versions behind'),
      '-3' => t('more than 3 versions behind'),
      '-2' => t('more than 2 versions behind'),
      '-1' => t('more than 1 versions behind'),
    ),
    '#default_value' => variable_get('bu_browsers_chrome', '-4'),
  );
  $form['bu_browsers']['bu_settings_insecure'] = array(
    '#type' => 'checkbox',
    '#title' => t('Notify all browser versions with severe security issues.'),
    '#default_value' => variable_get('bu_settings_insecure', TRUE),
  );
  $form['bu_browsers']['bu_settings_unsupported'] = array(
    '#type' => 'checkbox',
    '#title' => t('Also notify all browsers that are not supported by the vendor anymore.'),
    '#default_value' => variable_get('bu_settings_unsupported', FALSE),
  );
  $form['bu_browsers']['bu_settings_mobile'] = array(
    '#type' => 'checkbox',
    '#title' => t('Notify mobile browsers.'),
    '#default_value' => variable_get('bu_settings_mobile', TRUE),
  );

  $form['bu_settings'] = array(
    '#type' => 'fieldset',
    '#title' => 'Settings',
    '#description' => 'Additional settings to change behavior of browser update script.',
  );
  $form['bu_settings']['bu_settings_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Custom message bar text'),
    '#description' => t('This text will be used instead of the default browser update message.'),
    '#default_value' => variable_get('bu_settings_text', ''),
  );
  $form['bu_settings']['bu_settings_position'] = array(
    '#type' => 'select',
    '#title' => t('Position'),
    '#description' => t('Set the display location of the message.'),
    '#options' => array(
      'top' => t('Top'),
      'bottom' => t('Bottom'),
      'corner' => t('Corner'),
    ),
    '#default_value' => variable_get('bu_settings_position', 'top'),
  );
  $form['bu_settings']['bu_settings_reminder'] = array(
    '#type' => 'select',
    '#title' => t('Reminder (in hours)'),
    '#description' => t('Set after how many hours the message should reappear. A value of 0 means "show all the time".'),
    '#options' => drupal_map_assoc(array(0, 1, 2, 4, 6, 12, 24, 36, 48)),
    '#default_value' => variable_get('bu_settings_reminder', 3),
  );
  $form['bu_settings']['bu_settings_reminder_closed'] = array(
    '#type' => 'select',
    '#title' => t('Reminder after closing (in hours)'),
    '#description' => t('Set after how many hours the message should reappear if the user explicity closes it.'),
    '#options' => drupal_map_assoc(array(0, 1, 2, 4, 6, 12, 24, 36, 48)),
    '#default_value' => variable_get('bu_settings_reminder_closed', 3),
  );
  $form['bu_settings']['bu_settings_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug'),
    '#description' => 'Always show the message bar.',
    '#default_value' => variable_get('bu_settings_debug', 0),
  );
  $form['bu_settings']['bu_settings_blank'] = array(
    '#type' => 'checkbox',
    '#title' => t('New window'),
    '#description' => 'Should link show up in a new windows/tab.',
    '#default_value' => variable_get('bu_settings_blank', 0),
  );
  $form['bu_settings']['bu_settings_hide_ignore'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide Ignore Button.'),
    '#description' => t('Setting this checkbox will cause the "ignore" button to be hidden on the notification.'),
    '#default_value' => variable_get('bu_settings_hide_ignore', FALSE),
  );
  $form['bu_settings']['bu_settings_source'] = array(
    '#type' => 'textfield',
    '#title' => t('Source File (base)'),
    '#description' => t('This can be used to override the source file for the update script to point elsewhere. By default, the script is located at %url. Leave this setting blank to use the default.', array(
      '%url' => '//browser-update.org/update.js',
    )),
    '#default_value' => variable_get('bu_settings_source', '//browser-update.org/update.js'),
  );
  $form['bu_settings']['bu_settings_show_source'] = array(
    '#type' => 'textfield',
    '#title' => t('Source File (display)'),
    '#description' => t('This can be used to override the source file for the script which shows the notification. This is only loaded if the user actually has an outdated browser. By default, the script is located at %url', array(
      '%url' => '//browser-update.org/update.show.min.js',
    )),
    '#default_value' => variable_get('bu_settings_show_source', '//browser-update.org/update.show.min.js'),
  );

  $access = user_access('use PHP for settings');
  $options = array(
    BU_VISIBILITY_NOTLISTED => t('All pages except those listed'),
    BU_VISIBILITY_LISTED => t('Only the listed pages'),
  );
  $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

  $form['bu_settings']['bu_visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Load Browser update script on specific pages'),
    '#options' => $options,
    '#default_value' => variable_get('bu_visibility', BU_VISIBILITY_NOTLISTED),
  );
  $form['bu_settings']['bu_pages'] = array(
    '#type' => 'textarea',
    '#title' => '<span class="element-invisible">Pages</span>',
    '#default_value' => variable_get('bu_pages', ''),
    '#description' => $description,
  );
  return system_settings_form($form);
}
