<?php

/**
 * @file
 * Defines the mc_interest webform component.
 */

use Drupal\little_helpers\Webform\Webform;
use Drupal\campaignion_newsletters\NewsletterList;

require_once drupal_get_path('module', 'webform') . '/components/select.inc';

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_mc_interests() {
  $component = _webform_defaults_select();
  $component['extra']['multiple'] = TRUE;
  return $component;
}

/**
 * Helper function to set options based on the current node.
 */
function _webform_set_options_mc_interests(&$component) {
  // This might be called many times for one component in one request.
  $cache = &drupal_static(__FUNCTION__);
  $nid = $component['nid'];

  if (!isset($cache[$nid])) {
    // Get groups based on newsletter components in the same form.
    $webform = new Webform(node_load($component['nid']));
    $groups = [];
    foreach ($webform->componentsByType('opt_in') as $c) {
      if ($c['extra']['channel'] == 'email') {
        foreach ($c['extra']['lists'] as $list_id => $enabled) {
          if ($enabled) {
            $list = NewsletterList::load($list_id);
            $groups += $list->data->groups;
          }
        }
      }
    }
    $cache[$nid] = $groups;
  }

  $component['extra']['items'] = _webform_select_options_to_text($cache[$nid]);
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_mc_interests($component, $value = NULL, $filter = TRUE) {
  _webform_set_options_mc_interests($component);
  return _webform_render_select($component, $value, $filter);
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_mc_interests($component) {
  _webform_set_options_mc_interests($component);
  $form = _webform_edit_select($component);
  unset($form['extra']['items']);
  unset($form['extra']['options_source']);
  unset($form['extra']['other_option']);
  unset($form['extra']['other_text']);
  unset($form['items']['options']['option_settings']);
  // The options_element widget is left in for now although only the default
  // value has any effect. Otherwise there would be no way to select the values.
  /* unset($form['items']); */
  return $form;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_mc_interests($component, $value, $format = 'html', $submission = array()) {
  return _webform_display_select($component, $value, $format, $submission);
}

/**
 * Implements _webform_submit_component().
 */
function _webform_submit_mc_interests($component, $value) {
  _webform_set_options_mc_interests($component);
  return _webform_submit_select($component, $value);
}

/**
 * Implements _webform_analysis_component().
 */
function _webform_analysis_mc_interests($component, $sids = array(), $single = FALSE, $join = NULL) {
  _webform_set_options_mc_interests($component);
  return _webform_analysis_select($component, $sids = array(), $single, $join);
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_mc_interests($component, $value) {
  _webform_set_options_mc_interests($component);
  return _webform_table_select($component, $value);
}

/**
 * Implements _webform_action_set_component().
 */
function _webform_action_set_mc_interests($component, &$element, &$form_state, $value) {
  return _webform_action_set_select($component, $element, $form_state, $value);
}

/**
 * Implements _webform_csv_headers_component.
 */
function _webform_csv_headers_mc_interests($component, $export_options) {
  _webform_set_options_mc_interests($component);
  return _webform_csv_headers_select($component, $export_options);
}

/**
 * Implements _webform_csv_headers_component.
 */
function _webform_csv_data_mc_interests($component, $export_options, $value) {
  _webform_set_options_mc_interests($component);
  return _webform_csv_data_select($component, $export_options, $value);
}
