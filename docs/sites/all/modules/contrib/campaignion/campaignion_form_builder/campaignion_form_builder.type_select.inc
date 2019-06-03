<?php

/**
 * Invoked by campaignion_form_builder_form_builder_element_types().
 */
function _campaignion_form_builder_form_builder_types_select() {
  require_once drupal_get_path('module', 'webform') . '/components/select.inc';
  $fields = array();

  $select_element = function($component, $extra, $items, $weight) {
    $extra += [
      'aslist' => TRUE,
      'multiple' => FALSE,
      'items' => _webform_select_options_to_text($items),
    ];
    $element = [
      'title' => $component['name'],
      'weight' => $weight,
      'properties' => ['default_value', 'options', 'multiple', 'other_option', 'other_text',],
      'palette_group' => 'supporter_data',
      'unique' => TRUE,
      'default' => _form_builder_webform_default('select', $extra, $component),
    ];
    $element['default']['#form_builder']['element_type'] = 'select';
    return $element;
  };

  // Token default-values are not handled properly in webform yet.
  $fields['salutation'] = $select_element(
    ['name' => t('Salutation'), /*'value' => '[current-page:query:p1]'*/],
    ['other_option' => TRUE],
    ['fam' => t('Family'), 'mrs' => t('Mrs'), 'mr' => t('Mr'), 'org' => t('Organisation')],
    -82
  );

  $fields['gender'] = $select_element(
    ['name' => t('Gender'), /*'value' => '[current-page:query:p2]'*/],
    [],
    ['f' => t('female'), 'm' => t('male'), 'o' => t('other')],
    -81
  );

  return $fields;
}
