<?php

namespace Drupal\campaignion_opt_in;

use Drupal\form_builder_webform\Element;
use Drupal\little_helpers\ArrayConfig;

/**
 * Form builder integration for the newsletter webform component.
 */
class FormBuilderElementOptIn extends Element {

  /**
   * Build the component edit form as usual in webform.
   *
   * @param array $component
   *   The webform componenent array.
   *
   * @return array
   *   Form-API array representing the webform component’s edit form.
   */
  protected function componentEditForm($component) {
    $form_id = 'webform_component_edit_form';
    $form_state = form_state_defaults();

    // The full node is needed here so that the "private" option can be access
    // checked.
    $nid = $component['nid'] ?? NULL;
    $node = !isset($nid) ? (object) array('nid' => NULL, 'webform' => webform_node_defaults()) : node_load($nid);
    $form = $form_id([], $form_state, $node, $component);
    $form_state['build_info']['args'] = [$node, $component];
    // We want to avoid a full drupal_get_form() for now but some alter hooks
    // need defaults normally set in drupal_prepare_form().
    $form += ['#submit' => []];
    drupal_alter(['form', 'form_webform_component_edit_form'], $form, $form_state, $form_id);
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function configurationForm($form, &$form_state) {
    $form = parent::configurationForm($form, $form_state);
    $form['description']['#weight'] = 0;

    $component = $this->element['#webform_component'];

    // Only top-level elements can be assigned to property groups.
    // @see form_builder_field_configure_pre_render()
    $edit = $this->componentEditForm($component);
    $form['channel'] = $edit['extra']['channel'];
    $form['value'] = $edit['behavior']['value'];
    $dp['#form_builder']['property_group'] = 'display';
    $form['display'] = $edit['extra']['display'] + $dp;
    $form['checkbox_label'] = $edit['extra']['checkbox_label'] + $dp;
    $form['radio_labels'] = $edit['extra']['radio_labels'] + $dp;
    $form['optin_statement'] = $edit['extra']['optin_statement'];
    $form['no_is_optout'] = $edit['behavior']['no_is_optout'];
    $form['disable_optin'] = $edit['behavior']['disable_optin'];

    if (module_exists('campaignion_newsletters') && isset($edit['list_management'])) {
      $form['#property_groups']['lists'] = [
        'title' => t('Lists'),
        'weight' => 2,
      ];
      $form['lists'] = ['#type' => NULL] + $edit['list_management'];
      $form['lists']['#form_builder']['property_group'] = 'lists';
      $form['optout_all_lists'] = $edit['behavior']['optout_all_lists'];
    }

    return $form;
  }

  /**
   * Store component configuration just like webform would do it.
   *
   * The values are already at their proper places in `$form_state['values']`
   * because the `#parents` array is provided in `_webform_edit_opt_in()`.
   */
  public function configurationSubmit(&$form, &$form_state) {
    $component = $form_state['values'];
    ArrayConfig::mergeDefaults($component, $this->element['#webform_component']);
    $this->element['#webform_component'] = $component;
    parent::configurationSubmit($form, $form_state);
  }

}
