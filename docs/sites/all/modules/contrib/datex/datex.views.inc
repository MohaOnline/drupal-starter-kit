<?php

/**
 * @file
 * Datex views integration.
 */


// ============================================================================
// ================================================================= CONTEXTUAL
// ============================================================================

class _datex_views_handler_argument_node_created_year extends views_handler_argument_node_created_year {

  /**
   * Sets the fileter value to invalid state.
   */
  function _datex_invalid() {
    $this->argument = '000000';
    return FALSE;
  }

  /**
   * Convert the date back into gregorian, if possible.
   */
  function _datex_process() {
    if (isset($this->datexed) && $this->datexed) {
      return;
    }
    $this->datexed = TRUE;

    $calendar = _datex_date_field_calendar($this->options, FALSE);
    if (!$calendar) {
      return;
    }

    if (empty($this->argument) || strlen($this->argument) !== 4) {
      return $this->_datex_invalid();
    }
    $year = (int) $this->argument;
    if ($year < 0) {
      return $this->_datex_invalid();
    }

    $month = 6;
    $day = 25;

    $calendar->setDateLocale($year, $month, $day);
    $this->argument = $calendar->xFormat('Y');
  }

  /**
   * Fix the query for localized dates.
   */
  function query($group_by = FALSE) {
    $this->_datex_process();
    return parent::query($group_by);
  }

  /**
   * Datex schema option.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['datex_schema'] = ['default' => 'default'];
    return $options;
  }

  /**
   * Schema to use for the displayed date.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['datex_schema'] = [
      '#title' => t('Datex Schema'),
      '#type' => 'select',
      '#options' => _datex_schema_form_options(),
      '#default_value' => $this->options['datex_schema'],
    ];
  }

}

class _datex_views_handler_argument_node_created_year_month extends views_handler_argument_node_created_year_month {

  /**
   * Sets the fileter value to invalid state.
   */
  function _datex_invalid() {
    $this->argument = '000000';
    return FALSE;
  }

  /**
   * Convert the date back into gregorian, if possible.
   */
  function _datex_process() {
    if (isset($this->datexed) && $this->datexed) {
      return;
    }
    $this->datexed = TRUE;

    $calendar = _datex_date_field_calendar($this->options, FALSE);
    if (!$calendar) {
      return;
    }

    if (empty($this->argument) || strlen($this->argument) < 5 || strlen($this->argument) > 6) {
      return $this->_datex_invalid();
    }

    $split = str_split($this->argument, 4);
    if (count($split) !== 2 || strlen($split[1]) > 2 || empty($split[1])) {
      return $this->_datex_invalid();
    }
    $year = (int) $split[0];
    $month = (int) $split[1];
    if ($month < 1 || $month > 12) {
      return $this->_datex_invalid();
    }
    $day = 24;

    $calendar->setDateLocale($year, $month, $day);
    $this->argument = $calendar->xFormat('Ym');
  }

  /**
   * Fix the query for localized dates.
   */
  function query($group_by = FALSE) {
    $this->_datex_process();
    return parent::query($group_by);
  }

  /**
   * Datex schema option.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['datex_schema'] = ['default' => 'default'];
    return $options;
  }

  /**
   * Schema to use for the displayed date.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['datex_schema'] = [
      '#title' => t('Datex Schema'),
      '#type' => 'select',
      '#options' => _datex_schema_form_options(),
      '#default_value' => $this->options['datex_schema'],
    ];
  }

}

class _datex_views_handler_argument_node_created_fulldate extends views_handler_argument_node_created_fulldate {

  /**
   * Sets the fileter value to invalid state.
   */
  function _datex_invalid() {
    $this->argument = '000000';
    return FALSE;
  }

  /**
   * Convert the date back into gregorian, if possible.
   */
  function _datex_process() {
    if (isset($this->datexed) && $this->datexed) {
      return;
    }
    $this->datexed = TRUE;

    $calendar = _datex_date_field_calendar($this->options, FALSE);
    if (!$calendar) {
      return;
    }

    if (empty($this->argument) || strlen($this->argument) !== 8) {
      return $this->_datex_invalid();
    }
    $split0 = str_split($this->argument, 4);
    $split1 = str_split($split0[1], 2);
    $year = (int) $split0[0];
    $month = (int) $split1[0];
    $day = (int) $split1[1];
    if ($month < 1 || $month > 12) {
      return $this->_datex_invalid();
    }
    if ($day < 1 || $day > 31) {
      return $this->_datex_invalid();
    }

    $calendar->setDateLocale($year, $month, $day);
    $this->argument = $calendar->xFormat('Ym');
  }

  /**
   * Fix the query for localized dates.
   */
  function query($group_by = FALSE) {
    $this->_datex_process();
    return parent::query($group_by);
  }

  /**
   * Datex schema option.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['datex_schema'] = ['default' => 'default'];
    return $options;
  }

  /**
   * Schema to use for the displayed date.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['datex_schema'] = [
      '#title' => t('Datex Schema'),
      '#type' => 'select',
      '#options' => _datex_schema_form_options(),
      '#default_value' => $this->options['datex_schema'],
    ];
  }

}


class _datex_views_handler_argument_node_created_month extends views_handler_argument_node_created_month {

}

class _datex_views_handler_argument_node_created_week extends views_handler_argument_node_created_week {

}

class _datex_views_handler_argument_node_created_day extends views_handler_argument_node_created_day {

}


// ============================================================================
// ================================================================ DATE RENDER
// ============================================================================

class _datex_views_handler_field_date extends views_handler_field_date {

  // In this cases, Everything will be handled properly by parent and there
  // is no need for datex to interfere with views original handler.
  private static $parent_handled = [
    'raw time ago',
    'time ago',
    'raw time hence',
    'time hence',
    'raw time span',
    'inverse time span',
    'time span',
  ];

  /**
   * Datex specific options added to date fields.
   */
  function option_definition() {
    // Default option is to follow default global schema.
    $options = parent::option_definition();
    $options['datex_schema'] = ['default' => 'default'];

    // views_date_format_sql module compatibility.
    $options['datex_format_date_sql'] = ['default' => FALSE];

    return $options;
  }

  /**
   * Schema to use for the displayed date.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['datex_schema'] = [
      '#type' => 'select',
      '#title' => t('Datex schema'),
      '#options' => _datex_schema_form_options(),
      '#default_value' => isset($this->options['datex_schema']) ? $this->options['datex_schema'] : 'default',
    ];
    $form['datex_format_date_sql'] = [
      '#type' => 'checkbox',
      '#title' => t('Use SQL to format date (views_date_format_sql module compatibility)'),
      '#description' => t('Use the SQL database to format the date. This enables date values to be used in grouping aggregation.'
        . ' This option is also provided by views_date_format_sql too, but since datex overrides this module, this option is also provided here.'
        . " There won't be any issue activating both modules (probably through a dependency)."),
      '#default_value' => isset($this->options['datex_format_date_sql']) ? $this->options['datex_format_date_sql'] : FALSE,
    ];
  }

  /**
   * Mimic the functionality of views_date_format_sql module.
   */
  private function _sql_or_delegate_render_to_parent($values) {
    return !$this->_datex_is_views_date_format_sql()
      ? parent::query()
      : $this->get_value($values);
  }

  /**
   * Render the date, if it should be localized. Otherwise let parent handle it.
   */
  public function render($values) {
    $schema = _datex_element_schema($this);

    if (in_array($this->options['date_format'], self::$parent_handled)
      || $schema === 'disabled') {
      return $this->_sql_or_delegate_render_to_parent($values);
    }

    $value = $this->get_value($values);
    if (!$value) {
      return $this->_sql_or_delegate_render_to_parent($values);
    }

    $tz = empty($this->options['timezone'])
      ? drupal_get_user_timezone()
      : $this->options['timezone'];

    $calendar = datex_factory($tz, _datex_language_calendar_name($schema));
    if (!$calendar) {
      return $this->_sql_or_delegate_render_to_parent($values);
    }

    $format = '';
    switch ($this->options['date_format']) {
      case 'short':
        $format = variable_get('date_format_short', 'm/d/Y - H:i');
        break;
      case 'long':
        $format = variable_get('date_format_long', 'l, F j, Y - H:i');
        break;
      case 'custom':
        $format = $this->options['custom_date_format'];
        break;
      case 'medium':
        $format = variable_get('date_format_medium', 'D, m/d/Y - H:i');
        break;
      default:
        $format = variable_get('date_format_' . $this->options['date_format'], '');
        break;
    }
    if (empty($format)) {
      $format = variable_get('date_format_medium', 'D, m/d/Y - H:i');
    }

    if ($this->_datex_is_views_date_format_sql()) {
      $en = datex_factory($tz, 'gregorian', 'en');
      if (!$en || !$en->parse($value, $format)) {
        return '?';
      }
      $calendar->setTimestamp($en->getTimestamp());
      return $calendar->format($format);
    }
    else {
      $calendar->setTimestamp($value);
      return $calendar->format($format);
    }
  }

  public function query() {
    if (!isset($this->options['datex_format_date_sql']) || !$this->options['datex_format_date_sql']) {
      return parent::query();
    }

    $this->ensure_my_table();
    // Add the field.
    $params = $this->options['group_type'] != 'group' ? ['function' => $this->options['group_type']] : [];

    $format = $this->options['date_format'];
    $custom_format = $this->options['custom_date_format'];
    $format_string = $this->_datex_views_date_format_sql_get_date_format($format, $custom_format);
    $formula = views_date_sql_format($format_string, "$this->table_alias.$this->real_field");

    $this->field_alias = $this->query->add_field(NULL, $formula, "{$this->table_alias}_{$this->real_field}", $params);
    $this->query->add_groupby($this->field_alias);

    $this->add_additional_fields();
  }


  /**
   * From views_date_format_sql:
   *
   * Helper to retrieve the format to a given date format name.
   * see includes/common.inc:function format_date()
   */
  private function _datex_views_date_format_sql_get_date_format($type = 'medium', $format = '') {
    switch ($type) {
      case 'short':
        $format = variable_get('date_format_short', 'm/d/Y - H:i');
        break;

      case 'long':
        $format = variable_get('date_format_long', 'l, F j, Y - H:i');
        break;

      case 'custom':
        // No change to format.
        break;

      case 'medium':
      default:
        // Retrieve the format of the custom $type passed.
        if ($type != 'medium') {
          $format = variable_get('date_format_' . $type, '');
        }
        // Fall back to 'medium'.
        if ($format === '') {
          $format = variable_get('date_format_medium', 'D, m/d/Y - H:i');
        }
        break;
    }
    return $format;
  }

  private function _datex_is_views_date_format_sql() {
    return isset($this->options['datex_format_date_sql']) && $this->options['datex_format_date_sql'];
  }

}


// ============================================================================
// ================================================================= DATE FIELD
// ============================================================================

/**
 * Copied from date_views module. So we have no hard dependency on it.
 */
class _datex_date_views_argument_handler_simple extends views_handler_argument_date {

  /**
   * Handle the date and convert it accordingly.
   */
  function _datex() {
    if (isset($this->datexed)) {
      return;
    }
    $this->datexed = TRUE;
    $schema = _datex_element_schema($this->options);
    if ($schema === 'disabled') {
      return;
    }
    $calendar = datex_factory(NULL, _datex_language_calendar_name($schema));
    if (!$calendar) {
      return;
    }
    $gr = $this->options['granularity'];
    $arg = str_replace('-', '', $this->argument);
    switch ($gr) {
      case 'year':
        $calendar->setDateLocale($arg, 6, 24);
        $this->argument = $calendar->xFormat('Y');
        break;

      case 'month':
        if (strlen($arg) < 5) {
          return;
        }
        $calendar->setDateLocale(substr($arg, 0, 4), substr($arg, 4, 6), 24);
        $this->argument = $calendar->xFormat('Y-m');
        break;

      case 'day':
        if (strlen($arg) < 7) {
          return;
        }
        $calendar->setDateLocale(substr($arg, 0, 4),
          substr($arg, 4, 6),
          substr($argc, 6, 8));
        $this->argument = $calendar->xFormat('Y-m-d');
        break;
    }
  }

  /**
   * Fix the query for localized dates.
   */
  function query($group_by = FALSE) {
    try {
      $this->_datex();
    } catch (Exception $ignore) {
    }
    return $this->_query($group_by);
  }

  /**
   * Default value for the date_fields option.
   */
  function option_definition() {
    $options = $this->_option_definition();
    $options['datex_schema'] = ['default' => 'default'];
    return $options;
  }

  /**
   * Schema to use for the displayed date.
   */
  function options_form(&$form, &$form_state) {
    $this->_options_form($form, $form_state);
    $form['datex_schema'] = [
      '#title' => t('Datex Schema'),
      '#type' => 'select',
      '#options' => _datex_schema_form_options(),
      '#default_value' => $this->options['datex_schema'],
    ];
  }


  // -----------------------------------------------------------


  /**
   * Get granularity.
   *
   * Use it to create the formula and a format for the results.
   */
  function init(&$view, &$options) {
    parent::init($view, $options);


    // Add a date handler.
    module_load_include('inc', 'date_api', 'date_api_sql');
    $this->date_handler = new date_sql_handler(DATE_UNIX);
    if (!empty($this->definition['field_name'])) {
      $field = field_info_field($this->definition['field_name']);
      if (!empty($field) && !empty($field['type'])) {
        $this->date_handler->date_type = $field['type'];
        $this->original_table = $this->definition['table'];
      }
      $this->date_handler->db_timezone = date_get_timezone_db($field['settings']['tz_handling']);
      $this->date_handler->local_timezone = date_get_timezone($field['settings']['tz_handling']);
    }
    $this->date_handler->granularity = $this->options['granularity'];
    // This value needs to be initialized so
    // it exists even if the query doesn't run.
    $this->date_handler->placeholders = [];

    $this->format = $this->date_handler->views_formats($this->date_handler->granularity, 'display');
    $this->sql_format = $this->date_handler->views_formats($this->date_handler->granularity, 'sql');
    // $this->arg_format is the format the parent date
    // handler will use to create a default argument.
    $this->arg_format = $this->format();

    // Identify the base table for this field.
    // It will be used to call for the right query field options.
    $this->base_table = $this->table;

  }

  /**
   * {@inheritdoc}
   */
  function format() {
    if (!empty($this->options['granularity'])) {
      return $this->date_handler->views_formats($this->options['granularity']);
    }
    else {
      return !empty($this->options[$this->option_name]) ? $this->options[$this->option_name] : 'Y-m';
    }
  }

  /**
   * Set the empty argument value to the current date.
   *
   * Formatted appropriately for this argument.
   */
  function get_default_argument($raw = FALSE) {
    $is_default = FALSE;

    if (!$raw && $this->options['default_argument_type'] == 'date') {
      $granularity = $this->options['granularity'];
      if ($granularity == 'week') {
        $now = date_now();
        $week = date_week(date_format($now, 'Y-m-d'));
        $value = date_format($now, 'o') . '-W' . date_pad($week);
      }
      else {
        $value = date($this->arg_format, REQUEST_TIME);
      }
      drupal_alter('date_default_argument', $this, $value);

      return $value;
    }

    // Let the parent argument handle options like node created date.
    return parent::get_default_argument($raw);
  }

  /**
   * Default value for the date_fields option.
   */
  function _option_definition() {
    $options = parent::option_definition();
    $options['year_range'] = ['default' => '-3:+3'];
    $options['granularity'] = ['default' => 'month'];
    $options['granularity_reset'] = ['default' => FALSE];
    $options['default_argument_type']['default'] = 'date';
    $options['add_delta'] = ['default' => ''];
    $options['use_fromto'] = ['default' => ''];
    $options['title_format'] = ['default' => ''];
    $options['title_format_custom'] = ['default' => ''];
    return $options;
  }

  /**
   * Add a form element to select date_fields for this argument.
   */
  function _options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Add an option to control the format of the summary.
    $options = [
      '' => t('Default format'),
      'custom' => t('Custom format'),
    ];
    $example_month = date_format_date(date_example_date(), 'custom', $this->date_handler->views_formats('month', 'display'));
    $example_day = date_format_date(date_example_date(), 'custom', $this->date_handler->views_formats('day', 'display'));

    $form['title_format'] = [
      '#type' => 'select',
      '#title' => t('Date format options'),
      '#default_value' => $this->options['title_format'],
      '#options' => $options,
      '#description' => t('The date format used in titles and summary links for this argument. The default format is based on the granularity of the filter, i.e. month: @example_month, day: @example_day.', [
        '@example_month' => $example_month,
        '@example_day' => $example_day,
      ]),
      '#attributes' => ['class' => ['dependent-options']],
      '#states' => [
        'visible' => [
          ':input[name="options[default_action]"]' => [
            'value' => 'summary',
          ],
        ],
      ],
    ];

    $form['title_format_custom'] = [
      '#type' => 'textfield',
      '#title' => t('Custom summary date format'),
      '#default_value' => $this->options['title_format_custom'],
      '#description' => t("A custom format for the title and summary date format. Define a php date format string like 'm-d-Y H:i' (see <a href=\"@link\">http://php.net/date</a> for more details).", ['@link' => 'http://php.net/date']),
      '#attributes' => ['class' => ['dependent-options']],
      '#states' => [
        'visible' => [
          ':input[name="options[title_format]"]' => [
            'value' => 'custom',
          ],
        ],
      ],
    ];

    // Get default granularity options
    $options = $this->date_handler->date_parts();
    // Add the 'week' option.
    $options += [
      'week' => t('Week', [], [
        'context' => 'datetime',
      ]),
    ];

    $form['granularity'] = [
      '#title' => t('Granularity'),
      '#type' => 'radios',
      '#options' => $options,
      '#default_value' => $this->options['granularity'],
      '#description' => t("Select the type of date value to be used in defaults, summaries, and navigation. For example, a granularity of 'month' will set the default date to the current month, summarize by month in summary views, and link to the next and previous month when using date navigation."),
    ];

    $form['granularity_reset'] = [
      '#title' => t('Use granularity from argument value'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['granularity_reset'],
      '#description' => t("If the granularity of argument value is different from selected, use it from argument value."),
    ];

    $form['year_range'] = [
      '#title' => t('Date year range'),
      '#type' => 'textfield',
      '#default_value' => $this->options['year_range'],
      '#description' => t("Set the allowable minimum and maximum year range for this argument, either a -X:+X offset from the current year, like '-3:+3' or an absolute minimum and maximum year, like '2005:2010' . When the argument is set to a date outside the range, the page will be returned as 'Page not found (404)' ."),
    ];

    $form['use_fromto'] = [
      '#type' => 'radios',
      '#title' => t('Dates to compare'),
      '#default_value' => $this->options['use_fromto'],
      '#options' => [
        '' => t('Start/End date range'),
        'no' => t('Only this field'),
      ],
      '#description' => t("If selected the view will check if any value starting with the 'Start' date and ending with the 'End' date matches the view criteria. Otherwise the view will be limited to the specifically selected fields. Comparing to the whole Start/End range is the recommended setting when using this filter in a Calendar. When using the Start/End option, it is not necessary to add both the Start and End fields to the filter, either one will do."),
    ];

    $access = TRUE;
    if (!empty($this->definition['field_name'])) {
      $field = field_info_field($this->definition['field_name']);
      $access = $field['cardinality'] != 1;
    }
    $form['add_delta'] = [
      '#type' => 'radios',
      '#title' => t('Add multiple value identifier'),
      '#default_value' => $this->options['add_delta'],
      '#options' => ['' => t('No'), 'yes' => t('Yes')],
      '#description' => t('Add an identifier to the view to show which multiple value date fields meet the filter criteria. Note: This option may introduce duplicate values into the view. Required when using multiple value fields in a Calendar or any time you want the node view of multiple value dates to display only the values that match the view filters.'),
      // Only let mere mortals tweak this setting for multi-value fields.
      '#access' => $access,
    ];
  }

  /**
   * {@inheritdoc}
   */
  function options_validate(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_validate($form, $form_state);
    if (!preg_match('/^(?:\-[0-9]{1,4}|[0-9]{4}):(?:[\+\-][0-9]{1,4}|[0-9]{4})$/', $form_state['values']['options']['year_range'])) {
      form_error($form['year_range'], t('Date year range must be in the format -9:+9, 2005:2010, -9:2010, or 2005:+9'));
    }
  }

  /**
   * Provide a link to the next level of the view from the summary.
   */
  function summary_name($data) {
    $value = $data->{$this->name_alias};
    if (empty($value) && !empty($this->definition['empty field name'])) {
      return $this->definition['empty field name'];
    }
    elseif (empty($value)) {
      return $this->options['wildcard_substitution'];
    }
    $format = !empty($this->options['title_format_custom']) && !empty($this->options['title_format_custom']) ? $this->options['title_format_custom'] : $this->date_handler->views_formats($this->options['granularity'], 'display');
    $range = $this->date_handler->arg_range($value);
    return date_format_date($range[0], 'custom', $format);
  }

  /**
   * Provide a title for the view based on the argument value.
   */
  function title() {
    $format = !empty($this->options['title_format_custom']) && !empty($this->options['title_format_custom']) ? $this->options['title_format_custom'] : $this->date_handler->views_formats($this->options['granularity'], 'display');
    $range = $this->date_handler->arg_range($this->argument);
    return date_format_date($range[0], 'custom', $format);
  }

  /**
   * Provide the argument to use to link from the summary to the next level.
   *
   * This will be called once per row of a summary, and used as part of
   * $view->get_url().
   *
   * @param object $data
   *   The query results for the row.
   */
  function summary_argument($data) {
    $format = $this->date_handler->views_formats($this->options['granularity'], 'sql');
    $value = $data->{$this->name_alias};
    if (empty($value)) {
      return $this->options['exception']['value'];
    }
    $range = $this->date_handler->arg_range($value);
    return date_format_date($range[0], 'custom', $format);
  }

  /**
   * Inject a test for valid date range before the summary query.
   */
  function summary_query() {

    // @TODO The summary values are computed by the database.
    // Unless the database has built-in timezone handling it will use
    // a fixed offset, which will not be right for all dates.
    // The only way I can see to make this work right is to store the offset
    // for each date in the database so it can be added to the base
    // date value before the database formats the result. Because this is a huge
    // architectural change, it won't go in until we start a new branch.
    $this->formula = $this->date_handler->sql_format($this->sql_format, $this->date_handler->sql_field("***table***.$this->real_field"));
    $this->ensure_my_table();
    // Now that our table is secure, get our formula.
    $formula = $this->get_formula();

    // Add the field, give it an alias that does NOT match the actual
    // field name or grouping won't work right.
    $this->base_alias = $this->name_alias = $this->query->add_field(NULL, $formula, $this->field . '_summary');
    $this->query->set_count_field(NULL, $formula, $this->field);

    return $this->summary_basics(FALSE);
  }

  /**
   * Inject a test for valid date range before the regular query.
   *
   * Override the parent query to be able to control the $group.
   */
  function _query($group_by = FALSE) {
    // @TODO Not doing anything with $group_by yet,
    // need to figure out what has to be done.

    if ($this->date_forbid()) {
      return;
    }

    // See if we need to reset granularity based on an argument value.
    // Make sure we don't try to reset to some bogus value if someone has
    // typed in an unexpected argument.
    if ($this->options['granularity_reset'] && $granularity = $this->date_handler->arg_granularity($this->argument)) {
      $this->date_handler->granularity = $granularity;
      $this->format = $this->date_handler->views_formats($this->date_handler->granularity, 'display');
      $this->sql_format = $this->date_handler->views_formats($this->date_handler->granularity, 'sql');
    }
    $this->granularity = $this->date_handler->granularity;
    $this->ensure_my_table();
    $group = !empty($this->options['date_group']) ? $this->options['date_group'] : 0;

    // If requested, add the delta field to the view so
    // we can later find the value that matched our query.
    if (!empty($this->options['add_delta']) && (substr($this->real_field, -6) == '_value' || substr($this->real_field, -7) == '_value2')) {
      $this->query->add_field($this->table_alias, 'delta');
      $real_field_name = str_replace([
        '_value',
        '_value2',
      ], '', $this->real_field);
      $this->query->add_field($this->table_alias, 'entity_id', 'date_id_' . $real_field_name);
      $this->query->add_field($this->table_alias, 'delta', 'date_delta_' . $real_field_name);
    }

    $format = $this->date_handler->granularity == 'week' ? DATE_FORMAT_DATETIME : $this->sql_format;
    $view_min = date_format($this->min_date, $format);
    $view_max = date_format($this->max_date, $format);
    $view_min_placeholder = $this->placeholder();
    $view_max_placeholder = $this->placeholder();
    $this->date_handler->placeholders = [
      $view_min_placeholder => $view_min,
      $view_max_placeholder => $view_max,
    ];

    // Are we comparing this field only or the Start/End date range
    // to the view criteria?
    if (!empty($this->options['use_fromto'])) {

      // The simple case, match the field to the view range.
      $field = $this->date_handler->sql_field($this->table_alias . '.' . $this->real_field, NULL, $this->min_date);
      $field = $this->date_handler->sql_format($format, $field);
      $this->query->add_where_expression($group, "$field >= $view_min_placeholder AND $field <= $view_max_placeholder", [
        $view_min_placeholder => $view_min,
        $view_max_placeholder => $view_max,
      ]);

    }
    else {

      // Look for the intersection of the range
      // of the date field with the range of the view.
      // Get the Start/End values for this field.
      // Retrieve using the original table name.
      // Swap the current table name (adjusted for relationships)
      // into the query.
      // @TODO We may be able to use Views substitutions here,
      // investigate that later.
      $fields = date_views_fields($this->base_table);
      $fields = $fields['name'];
      $fromto = $fields[$this->original_table . '.' . $this->real_field]['fromto'];

      $value_min = str_replace($this->original_table, $this->table_alias, $fromto[0]);
      $value_max = str_replace($this->original_table, $this->table_alias, $fromto[1]);
      $field_min = $this->date_handler->sql_field($value_min, NULL, $this->min_date);
      $field_min = $this->date_handler->sql_format($format, $field_min);
      $field_max = $this->date_handler->sql_field($value_max, NULL, $this->max_date);
      $field_max = $this->date_handler->sql_format($format, $field_max);
      $this->query->add_where_expression($group, "$field_max >= $view_min_placeholder AND $field_min <= $view_max_placeholder", [
        $view_min_placeholder => $view_min,
        $view_max_placeholder => $view_max,
      ]);
    }
  }

  /**
   * Add a callback.
   *
   * To determine if we have moved outside
   * the valid date range for this argument.
   */
  function date_forbid() {
    if (empty($this->argument)) {
      return TRUE;
    }
    $this->date_range = $this->date_handler->arg_range($this->argument);
    $this->min_date = $this->date_range[0];
    $this->max_date = $this->date_range[1];
    $this->limit = date_range_years($this->options['year_range']);
    $group = !empty($this->options['date_group']) ? $this->options['date_group'] : 0;

    // See if we're outside the allowed date range for our argument.
    if (date_format($this->min_date, 'Y') < $this->limit[0] || date_format($this->max_date, 'Y') > $this->limit[1]) {
      $this->forbid = TRUE;
      $this->view->build_info['fail'] = TRUE;
      return TRUE;
    }
    return FALSE;
  }

}

