<?php

/**
 * Implements hook_form_alter().
 *
 * Disable client-side HTML5 form validation.
 */
function dvg_global_form_alter(&$form, &$form_state, $form_id) {
  if (variable_get('dvg_global_disable_html5_validation', TRUE)) {
    $form['#attributes']['novalidate'] = 'novalidate';
  }
}

/**
 * Implements hook_form_BASE_ID_alter() for taxonomy_overview_vocabularies.
 */
function dvg_global_form_taxonomy_overview_vocabularies_alter(&$form, &$form_state, $form_id) {
  $vocabularies = taxonomy_get_vocabularies();
  foreach ($vocabularies as $vocabulary) {
    $form[$vocabulary->vid]['name']['#markup'] = entity_label('taxonomy_vocabulary', $vocabulary);
  }
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function dvg_global_form_node_form_alter(&$form, $form_state) {
  if (!empty($form['field_highlight'])) {
    $form['#process'][] = 'dvg_global_form_node_form_process_highlight';
  }

  // Make sure fields do not have multiple required markers, due to states.
  $form['#attached']['js'][] = drupal_get_path('module', 'dvg_global') . '/scripts/dvg_global.js';

  // General DVG options should go into this vertical tab.
  $form['dvg_options']['#type'] = 'fieldset';
  $form['dvg_options']['#title'] = t('Extra options');
  $form['dvg_options']['#group'] = 'additional_settings';
  $form['dvg_options']['#attributes']['class'][] = 'dvg-options';
  $form['dvg_options']['#attached']['js'][] = drupal_get_path('module', 'dvg_global') . '/scripts/dvg_global.node.js';
  $form['dvg_options']['#after_build'][] = '_dvg_global_hide_empty_element';
}

/**
 * After build function to hide empty elements.
 */
function _dvg_global_hide_empty_element($form, &$form_state) {
  $form['#access'] = count(element_get_visible_children($form)) > 0;
  return $form;
}

/**
 * Process function that sets required and/or states for the highlight-fields.
 */
function dvg_global_form_node_form_process_highlight($form, $form_state) {
  $is_highlighted = !empty($form_state['node']->field_highlight) && (bool)$form_state['node']->field_highlight[LANGUAGE_NONE][0]['value'];
  if (!empty($form_state['input']['field_highlight'])) {
    $is_highlighted = ($form_state['input']['field_highlight'][LANGUAGE_NONE] == 1);
  }

  $form['field_highlight_text'][LANGUAGE_NONE][0]['value']['#required'] =
  $form['field_highlight_more_label'][LANGUAGE_NONE][0]['value']['#required'] = $is_highlighted;

  // Use states to mark as required.
  $form['field_highlight_text']['#states'] =
  $form['field_highlight_more_label']['#states'] = array(
    'required' => array(
      ':input[name="field_highlight[' . $form['field_highlight']['#language'] . ']"]' => array('checked' => TRUE),
    ),
  );

  return $form;
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function dvg_global_form_taxonomy_form_vocabulary_alter(&$form, $form_state) {
  // Don't add fields on the vocabulary-delete confirm-form.
  if (!empty($form_state['confirm_delete'])) {
    return;
  }

  $hide_description = 0;
  if (!empty($form['#vocabulary']->machine_name)) {
    $hide_description = variable_get('dvg_vocabulary__hide_description__' . $form['#vocabulary']->machine_name, 0);
  }
  $form['hide_description'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide description'),
    '#description' => t('This will hide the description field for terms.'),
    '#default_value' => $hide_description,
  );

  $hide_relations = 0;
  if (!empty($form['#vocabulary']->machine_name)) {
    $hide_relations = variable_get('dvg_vocabulary__hide_relations__' . $form['#vocabulary']->machine_name, 0);
  }
  $form['hide_relations'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide relations'),
    '#description' => t('This will hide the relations fieldset for terms.'),
    '#default_value' => $hide_relations,
  );

  $form['#submit'][] = 'dvg_global_form_taxonomy_form_vocabulary_submit';
}

/**
 * Submit handler that stores the hide description/relations variables.
 */
function dvg_global_form_taxonomy_form_vocabulary_submit($form, $form_state) {
  // Remove old variables if the machine_name changes.
  if (!empty($form['#vocabulary']->old_machine_name) && $form['#vocabulary']->old_machine_name != $form['#vocabulary']->machine_name) {
    variable_del('dvg_vocabulary__hide_description__' . $form['#vocabulary']->old_machine_name);
    variable_del('dvg_vocabulary__hide_relations__' . $form['#vocabulary']->old_machine_name);
  }

  variable_set('dvg_vocabulary__hide_description__' . $form['#vocabulary']->machine_name, (bool)$form_state['values']['hide_description']);
  variable_set('dvg_vocabulary__hide_relations__' . $form['#vocabulary']->machine_name, (bool)$form_state['values']['hide_relations']);
}

/**
 * Implements hook_taxonomy_vocabulary_delete().
 */
function dvg_global_taxonomy_vocabulary_delete($vocabulary) {
  variable_del('dvg_vocabulary__hide_description__' . $vocabulary->machine_name);
  variable_del('dvg_vocabulary__hide_relations__' . $vocabulary->machine_name);
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function dvg_global_form_taxonomy_form_term_alter(&$form, $form_state) {
  if (!empty($form['#bundle'])) {
    $form['description']['#access'] = !variable_get('dvg_vocabulary__hide_description__' . $form['#bundle'], FALSE);
    $form['relations']['#access'] = !variable_get('dvg_vocabulary__hide_relations__' . $form['#bundle'], FALSE);
  }
}

/**
 * Implements hook_features_pipe_COMPONENT_alter().
 *
 * Automatically select the correct variables for a vocabulary.
 */
function dvg_global_features_pipe_taxonomy_alter(&$pipe, $data, $export) {
  foreach ($data as $type) {
    $pipe['variable'][] = 'dvg_vocabulary__hide_description__' . $type;
    $pipe['variable'][] = 'dvg_vocabulary__hide_relations__' . $type;
  }
}
