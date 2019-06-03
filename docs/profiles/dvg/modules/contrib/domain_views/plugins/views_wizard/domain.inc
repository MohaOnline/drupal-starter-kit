<?php

/**
 * @file
 * Views wizard for domain views.
 */

$plugin = array(
  'name' => 'domain',
  'base_table' => 'domain',
  'created_column' => 'weight',
  'available_sorts' => array(
    'weight:DESC' => t('Sort order'),
  ),
  'form_wizard_class' => array(
    'file' => 'views_ui_base_views_wizard.class.php',
    'class' => 'ViewsUiBaseViewsWizard',
    'path' => drupal_get_path('module', 'views') . '/plugins/views_wizard',
  ),
  'title' => t('Domains'),
  'filters' => array(
    'valid' => array(
      'value' => 1,
      'table' => 'domain',
      'field' => 'valid',
    ),
  ),
);

