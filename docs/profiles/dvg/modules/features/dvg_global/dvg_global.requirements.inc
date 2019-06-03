<?php

/**
 * Implements hook_requirements().
 */
function dvg_global_requirements($phase) {
  if ($phase == 'runtime') {
    $requirements = array(
      'dvg_global' => array(
        'title' => t('DVG configuration'),
        'value' => t('Ok'),
        'severity' => REQUIREMENT_OK,
      ),
    );
    if (dvg_global_status_report(TRUE)) {
      $requirements['dvg_global']['value'] = t('Problems detected');
      $requirements['dvg_global']['severity'] = REQUIREMENT_ERROR;
      $link = l(t('DVG Status'), 'admin/reports/dvg-status');
      $requirements['dvg_global']['description'] = t('Check the !link for problems', array('!link' => $link));
    }
    return $requirements;
  }
}

/**
 * Page callback that displays a status report of DVG configuration.
 */
function dvg_global_status_report($check = FALSE) {
  // Load .install files
  include_once DRUPAL_ROOT . '/includes/install.inc';

  // Check run-time requirements and status information.
  $requirements = module_invoke_all('dvg_requirements');
  usort($requirements, '_system_sort_requirements');

  if ($check) {
    return drupal_requirements_severity($requirements) == REQUIREMENT_ERROR;
  }

  return theme('status_report', array('requirements' => $requirements));
}

/**
 * Implements hook_dvg_requirements().
 */
function dvg_global_dvg_requirements() {
  $requirements = array();

  // Caching.
  $cache_description = l(t('Caching configuration'), 'admin/config/development/performance');
  $requirements['dvg_global_cache'] = array(
    'title' => t('Cache pages'),
    'description' => variable_get('cache') ? '' : $cache_description,
    'value' => variable_get('cache') ? t('Enabled') : t('Disabled'),
    'severity' => REQUIREMENT_INFO,
  );
  $requirements['dvg_global_block_cache'] = array(
    'title' => t('Cache blocks'),
    'description' => variable_get('block_cache') ? '' : $cache_description,
    'value' => variable_get('block_cache') ? t('Enabled') : t('Disabled'),
    'severity' => REQUIREMENT_INFO,
  );

  // Features.
  $requirements['dvg_global_features'] = array(
    'title' => t('Features'),
  );
  if ($GLOBALS['language']->language != 'en') {
    $requirements['dvg_global_features']['severity'] = REQUIREMENT_WARNING;
    $requirements['dvg_global_features']['value'] = l(t('Check the features manually.'), 'admin/structure/features');
    $requirements['dvg_global_features']['description'] = t("The features could not be checked because the current language isn't english");

  }
  else {
    module_load_include('inc', 'features', 'features.export');
    $features = features_get_features();
    $overridden_features = array();
    foreach ($features as $feature) {
      // Only check enabled features.
      // We consider any feature that isn't default to be overridden.
      if ($feature->status && features_get_storage($feature->name) != FEATURES_DEFAULT) {
        $overridden_features[$feature->name] = features_get_feature_title($feature);
      }
    }
    if (empty($overridden_features)) {
      $requirements['dvg_global_features']['value'] = t('Ok');
      $requirements['dvg_global_features']['severity'] = REQUIREMENT_OK;
    }
    else {
      $feature_links = array();
      foreach ($overridden_features as $feature_name => $feature_title) {
        $feature_links[] = l($feature_title, 'admin/structure/features/' . $feature_name . '/diff');
      }

      $link = l(t('features'), 'admin/structure/features');
      $requirements['dvg_global_features']['description'] = t('Check the !link. There are some overridden features.', array('!link' => $link));
      $requirements['dvg_global_features']['value'] = t('Overridden') . ': ' . implode(', ', $feature_links);
      $requirements['dvg_global_features']['severity'] = REQUIREMENT_ERROR;
    }
  }

  // Empty texts.
  $has_empty_texts = TRUE;

  $contact_information = variable_get('dvg_custom__contact_information', array());
  $has_empty_texts = $has_empty_texts && empty($contact_information['title']);

  $texts = variable_get('dvg_custom__footer_texts', array());
  foreach ($texts as $text) {
    $has_empty_texts = $has_empty_texts && empty($text['value']);
  }

  $requirements['dvg_global_texts'] = array(
    'title' => t('Default texts'),
  );
  if (!$has_empty_texts) {
    $requirements['dvg_global_texts']['value'] = t('Ok');
    $requirements['dvg_global_texts']['severity'] = REQUIREMENT_OK;
  }
  else {
    $requirements['dvg_global_texts']['value'] = l(t('Configure default texts.'), 'admin/config/content/texts');
    $requirements['dvg_global_texts']['severity'] = REQUIREMENT_ERROR;
  }

  // Dev settings & mail_filter.
  $dev_settings_enabled = module_exists('dvg_dev_settings');
  $requirements['dvg_dev_settings'] = array(
    'title' => t('DVG Development settings'),
    'value' => $dev_settings_enabled ? t('Enabled') : t('Disabled'),
    'severity' => $dev_settings_enabled ? REQUIREMENT_WARNING : REQUIREMENT_OK,
  );
  if (!$dev_settings_enabled && module_exists('mail_filter')) {
    $requirements['dvg_dev_settings_mail_filter'] = array(
      'title' => t('Mail filter'),
      'value' => t('Enabled'),
      'severity' => REQUIREMENT_ERROR,
    );
  }

  return $requirements;
}
