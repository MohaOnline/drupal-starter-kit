<?php

/**
 * @file
 * Easychart admin pages.
 */

/**
 * Returns a form with configuration options for the options.
 */
function easychart_admin_options($form, $form_state) {

  if (!isset($form_state['storage']['confirm'])) {
    $form = array();

    $form['easychart_options_intro'] = array(
      '#markup' => t("These Highcharts options will be configurable in the Easychart interface when creating/editing a chart. See !url for all available options.", array('!url' => l('http://api.highcharts.com/highcharts', 'http://api.highcharts.com/highcharts', array('attributes' => array('target' => '_blank'))))),
    );

    $default_options = '';
    if (file_exists('public://easychart-options.json')) {
      $default_options = file_get_contents('public://easychart-options.json');
    }

    $form['easychart_options'] = array(
      '#type' => 'textarea',
      '#default_value' => $default_options,
      '#attributes' => array('class' => array('easychart-options')),
      '#rows' => 15,
    );

    libraries_load('highcharts');
    libraries_load('easychart');
    drupal_add_library('system', 'ui.sortable');
    drupal_add_js(drupal_get_path('module', 'easychart') . '/js/easychart.admin.js');
    drupal_add_css(drupal_get_path('module', 'easychart') . '/css/easychart.admin.css');

    // Add the reset button.
    $form['actions']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset to defaults'),
      '#submit' => array('easychart_admin_options_submit'),
      '#limit_validation_errors' => array(),
      '#weight' => 100,
    );

    // Extra validate to handle merging of options object with dump.json.
    $form = system_settings_form($form);
    $form['#submit'][0] = 'easychart_admin_options_merge_presave';
    $form['#submit'][1] = 'system_settings_form_submit';
    return $form;
  }
  else {
    return confirm_form($form, t('Are you sure you want to reset the options to their default values?'), current_path());
  }

}

/**
 * Form submit handler
 */
function easychart_admin_options_merge_presave($form, &$form_state) {
  // Merge options with dump.xml
  $current_options = json_decode($form_state['values']['easychart_options']);

  // Get all options.
  $full_options = drupal_json_decode(file_get_contents(libraries_get_path('easychart') . '/src/js/config/dump.json'));
  $all_options = array();
  foreach ($full_options as $key => $value) {
    $all_options[$value['fullname']] = $value;
  }

  // Start iterating and find the panes.
  foreach ($current_options as $key => $info) {
    if (isset($info->panes)) {
      foreach ($info->panes as $s_key => $s_value) {
        if (isset($s_value->options)) {
          foreach ($s_value->options as $ss_key => $ss_value) {
            if ($ss_value->fullname) {
              $current_options[$key]->panes[$s_key]->options[$ss_key] = (object) array_merge((array) $ss_value, $all_options[$ss_value->fullname]);
            }
          }
        }
      }
    }
  }

  // Write to file.
  file_unmanaged_save_data(json_encode($current_options), 'public://easychart-options.json', FILE_EXISTS_REPLACE);
}

/**
 * Form submit handler
 */
function easychart_admin_options_submit($form, &$form_state) {
  // Handle reset button.
  if (!isset($form_state['storage']['confirm'])) {
    $form_state['storage']['values'] = $form_state['values'];
    $form_state['storage']['confirm'] = TRUE;
    $form_state['rebuild'] = TRUE;
  }
  else {
    // Exclude unnecessary elements.
    form_state_values_clean($form_state);

    // Remove the stored options.
    file_unmanaged_delete('public://easychart-options.json');

    drupal_set_message(t('The configuration options have been reset to their default values.'));
  }
}

/**
 * Returns a form with templates.
 */
function easychart_admin_templates($form, $form_state) {

  if(!isset($form_state['storage']['confirm'])) {
    $form = array();

    libraries_load('highcharts');
    libraries_load('easychart');
    drupal_add_js(drupal_get_path('module', 'easychart') . '/js/easychart.admin.js');
    drupal_add_css(drupal_get_path('module', 'easychart') . '/css/easychart.admin.css');

    $form['easychart_templates'] = array(
      '#type' => 'textarea',
      '#title' => t('Available templates'),
      '#default_value' => variable_get('easychart_templates', ''),
      '#description' => t("These templates will be selectable in the Easychart interface when creating/editing a chart."),
      '#attributes' => array('class' => array('easychart-templates')),
      '#rows' => 15,
    );

    // Add the reset button.
    $form['actions']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset to defaults'),
      '#submit' => array('easychart_admin_templates_submit'),
      '#limit_validation_errors' => array(),
      '#weight' => 100,
    );
  }
  else {
    return confirm_form($form, t('Are you sure you want to reset the templates to their default values?'), current_path());
  }

  return system_settings_form($form);
}

/**
 * Form submit handler
 */
function easychart_admin_templates_submit($form, &$form_state) {
  if(!isset($form_state['storage']['confirm'])) {
    $form_state['storage']['values'] = $form_state['values'];
    $form_state['storage']['confirm'] = TRUE;
    $form_state['rebuild'] = TRUE;
  }
  else {
    // Exclude unnecessary elements.
    form_state_values_clean($form_state);

    // Remove the variables.
    variable_del('easychart_templates');

    drupal_set_message(t('The configuration options have been reset to their default values.'));
  }
}

/**
 * Admin presets.
 */
function easychart_admin_presets($form, $form_state) {

  if (!isset($form_state['storage']['confirm'])) {
    $form = array();

    libraries_load('highcharts');
    libraries_load('easychart');
    drupal_add_library('system', 'ui.sortable');
    drupal_add_js(drupal_get_path('module', 'easychart') . '/js/easychart.admin.js');
    drupal_add_css(drupal_get_path('module', 'easychart') . '/css/easychart.admin.css');

    $form['easychart_presets'] = array(
      '#type' => 'textarea',
      '#title' => t('Presets'),
      '#default_value' => variable_get('easychart_presets', ''),
      '#description' => t('Presets for every Easychart chart. If these preset are also mentioned in the available options, they will be shown, but not editable. <a href="@url" target="_blank">View an example</a>.', array(
        '@url' => 'https://github.com/bestuurszaken/easychart/tree/3.x#instancesetpresets',
      )),
      '#attributes' => array('class' => array('easychart-presets')),
      '#rows' => 10,

    );

    // Add the reset button.
    $form['actions']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset to defaults'),
      '#submit' => array('easychart_admin_presets_submit'),
      '#limit_validation_errors' => array(),
      '#weight' => 100,
    );
  }
  else {
    return confirm_form($form, t('Are you sure you want to reset the presets to their default values?'), current_path());
  }

  return system_settings_form($form);
}

/**
 * Form submit handler
 */
function easychart_admin_presets_submit($form, &$form_state) {
  if (!isset($form_state['storage']['confirm'])) {
    $form_state['storage']['values'] = $form_state['values'];
    $form_state['storage']['confirm'] = TRUE;
    $form_state['rebuild'] = TRUE;
  }
  else {
    // Exclude unnecessary elements.
    form_state_values_clean($form_state);

    // Remove the variables.
    variable_del('easychart_presets');

    drupal_set_message(t('The configuration options have been reset to their default values.'));
  }
}

/**
 * Settings form.
 */
function easychart_admin_settings($form, $form_state) {
  $form = array();

  $form['easychart_url_update_frequency'] = array(
    '#type' => 'select',
    '#options' => array(0 => t('Never')) + drupal_map_assoc(array(3600, 10800, 21600, 43200, 86400, 604800), 'format_interval'),
    '#title' => t('External data update interval'),
    '#description' => t('Data from external csv files are cached for performance reasons. Decide how often this data should be refreshed.'),
    '#default_value' => variable_get('easychart_url_update_frequency', 60 * 60 * 1),
  );

  return system_settings_form($form);
}
