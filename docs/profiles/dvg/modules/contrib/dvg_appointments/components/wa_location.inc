<?php

/**
 * @file
 * WA location component.
 */

/**
 * Implements _webform_defaults_COMPONENT().
 */
function _webform_defaults_wa_location() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'required' => 0,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'extra' => array(
      'items' => '',
      'multiple' => NULL,
      'aslist' => NULL,
      'empty_option' => '',
      'optrand' => 0,
      'other_option' => NULL,
      'other_text' => t('Other...'),
      'title_display' => 0,
      'description' => '',
      'description_above' => FALSE,
      'custom_keys' => FALSE,
      'options_source' => '',
      'private' => FALSE,
      'analysis' => TRUE,
    ),
  );
}

/**
 * Implements _webform_edit_COMPONENT().
 */
function _webform_edit_wa_location($component) {
  return array('extra' => array());
}

/**
 * Implements _webform_render_COMPONENT().
 */
function _webform_render_wa_location($component, $value = NULL, $filter = TRUE) {
  $form_item = array(
    '#type' => 'wa_location',
    '#title' => $filter ? webform_filter_xss($component['name']) : $component['name'],
    '#required' => $component['required'],
    '#weight' => $component['weight'],
    '#description'   => $filter ? webform_filter_descriptions($component['extra']['description']) : $component['extra']['description'],
    '#prefix' => '<div class="webform-component-wa-location" id="webform-component-' . $component['form_key'] . '">',
    '#suffix' => '</div>',
  );

  if (!empty($value[0])) {
    $form_item['#default_value'] = $value[0];
  }

  return $form_item;
}

/**
 * Implements _webform_display_COMPONENT().
 */
function _webform_display_wa_location($component, $value, $format = 'html') {
  $location = dvg_appointments_get_location_information($value[0]);
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#required' => $component['required'],
    '#theme' => 'webform_display_textfield',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#post_render' => array('webform_element_wrapper'),
    '#component' => $component,
    '#format' => $format,
    '#value' => $location,
  );
}

/**
 * Returns HTML for the product/count fields on form.
 */
function theme_dvg_appointments_location($variables) {
  $element = $variables['element'];
  return $element['#children'];
}

/**
 * Element process callback for the wa_location elements.
 */
function dvg_appointments_location_element_process($element, &$form_state, $form) {
  $client = dvg_appointments_get_client_api();
  $selected_products = dvg_appointments_get_selected_products($form_state, $form);

  $available_locations = $client->get_filtered_locations($selected_products);
  $location_options = array();
  foreach ($available_locations as $location) {
    // The 'available_products' index might not exist in the location array if there is no location for the selected product.
    // Thus, check if the 'available_products' index exist in the location array to prevent "Undefined index" error.
    if (array_key_exists('available_products', $location) && count($selected_products) == count($location['available_products'])) {
      $location_options[$location['id']] = $location['detail']->locationDesc;
    }
  }

  if ($count_location = count($location_options)) {
    if (count($available_locations) > $count_location) {
      $element['error_message'] = [
        '#markup' => t("Some products are not available at all locations. Your appointment can only be made with the following location(s)."),
      ];
    }

    $element['location'] = array(
      '#type' => 'radios',
      '#title' => $element['#title'],
      '#options' => $location_options,
      '#attributes' => array('class' => array('wa-location')),
    );
    if ($count_location > 5) {
      $element['location']['#type'] = 'select';
      // Prepend empty option while preserving keys.
      $element['location']['#options'] =  array('' => t('Select a location')) + $element['location']['#options'];
    }

    if (!empty($element['#default_value'])) {
      $element['location']['#default_value'] = $element['#default_value'];
    }
  }
  else {
    $element['error_message'] = [
      '#markup' => t("The selected products are not available at any location. Try making separate appointments for each product."),
    ];

    $form_state['complete form']['actions']['next']['#attributes']['disabled'] = 'disabled';
    $form_state['complete form']['actions']['next']['#attributes']['title'] = t('Location needs to be selected to be able to proceed.');
  }

  return $element;
}

/**
 * Implements _webform_submit_COMPONENT().
 */
function _webform_submit_wa_location($component, $value) {
  if (isset($value['location'])) {
    return $value['location'];
  }
  return array();
}
