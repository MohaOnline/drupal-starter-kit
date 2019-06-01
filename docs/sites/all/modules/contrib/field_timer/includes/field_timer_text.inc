<?php

/**
 * @file
 * Contains help classes to perform field_timer_text formatter related actions.
 */

/**
 * Base class for field_timer_text formatter.
 */
abstract class field_timer_text_base extends field_timer_formatter_base {
  /**
   * @inheritdoc
   */
  public function enableFormatter() {
    return TRUE;
  }

  /**
   * @inheritdoc
   */
  final public function getFormatterDefinition() {
    return array(
      'label' => t('Text'),
      'description' => t('Simple text timer and countdown.'),
    );
  }


  /**
   * @inheritdoc
   */
  public function getDefaultSettings() {
    return array(
      'type' => 'auto',
      'granularity_type' => 'auto',
      'granularity_auto' => 2,
      'granularity_manual' => array('day' => 'day'),
    );
  }

  /**
   * @inheritdoc
   */
  public function getSettingsSummary($settings, $field) {
    $summary = array();
    $types = $this->formatterTypes();
    $granularity_options = $this->granularitySettingsSummary($settings['granularity_type'], $settings['granularity_auto'], $settings['granularity_manual']);
    $summary[] = t('Type: %type', array('%type' => $types[$settings['type']]));
    $summary[] = t('Granularity: %granularity_type (%granularity)', $granularity_options);

    return $summary;
  }

  /**
   * @inheritdoc
   */
  public function getSettingsForm($settings, $field) {
    $settings_form = array();
    $settings_form['type'] = array(
      '#type' => 'select',
      '#title' => t('Type'),
      '#options' => $this->formatterTypes(),
      '#default_value' => $settings['type'],
    );

    $settings_form['granularity_type'] = array(
      '#type' => 'select',
      '#title' => t('Granularity'),
      '#options' => $this->granularityTypes(),
      '#default_value' => $settings['granularity_type'],
      '#attributes' => array(
        'class' => array('field-timer-text-granularity-type'),
      ),
    );

    $settings_form['granularity_auto'] = array(
      '#type' => 'select',
      '#options' => $this->formatterGranularityAuto(),
      '#default_value' => $settings['granularity_auto'],
      '#states' => array(
        'visible' => array(
          'select.field-timer-text-granularity-type' => array('value' => 'auto'),
        ),
      ),
    );

    $settings_form['granularity_manual'] = array(
      '#type' => 'select',
      '#multiple' => TRUE,
      '#options' => $this->formatterGranularityManual(),
      '#default_value' => $settings['granularity_manual'],
      '#states' => array(
        'visible' => array(
          'select.field-timer-text-granularity-type' => array('value' => 'manual'),
        ),
      ),
    );

    return $settings_form;
  }

  public function renderItems($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
    $elements = array();
    $settings = $display['settings'];
    $granularity_option = $settings['granularity_' . $settings['granularity_type']];

    foreach ($items as $delta => $item) {
      $elements[$delta] = array(
        '#type' => 'markup',
        '#markup' => $this->renderItem($settings['type'], $settings['granularity_type'], $granularity_option, $this->getTimestamp($item, $field)),
      );
    }

    return $elements;
  }

  /**
   * @inheritdoc
   */
  public function getFormatterName() {
    return 'field_timer_text';
  }

  /**
   * Render field item.
   */
  protected function renderItem($type, $granularity_type, $granularity_option, $timestamp) {
    $time = '';
    // Check what we should display - timer or countdown.
    switch ($type) {
      case 'auto':
        $interval = REQUEST_TIME - $timestamp;
        $sign = ($interval > 0) ? '+' : '-';
        $time = $sign . $this->formatInterval(abs($interval), $granularity_type, $granularity_option);
        break;

      case 'timer':
        $time = $this->formatInterval(REQUEST_TIME - $timestamp, $granularity_type, $granularity_option);
        break;

      case 'countdown':
        $time = $this->formatInterval($timestamp - REQUEST_TIME, $granularity_type, $granularity_option);
        break;
    }

    return $time;
  }

  /**
   * Formats interval according to granularity settings.
   */
  protected function formatInterval($interval, $granularity_type, $granularity_option) {
    $result = '';
    switch ($granularity_type) {
      case 'auto':
        $result = format_interval($interval, $granularity_option);
        break;

      case 'manual':
        // @see function format_interval
        $units = array(
          '1 year|@count years' => 31536000,
          '1 month|@count months' => 2592000,
          '1 week|@count weeks' => 604800,
          '1 day|@count days' => 86400,
          '1 hour|@count hours' => 3600,
          '1 min|@count min' => 60,
          '1 sec|@count sec' => 1,
        );
        $granularity_options = array_keys($this->formatterGranularityManual());
        $output = '';
        foreach ($units as $key => $value) {
          if (isset($granularity_option[array_pop($granularity_options)])) {
            $key = explode('|', $key);
            if ($interval >= $value) {
              $output .= ($output ? ' ' : '') . format_plural(floor($interval / $value), $key[0], $key[1]);
              $interval %= $value;
            }
          }
        }
        $result = $output ? $output : (isset($key) ? format_plural(0, $key[0], $key[1]) : format_interval($interval));
        break;
    }

    return $result;
  }

  protected function granularitySettingsSummary($granularity_type, $granularity_auto, $granularity_manual) {
    $granularity_types = $this->granularityTypes();
    switch ($granularity_type) {
      case 'auto':
        $granularity = $granularity_auto;
        break;
      case 'manual':
        $granularity = array();
        $granularity_manual_options = $this->formatterGranularityManual();
        foreach ($granularity_manual as $key => $value) {
          if ($value) {
            $granularity[] = $granularity_manual_options[$key];
          }
        }
        $granularity = implode(', ', $granularity);
        break;
    }
    return array(
      '%granularity_type' => $granularity_types[$granularity_type],
      '%granularity' => $granularity,
    );
  }

  protected function formatterTypes() {
    return array(
      'auto' => t('Auto'),
      'timer' => t('Timer'),
      'countdown' => t('Countdown'),
    );
  }

  protected function granularityTypes() {
    return array(
      'auto' => t('Auto'),
      'manual' => t('Manual'),
    );
  }

  protected function formatterGranularityManual() {
    return array(
      'second' => t('Second'),
      'minute' => t('Minute'),
      'hour' => t('Hour'),
      'day' => t('Day'),
      'week' => t('Week'),
      'month' => t('Month'),
      'year' => t('Year'),
    );
  }

  protected function formatterGranularityAuto() {
    $options = range(0, 7);
    unset($options[0]);
    return $options;
  }

}

/**
 * Class for field_timer_text formatter and field_timer field type.
 */
class field_timer_text_field_timer extends field_timer_text_base {

  /**
   * @inheritdoc
   */
  public function getFieldType() {
    return 'field_timer';
  }

  /**
   * @inheritdoc
   */
  protected function getTimestamp($item, $field, $key = 'timestamp') {
    return $item[$key];
  }

}

/**
 * Base class for field_timer_text formatter and date field types.
 */
abstract class field_timer_text_date_field_base extends field_timer_text_base {

  /**
   * @inheritdoc
   */
  public function getDefaultSettings() {
    return parent::getDefaultSettings() + array(
      'title' => '',
      'title2' => '',
      'type2' => 'auto',
      'granularity_type2' => 'auto',
      'granularity_auto2' => 2,
      'granularity_manual2' => array('day' => 'day'),
      'fromto' => 'both',
    );
  }

  /**
   * @inheritdoc
   */
  public function getSettingsSummary($settings, $field) {
    $summary = array();
    if (!empty($field['settings']['todate'])) {
      $types = $this->formatterTypes();
      $displays = $this->formatterDisplays();
      $granularity_options = $this->granularitySettingsSummary($settings['granularity_type'], $settings['granularity_auto'], $settings['granularity_manual']);
      $granularity_options2 = $this->granularitySettingsSummary($settings['granularity_type2'], $settings['granularity_auto2'], $settings['granularity_manual2']);
      $summary[] = t('Type for Start date: %type', array('%type' => $types[$settings['type']]));
      $summary[] = t('Granularity for Start date: %granularity_type (%granularity)', $granularity_options);
      $summary[] = t('Title for Start date: %title', array('%title' => $settings['title']));
      $summary[] = t('Type for End date: %type', array('%type' => $types[$settings['type2']]));
      $summary[] = t('Granularity for End date: %granularity_type (%granularity)', $granularity_options2);
      $summary[] = t('Title for End date: %title', array('%title' => $settings['title2']));
      $summary[] = t('Display: %display', array('%display' => $displays[$settings['fromto']]));
    }
    else {
      $summary[] = t('Title: %title', array('%title' => $settings['title']));
      $summary = array_merge($summary, parent::getSettingsSummary($settings, $field));
    }

    return $summary;
  }

  /**
   * @inheritdoc
   */
  public function getSettingsForm($settings, $field) {
    $settings_form = parent::getSettingsForm($settings, $field);

    $settings_form['title'] = array(
      '#type' => 'textfield',
      '#title' => empty($field['settings']['todate']) ? t('Title') : t('Title for Start date'),
      '#default_value' => $settings['title'],
      '#weight' => -10,
    );

    if (!empty($field['settings']['todate'])) {
      $settings_form['type']['#title'] = t('Type for Start date');
      $settings_form['granularity_type']['#title'] = t('Granularity for Start date');
      $settings_form['title2'] = array(
        '#type' => 'textfield',
        '#title' => t('Title for End date'),
        '#default_value' => $settings['title2'],
      );

      $settings_form['type2'] = array(
        '#type' => 'select',
        '#title' => t('Type for End date'),
        '#options' => $this->formatterTypes(),
        '#default_value' => $settings['type2'],
      );

      $settings_form['granularity_type2'] = array(
        '#type' => 'select',
        '#title' => t('Granularity for End date'),
        '#options' => $this->granularityTypes(),
        '#default_value' => $settings['granularity_type2'],
        '#attributes' => array(
          'class' => array('field-timer-text-granularity-type-2'),
        ),
      );

      $settings_form['granularity_auto2'] = array(
        '#type' => 'select',
        '#options' => $this->formatterGranularityAuto(),
        '#default_value' => $settings['granularity_auto2'],
        '#states' => array(
          'visible' => array(
            'select.field-timer-text-granularity-type-2' => array('value' => 'auto'),
          ),
        ),
      );

      $settings_form['granularity_manual2'] = array(
        '#type' => 'select',
        '#multiple' => TRUE,
        '#options' => $this->formatterGranularityManual(),
        '#default_value' => $settings['granularity_manual2'],
        '#states' => array(
          'visible' => array(
            'select.field-timer-text-granularity-type-2' => array('value' => 'manual'),
          ),
        ),
      );

      $settings_form['fromto'] = array(
        '#type' => 'select',
        '#title' => t('Display'),
        '#options' => $this->formatterDisplays(),
        '#default_value' => $settings['fromto'],
      );
    }

    return $settings_form;
  }

  /**
   * @inheritdoc
   */
  public function renderItems($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
    $settings = $display['settings'];
    $elements = ($settings['fromto'] != 'to') ? parent::renderItems($entity_type, $entity, $field, $instance, $langcode, $items, $display) : array();

    $granularity_option = $settings['granularity_' . $settings['granularity_type2'] . '2'];
    foreach ($items as $delta => $item) {
      if ($settings['fromto'] != 'to' && !empty($settings['title'])) {
        $elements[$delta]['#markup'] = '<span class="field-timer-start-date-title">'
        . $settings['title'] . '</span> ' . $elements[$delta]['#markup'];
      }
      if ($settings['fromto'] == 'both' && !empty($field['settings']['todate'])) {
        $elements[$delta]['#markup'] = '<div class="field-timer-start-date">'
        . $elements[$delta]['#markup'] . '</div>';
      }
      if ($settings['fromto'] != 'from' && !empty($field['settings']['todate'])) {
        $title = (!empty($settings['title2'])) ? '<span class="field-timer-end-date-title">' . $settings['title2'] . '</span> ' : '';
        $time = $this->renderItem($settings['type2'], $settings['granularity_type2'], $granularity_option, $this->getTimestamp($item, $field, 'value2'));
        if ($settings['fromto'] == 'to') {
          $elements[$delta] = array(
            '#type' => 'markup',
            '#markup' => $title . $time,
          );
        }
        else {
          $elements[$delta]['#markup'] .= '<div class="field-timer-end-date">'
          . $title . $time . '</div>';
        }
      }
    }

    return $elements;
  }

  protected function formatterDisplays() {
    return array(
      'both' => t('Both Start and End dates'),
      'from' => t('Start date only'),
      'to' => t('End date only'),
    );
  }

}

/**
 * Class for field_timer_text formatter and datestamp field type.
 */
class field_timer_text_datestamp extends field_timer_text_date_field_base {

  /**
   * @inheritdoc
   */
  public function getFieldType() {
    return 'datestamp';
  }
  /**
   * @inheritdoc
   */
  protected function getTimestamp($item, $field, $key = 'value') {
    return $item[$key];
  }
}

/**
 * Class for field_timer_text formatter and date field type.
 */
class field_timer_text_date extends field_timer_text_date_field_base {

  /**
   * @inheritdoc
   */
  public function getFieldType() {
    return 'date';
  }

  /**
   * @inheritdoc
   */
  protected function getTimestamp($item, $field, $key = 'value') {
    $timezone_db = date_get_timezone_db($field['settings']['tz_handling']);
    $db_format = date_type_format($field['type']);
    $date = new DateObject($item[$key], $timezone_db, $db_format);
    return $date->getTimestamp();
  }

}

/**
 * Class for field_timer_text formatter and datetime field type.
 */
class field_timer_text_datetime extends field_timer_text_date_field_base {

  /**
   * @inheritdoc
   */
  public function getFieldType() {
    return 'datetime';
  }

  /**
   * @inheritdoc
   */
  protected function getTimestamp($item, $field, $key = 'value') {
    $timezone_db = date_get_timezone_db($field['settings']['tz_handling']);
    $db_format = date_type_format($field['type']);
    $date = new DateObject($item[$key], $timezone_db, $db_format);
    return $date->getTimestamp();
  }

}
