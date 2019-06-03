<?php

namespace Drupal\campaignion\Forms;

class EntityFieldForm {
  protected $entity_type;
  protected $bundle;
  protected $entity;
  protected $field_names;
  protected $instances;

  public function __construct($entity_type, $entity, $field_names, $language = NULL) {
    $this->entity_type = $entity_type;
    $this->entity = $entity;
    $this->field_names = $field_names;
    $ids = entity_extract_ids($entity_type, $entity);
    $this->bundle = $ids[2];

    $this->instances = array();
    foreach ($field_names as $field_name) {
      $instance = field_info_instance($entity_type, $field_name, $this->bundle);
      $field = field_info_field_by_id($instance['field_id']);
      $instance['field'] = $field;
      $available_languages = field_available_languages($entity_type, $field);
      $instance['languages'] = _field_language_suggestion($available_languages, $language, $field_name);
      $this->instances[$field_name] = $instance;
    }
  }

  public function formArray(&$form_state) {
    $field_forms = array('#parents' => array());

    $field_forms += $this->fieldInvoke('form', $field_forms, $form_state, TRUE);

    $field_forms['#pre_render'][] = '_field_extra_fields_pre_render';
    $field_forms['#entity_type']  = $this->entity_type;
    $field_forms['#bundle']       = $this->bundle;

    return $field_forms;
  }

  public function validate(&$form, &$form_state) {
    $this->fieldInvoke('extract_form_values', $form, $form_state, TRUE);
    $this->fieldInvoke('submit', $form, $form_state, TRUE);

    try {
      $errors = array();
      $null = NULL;
      $this->fieldInvoke('validate', $errors, $null, TRUE);
      $this->fieldInvoke('validate', $errors, $null, FALSE);

      if ($errors) {
        throw new \FieldValidationException($errors);
      }
    }
    catch (\FieldValidationException $e) {
      // Pass field-level validation errors back to widgets for accurate error
      // flagging.
      foreach ($e->errors as $field_name => $field_errors) {
        foreach ($field_errors as $langcode => $errors) {
          $field_state = field_form_get_state($form['#parents'], $field_name, $langcode, $form_state);
          $field_state['errors'] = $errors;
          field_form_set_state($form['#parents'], $field_name, $langcode, $form_state, $field_state);
        }
      }
      $this->fieldInvoke('form_errors', $form, $form_state, TRUE);
      $this->fieldInvoke('form_errors', $form, $form_state, FALSE);
    }
  }

  public function submit(&$form, &$form_state) {
    entity_save($this->entity_type, $this->entity);
  }

  protected function fieldInvoke($op, &$a, &$b, $default = FALSE) {
    // Iterate through the instances and collect results.
    $return = array();
    foreach ($this->instances as $instance) {
      // field_info_field() is not available for deleted fields, so use
      // field_info_field_by_id().
      $field_name = $instance['field']['field_name'];
      $function = $default ? 'field_default_' . $op : $instance['field']['module'] . '_field_' . $op;

      if (!function_exists($function)) {
        continue;
      }

      foreach ($instance['languages'] as $langcode) {
        $items = isset($this->entity->{$field_name}[$langcode]) ? $this->entity->{$field_name}[$langcode] : array();
        $result = $function($this->entity_type, $this->entity, $instance['field'], $instance, $langcode, $items, $a, $b);
        if (isset($result)) {
          // For hooks with array results, we merge results together.
          // For hooks with scalar results, we collect results in an array.
          if (is_array($result)) {
            $return = array_merge($return, $result);
          }
          else {
            $return[] = $result;
          }
        }

        // Populate $items back in the field values, but avoid replacing missing
        // fields with an empty array (those are not equivalent on update).
        if ($items !== array() || isset($this->entity->{$field_name}[$langcode])) {
          $this->entity->{$field_name}[$langcode] = $items;
        }
      }
    }
    return $return;
  }
}
