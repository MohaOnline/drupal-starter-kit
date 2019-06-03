<?php

/**
 * @file
 * Admin functions for the funnelback module.
 */

/**
 * Form builder for the Funnelback settings page.
 */
function funnelback_admin_form($form, &$form_state) {

  $form['description'] = [
    '#value' => 'These are the settings for Funnelback search integration.',
    '#prefix' => '<div>',
    '#suffix' => '</div>',
  ];
  $form['funnelback_base_url'] = [
    '#type' => 'textfield',
    '#title' => t('Base Url'),
    '#description' => t('The base url for the Funnelback interface (excluding trailing slash). For example: https://example.funnelback.com/'),
    '#size' => 60,
    '#maxlength' => 255,
    '#default_value' => variable_get('funnelback_base_url', 'https://example.funnelback.com/'),
    '#required' => TRUE,
  ];
  $form['funnelback_collection'] = [
    '#type' => 'textfield',
    '#title' => t('Collection Name'),
    '#description' => t('The Funnelback collection name'),
    '#size' => 30,
    '#maxlength' => 255,
    '#default_value' => variable_get('funnelback_collection', 'example'),
    '#required' => TRUE,
  ];
  $form['funnelback_profile'] = [
    '#type' => 'textfield',
    '#title' => t('Profile Name'),
    '#description' => t('Funnelback profile name'),
    '#size' => 30,
    '#maxlength' => 255,
    '#default_value' => variable_get('funnelback_profile', '_default'),
    '#required' => TRUE,
  ];
  $form['funnelback_autocomplete'] = [
    '#type' => 'fieldset',
    '#title' => t('Autocomplete settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  ];
  $form['funnelback_autocomplete']['funnelback_enable_autocomplete'] = [
    '#type' => 'checkbox',
    '#title' => t('Auto-completion'),
    '#default_value' => variable_get('funnelback_enable_autocomplete', NULL),
  ];
  $form['funnelback_autocomplete']['funnelback_autocomplete_number'] = [
    '#type' => 'textfield',
    '#title' => t('Results number'),
    '#description' => t('Set the number of results that autocomplete will popup.'),
    '#size' => 30,
    '#default_value' => variable_get('funnelback_autocomplete_number', '10'),
  ];
  $form['funnelback_result'] = [
    '#type' => 'fieldset',
    '#title' => t('Result display settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  ];
  $form['funnelback_result']['funnelback_enable_display_mode'] = [
    '#type' => 'checkbox',
    '#title' => t('Use display mode to render results'),
    '#default_value' => variable_get('funnelback_enable_display_mode', NULL),
  ];
  // Get all view modes list.
  $entity_info = entity_get_info('node');
  $view_modes = [];
  foreach ($entity_info['view modes'] as $key => $view_mode) {
    $view_modes[$key] = $view_mode['label'];
  }
  $form['funnelback_result']['funnelback_display_mode'] = [
    '#type' => 'select',
    '#title' => t('Select display mode to render you search results'),
    '#description' => t('You will need to add nodeId to your metamap in funnelback admin dashboard to use this feature. Content from remote site will use default search result layout.'),
    '#options' => $view_modes,
    '#default_value' => variable_get('funnelback_display_mode', NULL),
  ];
  $form['funnelback_result']['funnelback_custom_template'] = [
    '#type' => 'textfield',
    '#description' => t('Your custom template name if you are using a custom template. Warning: This is an experiment feature, related fields can be missing in custom template, make sure all necessary fields are included in your template. '),
    '#title' => t('Custom template name'),
    '#default_value' => variable_get('funnelback_custom_template', NULL),
  ];
  $form['funnelback_result']['funnelback_no_result_text'] = [
    '#type' => 'textarea',
    '#description' => t('Full HTML format is supported in this field. Use [funnelback-query] as token for the current search query.'),
    '#title' => t('Text when no result found'),
    '#default_value' => variable_get('funnelback_no_result_text', NULL),
  ];
  $form['funnelback_debug_mode'] = [
    '#type' => 'select',
    '#title' => t('Debugging'),
    '#options' => [
      'none' => t('None'),
      'log' => t('Log requests'),
      'verbose' => t('Verbose output'),
    ],
    '#default_value' => variable_get('funnelback_debug_mode', 'none'),
  ];

  return system_settings_form($form);
}
