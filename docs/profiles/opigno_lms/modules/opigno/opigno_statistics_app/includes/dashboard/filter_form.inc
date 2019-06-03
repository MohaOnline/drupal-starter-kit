<?php

/**
 * Implements hook_form().
 */
function opigno_statistics_app_dashboard_filter_form(array $form, array &$form_state) {
  $form['#attributes'] = array('class' => 'clearfix');

  $years = range(opigno_statistics_app_install_year(), opigno_statistics_app_current_year());
  $default_value=variable_get('opigno_statistics_app_dashboard_filter_form_values');
  $form['year'] = array(
    '#title' => t('Year'),
    '#type' => 'select',
    '#options' => array_combine($years, $years),
    '#ajax' => array(
      'callback' => 'opigno_statistics_app_dashboard_filter_form_ajax_submit',
      'wrapper' => 'opigno-statistics-app-dashboard',
      'method' => 'append'
    ),
    '#default_value' => $default_value['year'],
  );

  $months = date_month_names();
  $form['month'] = array(
    '#title' => t('Month'),
    '#type' => 'select',
    '#options' => $months,
    '#ajax' => array(
      'callback' => 'opigno_statistics_app_dashboard_filter_form_ajax_submit',
      'wrapper' => 'opigno-statistics-app-dashboard',
      'method' => 'append'
    ),
    '#default_value' => $default_value['month'],
  );

  $categories = db_query('SELECT DISTINCT category_taxonomy_term_id, category_taxonomy_term_name FROM {opigno_statistics_group}')->fetchAllKeyed();
  $categories[''] = '';

  $form['category'] = array(
    '#title' => t('Category'),
    '#type' => 'select',
    '#options' => $categories,
    '#ajax' => array(
      'callback' => 'opigno_statistics_app_dashboard_filter_form_ajax_submit',
      'wrapper' => 'opigno-statistics-app-dashboard',
      'method' => 'append'
    ),
    '#default_value' => $default_value['category'],
  );

  return $form;
}

/**
 * Implements hook_form_ajax_submit().
 */
function opigno_statistics_app_dashboard_filter_form_ajax_submit($form, &$form_state) {
  $form_values = $form_state['values'];
  variable_set('opigno_statistics_app_dashboard_filter_form_values', $form_values);
  $has_month = !empty($form_values['month']);
  $month_year = ($has_month? mktime(0, 0, 0, $form_values['month'], 1, $form_values['year']) : mktime(0, 0, 0, 1, 1, $form_values['year']));
  variable_set('opigno_statistics_app_dashboard_month_year', $month_year);
  variable_set('opigno_statistics_app_dashboard_has_month', $has_month);
  variable_set('opigno_statistics_app_dashboard_category', $form_values['category']);
  return '<script type="text/javascript">document.location.reload();</script>';
}