<?php

/**
 * @file
 * DVG Webform component bsn.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_bsn() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'required' => FALSE,
    'extra' => array(
      'width' => '',
      'disabled' => FALSE,
      'description' => '',
      'private' => FALSE,
    ),
  );
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_bsn() {
  return array(
    'webform_display_bsn' => array(
      'render element' => 'element',
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_bsn($component) {
  $form = array();
  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Default value'),
    '#default_value' => $component['value'],
    '#description' => t('The default value of the field.') . theme('webform_token_help'),
    '#size' => 10,
    '#weight' => -1,
  );
  $form['display']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => $component['extra']['width'],
    '#description' => t('Width of the BSN field.') . ' ' . t('Leaving blank will use the default size.'),
    '#size' => 5,
    '#maxlength' => 10,
    '#weight' => 0,
    '#parents' => array('extra', 'width'),
  );
  $form['display']['disabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disabled'),
    '#return_value' => 1,
    '#description' => t('Make this field non-editable. Useful for setting an unchangeable default value.'),
    '#weight' => 1,
    '#default_value' => $component['extra']['disabled'],
    '#parents' => array('extra', 'disabled'),
  );
  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_bsn($component, $value = NULL, $filter = TRUE) {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;

  $element = array(
    '#type' => 'textfield',
    '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#default_value' => $filter ? _webform_filter_values($component['value'], $node, NULL, NULL, FALSE) : $component['value'],
    '#description' => $filter ? webform_filter_descriptions($component['extra']['description'], $node) : $component['extra']['description'],
    '#required' => $component['required'],
    '#weight' => $component['weight'],
    '#theme_wrappers' => array('webform_element'),
    '#translatable' => array('title', 'description'),
  );

  // Handle disabling.
  if ($component['extra']['disabled']) {
    if ($filter) {
      $element['#attributes']['readonly'] = 'readonly';
    }
    else {
      $element['#disabled'] = TRUE;
    }
  }

  $element['#value_callback'] = 'dvg_webform_components_bsn_value';

  $element['#element_validate'] = array('dvg_webform_components_bsn_validate');

  // Change the 'width' option to the correct 'size' option.
  if ($component['extra']['width'] > 0) {
    $element['#size'] = $component['extra']['width'];
  }

  if (isset($value)) {
    $element['#default_value'] = $value[0];
  }
  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_bsn($component, $value, $format = 'html') {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_bsn',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#format' => $format,
    '#value' => $value[0],
    '#translatable' => array('title'),
  );
}

/**
 * Custom Theme function for bsn value.
 */
function theme_webform_display_bsn($variables) {
  $element = $variables['element'];
  $value = $element['#value'];
  if (!empty($value)) {
    $value = str_pad($value, 9, '0', STR_PAD_LEFT);
  }
  return $value;
}

/**
 * Implements _webform_analysis_component().
 */
function _webform_analysis_bsn($component, $sids = array()) {
  $query = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC))
    ->fields('wsd', array('data'))
    ->condition('nid', $component['nid'])
    ->condition('cid', $component['cid']);

  if (count($sids)) {
    $query->condition('sid', $sids, 'IN');
  }

  $nonblanks = 0;
  $submissions = 0;

  // This could probably be extended to count submissions by
  // country using the bsn_validation API.
  $result = $query->execute();
  foreach ($result as $data) {
    if (drupal_strlen(trim($data['data'])) > 0) {
      $nonblanks++;
    }
    $submissions++;
  }

  $rows[0] = array(t('Left Blank'), ($submissions - $nonblanks));
  $rows[1] = array(t('User entered value'), $nonblanks);
  return $rows;
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_bsn($component, $value) {
  return check_plain(empty($value[0]) ? '' : $value[0]);
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_bsn($component, $export_options) {
  $header = array();
  $header[0] = '';
  $header[1] = '';
  $header[2] = $export_options['header_keys'] ? $component['form_key'] : $component['name'];
  return $header;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_bsn($component, $export_options, $value) {
  return !isset($value[0]) ? '' : $value[0];
}

/**
 * Implements hook_validate().
 *
 * Validate BSN.
 */
function dvg_webform_components_bsn_validate($element, &$form_state) {

  if ($element['#value'] === '') {
    return;
  }

  $result = _bsn_validation_validate($element['#value']);
  if (!empty($result)) {
    form_error($element, $result);
  }

}

/**
 * Parse bsn value before save.
 */
function dvg_webform_components_bsn_value($element, $input = FALSE, $form_state) {
  // Check for input.
  if ($input) {
    // Strip whitespace from bsn.
    $trimmed_input = str_replace(array('.', ',', ' '), '', trim($input));
    // Pad bsn to 9 digits.
    return str_pad($trimmed_input, 9, '0', STR_PAD_LEFT);
  }
}

/**
 * Controleer Burger Service Nummer (BSN)
 *
 * @param string $bsn
 *   Het Burger Service Nummer.
 *
 * @return mixed
 *   Foutboodschap of lege string als geen fout gevonden
 */
function _bsn_validation_validate($bsn) {
  // Verwijder tussenliggende punten, komma's en spaties.
  $bsnr = str_replace(array('.', ',', ' '), '', trim($bsn));

  // BSN: alleen cijfers.
  if (!is_numeric($bsnr)) {
    return t('The entered BSN "@bsn" contains invalid characters.', array('@bsn' => $bsn));
  }

  // Verwijder voorloopnullen.
  while (substr($bsnr, 0, 1) == '0' && strlen($bsnr > 0)) {
    $bsnr = substr($bsnr, 1);
  }

  // BSN: controle op lengte en juistheid.
  $len = strlen($bsnr);
  if ($len > 9) {
    return t('The entered BSN "@bsn" contains to many digits.', array('@bsn' => $bsn));
  }
  if ($len < 8) {
    return t('The entered BSN "@bsn" has to contain at least 8 digits.', array('@bsn' => $bsn));
  }
  elseif ($bsnr <= 0 || _bsn_elfproef($bsnr)) {
    return t('The entered BSN "@bsn" does not appear valid.', array('@bsn' => $bsn));
  }
  return '';
}

/**
 * Function _bsn_elfproef().
 *
 * Controleer een BSN mbv de speciale elfproef (Modulo 11)
 * Wijkt af van de elfproef voor bankrekeningnummers:
 * het meest rechtercijfer wordt afgetrokken van het voorgaande resultaat.
 *
 * @param string $bsnr
 *   Het geheel numerieke Burger Service Nummer.
 *
 * @return:
 *   True als fout, false als voldoet aan de proef.
 */
function _bsn_elfproef($bsnr) {
  $res = 0;
  $verm = strlen($bsnr);

  for ($i = 0; $i < strlen($bsnr); $i++, $verm--) {
    if ($verm == 1) {
      $verm = -1;
    }
    $res += substr($bsnr, $i, 1) * $verm;
  }
  return ($res % 11);
}
