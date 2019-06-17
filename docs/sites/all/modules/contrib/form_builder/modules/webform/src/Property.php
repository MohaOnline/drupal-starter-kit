<?php

namespace Drupal\form_builder_webform;

use Drupal\form_builder\PropertyBase;

class Property extends PropertyBase {

  protected $storageParents;

  /**
   * {@inheritdoc}
   */
  public function __construct($property, $params, $form_type_name) {
    $params += array('storage_parents' => array($property));
    parent::__construct($property, $params, $form_type_name);
    $this->storageParents = $params['storage_parents'];
  }

  /**
   * {@inheritdoc}
   */
  public function setValue(&$component, $value) {
    drupal_array_set_nested_value($component, $this->storageParents, $value);
  }

  /**
   * {@inheritdoc}
   */
  public function form(&$form_state, $element) {
    // We use the rendered element here to re-use the form-API functions.
    $e = $element->render();
    $e += array("#{$this->property}" => $this->getValue($e['#webform_component']));
    // Set weight to just anything. Element positions aren't configured in
    // this way in form_builder.
    $e['#webform_component']['weight'] = 0;
    if (isset($this->params['form']) && function_exists($this->params['form'])) {
      $function = $this->params['form'];
      return $function($form_state, $this->formTypeName, $e, $this->property);
    }
    return array();
  }

  /**
   * Read the value from a component array.
   */
  public function getValue($component) {
    return drupal_array_get_nested_value($component, $this->storageParents);
  }

}
