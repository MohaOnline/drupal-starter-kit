<?php

/**
 * @file
 * Office hours form elements and their theming and validation.
 *
 * This file is only included during the edit process to reduce memory usage.
 */

/**
 * Process an individual element.
 *
 * Build the form element. When creating a form using FAPI #process,
 * note that $element['#value'] is already set.
 */
function _office_hours_block_process($element, &$form_state, $form) {
  $element['#prefix'] = '';
  $element['#suffix'] = '';

  $has_comment = isset($element['#field_settings']['comment']) ? $element['#field_settings']['comment'] : FALSE;
  $blocks_per_day = $element['#field_settings']['cardinality'];
  $daydelta = $element['#daydelta'];
  $day = $element['#day'];
  $same_link = '';

  if ($daydelta == 0) {
    // This is the first block of the day, show the dayname.
    $label = t($element['#dayname']);
    $label_style = '';
    $block_style = '';
    // Add 'Same as above' link, also for first day in widget, which copies from last day.
    $same_link_text = t($day <> $element['#field_settings']['date_first_day'] ? 'Same as above' : 'Same as last day');
    $same_link = l($same_link_text, '', array('attributes' => array('class' => 'oh-same-link')));
  }
  elseif ($daydelta >= $blocks_per_day) {
    // In case the number of blocks per day was lowered by admin, this element
    // may have a value. Better clear it (in case a value was entered before).
    // The value will be removed upon the next 'Save' action.
    $label = '';
    $label_style = '';
    // The following style is only needed if js isn't working.
    $block_style = 'style = "display:none;"';

    if (empty($element['#value'])) {
      $element['#value'] = array();
    }
    $element['#value']['starthours'] = '';
    $element['#value']['endhours'] = '';
    // The following class is the trigger for js to hide the row. 
    $element['#prefix'] .= "<div class='oh-hide'>";
    $element['#suffix'] .= '</div>';
  }
  elseif (!empty($element['#value']['starthours'])) {
    // This is a following block with contents, so we show the times.
    $label = t('and');
    $label_style = 'style = "text-align:right;"';
    $block_style = '';
  }
  else {
    // This is an empty following block, so we show the 'add hours' js-link.
    $label = t('and');
    $label_style = 'style = "text-align:right;"';
    $block_style = 'style = "display:none;"';

    $link = l(t('Add new @node_type', array('@node_type' => t('Time'), '%type' => 'Time')), '', array('attributes' => array('class' => 'oh-add-more-link')));
    $element['#prefix'] .= $link;
  }
  $element['#prefix'] .= '<div class="form-item office-hours-block" ' . $block_style . ' data-day="' . $day . '">';
  $element['#prefix'] .= '<label ' . $label_style . '>' . $label . '</label>';

  // Show a 'Clear this line' js-link to each element.
  // Use text 'Remove', which has lots of translations.
  $clear_link = l(t('Remove'), '', array('attributes' => array('class' => 'oh-clear-link')));
  $element['#suffix'] .= '<div>' . $clear_link . '&nbsp;&nbsp;&nbsp;' . $same_link . '</div>';

  $element['#suffix'] .= '</div>'; // class="form-item office-hours-block ".

  $element['day'] = array(
    '#type' => 'value',
    '#value' => $element['#day'],
  );
  $element['daydelta'] = array(
    '#type' => 'value',
    '#value' => $element['#daydelta'],
  );
  $element['starthours'] = array(
    '#type' => 'office_hours_select',
    '#default_value' => isset($element['#value']['starthours']) ? $element['#value']['starthours'] : NULL,
    '#field_settings' => $element['#field_settings'],
  );
  $element['endhours'] = array(
    '#type' => 'office_hours_select',
    '#prefix' => t("@from to @to", array('@from' => '', '@to' => '')),
    '#default_value' => isset($element['#value']['endhours']) ? $element['#value']['endhours'] : NULL,
    '#field_settings' => $element['#field_settings'],
  );
  $element['comment'] = array(
    '#type' => $has_comment ? 'textfield' : 'hidden',
    '#prefix' => $has_comment ? t('Comment') . ' ' : '',
    '#default_value' => isset($element['#value']['comment']) ? $element['#value']['comment'] : NULL,
    '#size' => 25,
    '#maxlength' => 255,
  );

  return $element;
}

/**
 * Callback for office_hours_select element.
 *
 * Takes the #default_value and dissects it in hours, minutes and ampm indicator.
 */
function _office_hours_select_value_callback($element, $input = FALSE, &$form_state) {
  return _office_hours_field_widget_time_parse($element['#default_value'], $element['#field_settings']['hoursformat']);
}

/**
 * Process the office_hours_select element.
 */
function _office_hours_select_process($element, &$form_state, $form) {
  $element['hours'] = array(
    '#type' => 'select',
    '#options' => $element['#field_settings']['#hours'],
    '#default_value' => $element['#value']['hour'],
  );
  $element['minutes'] = array(
    '#type' => 'select',
    '#options' => $element['#field_settings']['#minutes'],
    '#default_value' => $element['#value']['minute'],
  );
  if ($element['#field_settings']['hoursformat'] == 1) {
    // ampm options, taken from date_ampm().
    $options = date_ampm();
    $element['ampm'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $element['#value']['ampm'],
    );
  }
  elseif ($element['#field_settings']['hoursformat'] == 3) {
    // ampm options, taken from date_ampm().
    $none = array('' => '');
    $ampm = array(
      'am' => t('a.m.', array(), array('context' => 'ampm')),
      'pm' => t('p.m.', array(), array('context' => 'ampm')),
    );
    $options = $none + $ampm;

    $element['ampm'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $element['#value']['ampm'],
    );
  }

  return $element;
}

/**
 * Validate the hours selector element.
 */
function _office_hours_select_validate($element, &$form_state) {
  $hours = $element['hours']['#value'];
  $minutes = ($element['minutes']['#value'] == 0) ? '00' : $element['minutes']['#value'];
  if ($element['#field_settings']['hoursformat'] == 1) {
    // 12 hrs format with 'a.m.' or 'am'.
    if ($element['ampm']['#value'][0] == 'p' && $hours < 1200) {
      $hours += 1200;
    }
    if ($element['ampm']['#value'][0] == 'a' && $hours == 1200) {
      $hours = '0000';
    }
  }

  if ($hours != '' && $minutes != '') {
    form_set_value($element, _office_hours_time_to_mil($hours + $minutes), $form_state);
  }
  else {
    form_set_value($element, '', $form_state);
  }
  if ($hours < 0000 || $hours > 2300) {
    form_error($element, t('Hours should be between 0 and 23.', array(), array('office_hours')));
  }
  if ($minutes < 0 || $minutes > 59) {
    form_error($element, t('Minutes should be between 0 and 59.', array(), array('office_hours')));
  }
}

/**
 * Implements a callback for _office_hours_elements().
 *
 * For 'office_hours_block' (day) and 'office_hours_select' (hour) elements.
 * You can find the value in $element['#value'], but better in $form_state['values'],
 * which is set in _office_hours_select_validate().
 */
function _office_hours_block_validate($element, &$form_state) {
  $item = drupal_array_get_nested_value($form_state['values'], $element['#parents']);

  $error_text = '';
  $valhrs = $element['#field_settings']['valhrs'];
  $limitstart = $element['#field_settings']['limitstart'];
  $limitend = $element['#field_settings']['limitend'];
  $starthours = $item['starthours'];
  $endhours = $item['endhours'];
  // Do not use empty(), since '0' is a valid value, too.
  if ($starthours == '' xor $endhours == '') {
    $error_text = 'Both Opening hours and Closing hours must be set.';
  }
  elseif ($valhrs && ($starthours > $endhours) && ($endhours != 0)) {
    // Endhours can be '0' because we can close at midnight.
    $error_text = 'Closing hours are earlier than Opening hours.';
  }
  elseif (!empty($limitstart) || !empty($limitend)) {
    if (($starthours && $limitstart > $starthours) || ($endhours && $limitend < $endhours)) {
      $error_text = 'Hours are outside limits ( !start - !end ).';
    }
  }

  if ($error_text) {
    $error_text =
      t($element['#dayname'])
      . ': '
      . t($error_text,
        array(
          '!start' => _office_hours_time_to_24hr($limitstart),
          '!end' => _office_hours_time_to_24hr($limitend),
        ),
        array('context' => 'office_hours')
      );
    form_error($element, check_plain($error_text));
  }
}
