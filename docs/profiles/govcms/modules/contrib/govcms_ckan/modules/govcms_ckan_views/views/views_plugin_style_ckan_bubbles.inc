<?php

/**
 * Class views_plugin_style_ckan_bubbles
 *
 * Style plugin for views to show bubbles for CKAN data.
 */

class views_plugin_style_ckan_bubbles extends views_plugin_style_ckan_visualisation {

  public function uses_fields() {
    return TRUE;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['ckan_visualisation']['visualisation_config']['visualisation']['#default_value'] = 'bubble_chart';
    $form['ckan_visualisation']['visualisation_config']['visualisation']['#options'] = array('bubble_chart' => 'Bubble Chart');

  }

  public function render() {
    $this->render_fields($this->view->result);

    if (!empty($this->rendered_fields)) {
      return implode($this->rendered_fields[0]);
    }
  }

}
