<?php

/**
 * @file
 * Help file.
 * Contains help classes to perform field_timer_jquery_countdown formatter
 * related actions.
 */

/**
 * Base class for field_timer_jquery_countdown formatter.
 */
abstract class field_timer_jquery_countdown_base extends field_timer_jquery_countdown_js_base {

  /**
   * @inheritdoc
   */
  final public function getFormatterDefinition() {
    return array(
      'label' => t('jQuery Countdown'),
      'description' => t('Use jQuery Countdown plugin to display countdown and timer.'),
    );
  }

  /**
   * @inheritdoc
   */
  public function getDefaultSettings() {
    return array(
      'type' => 'auto',
      'regional' => 'en',
      'format' => 'dHMS',
      'layout' => '',
      'compact' => 0,
      'significant' => 0,
      'timeSeparator' => ':',
      'padZeroes' => 0,
    );
  }

  /**
   * @inheritdoc
   */
  public function getSettingsSummary($settings, $field) {
    $summary = array();
    $types = $this->formatterTypes();
    $regions = $this->formatterRegions();

    $summary[] = t('Type: %type', array('%type' => $types[$settings['type']]));
    $summary[] = t('Region: %regional', array('%regional' => $regions[$settings['regional']]));
    $summary[] = t('Format: %format', array('%format' => $settings['format']));
    $summary[] = t('Layout: %layout', array('%layout' => $settings['layout']));
    $summary[] = t('Compact: %compact', array('%compact' => $settings['compact'] ? t('Yes') : t('No')));
    $summary[] = t('Granularity: %significant', array('%significant' => $settings['significant']));
    $summary[] = t('Time separator: %timeSeparator', array('%timeSeparator' => $settings['timeSeparator']));
    $summary[] = t('Pad with zeroes: %padZeroes', array('%padZeroes' => $settings['padZeroes'] ? t('Yes') : t('No')));

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

    $settings_form['regional'] = array(
      '#type' => 'select',
      '#title' => t('Region'),
      '#default_value' => $settings['regional'],
      '#options' => $this->formatterRegions(),
    );

    $settings_form['format'] = array(
      '#type' => 'textfield',
      '#title' => t('Format'),
      '#default_value' => $settings['format'],
      '#description' => t('See !documentation for this parameter.', array(
        '!documentation' => l(t('documentation'), 'http://keith-wood.name/countdownRef.html', array('fragment' => 'format')),
      )),
    );

    $settings_form['layout'] = array(
      '#type' => 'textarea',
      '#rows' => 3,
      '#cols' => 60,
      '#title' => t('Layout'),
      '#default_value' => $settings['layout'],
      '#description' => t('See !documentation for this parameter.', array(
        '!documentation' => l(t('documentation'), 'http://keith-wood.name/countdownRef.html', array('fragment' => 'layout')),
      )),
    );

    $settings_form['compact'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display in compact format'),
      '#default_value' => $settings['compact'],
    );

    $settings_form['significant'] = array(
      '#type' => 'select',
      '#title' => t('Granularity'),
      '#options' => range(0, 7),
      '#default_value' => $settings['significant'],
    );

    $settings_form['timeSeparator'] = array(
      '#type' => 'textfield',
      '#title' => t('Time separator'),
      '#default_value' => $settings['timeSeparator'],
    );

    $settings_form['padZeroes'] = array(
      '#type' => 'checkbox',
      '#title' => t('Pad with zeroes'),
      '#default_value' => $settings['padZeroes'],
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

    foreach ($items as $delta => $item) {
      $elements[$delta] = array(
        '#type' => 'markup',
        '#markup' => '<span id="jquery-countdown-' . $entity_type . '_'
        . $entity->{$id_key} . '_' . $field['field_name'] . '_' . $delta
        . '" class="jquery-countdown"></span>',
      );
    }

    $this->loadJSandCSS($entity_type, $entity, $field, $instance, $langcode, $items, $display);

    return $elements;
  }

  /**
   * @inheritdoc
   */
  public function getFormatterName() {
    return 'field_timer_jquery_countdown';
  }

  /**
   * @inheritdoc
   */
  public function getPluginName() {
    return 'jquery.countdown';
  }

  protected function formatterRegions() {
    return array(
      'sq' => t('Albanian'),
      'ar' => t('Arabic'),
      'hy' => t('Armenian'),
      'bn' => t('Bengali/Bangla'),
      'bs' => t('Bosnian'),
      'bg' => t('Bulgarian'),
      'my' => t('Burmese'),
      'ca' => t('Catalan'),
      'zh-CN' => t('Chinese/Simplified'),
      'zh-TW' => t('Chinese/Traditional'),
      'hr' => t('Croatian'),
      'cs' => t('Czech'),
      'da' => t('Danish'),
      'nl' => t('Dutch'),
      'et' => t('Estonian'),
      'en' => t('English'),
      'fa' => t('Farsi/Persian'),
      'fi' => t('Finnish'),
      'fo' => t('Faroese'),
      'fr' => t('French'),
      'gl' => t('Galician'),
      'de' => t('German'),
      'el' => t('Greek'),
      'gu' => t('Gujarati'),
      'he' => t('Hebrew'),
      'hu' => t('Hungarian'),
      'id' => t('Indonesian'),
      'is' => t('Icelandic'),
      'it' => t('Italian'),
      'ja' => t('Japanese'),
      'kn' => t('Kannada'),
      'ko' => t('Korean'),
      'lv' => t('Latvian'),
      'lt' => t('Lithuanian'),
      'mk' => t('Macedonian'),
      'ml' => t('Malayalam'),
      'ms' => t('Malaysian'),
      'nb' => t('Norvegian'),
      'pl' => t('Polish'),
      'pt-BR' => t('Portuguese/Brazilian'),
      'ro' => t('Romanian'),
      'ru' => t('Russian'),
      'sr' => t('Serbian'),
      'sk' => t('Slovak'),
      'sl' => t('Slovenian'),
      'es' => t('Spanish'),
      'sv' => t('Swedish'),
      'th' => t('Thai'),
      'tr' => t('Turkish'),
      'uk' => t('Ukrainian'),
      'ur' => t('Urdu'),
      'uz' => t('Uzbek'),
      'vi' => t('Vietnamese'),
      'cy' => t('Welsh'),
    );
  }

}

/**
 * Class for field_timer_jquery_countdown_formatter and field_timer field type.
 */
class field_timer_jquery_countdown_field_timer extends field_timer_jquery_countdown_base {

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

  /**
   * @inheritdoc
   */
  protected function loadJSandCSS($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
    parent::loadJSandCSS($entity_type, $entity, $field, $instance, $langcode, $items, $display);
    if ($display['settings']['regional'] != 'en') {
      $path = libraries_get_path('jquery.countdown');
      drupal_add_js($path . '/js/jquery.countdown-' . $display['settings']['regional'] . '.js');
    }
  }

}

/**
 * Base class for field_timer_jquery_countdown formatter and date field types.
 */
abstract class field_timer_jquery_countdown_date_field_base extends field_timer_jquery_countdown_base {
  /**
   * @inheritdoc
   */
  public function getDefaultSettings() {
    return parent::getDefaultSettings() + array(
      'title' => '',
      'title2' => '',
      'type2' => 'auto',
      'regional2' => 'en',
      'format2' => 'dHMS',
      'layout2' => '',
      'compact2' => 0,
      'significant2' => 0,
      'timeSeparator2' => ':',
      'padZeroes2' => 0,
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
      $regions = $this->formatterRegions();
      $displays = $this->formatterDisplays();
      $summary[] = t('Title for Start date: %title', array('%title' => $settings['title']));
      $summary[] = t('Type for Start date: %type', array('%type' => $types[$settings['type']]));
      $summary[] = t('Region for Start date: %regional', array('%regional' => $regions[$settings['regional']]));
      $summary[] = t('Format for Start date: %format', array('%format' => $settings['format']));
      $summary[] = t('Layout for Start date: %layout', array('%layout' => $settings['layout']));
      $summary[] = t('Compact for Start date: %compact', array('%compact' => $settings['compact'] ? t('Yes') : t('No')));
      $summary[] = t('Granularity for Start date: %significant', array('%significant' => $settings['significant']));
      $summary[] = t('Time separator for Start date: %timeSeparator', array('%timeSeparator' => $settings['timeSeparator']));
      $summary[] = t('Pad with zeroes for Start date: %padZeroes', array('%padZeroes' => $settings['padZeroes'] ? t('Yes') : t('No')));
      $summary[] = t('Title for End date: %title', array('%title' => $settings['title2']));
      $summary[] = t('Type for End date: %type', array('%type' => $types[$settings['type2']]));
      $summary[] = t('Region for End date: %regional', array('%regional' => $regions[$settings['regional2']]));
      $summary[] = t('Format for End date: %format', array('%format' => $settings['format2']));
      $summary[] = t('Layout for End date: %layout', array('%layout' => $settings['layout2']));
      $summary[] = t('Compact for End date: %compact', array('%compact' => $settings['compact2'] ? t('Yes') : t('No')));
      $summary[] = t('Granularity for End date: %significant', array('%significant' => $settings['significant2']));
      $summary[] = t('Time separator for End date: %timeSeparator', array('%timeSeparator' => $settings['timeSeparator2']));
      $summary[] = t('Pad with zeroes for End date: %padZeroes', array('%padZeroes' => $settings['padZeroes2'] ? t('Yes') : t('No')));
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
      $settings_form['regional']['#title'] = t('Region for Start date');
      $settings_form['format']['#title'] = t('Format for Start date');
      $settings_form['layout']['#title'] = t('Layout for Start date');
      $settings_form['compact']['#title'] = t('Display Start date in compact format');
      $settings_form['significant']['#title'] = t('Granularity for Start date');
      $settings_form['timeSeparator']['#title'] = t('Time separator for Start date');
      $settings_form['padZeroes']['#title'] = t('Pad with zeroes for Start date');

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

      $settings_form['regional2'] = array(
        '#type' => 'select',
        '#title' => t('Region for End date'),
        '#default_value' => $settings['regional2'],
        '#options' => $this->formatterRegions(),
      );

      $settings_form['format2'] = array(
        '#type' => 'textfield',
        '#title' => t('Format for End date'),
        '#default_value' => $settings['format2'],
        '#description' => t("See !documentation for this parameter.", array(
          '!documentation' => l(t('documentation'), 'http://keith-wood.name/countdownRef.html', array('fragment' => 'format')),
        )),
      );

      $settings_form['layout2'] = array(
        '#type' => 'textarea',
        '#rows' => 3,
        '#cols' => 60,
        '#title' => t('Layout for End date'),
        '#default_value' => $settings['layout2'],
        '#description' => t('See !documentation for this parameter.', array(
          '!documentation' => l(t('documentation'), 'http://keith-wood.name/countdownRef.html', array('fragment' => 'layout'))
        )),
      );

      $settings_form['compact2'] = array(
        '#type' => 'checkbox',
        '#title' => t('Display End date in compact format'),
        '#default_value' => $settings['compact2'],
      );

      $settings_form['significant2'] = array(
        '#type' => 'select',
        '#title' => t('Granularity for End date'),
        '#options' => range(0, 7),
        '#default_value' => $settings['significant2'],
      );

      $settings_form['timeSeparator2'] = array(
        '#type' => 'textfield',
        '#title' => t('Time separator for End date'),
        '#default_value' => $settings['timeSeparator2'],
      );

      $settings_form['padZeroes2'] = array(
        '#type' => 'checkbox',
        '#title' => t('Pad with zeroes for End date'),
        '#default_value' => $settings['padZeroes2'],
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
    $info = entity_get_info($entity_type);
    $id_key = $info['entity keys']['id'];

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
        $time = '<span id="jquery-countdown-' . $entity_type . '_'
        . $entity->{$id_key} . '_' . $field['field_name'] . '_' . $delta . '_2'
        . '" class="jquery-countdown"></span>';
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

  /**
   * @inheritdoc
   */
  protected function loadJSandCSS($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
    parent::loadJSandCSS($entity_type, $entity, $field, $instance, $langcode, $items, $display);
    if ($display['settings']['regional'] != 'en') {
      $path = libraries_get_path('jquery.countdown');
      drupal_add_js($path . '/js/jquery.countdown-' . $display['settings']['regional'] . '.js');
    }
    if ($display['settings']['regional2'] != 'en') {
      $path = libraries_get_path('jquery.countdown');
      drupal_add_js($path . '/js/jquery.countdown-' . $display['settings']['regional2'] . '.js');
    }
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
class field_timer_jquery_countdown_datestamp extends field_timer_jquery_countdown_date_field_base {

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
class field_timer_jquery_countdown_date extends field_timer_jquery_countdown_date_field_base {

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
class field_timer_jquery_countdown_datetime extends field_timer_jquery_countdown_date_field_base {

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
