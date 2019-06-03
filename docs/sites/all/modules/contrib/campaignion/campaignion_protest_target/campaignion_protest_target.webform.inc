<?php
/**
 * @file
 */

/**
 * Implements _webform_defaults_<webform component>().
 */
function _webform_defaults_protest_target() {
  $element = array(
    'name'      => t('Target Selector'),
    'form_key'  => NULL,
    'mandatory' => 0,
    'pid'       => 0,
    'value'     => '',
    'weight'    => 0,
    'extra'     => array(
      'items' => '',
      'multiple' => NULL,
      'aslist' => NULL,
      'description' => t('Select an email protest target.'),
      'private' => FALSE,
      'required' => FALSE,
    ),
  );

  return $element;
}

/**
 * Implements _webform_edit_<webform component>().
 */
function _webform_edit_protest_target($component) {
  $element['value'] = array(
    '#description' => '',
    '#title'       => t('Select default target'),
    '#weight'      => 0,
  ) + _webform_render_protest_target($component);
  return $element;
}

/**
 * Helper function to get the current node object
 */
function _campaignion_protest_target_get_node(&$component = NULL) {
  static $node = NULL;

  if ($node == NULL) {
    if (!empty($component['nid'])) {
      $node = node_load($component['nid']);
    }
    else {
      if (($node = menu_get_object()) == NULL) {
        $get = NULL;
        if (isset($_GET['destination']) == TRUE) {
          $get = $_GET['destination'];
        }
        elseif (isset($_GET['q']) == TRUE) {
          $get = $_GET['q'];
        }
        if (   $get != NULL
            && is_numeric(($nid = preg_replace('/^.*node\/(\d+)\/.*$/', '$1', $get))) == TRUE) {
          $node = node_load($nid);
        }
      }
    }
  }
  return $node;
}

/**
 * Implements _webform_render_<webform component>().
 */
function _webform_render_protest_target($component, $value = NULL, $filter = TRUE) {
  $component['extra'] += [
    'title_display' => 'before',
  ];
  // We have to add those in any case - form_builder expects those.
  $element = array(
    '#webform_component' => $component,
    '#title_display' => $component['extra']['title_display'],
    '#title'         => $component['name'],
    '#description'   => $component['extra']['description'],
    '#default_value' => $component['value'],
    '#weight'        => $component['weight'],
    '#required'      => $component['mandatory'],
    '#translatable'  => array('title', 'description'),
  );

  $node = _campaignion_protest_target_get_node($component);
  if (empty($node)) {
    return NULL;
  }
  $protest_target_options = field_get_items('node', $node, 'field_protest_target_options');

  if (   isset($protest_target_options[0]['value']) == TRUE
      && $protest_target_options[0]['value'] == 'choose_one') {

    $protest_target = field_get_items('node', $node, 'field_protest_target');

    if(isset($protest_target[0]['target_id']) == TRUE) {
      $options = array();
      foreach ($protest_target as $target) {
        $contact = redhen_contact_load($target['target_id']);
        $options[$target['target_id']] = $contact->first_name . " " . $contact->last_name;
      }
      reset($options);
      $element += array(
        '#theme_wrappers' => ['webform_element'],
        '#type'     => 'select',
        '#options'  => $options,
        '#multiple' => FALSE,
        '#other'    => FALSE,
      );
    }
  }
  else {
    $element += array(
      '#type' => 'markup',
      '#markup' => '',
    );
  }

  return $element;
}

/**
 * Implements _webform_CALLBACK_TYPE().
 * Implements _webform_form_builder_map_<form builder type>().
 */
function _webform_form_builder_map_protest_target() {
  return array(
    'form_builder_type' => 'protest_target',
    'properties' => array(
      'default_target' => array(
        'storage_parents' => array('value'),
      ),
    ),
  );
}

/**
 * Implements _webform_table_<webform component>().
 */
function _webform_table_protest_target($component, $value) {
  if (!empty($value[0]) && ($contact = entity_load_single('redhen_contact', $value[0]))) {
    return check_plain(redhen_contact_property_full_name_get($contact, [], 'full_name', 'redhen_contact'));
  }
  return '';
}

/**
 * Implements  _webform_csv_headers_<webform component>().
 */
function _webform_csv_headers_protest_target($component, $export_options) {
  $header = array();
  $header[0] = '';
  $header[1] = '';
  $header[2] = $export_options['header_keys'] ? $component['form_key'] : $component['name'];
  return $header;
}

/**
 * Implements _webform_csv_data_<webform component>().
 */
function _webform_csv_data_protest_target($component, $export_options, $value) {
  if (!empty($value[0]) && ($contact = entity_load_single('redhen_contact', $value[0]))) {
    return redhen_contact_property_full_name_get($contact, [], 'full_name', 'redhen_contact');
  }
  return '';
}

/*
 * Callback referenced in _webform_form_builder_properties_<webform-component> hook
 */
function campaignion_protest_target_form_builder_property_default_target_form(&$form_state, $form_type, $element, $property) {
  $form['options'] = array(
    '#form_builder' => array('property_group' => 'options'),
    '#tree' => TRUE,
  );
  // we need to redo the mapping here because storage_parents arent resolved until
  // the form is saved for good.
  if (isset($element['#default_target'])) {
    $element['#webform_component']['value'] = $element['#default_target'];
  }
  $form['options'] += _webform_edit_protest_target($element['#webform_component']);

  return $form;
}

/*
 * Callback referenced in _webform_form_builder_properties_<webform-component> hook
 */
function campaignion_protest_target_form_builder_property_default_target_submit(&$form, &$form_state) {
  $form_state['values']['default_target'] = $form_state['values']['options']['value'];
}

/**
 * Implements _webform_form_builder_properties_<webform-component>().
 */
function _webform_form_builder_properties_protest_target() {
  return array(
    'default_target' => array(
      'form'   => 'campaignion_protest_target_form_builder_property_default_target_form',
      'submit' => array('campaignion_protest_target_form_builder_property_default_target_submit'),
    ),
  );
}
