<?php

/**
 * @file
 * The default format for adresses.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_address() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'required' => FALSE,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'po_box' => FALSE,
      'addresscheck' => 'none',
      'city_options' => 'textfield',
      'cities_piped_list' => '',
      'internal_postal_codes' => FALSE,
      'internal_postal_codes_list' => '',
      'placeholders' => FALSE,
      'private' => FALSE,
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_address($component) {
  $form = array();

  $form['extra']['po_box'] = array(
    '#type' => 'checkbox',
    '#title' => t('PO Box'),
    '#description' => t('Check this if this is a PO box address. Street name, number and addition will dissapear and PO box field becomes available. NOTE: address checks are not available with a PO box.'),
    '#default_value' => $component['extra']['po_box'],
  );
  $addresscheck_options = array();
  $addresscheck_options['none'] = t('None - no checks are performed');
  $dvg_postcodenl_key = variable_get('dvg_postcodenl_key');
  if (!empty($dvg_postcodenl_key)) {
    $addresscheck_options['postcodenl'] = t('Postcode.nl - only addresses in the Netherlands are allowed');
  }
  $dvg_stuf_bg_aoa_api_url = variable_get('dvg_stuf_bg_aoa_api_url');
  if (function_exists('dvg_stuf_bg_adresgegevens') && !empty($dvg_stuf_bg_aoa_api_url)) {
    $addresscheck_options['stuf-bg'] = t('StUF-BG Addresscheck - only addresses in council are allowed');
    $addresscheck_options['stuf-bg-only-zipcode-housenumber'] = t('StUF-BG Addresscheck (only zipcode and housenumber) - only addresses in council are allowed');
  }

  drupal_alter('dvg_webform_components_address_options', $addresscheck_options);

  $form['extra']['addresscheck'] = array(
    '#type' => 'select',
    '#title' => t('Address check'),
    '#description' => t('Optional. Select the service to complete streetname and city based on postal code + house number.'),
    '#options' => $addresscheck_options,
    '#default_value' => $component['extra']['addresscheck'],
  );
  $form['extra']['city_options'] = array(
    '#type' => 'select',
    '#title' => t('City options'),
    '#description' => t('Do you want to display the city field as a select list or free textfield? NOTE: address checks are not available if predefined list is used.'),
    '#options' => array(
      'textfield' => t('Textfield'),
      'select' => t('Predefined list'),
    ),
    '#default_value' => $component['extra']['city_options'],
  );
  $form['extra']['cities_piped_list'] = array(
    '#type' => 'textarea',
    '#title' => t('Predefined list of cities'),
    '#description' => t('If you choose predefined list above, please provide a piped list key|value of all available cities. Enter one key|value per line. Key may not contain space as prefix or suffix.'),
    '#default_value' => $component['extra']['cities_piped_list'],
    '#states' => [
      'required' => [
        'select[name="extra[city_options]"]' => ['value' => 'select'],
      ],
    ],
    '#element_validate' => ['_address_validate_cities_piped_list'],
  );
  $form['extra']['internal_postal_codes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable internal predefined postal code list'),
    '#description' => t('Check this if you want an internal validation on postal codes with a list'),
    '#default_value' => $component['extra']['internal_postal_codes'],
  );
  $form['extra']['internal_postal_codes_list'] = array(
    '#type' => 'textarea',
    '#title' => t("Predefined list of postal code's"),
    '#description' => t('If you choose predefined list above, please provide a comma separated list of all available postal codes.'),
    '#default_value' => $component['extra']['internal_postal_codes_list'],
    // Show this field only if the name field is filled.
    '#states' => array(
      'visible' => array(
        'input[name="extra[internal_postal_codes]"]' => array('checked' => TRUE),
      ),
    ),
    '#element_validate' => array('_address_validate_list_internal_postal_codes'),
  );
  $form['extra']['placeholders'] = array(
    '#type' => 'checkbox',
    '#title' => t('Placeholders'),
    '#description' => t('Use a placeholder instead of the label. The placeholder will be shown in the field until the user starts entering a value.'),
    '#default_value' => $component['extra']['placeholders'],
  );
  $form['extra']['default_values'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default values'),
  );
  $form['extra']['default_values']['postal_code'] = array(
    '#type' => 'textfield',
    // @codingStandardsIgnoreLine
    '#title' => t('Default value') . ' : ' . t('Postal code'),
    '#default_value' => isset($component['extra']['default_values']['postal_code']) ? $component['extra']['default_values']['postal_code'] : '',
    '#description' => t('The default value of the field.') . theme('webform_token_help'),
  );
  $form['extra']['default_values']['house_number'] = array(
    '#type' => 'textfield',
    // @codingStandardsIgnoreLine
    '#title' => t('Default value') . ' : ' . t('House number'),
    '#default_value' => isset($component['extra']['default_values']['house_number']) ? $component['extra']['default_values']['house_number'] : '',
    '#description' => t('The default value of the field.') . theme('webform_token_help'),
  );
  $form['extra']['default_values']['house_letter'] = array(
    '#type' => 'textfield',
    // @codingStandardsIgnoreLine
    '#title' => t('Default value') . ' : ' . t('House letter'),
    '#default_value' => isset($component['extra']['default_values']['house_letter']) ? $component['extra']['default_values']['house_letter'] : '',
    '#description' => t('The default value of the field.') . theme('webform_token_help'),
  );
  $form['extra']['default_values']['house_number_addition'] = array(
    '#type' => 'textfield',
    // @codingStandardsIgnoreLine
    '#title' => t('Default value') . ' : ' . t('Number addition'),
    '#default_value' => isset($component['extra']['default_values']['house_number_addition']) ? $component['extra']['default_values']['house_number_addition'] : '',
    '#description' => t('The default value of the field.') . theme('webform_token_help'),
  );
  $form['extra']['default_values']['street_name'] = array(
    '#type' => 'textfield',
    // @codingStandardsIgnoreLine
    '#title' => t('Default value') . ' : ' . t('Street name'),
    '#default_value' => isset($component['extra']['default_values']['street_name']) ? $component['extra']['default_values']['street_name'] : '',
    '#description' => t('The default value of the field.') . theme('webform_token_help'),
  );
  $form['extra']['default_values']['po_box'] = array(
    '#type' => 'textfield',
    // @codingStandardsIgnoreLine
    '#title' => t('Default value') . ' : ' . t('PO Box'),
    '#default_value' => isset($component['extra']['default_values']['po_box']) ? $component['extra']['default_values']['po_box'] : '',
    '#description' => t('The default value of the field.') . theme('webform_token_help'),
  );
  $form['extra']['default_values']['city'] = array(
    '#type' => 'textfield',
    // @codingStandardsIgnoreLine
    '#title' => t('Default value') . ' : ' . t('City'),
    '#default_value' => isset($component['extra']['default_values']['city']) ? $component['extra']['default_values']['city'] : '',
    '#description' => t('The default value of the field.') . theme('webform_token_help'),
  );
  $form['extra']['default_values']['council_code'] = array(
    '#type' => 'textfield',
    // @codingStandardsIgnoreLine
    '#title' => t('Default value') . ' : ' . t('Council code'),
    '#default_value' => isset($component['extra']['default_values']['council_code']) ? $component['extra']['default_values']['council_code'] : '',
    '#description' => t('The default value of the field.') . theme('webform_token_help'),
  );
  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_address($component, $value = NULL, $filter = TRUE) {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;

  // Wrap the form in a container, in order to correctly apply the weight.
  $elements = array(
    '#type' => 'container',
    '#description' => $filter ? webform_filter_descriptions($component['extra']['description'], $node) : $component['extra']['description'],
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'dvg_webform_components') . '/css/address.css',
      ),
      'js' => array(
        drupal_get_path('module', 'dvg_webform_components') . '/js/address.js',
      ),
    ),
    '#weight' => $component['weight'],
    '#theme_wrappers' => array('webform_element'),
    '#webform_component' => $component,
  );
  $elements['postal_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Postal code'),
    '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
    '#default_value' => _dvg_webform_components_address_default_value('postal_code', $value, $component),
    '#required' => $component['required'],
    '#attributes' => array(),
    '#size' => 10,
    '#theme_wrappers' => array('webform_element'),
    '#webform_component' => array('type' => $component['type'] . '_postal_code') + $component,
  );
  if (empty($component['extra']['po_box'])) {
    $elements['house_number'] = array(
      '#type' => 'textfield',
      '#title' => t('House number'),
      '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
      '#default_value' => _dvg_webform_components_address_default_value('house_number', $value, $component),
      '#required' => $component['required'],
      '#attributes' => array(),
      '#size' => 10,
      '#theme_wrappers' => array('webform_element'),
      '#webform_component' => array('type' => $component['type'] . '_house_number') + $component,
    );
    $elements['house_letter'] = array(
      '#type' => 'textfield',
      '#title' => t('House letter'),
      '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
      '#default_value' => _dvg_webform_components_address_default_value('house_letter', $value, $component),
      '#required' => FALSE,
      '#attributes' => array(),
      '#size' => 6,
      '#theme_wrappers' => array('webform_element'),
      '#webform_component' => array('type' => $component['type'] . '_house_letter') + $component,
    );
    $elements['house_number_addition'] = array(
      '#type' => 'textfield',
      '#title' => t('Number addition'),
      '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
      '#default_value' => _dvg_webform_components_address_default_value('house_number_addition', $value, $component),
      '#required' => FALSE,
      '#attributes' => array(),
      '#size' => 10,
      '#theme_wrappers' => array('webform_element'),
      '#webform_component' => array('type' => $component['type'] . '_house_number_addition') + $component,
    );
    $elements['block'] = array(
      '#type' => 'container',
      '#theme_wrappers' => array('webform_element'),
      '#webform_component' => array('type' => $component['type'] . '_block') + $component,
    );
    $elements['street_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Street name'),
      '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
      '#default_value' => _dvg_webform_components_address_default_value('street_name', $value, $component),
      '#required' => $component['required'],
      '#attributes' => array(),
      '#theme_wrappers' => array('webform_element'),
      '#webform_component' => array('type' => $component['type'] . '_street_name') + $component,
    );
    $elements['street_block'] = array(
      '#type' => 'container',
      '#theme_wrappers' => array('webform_element'),
      '#webform_component' => array('type' => $component['type'] . '_block') + $component,
    );
    $elements['council_code'] = array(
      '#type' => 'hidden',
      '#title' => t('Council code'),
      '#title_display' => 'before',
      '#default_value' => _dvg_webform_components_address_default_value('council_code', $value, $component),
      '#required' => FALSE,
      '#theme' => 'webform_display_council_code',
      '#attributes' => array(),
    );
    if ($component['extra']['addresscheck'] == 'stuf-bg' || $component['extra']['addresscheck'] == 'stuf-bg-only-zipcode-housenumber') {
      $elements['street_name']['#attributes']['readonly'] = 'readonly';
    }
  }
  else {
    $elements['po_box'] = array(
      '#type' => 'textfield',
      '#title' => t('PO Box'),
      '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
      '#default_value' => _dvg_webform_components_address_default_value('po_box', $value, $component),
      '#required' => $component['required'],
      '#attributes' => array(),
      '#size' => 12,
      '#theme_wrappers' => array('webform_element'),
      '#webform_component' => array('type' => $component['type'] . '_po_box') + $component,
    );
    $elements['po_box_block'] = array(
      '#type' => 'container',
      '#theme_wrappers' => array('webform_element'),
      '#webform_component' => array('type' => $component['type'] . '_block') + $component,
    );
  }

  if ($component['extra']['city_options'] == 'textfield') {
    $elements['city'] = array(
      '#type' => 'textfield',
      '#title' => t('City'),
      '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
      '#default_value' => _dvg_webform_components_address_default_value('city', $value, $component),
      '#required' => $component['required'],
      '#attributes' => array(),
      '#theme_wrappers' => array('webform_element'),
      '#webform_component' => array('type' => $component['type'] . '_city') + $component,
    );
    $elements['city_block'] = array(
      '#type' => 'container',
      '#theme_wrappers' => array('webform_element'),
      '#webform_component' => array('type' => $component['type'] . '_block') + $component,
    );
    if ($component['extra']['addresscheck'] == 'stuf-bg' || $component['extra']['addresscheck'] == 'stuf-bg-only-zipcode-housenumber') {
      $elements['city']['#attributes']['readonly'] = 'readonly';
    }
  }
  elseif ($component['extra']['city_options'] == 'select') {
    $options = list_extract_allowed_values($component['extra']['cities_piped_list'], 'list_text', FALSE);
    $elements['city'] = array(
      '#type' => 'select',
      '#title' => t('City'),
      '#options' => $options,
      '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
      '#default_value' => _dvg_webform_components_address_default_value('city', $value, $component),
      '#required' => $component['required'],
      '#attributes' => array(),
      '#theme_wrappers' => array('webform_element'),
      '#webform_component' => array('type' => $component['type'] . '_city') + $component,
    );
    $elements['city_block'] = array(
      '#type' => 'container',
      '#theme_wrappers' => array('webform_element'),
      '#webform_component' => array('type' => $component['type'] . '_block') + $component,
    );
  }

  $dvg_stuf_bg_aoa_api_url = variable_get('dvg_stuf_bg_aoa_api_url');
  if (($component['extra']['addresscheck'] == 'stuf-bg' || $component['extra']['addresscheck'] == 'stuf-bg-only-zipcode-housenumber') && empty($dvg_stuf_bg_aoa_api_url)) {
    watchdog('dvg_webform_components', 'Addresscheck set to stuf-bg but variable dvg_stuf_bg_aoa_api_url not set ', array(), WATCHDOG_WARNING, $link = NULL);
  }
  $dvg_postcodenl_key = variable_get('dvg_postcodenl_key');
  if ($component['extra']['addresscheck'] == 'postcodenl' && empty($dvg_postcodenl_key)) {
    watchdog('dvg_webform_components', 'Addresscheck set to postcode.nl but variable dvg_postcodenl_key not set ', array(), WATCHDOG_WARNING, $link = NULL);
  }
  $dvg_postcodenl_secret = variable_get('dvg_postcodenl_secret');
  if ($component['extra']['addresscheck'] == 'postcodenl' && empty($dvg_postcodenl_secret)) {
    watchdog('dvg_webform_components', 'Addresscheck set to postcode.nl but variable dvg_postcodenl_secret not set ', array(), WATCHDOG_WARNING, $link = NULL);
  }

  $ajax = FALSE;
  if ($component['extra']['addresscheck'] != 'none' && $component['extra']['city_options'] != 'select' && !$component['extra']['po_box']) {
    $ajax = TRUE;
  }

  /* Ajax calls for addresschecks only added if :
   * - selected service
   * - cities not a list
   * - not a po box
   */
  if ($ajax) {
    $elements['postal_code']['#ajax'] = array(
      'callback' => '_address_ajax_callback',
    );
    $elements['house_number']['#ajax'] = array(
      'callback' => '_address_ajax_callback',
    );
    if ($component['extra']['addresscheck'] == 'stuf-bg') {
      $elements['house_letter']['#ajax'] = array(
        'callback' => '_address_ajax_callback',
      );
      $elements['house_number_addition']['#ajax'] = array(
        'callback' => '_address_ajax_callback',
      );

    }
    $elements['street_name']['#attributes']['class'] = array(drupal_html_class('ajax-callback-street-name--' . $component['form_key']));
    $elements['city']['#attributes']['class'] = array(drupal_html_class('ajax-callback-city--' . $component['form_key']));
  }

  $elements['#element_validate'] = array('dvg_webform_components_address_validate_addresscheck');

  // Set the label as a place holder.
  if ($component['extra']['placeholders']) {

    $elements['postal_code']['#title_display'] = 'invisible';
    $elements['postal_code']['#attributes']['placeholder'] = t('Postal code');

    $elements['city']['#title_display'] = 'invisible';
    $elements['city']['#attributes']['placeholder'] = t('City');

    if (empty($component['extra']['po_box'])) {
      $elements['house_number']['#title_display'] = 'invisible';
      $elements['house_number']['#attributes']['placeholder'] = t('House number');

      $elements['house_letter']['#title_display'] = 'invisible';
      $elements['house_letter']['#attributes']['placeholder'] = t('House letter');

      $elements['house_number_addition']['#title_display'] = 'invisible';
      $elements['house_number_addition']['#attributes']['placeholder'] = t('Addition');

      $elements['street_name']['#title_display'] = 'invisible';
      $elements['street_name']['#attributes']['placeholder'] = t('Street name');
    }
    else {
      $elements['po_box']['#title_display'] = 'invisible';
      $elements['po_box']['#attributes']['placeholder'] = t('PO Box');
    }
  }

  // Disable Drupal validation and add Webform validation on all components so conditionals and other stuff is handled properly.
  // Add value callback to trim all values.
  foreach (element_children($elements) as $key) {
    $elements[$key]['#process'] = array('dvg_webform_components_expand_address_childs');
    $elements[$key]['#process'] += element_info_property($elements[$key]['#type'], '#process', array());
    if (!isset($elements[$key]['#value_callback'])) {
      $elements[$key]['#value_callback'] = 'dvg_webform_components_trim_value';
    }
  }
  return $elements;
}

/**
 * Logic for finding the default value for a component.
 */
function _dvg_webform_components_address_default_value($name, $value, $component) {
  if (isset($value[$name])) {
    return $value[$name];
  }
  if (isset($component['extra']['default_values'][$name])) {
    return token_replace($component['extra']['default_values'][$name], array(), array('clear' => TRUE));
  }
  return '';
}

/**
 * Process function to ensure address elements validate properly.
 */
function dvg_webform_components_expand_address_childs($element) {
  $element['#validated'] = TRUE;
  $element['#webform_validated'] = FALSE;
  return $element;
}

/**
 * Retrieve street name and city belonging to postal code and house number.
 *
 * Callback for _webform_render_address().
 *
 * NB: Normally, this function should be kept as simple as possible (i.e., just
 *     return the portion of the form that should be adjusted). However, since
 *     Webform doesn't provide the $form_state in a simple manner and we only
 *     provide additional functionallity to help the visitor, we allow us to do
 *     a little bit more in this function.. (otherwise we have to use the
 *     hook_form_alter() function to add AJAX components).
 */
function _address_ajax_callback($form, $form_state) {
  // To authenticate to the postcode.nl API, we need the login information.
  $service = $form_state['triggering_element']['#webform_component']['extra']['addresscheck'];

  // Get the key used for this Webform component.
  $form_key = $form_state['triggering_element']['#webform_component']['form_key'];

  // Get the provided postal code and house number.
  $values = $form_state['values'];
  $block_keys = $form_state['triggering_element']['#parents'];
  array_shift($block_keys);
  array_pop($block_keys);
  $block_key = implode($block_keys, '--');
  foreach ($form_state['triggering_element']['#parents'] as $parent) {
    $values = $values[$parent];

    if ($parent == $form_key) {
      break;
    }
  }

  $errors = array();
  $postal_code = $values['postal_code'];
  _address_validate_postal_code($postal_code, $errors);
  if ($form_state['triggering_element']['#webform_component']['extra']['internal_postal_codes'] == TRUE) {
    $internal_postal_codes = explode(',', $form_state['triggering_element']['#webform_component']['extra']['internal_postal_codes_list']);
    _address_validate_internal_postal_codes($postal_code, $internal_postal_codes, $errors);
  }
  if (empty($form_state['triggering_element']['#webform_component']['extra']['po_box'])) {
    $house_number = $values['house_number'];
    $house_letter = $values['house_letter'];
    $house_number_addition = $values['house_number_addition'];
    _address_validate_house_number($house_number, $errors);
    _address_validate_house_letter($house_letter, $errors);
    _address_validate_house_number_addition($house_number_addition, $errors);
  }
  $commands = array();
  if (empty($errors) && !empty($postal_code) && !empty($house_number)) {
    // Perform the lookup using the service.
    $data = _address_autofill($postal_code, $house_number, $house_letter, $house_number_addition, $service);
    if (!empty($data['street']) && !empty($data['city'])) {
      // React on the provided information.
      $arguments = array(
        'formKey' => drupal_html_class($form_key),
        'street' => $data['street'],
        'city' => $data['city'],
      );
      $arguments = drupal_json_encode($arguments);
      $commands[] = ajax_command_invoke(NULL, 'webformAddressInject', array($arguments));
    }
  }
  if ($service == 'stuf-bg' || $service == 'stuf-bg-only-zipcode-housenumber') {
    // Remove the old messages div, clearing existing messages.
    $commands[] = ajax_command_remove('.webform-component--' . drupal_html_class($block_key) . ' .messages.error');
    $location = '.webform-component--' . drupal_html_class($block_key) . ' .webform-component--' . drupal_html_class($block_key) . '--block';
    if (empty($errors)) {
      if (!empty($postal_code) && !empty($house_number) && empty($data['street']) && empty($data['city'])) {
        $error = t('This address is not available in %site.', array('%site' => variable_get('site_name', 'this site')));
        $commands[] = ajax_command_after($location, '<div class="messages error messages-inline">' . $error . '</div>');
      }
    }
    else {
      if (count($errors) > 1) {
        $items = array(
          'items' => $errors,
          'title' => '',
          'type' => 'ul',
          'attributes' => array(),
        );
        $error = theme_item_list($items);
      }
      else {
        $error = $errors[0];
      }
      $commands[] = ajax_command_after($location, '<div class="messages error messages-inline">' . $error . '</div>');

    }
  }

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );

}

/**
 * Wrapper function which calls different services based on settings.
 *
 * @param string $postal_code
 *   The postal code of the address.
 * @param string $house_number
 *   The house number of the address.
 * @param string $service
 *   Type of service: currently: postcodenl, aoa (dvg-stuf-bg)
 *
 * @codingStandardsIgnoreStart
 */
function _address_autofill($postal_code, $house_number, $house_letter = '', $house_number_addition = '', $service) {

  // @codingStandardsIgnoreEnd
  $data = array();
  $data['street'] = '';
  $data['city'] = '';

  switch ($service) {
    case 'stuf-bg':
      $prefill = dvg_stuf_bg_adresgegevens($postal_code, $house_number, $house_letter, $house_number_addition);
      if ($prefill) {
        $data['street'] = $prefill->{'gor.straatnaam'}->_;
        $data['city'] = $prefill->{'wpl.woonplaatsNaam'}->_;
      }
      break;

    case 'stuf-bg-only-zipcode-housenumber':
      $prefill = dvg_stuf_bg_adresgegevens($postal_code, $house_number, '', '');
      if ($prefill) {
        $data['street'] = $prefill->{'gor.straatnaam'}->_;
        $data['city'] = $prefill->{'wpl.woonplaatsNaam'}->_;
      }
      break;

    case 'postcodenl':
      $key = variable_get('dvg_postcodenl_key');
      $secret = variable_get('dvg_postcodenl_secret');
      $data = _address_postcode_nl_lookup($key, $secret, $postal_code, $house_number);
      break;
  }

  $address = [
    'postal_code' => $postal_code,
    'house_number' => $house_number,
    'house_letter' => $house_letter,
    'house_number_addition' => $house_number_addition,
  ];

  drupal_alter('dvg_webform_components_address_autofill', $data, $address, $service);

  return $data;
}

/**
 * Retrieve street name and city belonging to postal code and house number.
 *
 * This function makes a call to the postcode.nl API to get the street name and
 * city that is associated with the postal code and house number.
 *
 * @param string $key
 *   The key that should be used to identify at the postcode.nl API service.
 * @param string $secret
 *   The secret that belongs to the $key.
 * @param string $postal_code
 *   The postal code of the address.
 * @param string $house_number
 *   The house number of the address.
 *
 * @return string[]
 *   The name of the street can be found at the index 'street' and the name of
 *   the name of the city at the index 'city' in the array.
 */
function _address_postcode_nl_lookup($key, $secret, $postal_code, $house_number) {
  // First, parse the postal code and house number (which should split the
  // addition).
  $postal_code = _address_parse_postal_code($postal_code);
  list($house_number, $house_number_addition) = _address_parse_house_number($house_number);

  // Both the postal code and the house number should have a value.
  if (empty($postal_code) || empty($house_number)) {
    return array();
  }

  // We prevent making unnecessary lookups by caching results, also wrong
  // lookups!
  $cache_key = 'dvg_address:' . $postal_code . ':' . $house_number . ':' . $house_number_addition;

  // Use the local memory as the first level cache.
  $data = &drupal_static(__FUNCTION__);
  if (!isset($data)) {
    // When not stored in local memory, check the cache stored in the database.
    if ($cache = cache_get($cache_key)) {
      $data = $cache->data;
    }
    else {
      // Construct the URL to perform the lookup with.
      $url = 'https://' . $key . ':' . $secret . '@api.postcode.nl/rest/addresses/' . $postal_code . '/' . $house_number . '/' . $house_number_addition;

      $response = drupal_http_request(url($url));

      // When the lookup went correctly, parse the JSON data.
      if ($response->code == 200) {
        $data = drupal_json_decode($response->data);
      }

      // Store the result in the cache of the database.
      //
      // NB: if the lookup didn't responded in a correct result, we still store
      // a 'mark' in the database. In this way also invalid lookups will be
      // served from cache the next time.
      cache_set($cache_key, $data, 'cache', time() + 900);
    }
  }

  return $data;
}

/**
 * Parse a postal code to be used in the request to postcode.nl API.
 *
 * @param string $postal_code
 *   The postal code that should be normalized.
 *
 * @return string
 *   The normalized postal code.
 */
function _address_parse_postal_code($postal_code) {
  // First, remove any whitespaces.
  $postal_code = str_replace(' ', '', trim($postal_code));

  // If the postal code is in the form of 1234AB, than return the trimmed
  // version.
  if ((boolean) preg_match('~^\d{4}[[:alpha:]]{2}$~', $postal_code)) {
    return $postal_code;
  }

  return '';
}

/**
 * Parse the house number by separating the number from the additional value.
 *
 * @param string $house_number_full
 *   The house number of the address (including an possible addition).
 *
 * @return string[]
 *   Normalized version of the house number, where the first index of the array
 *   contains the number and the second value is the addition.
 */
function _address_parse_house_number($house_number_full) {
  // Remove white spaces before and after the provided house number (including
  // addition).
  $house_number_full = trim($house_number_full);

  $house_number = '';
  $house_number_addition = '';
  if (preg_match('~^(?P<house_number>\d+)[^[:alnum:]]*(?P<addition>[[:alnum:]]*)$~', $house_number_full, $match)) {
    $house_number = $match['house_number'];
    $house_number_addition = !empty($match['addition']) ? $match['addition'] : '';
  }

  return array($house_number, $house_number_addition);
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_address($component, $value, $format = 'html') {
  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_address',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#post_render' => array('webform_element_wrapper'),
    '#component' => $component,
    '#format' => $format,
    '#value' => isset($value) ? address_display_format($value) : '',
  );
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_address() {
  return array(
    'webform_display_address' => array(
      'render element' => 'element',
    ),
    'webform_display_council_code' => array(
      'render element' => 'element',
    ),
  );
}

/**
 * Returns HTML for displaying the address.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #format, #value.
 *
 * @see theme_webform_element
 *
 * @ingroup themeable
 */
function theme_webform_display_address(array $variables) {
  $element = $variables['element'];
  $value = $element['#format'] == 'html' ? check_plain($element['#value']) : $element['#value'];

  return $value !== '' ? $value : ' ';
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_address($component, $value) {
  $address = '';
  if (isset($value['street_name'])) {
    $address .= $value['street_name'];
  }
  if (isset($value['house_number'])) {
    $address .= ' ' . $value['house_number'];
  }
  if (isset($value['house_letter'])) {
    $address .= ' ' . $value['house_letter'];
  }
  if (isset($value['house_number_addition'])) {
    $address .= ' ' . $value['house_number_addition'];
  }
  if (isset($value['postal_code'])) {
    $address .= ', ' . $value['postal_code'];
  }
  if (isset($value['city'])) {
    $address .= ' ' . $value['city'];
  }
  return check_plain($address);
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_address($component, $export_options) {
  $header = array();
  $header[0] = '';
  if (empty($component['extra']['po_box'])) {
    $header[1] = [
      $export_options['header_keys'] ? $component['form_key'] : $component['name'],
      '',
      '',
      '',
      '',
      '',
    ];
    $header[2] = [
      $export_options['header_keys'] ? 'street_name' : t('Street name'),
      $export_options['header_keys'] ? 'house_number' : t('House number'),
      $export_options['header_keys'] ? 'house_letter' : t('House letter'),
      $export_options['header_keys'] ? 'house_number_addition' : t('Number addition'),
      $export_options['header_keys'] ? 'postal_code' : t('Postal code'),
      $export_options['header_keys'] ? 'city' : t('City'),
    ];
  }
  else {
    $header[1] = [
      $export_options['header_keys'] ? $component['form_key'] : $component['name'],
      '',
      '',
    ];
    $header[2] = [
      $export_options['header_keys'] ? 'po_box' : t('PO Box'),
      $export_options['header_keys'] ? 'postal_code' : t('Postal code'),
      $export_options['header_keys'] ? 'city' : t('City'),
    ];
  }
  return $header;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_address($component, $export_options, $value) {
  $return = array();
  if (empty($component['extra']['po_box'])) {
    $return[] = $value['street_name'];
    $return[] = $value['house_number'];
    $return[] = $value['house_letter'];
    $return[] = $value['house_number_addition'];
  }
  else {
    $return[] = $value['po_box'];
  }
  $return[] = $value['postal_code'];
  $return[] = $value['city'];

  return $return;
}

/**
 * Format the address in a textual representation.
 *
 * @param string[] $value
 *   An associative array containing the address information (i.e., street name,
 *   house number, postal code and the city).
 *
 * @return string
 *   A formatted string representing the address.
 */
function address_display_format(array $value) {
  $address = '';
  if (isset($value['street_name'])) {
    $address .= $value['street_name'];
  }
  if (isset($value['house_number'])) {
    $address .= ' ' . $value['house_number'];
  }
  if (isset($value['house_letter'])) {
    $address .= ' ' . $value['house_letter'];
  }
  if (isset($value['house_number_addition'])) {
    $address .= ' ' . $value['house_number_addition'];
  }
  if (isset($value['postal_code'])) {
    $address .= ', ' . $value['postal_code'];
  }
  if (isset($value['city'])) {
    $address .= ' ' . $value['city'];
  }
  return check_plain($address);
}

/**
 * Validate entire form again, on submit.
 */
function dvg_webform_components_address_validate_addresscheck($element, &$form_state) {
  // Only on total form submit, not ajax calls.
  if ($form_state['triggering_element']['#type'] != 'submit') {
    return;
  }

  $errors = array();
  $postal_code = $element['postal_code']['#value'];
  _address_validate_postal_code($postal_code, $errors);

  if ($element['#webform_component']['extra']['internal_postal_codes'] == TRUE) {
    $internal_postal_codes = explode(',', $element['#webform_component']['extra']['internal_postal_codes_list']);
    _address_validate_internal_postal_codes($postal_code, $internal_postal_codes, $errors);
  }

  if (!empty($element['#webform_component']['extra']['po_box'])) {
    $po_box = $element['po_box']['#value'];
    _address_validate_po_box($element, $po_box);
  }
  else {
    $house_number = $element['house_number']['#value'];
    $house_letter = $element['house_letter']['#value'];
    $house_number_addition = $element['house_number_addition']['#value'];
    _address_validate_house_number($house_number, $errors);
    _address_validate_house_letter($house_letter, $errors);
    _address_validate_house_number_addition($house_number_addition, $errors);
  }

  if ($element['#webform_component']['extra']['city_options'] != 'select' && !$element['#webform_component']['extra']['po_box']) {
    $service = $element['#webform_component']['extra']['addresscheck'];
    if (empty($errors) && !empty($postal_code) && !empty($house_number)) {
      // Perform the lookup once again.
      $data = _address_autofill($postal_code, $house_number, $house_letter, $house_number_addition, $service);
      // React on the provided information.
      if (empty($errors) && (empty($data) || (empty($data['street']) || empty($data['city'])))) {
        switch ($service) {
          case 'postcodenl';
            $errors[] = t('This address is not available in the Netherlands.');
            break;

          case 'stuf-bg';
            $errors[] = t('This address is not available in %site.', array('%site' => variable_get('site_name', 'this site')));
            break;

          case 'stuf-bg-only-zipcode-housenumber';
            $errors[] = t('This address is not available in %site.', array('%site' => variable_get('site_name', 'this site')));
            break;
        }
      }
      else {
        // Fill the street_name and city if incorrect.
        $field_parents = $element['#parents'];
        $field_parents[] = 'street_name';
        drupal_array_set_nested_value($form_state['input'], $field_parents, $data['street'], TRUE);
        drupal_array_set_nested_value($form_state['values'], $field_parents, $data['street'], TRUE);
        $element['street_name']['#value'] = $data['street'];
        $field_parents = $element['#parents'];
        $field_parents[] = 'city';
        drupal_array_set_nested_value($form_state['input'], $field_parents, $data['city'], TRUE);
        drupal_array_set_nested_value($form_state['values'], $field_parents, $data['city'], TRUE);
        $element['city']['#value'] = $data['city'];
      }
    }
  }

  if (!empty($errors)) {
    $form_keys = $element['block']['#parents'];
    array_pop($form_keys);
    if (count($errors) > 1) {
      $items = array(
        'items' => $errors,
        'title' => '',
        'type' => 'ul',
        'attributes' => array(),
      );
      $errors = theme_item_list($items);
    }
    else {
      $errors = $errors[0];
    }

    form_set_error(implode('][', $form_keys), $errors);
  }
}

/**
 * Validation handler of the predefined list of cities.
 *
 * If key of 'Predefined list of cities' contains space as prefix or suffix, the validation will fail when user selects
 * an option which it's key contains space as prefix or suffix.
 */
function _address_validate_cities_piped_list($element, &$form_state) {
  if (isset($form_state['input']['extra']['city_options']) &&
    $form_state['input']['extra']['city_options'] == 'select' &&
    empty($form_state['input']['extra']['cities_piped_list'])) {
    form_error($element,
      t('"!field_name" is required when "!city_opt_selected" is selected for "!field_city_option". Please enter key|value for "!field_name".', [
        '!field_name' => t($element['#title']),
        '!field_city_option' => t('City options'),
        '!city_opt_selected' => t('Predefined list'),
      ])
    );
  }

  if (!empty($form_state['input']['extra']['cities_piped_list'])) {
    $options = explode("\n", ($form_state['values']['extra']['cities_piped_list']));

    // Only validate the key if it contains space as prefix or suffix.
    // A key of two words with space in between is allowed e.g. den haag. Value may contain space.
    foreach ($options as $option) {
      $key = substr($option, 0, strpos($option, '|'));

      if ($key !== trim($key)) {
        form_error($element, t('Key of the "!field_name" field may not contain space as prefix or suffix. Please remove space from key.', ['!field_name' => t($element['#title'])]));
        break;
      }
    }
  }
}

/**
 * Postal code validation and space removal.
 */
function _address_validate_postal_code(&$postal_code, &$errors) {
  $postal_code = trim(str_replace(' ', '', $postal_code));
  if (!empty($postal_code)) {
    preg_match('/^([1-9][0-9]{3}) ?([A-Za-z]{2})$/', $postal_code, $matches);
    if (empty($matches) || (isset($matches[2]) && in_array($matches[2], array(
      'SS',
      'SD',
      'SA',
    ), TRUE))) {
      $errors[] = t('Invalid postal code. Postal codes in the Netherlands are like "9999 AA". They never start with zero and the letters are never "SS", "SD", or "SA".');
    }
  }
}

/**
 * Validation handler for internal postal codes.
 */
function _address_validate_internal_postal_codes($postal_code, $internal_postal_codes, &$errors) {
  // Strip the postal code to only numbers.
  $postal_code = substr($postal_code, 0, 4);
  if (!in_array($postal_code, $internal_postal_codes)) {
    $errors[] = t("The postal code isn't available in this region.");
  }
}

/**
 * Validation handler of the internal postal codes list.
 */
function _address_validate_list_internal_postal_codes($element, &$form_state) {
  if (isset($form_state['input']['extra']['internal_postal_codes_list'])) {
    $matches = preg_match_all('/([0-9]{4},?)/x', $form_state['input']['extra']['internal_postal_codes_list']);
    if ($matches <> count(explode(',', $form_state['input']['extra']['internal_postal_codes_list'])) && $form_state['input']['extra']['internal_postal_codes']) {
      form_error($element, t('The "!name" option must contain a valid value. You must enter a comma separated list with only four digit numbers.', array('!name' => t($element['#title']))));
    }
  }
}

/**
 * Validation handler for house number.
 */
function _address_validate_house_number($house_number, &$errors) {
  $value = trim($house_number);
  if (!empty($value) && !preg_match('/^[1-9]{1}[0-9]{0,4}$/i', $value)) {
    $errors[] = t('The house number has to be an integer in the range 1 till 99999');
  }
}

/**
 * Validation handler for house letter.
 */
function _address_validate_house_letter($house_letter, &$errors) {
  $value = trim($house_letter);
  if (!empty($value) && !preg_match('/^[a-z]{0,1}$/i', $value)) {
    $errors[] = t('The house letter is max 1 character and exists of the letters (A till Z)');
  }
}

/**
 * Validation handler for house number remark / addition.
 */
function _address_validate_house_number_addition($house_number_addition, &$errors) {
  $value = trim($house_number_addition);
  if (!empty($value) && !preg_match('/^[a-z0-9]{0,4}$/i', $value)) {
    $errors[] = t('The house number remark is max 4 characters and exists of the letters (A till Z) and/or digits (0 till 9)');
  }
}

/**
 * Validation handler for po box number.
 */
function _address_validate_po_box($element, $po_box) {
  $value = trim($po_box);
  if (!empty($value) && !preg_match('/^[1-9]{1}[0-9]{0,4}$/i', $value)) {
    $errors = t('The PO box has to be an integer in the range 1 till 99999');
    $form_keys = $element['po_box_block']['#parents'];
    array_shift($form_keys);
    form_set_error('submitted][' . implode('][', $form_keys), $errors);
  }
}

/**
 * Implements _webform_submit_component().
 *
 * Always uppercase and remove any spaces in postal code (format: 1234AB).
 * Always uppercase house letter (format: A).
 */
function _webform_submit_address($component, $value) {
  if (isset($value['postal_code'])) {
    $value['postal_code'] = str_replace(' ', '', strtoupper($value['postal_code']));
  }
  if (isset($value['house_letter'])) {
    $value['house_letter'] = strtoupper($value['house_letter']);
  }
  return $value;
}

/**
 * Custom Theme function for council code.
 */
function theme_webform_display_council_code($variables) {
  return '';
}

/**
 * #value_callback for webform elements.
 * Trims values.
 */
function dvg_webform_components_trim_value($element, $input = FALSE) {
  // According to spec return nothing for default value processing.
  if (!$input) {
    return;
  }
  return trim($input);
}
