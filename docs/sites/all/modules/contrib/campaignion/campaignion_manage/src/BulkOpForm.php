<?php

namespace Drupal\campaignion_manage;

class BulkOpForm {
  protected $ops;
  public function __construct($ops = array()) {
    $this->ops = array();
    foreach ($ops as $name => $op) {
      $this->ops[$name] = $op;
    }
  }
  public function form(&$form, &$form_state) {
    $form['bulk-wrapper'] = array(
      '#type' => 'fieldset',
      '#attributes' => array('class' => array('bulkops')),
      '#title' => t('Bulk edit your selection'),
    );
    $form['bulk-wrapper']['info'] = array(
      '#markup' => '<div class="bulkop-selected">' . t('!count items selected.', array('!count' => '<span class="bulkop-count"></span>')) . '</div>',
    );
    $form['bulk-wrapper']['operations'] = array(
      '#type' => 'radios',
      '#title' => t('Selected bulk operation'),
      '#options' => array(),
      '#attributes' => array('class' => array('bulkops-radios')),
    );
    $form['bulk-wrapper']['op-wrapper'] = array(
      '#type' => 'container',
      '#attributes' => array('class' => array('bulkops-ops')),
    );

    foreach ($this->ops as $name => $op) {
      $form['bulk-wrapper']['operations']['#options'][$name] = $op->title();
      $form['bulk-wrapper']['op-wrapper']['op'][$name] = array(
        '#type' => 'fieldset',
        '#title' => $op->title(),
        '#attributes' => array('class' => array('bulkops-op', 'bulkops-op-' . $name)),
      );
      $element = &$form['bulk-wrapper']['op-wrapper']['op'][$name];
      $element['helptext'] = array(
        '#type' => 'container',
        '#attributes' => array('class' => array('help-text')),
        'text' => array('#markup' => $op->helpText()),
      );
      $op->formElement($element, $form_state);
    }

    $form['bulk-wrapper']['actions'] = array(
      '#type' => 'container',
      '#attributes' => array('class' => array('actions')),
    );
    $form['bulk-wrapper']['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Apply'),
    );

    $form['#attributes']['class'][] = 'campaignion-manage-bulkops';
  }
  public function submit(&$form, &$form_state, $result) {
    $values  = &$form_state['values']['bulkop']['bulk-wrapper'];
    $op_name = $values['operations'];
    if (!isset($this->ops[$op_name])) {
      return;
    }
    $op = $this->ops[$op_name];
    $op_parameters = isset($values['op-wrapper']['op'][$op_name]) ? $values['op-wrapper']['op'][$op_name] : NULL;
    $messages = $op->apply($result, $op_parameters);

    if (is_array($messages)) {
      foreach ($messages as $msg) {
        drupal_set_message($msg, 'error');
      }
    }
  }
}
