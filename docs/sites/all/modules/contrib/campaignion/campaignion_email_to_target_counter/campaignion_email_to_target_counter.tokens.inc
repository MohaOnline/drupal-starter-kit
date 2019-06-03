<?php

use Drupal\polling\UrlGenerator;

use Drupal\campaignion_email_to_target_counter\CountPolling;

/**
 * Implements hook_token_info().
 */
function campaignion_email_to_target_counter_token_info() {
  if (!drupal_static('menu_get_item') || !($node = menu_get_object())) {
    return;
  }
  $targets = new CountPolling($node->nid);

  $types['email-to-target-counter'] = [
    'name' => t('Email to target counter'),
    'description' => t('Shows an animated number for how many emails were sent to a target.'),
  ];

  $e2t = [];
  foreach ($targets as $id => $target) {
    $e2t[$id] = [
      'name' => t('Number of emails sent to @name', ['@name' => $target['label']]),
    ];
  }

  $tokens['email-to-target-counter'] = $e2t;
  return ['types' => $types, 'tokens' => $tokens];
}

/**
 * Recursively look at the parents of a field_collection_item to find a node.
 *
 * @param \FielCollectionItemEntity $item
 *   The field collection item to start with.
 *
 * @return null|object
 *   The first node found or NULL if there is no node.
 */
function _campaignion_email_to_target_field_collection_node(FieldCollectionItemEntity $item) {
  while ($item) {
    switch ($item->hostEntityType()) {
      case 'field_collection_item':
        $item = $item->hostEntity();
        break;
      case 'node':
        return $item->hostEntity();
      default:
        return NULL;
    }
  }
}

/**
 * Implements hook_tokens().
 */
function campaignion_email_to_target_counter_tokens($type, array $tokens, array $data = [], array $options = []) {
  if ($type != 'email-to-target-counter') {
    return;
  }
  // Support for field_collection.
  if (!isset($data['node']) && isset($data['field_collection_item'])) {
    $data['node'] = _campaignion_email_to_target_field_collection_node($data['field_collection_item']);
  }
  if (!isset($data['node'])) {
    return;
  }

  $settings = [];
  $counts = (new CountPolling($data['node']->nid))->getData()['campaignion_email_to_target_counter'];
  $url = UrlGenerator::instance()->entityUrl('node', $data['node']->nid);
  $replacements = [];
  $default_options = [
    'animated' => FALSE,
  ];
  foreach ($tokens as $name => $original) {
    $p = explode(':', $name);
    $target_id = array_shift($p);
    $options = $default_options;
    foreach ($p as $flag) {
      $options[$flag] = TRUE;
    }
    $html_id = drupal_html_id('e2t-counter');
    $count = isset($counts[$target_id]) ? $counts[$target_id]['count'] : 0;
    if ($options['animated']) {
      $settings['campaignion_email_to_target_counter'][$html_id] = [
        'target_id' => $target_id,
        'pollingURL' => $url,
        'current' => $count,
      ];
    }
    $replacements[$original] = "<span class=\"campaignion-email-to-target-counter\" data-counter-id=\"$html_id\">" . $count . "</span>";
  }
  if ($settings) {
    drupal_add_js(drupal_get_path('module', 'campaignion_email_to_target_counter') . '/campaignion_email_to_target_counter.js');
    drupal_add_js($settings, ['type' => 'setting']);
  }
  return $replacements;
}
