<?php

/**
 * @file
 * Help file.
 * Contains help classes to perform field_timer_jquery_countdown_led formatter
 * related actions.
 */

/**
 * Base class for field_timer_jquery_countdown_led formatter.
 */
abstract class field_timer_jquery_countdown_led_base extends field_timer_jquery_countdown_js_base {

  /**
   * @inheritdoc
   */
  final public function getFormatterDefinition() {
    return array(
      'label' => t('jQuery Countdown LED'),
      'description' => t('Display timer and countdown as a LED.'),
    );
  }


  /**
   * @inheritdoc
   */
  public function getDefaultSettings() {
    return array(
      'type' => 'auto',
      'countdown_theme' => 'green',
      'max_count_of_days' => 2,
      'display_days' => 1,
      'display_hours' => 1,
      'display_minutes' => 1,
      'display_seconds' => 1,
    );
  }

  /**
   * @inheritdoc
   */
  public function getSettingsSummary($settings, $field) {
    $summary = array();
    $types = $this->formatterTypes();
    $themes = $this->formatterThemes();
    $days = $this->formatterDays();

    $summary[] = t('Type: %type', array('%type' => $types[$settings['type']]));
    $summary[] = t('Theme: %theme', array('%theme' => $themes[$settings['countdown_theme']]));
    $summary[] = t('Display days: %display_days', array('%display_days' => $settings['display_days'] ? t('Yes') : t('No')));
    if ($settings['display_days']) {
      $summary[] = t('Maximum count of days: %max_count_of_days', array('%max_count_of_days' => $days[$settings['max_count_of_days']]));
    }
    $summary[] = t('Display hours: %display_hours', array('%display_hours' => $settings['display_hours'] ? t('Yes') : t('No')));
    $summary[] = t('Display minutes: %display_minutes', array('%display_minutes' => $settings['display_minutes'] ? t('Yes') : t('No')));
    $summary[] = t('Display seconds: %display_seconds', array('%display_seconds' => $settings['display_seconds'] ? t('Yes') : t('No')));

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

    $settings_form['countdown_theme'] = array(
      '#type' => 'select',
      '#title' => t('Theme'),
      '#options' => $this->formatterThemes(),
      '#default_value' => $settings['countdown_theme'],
    );

    $settings_form['display_days'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display days'),
      '#default_value' => $settings['display_days'],
      '#attributes' => array('class' => array('field-timer-dislay-days')),
    );

    $settings_form['max_count_of_days'] = array(
      '#type' => 'select',
      '#title' => t('Max count of days'),
      '#options' => $this->formatterDays(),
      '#default_value' => $settings['max_count_of_days'],
      '#states' => array(
        'invisible' => array(
          'input.field-timer-dislay-days' => array('checked' => FALSE),
        ),
      ),
    );

    $settings_form['display_hours'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display hours'),
      '#default_value' => $settings['display_hours'],
    );

    $settings_form['display_minutes'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display minutes'),
      '#default_value' => $settings['display_minutes'],
    );

    $settings_form['display_seconds'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display seconds'),
      '#default_value' => $settings['display_seconds'],
    );

    return $settings_form;
  }

  /**
   * @inheritdoc
   */
  public function renderItems($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
    if (!$this->loadLibrary()) {
      return array();
    }

    $info = entity_get_info($entity_type);
    $id_key = $info['entity keys']['id'];
    $elements = array();
    $settings = $display['settings'];

    foreach ($items as $delta => $item) {
      $layout = $this->renderLayout($settings['display_days'],
          $settings['max_count_of_days'], $settings['display_hours'],
          $settings['display_minutes'], $settings['display_seconds'],
          $settings['countdown_theme']);

      $elements[$delta] = array(
        '#type' => 'markup',
        '#markup' => '<span id="jquery-countdown-led-' . $entity_type . '_'
        . $entity->{$id_key} . '_' . $field['field_name'] . '_' . $delta
        . '" class="jquery-countdown-led ' . $settings['countdown_theme'] . '">'
        . $layout . '</span>',
      );
    }

    $this->loadJSandCSS($entity_type, $entity, $field, $instance, $langcode, $items, $display);

    return $elements;
  }

  /**
   * @inheritdoc
   */
  protected function loadJSandCSS($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
    parent::loadJSandCSS($entity_type, $entity, $field, $instance, $langcode, $items, $display);
    // Since we can't get path to image from CSS file, we add this rule here.
    $path = libraries_get_path('jquery.countdown');
    drupal_add_css('.jquery-countdown-led.green span{background: url(/' . $path . '/img/countdownLED.png) no-repeat 0px 0px;} .jquery-countdown-led.blue span{background: url(/' . $path . '/img/countdownGlowing.gif) no-repeat 0px 0px;}', 'inline');
  }

  /**
   * @inheritdoc
   */
  public function getFormatterName() {
    return 'field_timer_jquery_countdown_led';
  }

  /**
   * @inheritdoc
   */
  public function getPluginName() {
    return 'jquery.countdown.led';
  }

  /**
   * Render field item.
   */
  protected function renderLayout($display_days, $max_count_of_days, $display_hours, $display_minutes, $display_seconds, $countdown_theme) {
    $layout = '<div class="jquery-countdown-led-display-wrapper">';
    if ($display_days) {
      for ($i = $max_count_of_days; $i > 0; $i--) {
        $layout .= '<span class="%t% image{d1' . substr('000', 0, $i - 1) . '}"></span>';
      }
      $layout .= '<span class="%t% imageDay"></span><span class="%t% imageSpace"></span>';
    }
    if ($display_hours) {
      $layout .= '<span class="%t% image{h10}"></span><span class="%t% image{h1}"></span>';
    }
    if ($display_minutes) {
      $layout .= '<span class="%t% imageSep"></span>'
      . '<span class="%t% image{m10}"></span><span class="%t% image{m1}"></span>';
    }
    if ($display_seconds) {
      $layout .= '<span class="%t% imageSep"></span>'
      . '<span class="%t% image{s10}"></span><span class="%t% image{s1}"></span>';
    }

    return str_replace('%t%', $countdown_theme, $layout) . '</div>';
  }

  protected function formatterThemes() {
    return array(
      'green' => t('Green'),
      'blue' => t('Blue'),
    );
  }

  protected function formatterDays() {
    return array(
      1 => 9,
      2 => 99,
      3 => 999,
      4 => 9999,
    );
  }

}

/**
 * Class for field_timer_jquery_countdown_led formatter and field_timer field
 * type.
 */
class field_timer_jquery_countdown_led_field_timer extends field_timer_jquery_countdown_led_base {

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
 * Base class for field_timer_jquery_countdown_led formatter and date field
 * types.
 */
abstract class field_timer_jquery_countdown_led_date_field_base extends field_timer_jquery_countdown_led_base {

  /**
   * @inheritdoc
   */
  public function getDefaultSettings() {
    return parent::getDefaultSettings() + array(
      'title' => '',
      'title2' => '',
      'type2' => 'auto',
      'countdown_theme2' => 'green',
      'max_count_of_days2' => 2,
      'display_days2' => 1,
      'display_hours2' => 1,
      'display_minutes2' => 1,
      'display_seconds2' => 1,
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
      $themes = $this->formatterThemes();
      $days = $this->formatterDays();
      $displays = $this->formatterDisplays();

      $summary[] = t('Title for Start date: %title', array('%title' => $settings['title']));
      $summary[] = t('Type for Start date: %type', array('%type' => $types[$settings['type']]));
      $summary[] = t('Theme for Start date: %theme', array('%theme' => $themes[$settings['countdown_theme']]));
      $summary[] = t('Display days for Start date: %display_days', array('%display_days' => $settings['display_days'] ? t('Yes') : t('No')));
      if ($settings['display_days']) {
        $summary[] = t('Maximum count of days for Start date: %max_count_of_days', array('%max_count_of_days' => $days[$settings['max_count_of_days']]));
      }
      $summary[] = t('Display hours for Start date: %display_hours', array('%display_hours' => $settings['display_hours'] ? t('Yes') : t('No')));
      $summary[] = t('Display minutes for Start date: %display_minutes', array('%display_minutes' => $settings['display_minutes'] ? t('Yes') : t('No')));
      $summary[] = t('Display seconds for Start date: %display_seconds', array('%display_seconds' => $settings['display_seconds'] ? t('Yes') : t('No')));
      $summary[] = t('Title for End date: %title', array('%title' => $settings['title2']));
      $summary[] = t('Type for End date: %type', array('%type' => $types[$settings['type2']]));
      $summary[] = t('Theme for End date: %theme', array('%theme' => $themes[$settings['countdown_theme2']]));
      $summary[] = t('Display days for End date: %display_days', array('%display_days' => $settings['display_days2'] ? t('Yes') : t('No')));
      if ($settings['display_days2']) {
        $summary[] = t('Maximum count of days for End date: %max_count_of_days', array('%max_count_of_days' => $days[$settings['max_count_of_days2']]));
      }
      $summary[] = t('Display hours for End date: %display_hours', array('%display_hours' => $settings['display_hours2'] ? t('Yes') : t('No')));
      $summary[] = t('Display minutes for End date: %display_minutes', array('%display_minutes' => $settings['display_minutes2'] ? t('Yes') : t('No')));
      $summary[] = t('Display seconds for End date: %display_seconds', array('%display_seconds' => $settings['display_seconds2'] ? t('Yes') : t('No')));
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
      $settings_form['countdown_theme']['#title'] = t('Theme for Start date');
      $settings_form['display_days']['#title'] = t('Display days');
      $settings_form['max_count_of_days']['#title'] = t('Max count of days');
      $settings_form['display_hours']['#title'] = t('Display hours');
      $settings_form['display_minutes']['#title'] = t('Display minutes');
      $settings_form['display_seconds']['#title'] = t('Display seconds');

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

      $settings_form['countdown_theme2'] = array(
        '#type' => 'select',
        '#title' => t('Theme for End date'),
        '#options' => $this->formatterThemes(),
        '#default_value' => $settings['countdown_theme2'],
      );

      $settings_form['display_days2'] = array(
        '#type' => 'checkbox',
        '#title' => t('Display days for End date'),
        '#default_value' => $settings['display_days2'],
        '#attributes' => array('class' => array('field-timer-dislay-days-2')),
      );

      $settings_form['max_count_of_days2'] = array(
        '#type' => 'select',
        '#title' => t('Max count of days for End date'),
        '#options' => $this->formatterDays(),
        '#default_value' => $settings['max_count_of_days2'],
        '#states' => array(
          'invisible' => array(
            'input.field-timer-dislay-days-2' => array('checked' => FALSE),
          ),
        ),
      );

      $settings_form['display_hours2'] = array(
        '#type' => 'checkbox',
        '#title' => t('Display hours for End date'),
        '#default_value' => $settings['display_hours2'],
      );

      $settings_form['display_minutes2'] = array(
        '#type' => 'checkbox',
        '#title' => t('Display minutes for End date'),
        '#default_value' => $settings['display_minutes2'],
      );

      $settings_form['display_seconds2'] = array(
        '#type' => 'checkbox',
        '#title' => t('Display seconds for End date'),
        '#default_value' => $settings['display_seconds2'],
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
    $info = entity_get_info($entity_type);
    $id_key = $info['entity keys']['id'];
    $elements = parent::renderItems($entity_type, $entity, $field, $instance, $langcode, $items, $display);
    $settings = $display['settings'];

    foreach ($items as $delta => $item) {
      if (!empty($settings['title']) && $settings['fromto'] != 'to') {
        $elements[$delta]['#markup'] = '<span class="field-timer-start-date-title">'
        . $settings['title'] . '</span> ' . $elements[$delta]['#markup'];
      }
      if ($settings['fromto'] == 'both' && !empty($field['settings']['todate'])) {
        $elements[$delta]['#markup'] = '<div class="field-timer-start-date">'
        . $elements[$delta]['#markup'] . '</div>';
      }
      if ($settings['fromto'] != 'from' && !empty($field['settings']['todate'])) {
        $title = (!empty($settings['title2'])) ? '<span class="field-timer-end-date-title">' . $settings['title2'] . '</span> ' : '';
        $layout = $this->renderLayout($settings['display_days2'],
            $settings['max_count_of_days2'], $settings['display_hours2'],
            $settings['display_minutes2'], $settings['display_seconds2'],
            $settings['countdown_theme2']);
        $time = '<span id="jquery-countdown-led-' . $entity_type . '_'
        . $entity->{$id_key} . '_' . $field['field_name'] . '_' . $delta
        . '_2" class="jquery-countdown-led ' . $settings['countdown_theme2'] . '">'
        . $layout . '</span>';
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

  /**
   * @inheritdoc
   */
  protected function generateJSSettings($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
    $js_settings = parent::generateJSSettings($entity_type, $entity, $field, $instance, $langcode, $items, $display);
    if (!empty($field['settings']['todate'])) {
      $info = entity_get_info($entity_type);
      $id_key = $info['entity keys']['id'];
      $settings = $display['settings'];

      $settings2 = array();
      foreach ($settings as $name => $value) {
        if (substr($name, strlen($name) - 1) == '2') {
          $settings2[substr($name, 0, strlen($name) - 1)] = $value;
        }
      }

      foreach ($items as $delta => $item) {
        $key = $entity_type . '_' . $entity->{$id_key} . '_' . $field['field_name'] . '_' . $delta . '_2';
        $timestamp = $this->getTimestamp($item, $field, 'value2');
        $js_settings += $this->generateJSSettingsArray($key, $timestamp, $settings['type2'], $settings2);
      }
    }

    return $js_settings;
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
 * Class for field_timer_jquery_countdown formatter and datestamp field type.
 */
class field_timer_jquery_countdown_led_datestamp extends field_timer_jquery_countdown_led_date_field_base {

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
 * Class for field_timer_jquery_countdown formatter and date field type.
 */
class field_timer_jquery_countdown_led_date extends field_timer_jquery_countdown_led_date_field_base {

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
 * Class for field_timer_jquery_countdown formatter and datetime field type.
 */
class field_timer_jquery_countdown_led_datetime extends field_timer_jquery_countdown_led_date_field_base {

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
