<?php

namespace Drupal\campaignion_form_builder;

use Drupal\little_helpers\ArrayConfig;
use Drupal\form_builder_webform\Element;

/**
 * Form builder integration for the phone_number webform component.
 */
class PhoneElement extends Element {

  /**
   * {@inheritdoc}
   */
  public function configurationForm($form, &$form_state) {
    $form = parent::configurationForm($form, $form_state);
    $form['description']['#weight'] = 0;

    $component = $this->element['#webform_component'];

    // Only top-level elements can be assigned to property groups.
    // @see form_builder_field_configure_pre_render()
    $edit = _webform_edit_phone_number($component);
    $form['implies_optin'] = $edit['extra']['implies_optin'];
    $form['optin_statement'] = $edit['extra']['optin_statement'];

    return $form;
  }

  /**
   * Store component configuration just like webform would do it.
   *
   * The values are already at their proper places in `$form_state['values']`
   * because the `#parents` array is provided in `_webform_edit_phone_number()`.
   */
  public function configurationSubmit(&$form, &$form_state) {
    $component = $form_state['values'];
    ArrayConfig::mergeDefaults($component, $this->element['#webform_component']);
    $this->element['#webform_component'] = $component;
    parent::configurationSubmit($form, $form_state);
  }

}

