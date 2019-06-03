<?php
/**
 * @file The HostingForm class.
 */

abstract class HostingForm {

  // The form's definition, passed by reference.
  public $form = FALSE;

  // The form's state, passed by reference.
  public $form_state = FALSE;

  // The node associated with the form, if available.
  public $node = FALSE;

  /**
   * Contructor method.
   *
   * Classes based on HostingForm should be instantiated by passing in $form
   * and $form_state, such as from hook_form_alter(), validate or submit
   * callbacks. These get passed by reference, so that any changes made need
   * not be returned back to the calling function.
   */
  public function __construct(&$form = [], &$form_state = [], $node = FALSE) {
    $this->form =& $form;
    $this->form_state =& $form_state;
    if ($node) {
      $this->node = $node;
    }
    elseif (isset($form['#node'])) {
      $this->node = $form['#node'];
    }
    elseif (isset($form_state['node'])) {
      $this->node = $form_state['node'];
    }
    elseif (isset($form_state['build_info']['args'][0])) {
      $this->node = $form_state['build_info']['args'][0];
    }
  }

  /**
   * Properly close dialogs, or re-direct back to source node, depending on
   * whether overlay.module is enabled.
   *
   * TODO: Move something like this upstream (into hosting_task.module)?
   */
  function closeOverlay() {
    if (module_exists('overlay') && overlay_get_mode() == 'child') {
      overlay_close_dialog();
    }
    else {
      drupal_goto('node/' . $this->node->nid);
    }
    $this->form_state['rebuild'] = TRUE;
  }

  /**
   * Render a form field un-editable.
   */
  public function makeFieldReadOnly($field) {
    $this->form[$field]['#disabled'] = TRUE;
  }

  /**
   * Hide a form field.
   */
  protected function disableField($field) {
    $this->form[$field]['#access'] = FALSE;
  }

  /**
   * Determine whether we're creating a new node, or editing an existing one.
   */
  protected function isANewNode() {
    return isset($this->form['nid']) && is_null($this->form['nid']['#value']);
  }

  protected function language() {
    return $this->form['language']['#value'];
  }

  protected function setFieldListOptions($field, $options) {
    $this->form[$field][$this->language()]['#options'] = $options;
  }

  /**
   * Register a form validate handler.
   */
  public function addValidateHandler($callback) {
    $this->form['#validate'][] = $callback;
  }

  /**
   * Register a form submit handler.
   */
  public function addSubmitHandler($callback) {
    $this->form['#submit'][] = $callback;
  }

  public function error($field, $message) {
    form_error($this->form[$field], t($message));
  }

  protected function isAField($field) {
    $prefix = 'field_';
    return substr($field, 0, strlen($prefix)) === $prefix;
  }

  public function getFormStateValue($field, $index = 0) {
    if ($this->isAField($field)) {
      return $this->form_state['values'][$field][$this->language()][$index]['value'];
    }
    else {
      return $this->form_state['values'][$field];
    }
  }

  public function setFormStateValue($field, $value, $index = 0) {
    if ($this->isAField($field)) {
      $this->form_state['values'][$field][$this->language()][$index]['value'] = $value;
    }
    else {
      $this->form_state['values'][$field] = $value;
    }
  }

  protected function setFormDefaultValue($field, $value) {
    $this->form[$field][$this->language()]['#default_value'] = $value;
  }

  protected function moveFieldsIntoFieldset($fieldset, $fields) {
    if (!isset($this->form[$fieldset])) {
      $this->form[$fieldset] = [
        '#type' => 'fieldset',
      ];
    }
    foreach ($fields as $field) {
      $this->form[$fieldset][$field] = $this->form[$field];
      unset($this->form[$field]);
    }
  }

  protected function makeFieldsetsIntoRadios($radio, $fieldsets) {
    foreach ($fieldsets as $fieldset => $label) {
      $this->form[$radio][$this->language()]['#options'][$fieldset] = $label;
      $this->form[$fieldset]['#title'] = $label;
      $this->form[$fieldset]['#collapsible'] = FALSE;
      $this->form[$fieldset]['#weight'] = 10;
      $this->form[$fieldset]['#states'] = [
        'visible' => [
          ':input[name="' . $radio . '[' . $this->language() . ']"]' => ['value' => $fieldset],
        ],
      ];
    }
  }

}
