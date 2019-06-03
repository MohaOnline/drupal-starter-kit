<?php

use Drupal\campaignion_form_builder\PhoneElement;

/**
 * Invoked by campaignion_form_builder_form_builder_element_types().
 */
function _campaignion_form_builder_form_builder_types_textfield() {
  $fields = array();

  $text_element = function($component, $extra, $weight) {
    $element = [
      'title' => $component['name'],
      'weight' => $weight,
      'properties' => ['default_value'],
      'palette_group' => 'supporter_data',
      'unique' => TRUE,
      'default' => _form_builder_webform_default('textfield', $extra, $component),
    ];
    return $element;
  };

  $fields['first_name'] = $text_element([
    'name' => t('First name'),
    'value' => '[current-page:query:p3]',
  ], [], -80);

  $fields['last_name'] = $text_element([
    'name' => t('Last name'),
    'value' => '[current-page:query:p4]',
  ], [], -79);

  $fields['title'] = $text_element([
    'name' => t('Title'),
    'value' => '[current-page:query:p5]',
  ], [], -77);

  $fields['date_of_birth'] = $text_element([
    'name' => t('Date of birth'),
    'value' => '[current-page:query:p6]',
  ], ['description' => t('Please use the following format: 16/9/1983')], -76);

  $fields['street_address'] = $text_element([
    'name' => t('Street address'),
    'value' => '[current-page:query:p7]',
  ], [], -75);

  $fields['zip_code'] = $text_element([
    'name' => t('ZIP code'),
    'value' => '[current-page:query:p8]',
  ], [], -74);

  $fields['state'] = $text_element([
    'name' => t('Region'),
    'value' => '[current-page:query:p9]',
  ], [], -73);

  $fields['city'] = $text_element([
    'name' => t('City'),
    'value' => '[current-page:query:p10]',
  ], [], -72);

  $phone_element = function($component, $extra, $weight) {
    $element = [
      'class' => PhoneElement::class,
      'title' => $component['name'],
      'weight' => $weight,
      'properties' => ['default_value'],
      'palette_group' => 'supporter_data',
      'unique' => TRUE,
      'default' => _form_builder_webform_default('phone_number', $extra, $component),
    ];
    return $element;
  };

  $fields['phone_number'] = $phone_element([
    'name' => t('Phone number'),
    'value' => '[current-page:query:p11]',
  ], [], -71);

  $fields['mobile_number'] = $phone_element([
    'name' => t('Mobile number'),
    'value' => '[current-page:query:p12]',
  ], [], -70);

  return $fields;
}
