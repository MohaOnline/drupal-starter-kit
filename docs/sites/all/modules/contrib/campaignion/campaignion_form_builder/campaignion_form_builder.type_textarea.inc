<?php

/**
 * Invoked by campaignion_form_builder_form_builder_element_types().
 */
function _campaignion_form_builder_form_builder_types_textarea() {
  $fields = array();

  $text_element = function($component, $extra, $weight) {
    $element = [
      'title' => $component['name'],
      'weight' => $weight,
      'properties' => ['default_value'],
      'palette_group' => 'supporter_data',
      'unique' => TRUE,
      'default' => _form_builder_webform_default('textarea', $extra, $component),
    ];
    return $element;
  };

  $fields['comment'] = $text_element([
    'name' => t('Comment'),
    'value' => '[current-page:query:p13]',
  ], [], -69);

  return $fields;
}
