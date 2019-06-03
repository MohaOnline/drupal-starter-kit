<?php

/**
 * Class views_handler_filter_ckan visualisation.
 *
 * Generic filter for CKAN visulisation views. This will allow you to specify
 * the chart type that you want the data display.
 */

class views_handler_filter_ckan_visualisation extends views_handler_filter_many_to_one {

  function operators() {
    return [
      'or' => array(
        'title' => t('Is one of'),
        'short' => t('or'),
        'short_single' => t('='),
        'method' => 'add_chart',
        'values' => 1,
        'ensure_my_table' => 'helper',
      ),
    ];
  }

  function add_chart() {
    $this->view->visualisation = reset($this->value);
  }

  function get_value_options() { /* don't overwrite the value options */ }

  function expose_form(&$form, &$form_state) {
    parent::expose_form($form, $form_state);

    // Visualisation option should not be multiple select.
    unset($form['expose']['multiple']);
  }

  public function value_form(&$form, &$form_state) {

    // Get the CKAN views default visualisation setting.
    $style_options = $this->view->display_handler->get_option('style_options');
    $visualisation_config = $style_options['ckan_visualisation']['visualisation_config'];
    $default_visualisation = $visualisation_config['visualisation'];

    // Get visualisation options.
    $options = _govcms_ckan_media_field_widget_visualisation_options();
    $default_value = (array) $this->value;

    if (!empty($form_state['exposed'])) {
      $identifier = $this->options['expose']['identifier'];

      if (!empty($this->options['expose']['reduce'])) {
        $options = $this->reduce_value_options($options);
      }
    }

    $form['value'] = array(
      '#title' => t('Visualisation'),
      '#type' => 'select',
      '#multiple' => TRUE,
      '#default_value' => $default_value,
      '#options' => $options,
    );

    if (!empty($form_state['exposed']) && isset($identifier) && !isset($form_state['input'][$identifier])) {
      $form_state['input'][$identifier] = $default_visualisation;
    }

    if (empty($form_state['exposed'])) {
      // Retain the helper option.
      $this->helper->options_form($form, $form_state);

      // Show help text if not exposed to end users.
      $form['value']['#description'] = t('Leave blank for all.');
    }

    return $form;
  }

  function value_submit($form, &$form_state) {
    // prevent array_filter from messing up our arrays in parent submit.
  }

  function admin_summary() {
    // set up $this->value_options for the parent summary
    $this->value_options = array();

    // Get CKAN visualisation plugin options.
    $chart_options = _govcms_ckan_media_field_widget_visualisation_options();

    foreach ($chart_options as $option => $option_name) {
      if (in_array($option, $this->value)) {
        $this->value_options[$option] = $option_name;
      }
    }

    return parent::admin_summary();
  }

  public function exposed_form(&$form, &$form_state) {
    parent::exposed_form($form, $form_state);

    // We don't need all option here.
    unset($form['ckan_visualisation']['#options']['All']);
  }

}
