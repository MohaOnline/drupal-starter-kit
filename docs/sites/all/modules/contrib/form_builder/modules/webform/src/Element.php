<?php

namespace Drupal\form_builder_webform;

use Drupal\form_builder\ElementBase;

class Element extends ElementBase {

  /**
   * {@inheritdoc}
   */
  protected function setProperty($property, $value) {
    $component = &$this->element['#webform_component'];
    $properties = $this->getProperties();
    $properties[$property]->setValue($component, $value);
  }

  /**
   * {@inheritdoc}
   */
  public function render() {
    $element = $this->element;
    if (isset($element['#webform_component'])) {
      $component = $element['#webform_component'];
      $new_element = webform_component_invoke($component['type'], 'render', $component, NULL, FALSE);
      // Preserve the #weight. It may have been changed by the positions form.
      $new_element['#weight'] = $element['#weight'];
      $new_element['#key'] = $component['form_key'];
      $new_element['#webform_component'] = $component;
      $new_element['#form_builder'] = $element['#form_builder'];
      return $this->addPreRender($new_element);
    }
    return $this->addPreRender($element);
  }

  public function title() {
    return $this->element['#webform_component']['name'];
  }

  /**
   * Get the element’s form key.
   *
   * @return string
   *   The element’s form key.
   */
  public function key() {
    return $this->element['#webform_component']['form_key'];
  }

}

