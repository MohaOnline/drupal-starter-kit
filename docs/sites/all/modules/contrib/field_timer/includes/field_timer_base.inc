<?php

/**
 * @file
 * Contains base help classes to perform field formatter related actions.
 */

/**
 * Field formatter interface.
 */
interface field_timer_formatter_interface {

  /**
   * Check if it is possible to enable current formatter.
   * 
   * @return bool
   */
  public function enableFormatter();

  /**
   * Get formatter definition.
   *
   * @return array Formatter definition.
   */
  public function getFormatterDefinition();

  /**
   * Get default formatter settings.
   * All settings for each field type will be combined into one array.
   *
   * @return array Default settings.
   */
  public function getDefaultSettings();

  /**
   * Get formatter settings summary.
   *
   * @param array $settings Formatter settings array.
   * @param array $field Field definition.
   *
   * @return string array summary.
   */
  public function getSettingsSummary($settings, $field);

  /**
   * Get formatter settings form.
   *
   * @param array $settings Formatter settings array.
   * @param array $field Field definition.
   *
   * @return array Drupal form array to display formatter settings form.
   */
  public function getSettingsForm($settings, $field);

  /**
   * Render field items.
   *
   * @param @see hook_field_formatter_view().
   *
   * @return array Drupal renderable array.
   */
  public function renderItems($entity_type, $entity, $field, $instance, $langcode, $items, $display);

  /**
   * Get formatter name.
   *
   * @return string Formatter name.
   */
  public function getFormatterName();

  /**
   * Get field type.
   *
   * @return string Field type.
   */
  public function getFieldType();

}

/**
 * Base class.
 */
abstract class field_timer_formatter_base implements field_timer_formatter_interface {

  /**
   * Convert field item value to timestamp.
   *
   * @param array $item Field item.
   * @param array $field Field definition.
   * @param mixed $key Key to get timestamp from field item.
   *
   * @return int Timestamp.
   */
  abstract protected function getTimestamp($item, $field, $key = '');

}

/**
 * Base class for formatters using js library.
 */
abstract class field_timer_formatter_js_base extends field_timer_formatter_base {

  protected $library_loaded = FALSE;

  /**
   * Get jQuery plugin name.
   *
   * @return string Plugin name.
   */
  abstract public function getPluginName();

  /**
   * Load library files.
   *
   * @return bool Indicate if library files loaded successfully.
   */
  abstract protected function loadLibrary();

  /**
   * Load custom JS and CSS files and JS settings.
   *
   * @param @see hook_field_formatter_view().
   */
  protected function loadJSandCSS($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
    $js_settings = $this->generateJSSettings($entity_type, $entity, $field, $instance, $langcode, $items, $display);

    drupal_add_js(array('field_timer' => $js_settings), 'setting');
    drupal_add_js(drupal_get_path('module', 'field_timer') . '/js/field_timer.js');
  }

  /**
   * Generate JS settings.
   *
   * @param @see hook_field_formatter_view().
   *
   * @return array Drupal JS settings array.
   */
  abstract protected function generateJSSettings($entity_type, $entity, $field, $instance, $langcode, $items, $display);

}

/**
 * Base class for jquery_countdown formatters.
 */
abstract class field_timer_jquery_countdown_js_base extends field_timer_formatter_js_base {
  /**
   * @inheritdoc
   */
  public function enableFormatter() {
    $library = libraries_detect('jquery.countdown');
    return $library && !empty($library['installed']);
  }

  /**
   * @inheritdoc
   */
  protected function loadLibrary() {
    if (!$this->library_loaded) {
      $library = libraries_load('jquery.countdown');
      $this->library_loaded = $library && !empty($library['loaded']);
      $js_name = 'jquery.countdown.js';
      $js_name_min = 'jquery.countdown.min.js';
      $base_path = 'sites/all/libraries/jquery.countdown';
      if (!$this->library_loaded) {
        drupal_set_message(t("Can't load jQuery Countdown library. Please download !url plugin and extract it to @path, so @js and @js_min can be found at @full_path and @full_path_min.", array(
          '!url' => l(t('jQuery Countdown'), 'http://keith-wood.name/countdown.html', array('absolute' => TRUE)),
          '@path' => $base_path,
          '@js' => $js_name,
          '@js_min' => $js_name_min,
          '@full_path' => $base_path . '/' . $js_name,
          '@full_path_min' => $base_path . '/' . $js_name_min,
        )), 'error');
      }
    }

    return $this->library_loaded;
  }

  /**
   * @inheritdoc
   */
  protected function generateJSSettings($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
    $info = entity_get_info($entity_type);
    $id_key = $info['entity keys']['id'];
    $js_settings = array();
    $settings = $display['settings'];

    foreach ($items as $delta => $item) {
      $key = $entity_type . '_' . $entity->{$id_key} . '_' . $field['field_name'] . '_' . $delta;
      $timestamp = $this->getTimestamp($item, $field);
      $js_settings += $this->generateJSSettingsArray($key, $timestamp, $settings['type'], $settings);
    }

    return $js_settings;
  }

  /**
   * @inheritdoc
   */
  protected function loadJSandCSS($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
    parent::loadJSandCSS($entity_type, $entity, $field, $instance, $langcode, $items, $display);
    drupal_add_css(drupal_get_path('module', 'field_timer') . '/css/field_timer.css', 'file');
  }

  protected function generateJSSettingsArray($key, $timestamp, $type, $settings) {
    $js_settings = array();

    $js_settings[$key]['timestamp'] = $timestamp;
    $js_settings[$key]['options'] = $settings;
    if ($type == 'timer' || ($type == 'auto' && $timestamp <= REQUEST_TIME)) {
      $js_settings[$key]['options']['until'] = FALSE;
      $js_settings[$key]['options']['since'] = TRUE;
    }
    elseif ($type == 'countdown' || ($type == 'auto' && $timestamp > REQUEST_TIME)) {
      $js_settings[$key]['options']['until'] = TRUE;
      $js_settings[$key]['options']['since'] = FALSE;
    }
    $js_settings[$key]['plugin'] = $this->getPluginName();

    return $js_settings;
  }

  protected function formatterTypes() {
    return array(
      'auto' => t('Auto'),
      'timer' => t('Timer'),
      'countdown' => t('Countdown'),
    );
  }

}
