<?php

/**
 * @file
 * Help file.
 * Contains help classes to perform field_timer_county formatter related
 * actions.
 */

/**
 * Base class for field_timer_county formatter.
 */
abstract class field_timer_county_base extends field_timer_formatter_js_base {

  /**
   * @inheritdoc
   */
  public function enableFormatter() {
    $library = libraries_detect('county');
    return $library && !empty($library['installed']);
  }

  /**
   * @inheritdoc
   */
  final public function getFormatterDefinition() {
    return array(
      'label' => t('County'),
      'description' => t('Use County jQuery plugin to display countdown.'),
    );
  }


  /**
   * @inheritdoc
   */
  public function getDefaultSettings() {
    return array(
      'animation' => 'fade',
      'speed' => 500,
      'county_theme' => 'blue',
      'background' => '',
      'reflection' => 1,
      'reflectionOpacity' => 0.2,
    );
  }

  /**
   * @inheritdoc
   */
  public function getSettingsSummary($settings, $field) {
    $summary = array();
    $animations = $this->formatterAnimations();
    $themes = $this->formatterThemes();
    $summary[] = t('Animation: %animation', array('%animation' => $animations[$settings['animation']]));
    $summary[] = t('Speed: %speed', array('%speed' => $settings['speed'] . 'ms'));
    $summary[] = t('Theme: %theme', array('%theme' => $themes[$settings['county_theme']]));
    $summary[] = t('Background: %css', array('%css' => $settings['background']));
    $summary[] = t('Reflection: %state', array(
      '%state' => $settings['reflection'] ? 'Enabled' : 'Disabled',
    ));
    if ($settings['reflection']) {
      $summary[] = t('Reflection opacity: %opacity', array(
          '%opacity' => $settings['reflectionOpacity'],
      ));
    }

    return $summary;
  }

  /**
   * @inheritdoc
   */
  public function getSettingsForm($settings, $field) {
    $settings_form = array();
    $settings_form['animation'] = array(
      '#type' => 'select',
      '#title' => t('Animation'),
      '#options' => $this->formatterAnimations(),
      '#default_value' => $settings['animation'],
    );

    $settings_form['speed'] = array(
      '#type' => 'textfield',
      '#title' => t('Speed'),
      '#default_value' => $settings['speed'],
    );

    $settings_form['county_theme'] = array(
      '#type' => 'select',
      '#title' => t('Theme'),
      '#options' => $this->formatterThemes(),
      '#default_value' => $settings['county_theme'],
    );

    $settings_form['background'] = array(
      '#type' => 'textfield',
      '#title' => t('Background'),
      '#default_value' => $settings['background'],
      '#description' => t("Data from this field will be added to css property 'background'."),
    );

    $settings_form['reflection'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add reflection'),
      '#default_value' => $settings['reflection'],
      '#attributes' => array(
        'class' => array('field-timer-county-reflection'),
      ),
    );

    $settings_form['reflectionOpacity'] = array(
      '#type' => 'textfield',
      '#title' => t('Reflection opacity'),
      '#default_value' => $settings['reflectionOpacity'],
      '#description' => t('Float value between 0 and 1.'),
      '#states' => array(
        'invisible' => array(
          'input.field-timer-county-reflection' => array('checked' => FALSE),
        ),
      ),
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
      $elements[$delta] = array(
        '#type' => 'markup',
        '#markup' => '<div id="county-' . $entity_type . '_' . $entity->{$id_key}
        . '_' . $field['field_name'] . '_' . $delta . '" ' . 'style="background: ' . $settings['background'] . ';"></div>',
      );
    }

    $this->loadJSandCSS($entity_type, $entity, $field, $instance, $langcode, $items, $display);

    return $elements;
  }

  /**
   * @inheritdoc
   */
  public function getFormatterName() {
    return 'field_timer_county';
  }

  /**
   * @inheritdoc
   */
  public function getPluginName() {
    return 'county';
  }

  /**
   * @inheritdoc
   */
  protected function loadLibrary() {
    if (!$this->library_loaded) {
      $library = libraries_load('county');
      $this->library_loaded = $library && !empty($library['loaded']);
      $js_name = 'county.js';
      $base_path = 'sites/all/libraries/county';
      if (!$this->library_loaded) {
        drupal_set_message(t("Can't load County library. Please download !url jQuery plugin and extract it to @path, so @js can be found at @full_path.", array(
            '!url' => l(t('County'), 'http://www.egrappler.com/free-jquery-count-down-plugin-county/', array('absolute' => TRUE)),
            '@path' => $base_path,
            '@js' => $js_name,
            '@full_path' => $base_path . '/js/' . $js_name,
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
      $js_settings[$key]['timestamp'] = $this->getTimestamp($item, $field);
      $js_settings[$key]['options'] = $settings;
      $js_settings[$key]['plugin'] = $this->getPluginName();
    }

    return $js_settings;
  }

  protected function formatterAnimations() {
    return array(
      'fade' => t('Fade'),
      'scroll' => t('Scroll'),
    );
  }

  protected function formatterThemes() {
    return array(
      'black' => t('Black'),
      'gray' => t('Gray'),
      'red' => t('Red'),
      'blue' => t('Blue'),
    );
  }

}

/**
 * Class for field_timer_county_formatter and field_timer field type.
 */
class field_timer_county_field_timer extends field_timer_county_base {

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
abstract class field_timer_county_date_field_base extends field_timer_county_base {

  /**
   * @inheritdoc
   */
  public function getDefaultSettings() {
    return parent::getDefaultSettings() + array(
      'title' => '',
      'title2' => '',
      'animation2' => 'fade',
      'speed2' => 500,
      'county_theme2' => 'blue',
      'background2' => '',
      'reflection2' => 1,
      'reflectionOpacity2' => 0.2,
      'fromto' => 'both',
    );
  }

  /**
   * @inheritdoc
   */
  public function getSettingsSummary($settings, $field) {
    $summary = array();
    if (!empty($field['settings']['todate'])) {
      $animations = $this->formatterAnimations();
      $themes = $this->formatterThemes();
      $displays = $this->formatterDisplays();
      $summary[] = t('Title for Start date: %title', array('%title' => $settings['title']));
      $summary[] = t('Animation for Start date: %animation', array('%animation' => $animations[$settings['animation']]));
      $summary[] = t('Speed for  Start date: %speed', array('%speed' => $settings['speed'] . 'ms'));
      $summary[] = t('Theme for Start date: %theme', array('%theme' => $themes[$settings['county_theme']]));
      $summary[] = t('Background for Start date: %css', array('%css' => $settings['background']));
      $summary[] = t('Reflection for Start date: %state', array(
        '%state' => $settings['reflection'] ? 'Enabled' : 'Disabled',
      ));
      if ($settings['reflection']) {
        $summary[] = t('Reflection opacity for Start date: %opacity', array(
            '%opacity' => $settings['reflectionOpacity'],
        ));
      }
      $summary[] = t('Title for End date: %title', array('%title' => $settings['title2']));
      $summary[] = t('Animation for End date: %animation', array('%animation' => $animations[$settings['animation2']]));
      $summary[] = t('Speed for  End date: %speed', array('%speed' => $settings['speed2'] . 'ms'));
      $summary[] = t('Theme for End date: %theme', array('%theme' => $themes[$settings['county_theme2']]));
      $summary[] = t('Background for End date: %css', array('%css' => $settings['background2']));
      $summary[] = t('Reflection for End date: %state', array(
        '%state' => $settings['reflection2'] ? 'Enabled' : 'Disabled',
      ));
      if ($settings['reflection2']) {
        $summary[] = t('Reflection opacity for End date: %opacity', array(
            '%opacity' => $settings['reflectionOpacity2'],
        ));
      }
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
      $settings_form['animation']['#title'] = t('Animation for Start date');
      $settings_form['speed']['#title'] = t('Speed for Start date');
      $settings_form['county_theme']['#title'] = t('Theme for Start date');
      $settings_form['background']['#title'] = t('Background for Start date');
      $settings_form['reflection']['#title'] = t('Add reflection for Start date');
      $settings_form['reflectionOpacity']['#title'] = t('Reflection opacity for Start date');

      $settings_form['title2'] = array(
        '#type' => 'textfield',
        '#title' => t('Title for End date'),
        '#default_value' => $settings['title2'],
      );

      $settings_form['animation2'] = array(
        '#type' => 'select',
        '#title' => t('Animation for End date'),
        '#options' => $this->formatterAnimations(),
        '#default_value' => $settings['animation2'],
      );

      $settings_form['speed2'] = array(
        '#type' => 'textfield',
        '#title' => t('Speed for End date'),
        '#default_value' => $settings['speed2'],
      );

      $settings_form['county_theme2'] = array(
        '#type' => 'select',
        '#title' => t('Theme for End date'),
        '#options' => $this->formatterThemes(),
        '#default_value' => $settings['county_theme2'],
      );

      $settings_form['background2'] = array(
        '#type' => 'textfield',
        '#title' => t('Background for End date'),
        '#default_value' => $settings['background2'],
        '#description' => t("Data from this field will be added to css property 'background'."),
      );

      $settings_form['reflection2'] = array(
        '#type' => 'checkbox',
        '#title' => t('Add reflection for End date'),
        '#default_value' => $settings['reflection2'],
        '#attributes' => array(
          'class' => array('field-timer-county-reflection-2'),
        ),
      );

      $settings_form['reflectionOpacity2'] = array(
        '#type' => 'textfield',
        '#title' => t('Reflection opacity for End date'),
        '#default_value' => $settings['reflectionOpacity2'],
        '#description' => t('Float value between 0 and 1.'),
        '#states' => array(
          'invisible' => array(
            'input.field-timer-county-reflection-2' => array('checked' => FALSE),
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
        $time = '<div id="county-' . $entity_type . '_' . $entity->{$id_key}
        . '_' . $field['field_name'] . '_' . $delta . '_2" ' . 'style="background: ' . $settings['background2'] . ';"></div>';
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
        $js_settings[$key]['timestamp'] = $this->getTimestamp($item, $field, 'value2');
        $js_settings[$key]['options'] = $settings2;
        $js_settings[$key]['plugin'] = $this->getPluginName();
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
 * Class for field_timer_text formatter and datestamp field type.
 */
class field_timer_county_datestamp extends field_timer_county_date_field_base {

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
class field_timer_county_date extends field_timer_county_date_field_base {

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
class field_timer_county_datetime extends field_timer_county_date_field_base {

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
