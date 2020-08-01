<?php

use Drupal\little_helpers\Services\Container;

/**
 * Implements hook_field_info().
 */
function campaignion_email_to_target_field_info() {
  $info['email_to_target_options'] = [
    'label' => t('Email to target options'),
    'description' => t('Stores options for Email-To-Target actions.'),
    'settings' => [],
    'default_widget' => 'email_to_target_options',
    'default_formatter' => 'email_to_target_options',
  ];
  return $info;
}

/**
 * Implements hook_field_widget_info().
 */
function campaignion_email_to_target_field_widget_info() {
  $info['email_to_target_options'] = [
    'label' => t('Email to target options'),
    'field types' => ['email_to_target_options'],
    'settings' => [],
    'behaviors' => [
      'multiple values' => FIELD_BEHAVIOR_DEFAULT,
      'default values' => FIELD_BEHAVIOR_DEFAULT,
    ],
  ];
  return $info;
}

/**
 * Implements hook_field_formatter_info().
 */
function campaignion_email_to_target_field_formatter_info() {
  $info['email_to_target'] = array(
    'label' => t('No output.'),
    'field types' => ['email_to_target_options'],
  );
  return $info;
}

/**
 * Implements hook_field_is_empty().
 */
function campaignion_email_to_target_field_is_empty($item, $field) {
  return FALSE;
}

/**
 * Implements hook_field_widget_form().
 */
function campaignion_email_to_target_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $item = isset($items[$delta]) ? $items[$delta] : array();
  if (isset($instance['default_value'][$delta]) && !isset($items[$delta])) {
    $item = $instance['default_value'][$delta];
  }

  if ($field['type'] == 'email_to_target_options') {
    return _campaignion_email_to_target_options_field_widget_form($item);
  }
}

function _campaignion_email_to_target_options_field_widget_form($item) {
  $defaults = [
    'dataset_name' => '',
    'users_may_edit' => '',
    'selection_mode' => 'one_or_more',
  ];
  $item = drupal_array_merge_deep($defaults, $item);

  $options = [];
  try {
    $api = Container::get()->loadService('campaignion_email_to_target.api.Client');
    foreach ($api->getDatasetList() as $ds) {
      $options[$ds->key] = $ds->title;
    }
  }
  catch (\Exception $e) {
    watchdog_exception('campaignion_email_to_target', $e);
    drupal_set_message(t('We are sorry! The service is temporary unavailable. The administrators have been informed. Please try again in a few minutes …'), 'error');
  }
  $element['dataset_name'] = [
    '#type' => 'hidden',
    '#attributes' => ['class' => ['datasets-app-selected-dataset']],
    '#default_value' => $item['dataset_name'],
  ];
  $element['datasets_app'] = [
    '#type' => 'container',
    '#attributes' => ['class' => ['datasets-app']],
  ];
  $element['users_may_edit'] = [
    '#type' => 'checkbox',
    '#title' => t('Supporters may edit the message sent to the target(s).'),
    '#default_value' => $item['users_may_edit'],
  ];
  $element['selection_mode'] = [
    '#type' => 'radios',
    '#title' => t('Target selection mode'),
    '#default_value' => $item['selection_mode'],
    '#options' => Container::get()->loadService('campaignion_email_to_target.selection_mode.loader')->options(),
  ];
  return $element;
}

/**
 * Implements hook_field_formatter_view().
 */
function campaignion_email_to_target_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  return [];
}
