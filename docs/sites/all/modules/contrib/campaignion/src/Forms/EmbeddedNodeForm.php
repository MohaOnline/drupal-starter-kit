<?php

namespace Drupal\campaignion\Forms;

class EmbeddedNodeForm {
  protected $embed_state;
  protected $form;
  protected $parents;
  public function __construct($node, &$form_state, $parents = array(), $embedState = array()) {
    form_load_include($form_state, 'inc', 'node', 'node.pages');
    $form_state += array('embedded' => array(), 'field' => array());
    $a = &$form_state['embedded'];
    foreach ($parents as $k) {
      if (!isset($a[$k])) {
        $a[$k] = array();
      }
      $a = &$a[$k];
    }
    $this->embed_state = &drupal_array_get_nested_value($form_state['embedded'], $parents);
    $this->embed_state += $embedState;
    $this->embed_state['formObject'] = $this;
    $this->embed_state['node'] = $node;
    $this->embed_state['build_info'] = array(
      'form_id' => $node->type . '_node_form',
      'base_form_id' => 'node_form',
    );
    $this->embed_state['field'] = &$form_state['field'];
    $this->parents = $parents;

    $node->revision = FALSE;
  }

  /**
   * Calls form alter hooks.
   * @see hook_form_FORM_ID_alter().
   * @see hook_form_BASE_FORM_ID_alter().
   */
  protected function alterForm(&$form, &$form_state) {
    $form['#form_id'] = $this->embed_state['build_info']['form_id'];
    $hooks = array('form');
    if (isset($form_state['build_info']['base_form_id'])) {
      $hooks[] = 'form_' . $form_state['build_info']['base_form_id'];
    }
    $form_id = $form_state['build_info']['form_id'];
    $hooks[] = 'form_' . $form_id;
    drupal_alter($hooks, $form, $form_state, $form_id);

    // Fixup path.module
    if (isset($form['path'])) {
      unset($form['path']['#element_validate']);
    }
  }

  protected function embedFieldGroups(&$form) {
    if (count($this->parents) > 0) {
      $embed_name = implode('][', $this->parents);
      $form['#tree'] = TRUE;
      foreach ($form as $key => &$element) {
        if ($key[0] != '#' && isset($element['#type']) && $element['#type'] == 'fieldset' && isset($element['#group'])) {
          $element['#group'] = $embed_name . '][' . $element['#group'];
        }
      }
    }
  }

  public function formArray(&$outer_form) {
    $form['#parents'] = $this->parents;
    $form = node_form($form, $this->embed_state, $this->embed_state['node']);
    $this->alterForm($form, $this->embed_state);
    $this->embedFieldGroups($form);
    $this->remapClientsideValidations($form, $outer_form);
    $this->embed_state['handlers'] = [
      'validate' => !empty($form['#validate']) ? $form['#validate'] : [],
      'submit' => !empty($form['#submit']) ? $form['#submit'] : [],
    ];
    return $form;
  }

  protected function remapClientsideValidations(&$form, &$outer_form) {
    $s = '#clientside_validation_settings';
    $f = 'clientside_validation_form_after_build';
    if (!isset($form[$s])) {
      return;
    }
    foreach ($form['#after_build'] as $key => $callback) {
      if ($callback == $f) {
        unset($form['#after_build'][$key]);
      }
    }
    $outer_form += array('#after_build' => array(), $s => array());
    $outer_form[$s] = array_merge($form[$s], $outer_form[$s]);
    if (array_search($f, $outer_form['#after_build']) !== FALSE) {
      $outer_form['#after_build'][] = $f;
    }
  }

  public function validate($form, &$form_state) {
    $form = &drupal_array_get_nested_value($form, $this->parents);
    $this->embed_state['complete form'] = &$form;
    $this->embed_state['triggering_element'] = $form_state['triggering_element'];
    // field_attach_submit() needs the values properly nested while
    // entity_form_submit_build_entity() needs top-level node form values
    // therefore we have to provide both.
    $this->embed_state['values'] =& drupal_array_get_nested_value($form_state['values'], $this->parents);
    if (count($this->parents) > 0) {
      $parent = $this->parents[0];
      $this->embed_state['values'][$parent] = &$form_state['values'][$parent];
    }
    // Fixup for path.module
    if (isset($form['path']) && function_exists('path_form_element_validate')) {
      path_form_element_validate($form['path'], $this->embed_state, $form);
    }
    foreach ($this->embed_state['handlers']['validate'] as $function) {
      $function($form, $this->embed_state);
    }
  }

  public function submit($form, &$form_state) {
    $form = &drupal_array_get_nested_value($form, $this->parents);
    $this->embed_state['complete form'] = &$form;
    $submit_handlers = isset($form['#submit']) ? $form['#submit'] : FALSE;
    if ($submit_handlers) {
      unset($form['#submit']);
    }
    foreach ($this->embed_state['handlers']['submit'] as $function) {
      $function($form, $this->embed_state);
    }
    node_form_submit($form, $this->embed_state);
    if ($submit_handlers) {
      $form['#submit'] = $submit_handlers; unset($submit_handlers);
    }
  }

  public function node() {
    return $this->embed_state['node'];
  }
}
