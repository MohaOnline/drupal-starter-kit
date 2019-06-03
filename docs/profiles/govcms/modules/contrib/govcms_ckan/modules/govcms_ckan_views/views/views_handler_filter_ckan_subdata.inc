<?php

/**
 * Class views_handler_filter_ckan_subdata.
 *
 * Generic filter for CKAN visulisation views. This will allow you to specify
 * the subdata field that you want to query with. It will get the subdata values
 * directly from data.gov based on resource id filter value.
 */

class views_handler_filter_ckan_subdata extends views_handler_filter_string {

  /**
   * The CKAN field for subdata filter.
   *
   * @var string
   */
  var $ckan_subdata_field_name = NULL;

  public function init(&$view, &$options) {
    parent::init($view, $options);
    $this->ckan_subdata_field_name = $this->options['ckan_subdata_field_name'];
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['ckan_subdata_field_name'] = array('default' => '');
    $options['operator'] = array('default' => 'contains');

    return $options;
  }

  /**
   * A list of operators that this filter can support.
   *
   * @TODO: Review the API docs to see if we can specify any other operators. A
   * brief review suggested that unless we use postgres mode we are limited to
   * contains queries.
   *
   * @return array
   */
  function operators() {
    return array(
      'contains' => array(
        'title' => t('Contains'),
        'short' => t('contains'),
        'method' => 'add_filter',
        'values' => 1,
      ),
    );
  }

  /**
   * Add the filter to the views query.
   */
  function add_filter() {
    $values = is_array($this->value) ? $this->value : array($this->value);
    foreach ($values as $value) {
      $this->query->add_filter($this->ckan_subdata_field_name, $value);
    }
  }

  public function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);

    $form['ckan_subdata_field_name'] = array(
      '#type' => 'textfield',
      '#title' => t('CKAN Field Name for subdata'),
      '#description' => t('The name of the field as it appears in the CKAN dataset.'),
      '#default_value' => $this->ckan_subdata_field_name,
    );
  }

  /**
   * Alter the exposed form when this filter is used.
   *
   * @see parent::exposed_form
   */
  public function exposed_form(&$form, &$form_state) {
    parent::exposed_form($form, $form_state);
    // Because we require the field name as input it will be appended to the
    // exposed filter form as well. Unsetting this here removes it, when the
    // field is added to the query Views will know which field is to be used.
    unset($form['ckan_subdata_field_name']);

    $form[$this->options['id']]['#type'] = 'select';
    $form[$this->options['id']]['#multiple'] = TRUE;
    $form[$this->options['id']]['#options'] = $this->get_field_options();
    $form[$this->options['id']]['#empty_option'] = '- Select -';
    unset($form[$this->options['id']]['#size']);
  }

  /**
   * Make a request to the configured datasets and retrieve a list
   *
   * @return array
   *   As expected for
   */
  public function get_field_options() {

    $options = array();

    // We need resources to be selected first.
    if (empty($this->view->exposed_input['ckan_resource_id'])) {
      return $options;
    }

    $resources_value = $this->view->exposed_input['ckan_resource_id'];
    $resource_groups = is_array($resources_value) ? $resources_value : explode(PHP_EOL, $resources_value);

    foreach ($resource_groups as $resource_group) {
      $resources = explode(',', $resource_group);
      foreach ($resources as $resource) {
        if ($resource) {
          // Get subdata values from CKAN.
          $opts = array('id' => $resource);
          $client = govcms_ckan_client();
          $response = $client->get('action/datastore_search', $opts);

          // Parse the result to get unique labels.
          // @todo may need to apply label overridden in this filter.
          // The current code for label overridden is within views query plugin
          // which is not able to be used here.
          $parser = govcms_ckan_dataset_parser($response->data);
          $labels = $parser->getUniqueLabels($this->options['ckan_subdata_field_name']);
          $options = array_unique(array_merge($options, $labels), SORT_REGULAR);
        }
      }
    }

    return $options;
  }

}
