<?php

/**
 * @file
 * Datex administration forms.
 */

/**
 * Provides administration form for datex module (menu callback).
 */
function datex_admin_form($form, $form_state) {
  $form['datex_api_intl_status'] = [
    '#type' => 'markup',
    '#title' => t('PHP-Intl status'),
    '#markup' => !DATEX_USE_INTL
      ? t("<h1><b><a href='http://php.net/manual/en/book.intl.php'>" .
        'PHP-Intl</a> is not available in this environment. It is ' .
        "highly recommended that you enable it, else you'll have limited " .
        "functionality.</b></h1>")
      : t('PHP-Intl is available.'),
  ];

  $schema_options = [];
  foreach (array_keys(variable_get('datex_schema', [])) as $name) {
    $summary = '';
    $schema = variable_get('datex_schema');
    foreach ($schema[$name] as $sss_lang => $sss_display) {
      $available = _datex_available_calendars();
      $summary .= ' - ' . t('In language [@lang] display in [@display]', [
          '@display' => $available[$sss_display],
          '@lang' => $sss_lang,
        ]);
    }
    $path = 'admin/config/regional/date-time/datex/edit/' . $name;
    $schema_options[$name] = [
      'title' => [
        'data' => [
          '#type' => 'link',
          '#title' => check_plain($name),
          '#href' => $path,
          '#options' => ['inline'],
        ],
      ],
      'summary' => empty($summary) ? t('Disabled') : nl2br($summary),
    ];
  }
  $form['schema_manage'] = [
    '#type' => 'fieldset',
    '#description' => t("Mark schemas to delete after saving, click it's name to edit."),
    '#title' => t('Manage Schemas'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'datex_config',
  ];
  $form['schema_manage']['datex_schemas'] = [
    '#type' => 'tableselect',
    '#header' => ['title' => t('Title'), 'summary' => t('Summary')],
    '#options' => $schema_options,
  ];
  $form['schema_manage']['datex_new_schema_machine_name'] = [
    '#title' => t('New schema'),
    '#type' => 'machine_name',
    '#maxlength' => 31,
    '#description' => t('Schema machine readable name containing only letters, numbers and underscores.'),
    '#machine_name' => [
      'exists' => '_datex_machine_name_exists',
    ],
    '#required' => FALSE,
  ];

  $form['datex_popup_theme'] = [];

  if (module_exists('datex_popup')) {
    if (!module_exists('jquery_update')) {
      drupal_set_message(t('jquery_update is not found, make sure you have' .
        ' latest version of jquery, else datex_popup will not work and probably' .
        ' will blow up your website.'), 'warning');
    }
    $form['datex_popup_theme'] = [
      '#type' => 'select',
      '#title' => t('Popup Theme'),
      '#options' => drupal_map_assoc([
        'none',
        'blue',
        'cheerup',
        'dark',
        'redblack',
      ]),
      '#default_value' => variable_get('datex_popup_theme', 'none'),
    ];
  }

  $form['datex_block_count'] = [
    '#type' => 'textfield',
    '#title' => t('Block count'),
    '#description' => t('How many blocks should datex create.'),
    '#default_value' => variable_get('datex_block_count', 1),
  ];

  $form['actions']['submit'] = [
    '#type' => 'submit',
    '#submit' => ['_datex_admin_form_submit'],
    '#value' => t('Save'),
    '#name' => 'config',
  ];
  return $form;
}

/**
 * Form submit for datex admin form.
 *
 * This admin form could be implemented better.
 */
function _datex_admin_form_submit($form, &$form_state) {
  $v = $form_state['values'];

  $schema = variable_get('datex_schema');

  $new_name = empty($v['datex_new_schema_machine_name']) ? '' :
    $v['datex_new_schema_machine_name'];
  if ($new_name) {
    $schema[$new_name] = [];
    drupal_set_message(t("New schema created: @s", ['@s' => $new_name]));
  }

  foreach ($v['datex_schemas'] as $select) {
    if ($select !== 'default' && isset($schema[$select])) {
      unset($schema[$select]);
      drupal_set_message(check_plain(t('Schema removed') . ': ' . $select));
    }
    else {
      if ($select === 'default') {
        $schema['default'] = [];
        $message = t('Default schema was reset.');
        drupal_set_message(check_plain($message));
      }
    }
  }
  variable_set('datex_schema', $schema);

  if (module_exists('datex_popup')) {
    variable_set('datex_popup_theme', $v['datex_popup_theme']);
  }

  $bc = $v['datex_block_count'];
  $old_bc = variable_get('datex_block_count', 1);
  if ($bc !== '' && is_numeric($bc) && intval($bc) !== $old_bc) {
    variable_set('datex_block_count', $bc);
    drupal_set_message(t("Clear the cache for changes to take effect"));

    $fmt = variable_get('datex_block_format', []);
    $cal = variable_get('datex_block_calendar', []);
    for ($i = $bc; $i < $old_bc; $i++) {
      unset($fmt[$i]);
      unset($cal[$i]);
    }
    variable_set('datex_block_count', $bc);
    variable_set('datex_block_formats', $fmt);
  }

  drupal_set_message(t('Configuration saved.'));

  if ($new_name) {
    drupal_goto('admin/config/regional/date-time/datex/edit/' . $new_name);
  }
}

/**
 * Datex form element validation for schema name.
 */
function _datex_machine_name_exists($value, $element, $form_state) {
  $machine = $form_state['values']['datex_new_schema_machine_name'];
  return $machine === 'default'
    || $machine === 'disabled'
    || in_array($machine, array_keys(variable_get('datex_schema', [])));
}

/**
 * Menu callback, Admin form for editing datex schema.
 */
function datex_schema_edit_form($form, $form_state, $name) {
  $list = array_keys(variable_get('datex_schema', []));
  if (!in_array($name, $list)) {
    $form['notfound'] = [
      '#markup' => t('Not found'),
    ];
    return $form;
  }

  $schema = variable_get('datex_schema');
  $default = $schema[$name];

  $form['schemaname'] = [
    '#type' => 'markup',
    '#markup' => '<h2> Editing: ' . '<b>' . check_plain($name) . '</b></h2>',
  ];
  $form['name'] = [
    '#type' => 'value',
    '#value' => $name,
  ];

  foreach (language_list() as $code => $lang) {
    $t = ['@name' => $lang->name, '@native' => $lang->native];
    $form['datex_' . $code] = [
      '#type' => 'select',
      '#title' => t('Used calendar in @name (@native)', $t),
      '#options' => ['' => 'Disabled'] + _datex_available_calendars(),
      '#default_value' => isset($default[$code]) ? $default[$code] : '',
    ];
  }

  $form['actions']['submit'] = [
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => ['_datex_schema_edit_form_submit'],
  ];

  return $form;
}

/**
 * Form submit callback for: datex_schema_edit_form().
 */
function _datex_schema_edit_form_submit($form, $form_state) {
  $v = $form_state['values'];
  $name = $v['name'];
  $schema = variable_get('datex_schema');

  foreach (language_list() as $lang) {
    $code = $lang->language;
    if ($v['datex_' . $code] === '') {
      if (isset($schema[$name][$code])) {
        unset($schema[$name][$code]);
      }
    }
    else {
      $schema[$name][$code] = $v['datex_' . $code];
    }
  }

  variable_set('datex_schema', $schema);

  $message = t('New configuration for @schema schema saved.', ['@schema' => $name]);
  drupal_set_message($message);

  drupal_goto('admin/config/regional/date-time/datex');
}

/**
 * List of css files which jquery calendar has.
 */
function _datex_api_css_files_list() {
  return [
    //    'ui-black-tie.calendars.picker.css',
    //    'ui-blitzer.calendars.picker.css',
    //    'ui-cupertino.calendars.picker.css',
    //    'ui-dark-hive.calendars.picker.css',
    //    'ui-dot-luv.calendars.picker.css',
    //    'ui-eggplant.calendars.picker.css',
    //    'ui-excite-bike.calendars.picker.css',
    //    'ui-flick.calendars.picker.css',
    //    'ui-hot-sneaks.calendars.picker.css',
    //    'ui-humanity.calendars.picker.css',
    //    'ui-le-frog.calendars.picker.css',
    //    'ui-mint-choc.calendars.picker.css',
    //    'ui-overcast.calendars.picker.css',
    //    'ui-pepper-grinder.calendars.picker.css',
    //    'ui-redmond.calendars.picker.css',
    //    'ui-smoothness.calendars.picker.css',
    //    'ui-south-street.calendars.picker.css',
    //    'ui-start.calendars.picker.css',
    //    'ui-sunny.calendars.picker.css',
    //    'ui-swanky-purse.calendars.picker.css',
    //    'ui-trontastic.calendars.picker.css',
    //    'ui-ui-darkness.calendars.picker.css',
    //    'ui-ui-lightness.calendars.picker.css',
    //    'ui-vader.calendars.picker.css',

    'ui.calendars.picker.css',

    'flora.calendars.picker.css',
    'humanity.calendars.picker.css',
    'jquery.calendars.picker.css',
    'redmond.calendars.picker.css',
    'smoothness.calendars.picker.css',
  ];
}
