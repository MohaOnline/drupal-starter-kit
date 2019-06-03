<?php

/**
 * Class views_handler_ckan label key.
 *
 * Generic filter for CKAN visulisation views. This will allow you to specify
 * the label key which will represent on axis. Also it provide a additional
 * filter for axis orientation.
 */

class views_handler_filter_ckan_label_key extends views_handler_filter_string {

  /**
   * Stores the field name for label key options on which the filter filters.
   *
   * @var string
   */
  var $ckan_field_name = NULL;

  /**
   * Stores the label axis value on which the filter filters.
   *
   * @var string
   */
  var $label_axis = NULL;

  public function init(&$view, &$options) {
    parent::init($view, $options);
    $this->ckan_field_name = $this->options['ckan_field_name'];
    $this->label_axis = $this->options['label_axis'];
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['ckan_field_name'] = array('default' => '');
    $options['label_axis'] = array('default' => 'x');
    $options['operator'] = array('default' => 'contains');

    return $options;
  }

  function operators() {
    return [
      'contains' => [
        'title' => t('Contains'),
        'short' => t('contains'),
        'method' => 'add_label_key',
        'values' => 1,
      ],
    ];
  }

  /**
   * Add label key.
   */
  function add_label_key() {
    $this->view->label_key = $this->value;
  }

  /**
   * Add the Label axis filter.
   *
   * @see parent::value_form
   */
  public function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);

    $form['label_axis'] = [
      '#type' => 'select',
      '#title' => t('Label axis'),
      '#description' => t('Choose which axis should be used for label'),
      '#default_value' => $this->label_axis,
      '#options' => array(
        'x' => 'Horizontal',
        'y' => 'Vertical',
      ),
    ];

  }

  /**
   * Update the build group from to include the CKAN field name.
   *
   * This is called when you choose to expose a field by a group.
   */
  public function build_group_form(&$form, &$form_state) {
    $form['ckan_field_name'] = [
      '#type' => 'textfield',
      '#title' => t('CKAN Field Name'),
      '#description' => t('The name of the field as it appears in the CKAN dataset.'),
      '#default_value' => $this->ckan_field_name,
    ];

    return parent::build_group_form($form, $form_state);
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
    unset($form['ckan_field_name']);

    $form[$this->options['id']]['#type'] = 'select';
    $form[$this->options['id']]['#options'] = $this->get_field_options();
    $form[$this->options['id']]['#empty_option'] = '- Select -';
    unset($form[$this->options['id']]['#size']);
  }

  /**
   * Retrieve a list of vocabularies available for the site.
   *
   * @return array
   */
  public function get_vocabs() {
    $vocabs = taxonomy_get_vocabularies();
    $options = array();
    foreach ($vocabs as $vocab) {
      $options[$vocab->vid] = $vocab->name;
    }
    return $options;
  }


  /**
   * Get a list of label key options.
   *
   * @return array
   *   A array of field options for label key.
   */
  public function get_field_options() {
    $style_options = $this->view->display_handler->get_option('style_options');
    $visualisation_config = $style_options['ckan_visualisation']['visualisation_config'];

    // The label options should be inside marge keys. It's not required if we
    // only have one key and one label in the data set.
    $merge_keys = govcms_ckan_views_remove_unchecked_options($visualisation_config['merge_settings']['merge_keys']);

    $options = array();
    if (!empty($merge_keys)) {
      // Get keys.
      $keys = govcms_ckan_views_remove_unchecked_options($visualisation_config['keys']);

      // Label options should not include keys. Remove them.
      $options = array_diff($merge_keys, $keys);
    }

    return $options;
  }

}
