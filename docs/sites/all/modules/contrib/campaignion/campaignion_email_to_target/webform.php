<?php

/**
 * @file
 * Webform module email_to_target_selector component.
 */

use \Drupal\campaignion_action\Loader;
use \Drupal\campaignion_email_to_target\Message;
use \Drupal\little_helpers\ArrayConfig;
use \Drupal\little_helpers\Webform\Webform;

/**
 * Implements _webform_defaults_[component]().
 */
function _webform_defaults_e2t_selector() {
  return array(
    'name' => t('Select your targets'),
    'form_key' => 'email_to_target_selector',
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'mandatory' => 1,
    'extra' => array(
      'width' => '',
      'unique' => 0,
      'disabled' => 0,
      'title_display' => 'none',
      'description' => '',
      'attributes' => array(),
      'private' => FALSE,
    ),
  );
}

/**
 * Implements _webform_edit_[component]().
 */
function _webform_edit_e2t_selector($component) {
  return [];
}

/**
 * Implements _webform_render_[component]().
 */
function _webform_render_e2t_selector($component, $value = NULL, $filter = TRUE) {
  $component['extra'] += [
    'title_display' => 'before',
  ];
  $element = [
    '#type' => 'container',
    '#theme' => 'campaignion_email_to_target_selector_placeholder',
    '#theme_wrappers' => ['container', 'webform_element'],
    '#title' => $component['name'],
    '#title_display' => $component['extra']['title_display'],
    '#required' => TRUE,
    '#weight' => isset($component['weight']) == TRUE ? $component['weight'] : 0,
    '#webform_component' => $component,
  ];
  return $element;
}

/**
 * Implements _webform_display_[component]().
 */
function _webform_display_e2t_selector($component, $value, $format = 'html') {
  return [
    '#markup' => isset($value[0]) ? $value[0] : '',
  ];
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_e2t_selector($component, $value) {
  if (_webform_show_single_target_e2t_selector($component['nid']) && !empty($value[0])) {
    $data = (array) unserialize($value[0]);
    ArrayConfig::mergeDefaults($data, [
      'message' => [],
      'target' => ['salutation' => NULL, 'political_affiliation' => NULL],
      'constituency' => ['name' => NULL, 'country' => ['name' => NULL]],
    ]);
    $data['message'] = new Message($data['message']);
    return ['data' => [
      '#theme' => 'campaignion_email_to_target_results_table_entry',
      '#data' => $data,
      '#component' => $component,
    ]];
  }
  else {
    return check_plain(empty($value[0]) ? '' : $value[0]);
  }
}

function _webform_show_single_target_e2t_selector($nid) {
  // Static cache as workaround for not having a proper plugin-system.
  static $static_cache;
  if (!isset($static_cache)) {
    $static_cache = &drupal_static(__FUNCTION__, []);
  }
  if (isset($static_cache[$nid])) {
    return $static_cache[$nid];
  }
  if (empty($nid)) {
    return FALSE;
  }
  $return = FALSE;
  if (($node = node_load($nid)) && $node->type == 'email_to_target') {
    $action = Loader::instance()->actionFromNode($node);
    $return = $action->dataset()->key == 'mp';
  }
  $static_cache[$nid] = $return;
  return $return;
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_e2t_selector($component, $export_options) {
  $multi_column = user_access('view email_to_target messages') && _webform_show_single_target_e2t_selector($component['nid']);
  if ($multi_column) {
    $header = [
      ['', '', '', '', '', '', ''],
      [$component['name'], '', '', '', '', '', ''],
      [
        t('To'),
        t('Subject'),
        t('Message'),
        t('Constituency'),
        t('Target salutation'),
        t('Party'),
        t('Devolved country'),
      ],
    ];
  }
  else {
    $header = [];
    $header[0] = '';
    $header[1] = '';
    $header[2] = $component['name'];
  }
  return $header;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_e2t_selector($component, $export_options, $value) {
  if (!user_access('view email_to_target messages')) {
    return t('No permission to view email to target messages.');
  }
  if (_webform_show_single_target_e2t_selector($component['nid'])) {
    // Three columns: To, Subject, Message
    if (!empty($value[0])) {
      $data = (array) unserialize($value[0]);
      ArrayConfig::mergeDefaults($data, [
        'message' => [],
        'target' => ['name' => NULL, 'political_affiliation' => NULL],
        'constituency' => ['name' => NULL, 'country' => ['name' => NULL]],
      ]);
      $message = new Message($data['message']);
      $t = 'campaignion_email_to_target_mail';
      $m = theme([$t, $t . '_' . $component['nid']], ['message' => $message]);
      return [
        $message->to,
        $message->subject,
        $m,
        $data['constituency']['name'],
        $data['target']['salutation'],
        $data['target']['political_affiliation'],
        $data['constituency']['country']['name'],
      ];
    }
    else {
      return ['', '', '', '', '', '', ''];
    }
  }
  else {
    return empty($value[0]) ? '' : $value[0];
  }
}

/**
 * Implements _webform_form_builder_map_<webform-component>().
 */
function _webform_form_builder_map_e2t_selector() {
  return [
    'form_builder_type' => 'e2t_selector',
  ];
}
