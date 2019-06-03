<?php

/**
 * Implements hook_form().
 */
function lrs_stats_course_content_general_statistics_form(array $form, array &$form_state)
{
  $opigno_lrs=variable_get('opigno_lrs_stats_course_content_general_statistics_form_values');
  $form['display_period'] = array(
    '#type' => 'select',
    '#options' => array(
      OPIGNO_LRS_STATS_CURRENT_WEEK=> t('Current week'),
      OPIGNO_LRS_STATS_CURRENT_MONTH => t('Current month'),
      OPIGNO_LRS_STATS_CURRENT_YEAR => t('Current year'),
      OPIGNO_LRS_STATS_FOREVER => t('Forever')
    ),
    '#ajax' => array(
      'callback' => 'opigno_lrs_stats_course_content_general_statistics_form_ajax_submit',
      'wrapper' => 'lrs-stats-course-content',
      'method' => 'replace'
    ),
    '#default_value' => $opigno_lrs['display_period'],
  );

  return $form;
}

/**
 * Implements hook_form_submit().
 */
function opigno_lrs_stats_course_content_general_statistics_form_ajax_submit($form, &$form_state)
{
  variable_set('opigno_lrs_stats_course_content_general_statistics_form_values', $form_state['values']);
  return opigno_lrs_stats_course_content_page();
}