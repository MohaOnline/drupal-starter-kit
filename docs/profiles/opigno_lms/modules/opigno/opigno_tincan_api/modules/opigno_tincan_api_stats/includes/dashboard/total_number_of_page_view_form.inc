<?php

/**
 * Implements hook_form().
 */
function opigno_lrs_stats_dashboard_total_number_of_page_view_form(array $form, array &$form_state) {
  $opigno_lrs=variable_get('opigno_lrs_stats_dashboard_total_number_of_page_view_form_values');
  $form['display_period'] = array(
    '#type' => 'select',
    '#options' => array(
      OPIGNO_LRS_STATS_CURRENT_WEEK_AGGREGATE_DAY => t('Current week'). '/' . t('day'),
      OPIGNO_LRS_STATS_CURRENT_MONTH_AGGREGATE_DAY => t('Current month'). '/' . t('day'),
      OPIGNO_LRS_STATS_CURRENT_YEAR_AGGREGATE_DAY => t('Current year'). '/' . t('day'),
      OPIGNO_LRS_STATS_CURRENT_YEAR_AGGREGATE_MONTH => t('Current year'). '/' . t('month'),
      OPIGNO_LRS_STATS_FOREVER_AGGREGATE_DAY => t('Forever'). '/' . t('day'),
      OPIGNO_LRS_STATS_FOREVER_AGGREGATE_MONTH => t('Forever'). '/' . t('month'),
      OPIGNO_LRS_STATS_FOREVER_AGGREGATE_YEAR => t('Forever'). '/' . t('year'),
    ),
    '#ajax' => array(
      'callback' => 'opigno_lrs_stats_dashboard_total_number_of_page_view_form_ajax_submit',
      'wrapper' => 'lrs-stats-widget-dashboard-number-page-view',
      'method' => 'replace'
    ),
    '#default_value' => $opigno_lrs['display_period'],
  );

  return $form;
}

/**
 * Implements hook_form_submit().
 */
function opigno_lrs_stats_dashboard_total_number_of_page_view_form_ajax_submit($form, &$form_state)
{
  variable_set('opigno_lrs_stats_dashboard_total_number_of_page_view_form_values', $form_state['values']);
  return  theme('opigno_lrs_stats_dashboard_widget_total_number_of_page_view');
}