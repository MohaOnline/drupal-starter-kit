<?php

/**
 * @file
 * Definition of views_handler_field_ckan_resource.
 */

/**
 * A handler to run a field through check_markup, using a companion
 * format field.
 *
 * - format: (REQUIRED) Either a string format id to use for this field or an
 *           array('field' => {$field}) where $field is the field in this table
 *           used to control the format such as the 'format' field in the node,
 *           which goes with the 'body' field.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_ckan_resource extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();

    $options['ckan_resource_id'] = array('default' => '');
    $options['ckan_resource_parents'] = array('default' => '');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['ckan_resource_id'] = array(
      '#type' => 'textfield',
      '#title' => t('CKAN Resource id'),
      '#default_value' => $this->options['ckan_resource_id'],
      '#weight' => -100,
    );

    $form['ckan_resource_parents'] = array(
      '#type' => 'textfield',
      '#title' => t('CKAN Resource parents'),
      '#description' => t('Data parents. If there are more than one parents, should separate them by comma. The order from left to right should be from direct parent to root. Here is a parents example for data of city melbourne: "Victoria, Australia, Asia Pacific"'),
      '#default_value' => $this->options['ckan_resource_parents'],
      '#weight' => -100,
    );

  }

  function query() {
    if ($this->options['ckan_resource_id']) {
      // As we are getting all resource results by fields, so we put all
      // resources in one result row.
      $row_index = 0;
      $this->query->add_resource($this->options['ckan_resource_id'], $row_index);
    }
  }

  function render($data) {

    // Get this resource result.
    $resource_id = $this->options['ckan_resource_id'];
    $value = $data[$resource_id];

    // Get the config.
    $style_options = $this->view->display_handler->get_option('style_options');
    $visualisation_config = $style_options['ckan_visualisation']['visualisation_config'];

    // Set the visualisation chart type, should be bubble chart.
    $chart_type = $visualisation_config['visualisation'];

    // Parse the data.
    $data = new stdClass();

    // Get title for chart from CKAN.
    $client = govcms_ckan_client();
    $response = $client->get('action/resource_show', array('id' => $value['id']));
    $data->filename = isset($response->data->name) ? $response->data->name : '';

    $data->data = new stdClass();
    $data->data->records = $value['data'];

    // We will group multiple tables into one bubble chart based on parents.
    $visualisation_config['bubble_settings']['tables_to_bubble'] = 'true';

    // Get a resource parents structure array.
    $parents = array();
    if (!empty($this->options['ckan_resource_parents'])) {
      $parents = array_map('trim', explode(',', $this->options['ckan_resource_parents']));;
    }
    $visualisation_config['bubble_settings']['parents'] = $parents;
    $visualisation_config['bubble_settings']['label'] = $this->options['label'];
    // Add resource id to table for adding notes on bubble.
    $visualisation_config['bubble_settings']['resource'] = $this->options['ckan_resource_id'];

    // Get the rendered content.
    $output = govcms_ckan_get_plugin_view($chart_type, $data, ['settings' => []], $visualisation_config);

    return $output;
  }

}
