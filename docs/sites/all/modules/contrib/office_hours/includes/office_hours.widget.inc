<?php

/**
 * @file
 * Implements the office_hours widget.
 */

/**
 * Implements hook_field_widget_info().
 */
function office_hours_field_widget_info() {
  return array(
    'office_hours' => array(
      'label' => t('Office hours (week)'),
      'description' => t("A 7-day widget"),
      'field types' => array('office_hours'),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
        'default value' => FIELD_BEHAVIOR_DEFAULT,
      ),
    ),
    'office_hours_dynamic_widget' => array(
      'label' => t('Office hours (list)'),
      'description' => t("An 'add another' widget"),
      'field types' => array('office_hours'),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 *
 * Generates a set of time_block form-items.
 * N.B. when changing this function, be aware that it is also used in office_hours_field_settings_form().
 */
function office_hours_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $widget = array();

  // Get field settings, to make it accessible for each element in other functions.
  $settings = $field['settings'];
  $cardinality = $settings['cardinality'];
  $granularity = $settings['granularity'];

  // Get the day names.
  $daynames = date_week_days(FALSE);
  // Get the valid hours. Date API doesn't provide a straight method for this.
  $hours =  _office_hours_field_widget_hours($settings);
  //  $hours = drupal_map_assoc($hours); // $key == $value.
  // Get the valid minutes, using Date API.
  $minutes = date_minutes('i', FALSE, $granularity);
  $minutes = drupal_map_assoc($minutes);
  // Add the hours/minutes to the field settings, for best performance.
  $settings['#hours'] = $hours;
  $settings['#minutes'] = $minutes;

  switch ($instance ['widget']['type']) {

    case 'office_hours':
      // Create full widget with 7 days: an indexed two level array of time blocks.
      // First level are day numbers. Second level contains field values arranged by daydelta.
      $indexed_days = array_fill_keys(array(0, 1, 2, 3, 4, 5, 6), array());
      foreach ($items as $index => $item) {
        $indexed_days[(int) $item['day']][] = $item;
      }
      $items = $indexed_days;

      // Build elements, sorted by first_day_of_week.
      $elements = array();
      $days = _office_hours_week_days_ordered(range(0, 6), $settings['date_first_day']);
      foreach ($days as $index => $day) {
        // todo: theme_function clears values above cardinality. move it here.
        for ($daydelta = 0; $daydelta < max(2, $cardinality); $daydelta++) {
          $element['#type'] = 'office_hours_block';
          $element['#default_value'] = isset($items[$day][$daydelta]['starthours']) ? $items[$day][$daydelta] : NULL;
          $element['#day'] = $day;
          $element['#daydelta'] = $daydelta;
          $element['#dayname'] = $daynames[$day];
          $element['#field_settings'] = $settings;

          $elements[] = $element;
        }
      }

      // Build multi element widget.
      $widget = array(
          '#theme' => 'office_hours_week',
          '#field_name' => $field['field_name'],
          '#title' => check_plain($instance['label']),
          '#required' => $instance['required'],
          '#description' => field_filter_xss($instance['description']),
        ) + $elements;

      break;

    case 'office_hours_dynamic_widget':
      // Create small widget with 'add another' functionality.
      // N.B. Some duplicate code is introduced: @see _office_hours_block_process()

      $widget = $element;
      $widget['#delta'] = $delta;
      $widget['day'] = array(
        '#type' => 'select',
        '#title' => t('Day'),
        '#default_value' => isset($items[$delta]['day']) ? $items[$delta]['day'] : NULL,
        '#options' => $daynames,
      );
      $widget['starthours'] = array(
        '#type' => 'office_hours_select',
        // Using capitals enables automatic translation. mb_strtolower also converts 'À' to 'à'.
//        '#prefix' => mb_strtolower( t('From', array('context' => 'office_hours')) ) . ' ',
        '#default_value' => isset($items[$delta]['starthours']) ? $items[$delta]['starthours'] : NULL,
        '#field_settings' => $settings,
      );
      $widget['endhours'] = array(
        '#type' => 'office_hours_select',
        // Using capitals enables automatic translation. mb_strtolower also converts 'À' to 'à'.
        '#prefix' => t("@from to @to", array('@from' => '', '@to' => '')),
        '#default_value' => isset($items[$delta]['endhours']) ? $items[$delta]['endhours'] : NULL,
        '#field_settings' => $settings,
      );
      break;
  }

  return $widget;
}

/**
 * Implements hook_field_widget_error().
 *
 * This is necessary to show the errors thrown in hook_field_validate().
 */
function office_hours_field_widget_error($element, $error, $form, &$form_state) {
  form_error($element, $error['message']);
}
