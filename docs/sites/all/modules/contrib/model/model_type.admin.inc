<?php

/**
 * @file
 * Model type editing UI.
 */

/**
 * UI controller.
 */
class ModelTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
		$items[$this->path]['description'] = 'Manage model entity types, including adding
		and removing fields and the display of fields.';
    return $items;
  }
}

/**
 * Generates the model type editing form.
 */
function model_type_form($form, &$form_state, $model_type, $op = 'edit') {

  if ($op == 'clone') {
    $model_type->label .= ' (cloned)';
    $model_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $model_type->label,
    '#description' => t('The human-readable name of this model type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($model_type->type) ? $model_type->type : '',
    '#maxlength' => 32,
//    '#disabled' => $model_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'model_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this model type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['data']['#tree'] = TRUE;
  $form['data']['sample_data'] = array(
    '#type' => 'checkbox',
    '#title' => t('An interesting model switch'),
    '#default_value' => !empty($model_type->data['sample_data']),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save model type'),
    '#weight' => 40,
  );

  //Locking not supported yet
  /*if (!$model_type->isLocked() && $op != 'add') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete model type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('model_type_form_submit_delete')
    );
  }*/
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function model_type_form_submit(&$form, &$form_state) {
  $model_type = entity_ui_form_submit_build_entity($form, $form_state);
  $model_type->save();
  $form_state['redirect'] = 'admin/structure/model_types';
}

/**
 * Form API submit callback for the delete button.
 */
function model_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/model_types/manage/' . $form_state['model_type']->type . '/delete';
}
