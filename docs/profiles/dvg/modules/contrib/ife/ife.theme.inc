<?php
/**
 * @file
 * Theme functions.
 */

/**
 * Theming function for the admin settings page.
 */
function theme_ife_settings_form_ids($variables) {
  $form = $variables['form'];
  $header = array('form_id', t('Status'), t('Display type'), t('Field types'), t('Operations'));
  $rows = array();

  $keys = element_children($form);
  array_pop($keys);

  // Add existing forms to table as rows.
  foreach ($keys as $key) {
    $rows[] = array(
      drupal_render($form[$key]['form_id']),
      drupal_render($form[$key]['status']),
      drupal_render($form[$key]['display']),
      drupal_render($form[$key]['field_types']),
      l(t('Remove'), 'admin/config/user-interface/ife/' . $key . '/delete'),
    );
  }

  // Add adding form to table as a row.
  $rows[] = array(
    drupal_render($form['new_form_id']['form_id']),
    drupal_render($form['new_form_id']['status']),
    drupal_render($form['new_form_id']['display']),
    drupal_render_children($form['new_form_id']['field_types']),
    '',
  );

  $output = theme('table', array('header' => $header, 'rows' => $rows));
  return $output;
}

/**
 * Theme the form element, add the error, when one occurred.
 */
function theme_ife_form_element($variables) {
  $element = $variables['element']['#children'];

  // Add in the error if set.
  if (isset($variables['element']['#ife_error']) && $variables['element']['#ife_error']) {

    $error = '<div class="messages error messages-inline">' . $variables['element']['#ife_error'] . '</div>';

    if (isset($variables['element']['#ife_error_position']) && $variables['element']['#ife_error_position'] == IFE_POSITION_INLINE_MESSAGE_BEFORE) {
      $element = $error . $element;
    }
    else {
      $element .= $error;
    }
  }

  return $element;
}
