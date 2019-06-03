<?php
/**
 * @file
 * campaignion_test.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function campaignion_test_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function campaignion_test_node_info() {
  $items = array(
    'email_to_target' => array(
      'name' => t('Email to Target action'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'petition' => array(
      'name' => t('Petition'),
      'base' => 'node_content',
      'description' => t('Petitions can be used to collect signatures for a specifc cause through a form that you can customize.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'thank_you_page' => array(
      'name' => t('Thank You page'),
      'base' => 'node_content',
      'description' => t('Your supporters will land on thank you pages after taking action or making a donation.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}

/**
 * Implements hook_default_redhen_contact_type().
 */
function campaignion_test_default_redhen_contact_type() {
  $items = array();
  $items['contact'] = entity_import('redhen_contact_type', '{
    "name" : "contact",
    "label" : "Contact",
    "locked" : "0",
    "weight" : "0",
    "data" : null
  }');
  return $items;
}
