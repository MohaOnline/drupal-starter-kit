<?php

/**
 * Class views_handler_filter_ckan.
 *
 * Generic filter for CKAN visulisation views. This will allow you to specify
 * the field that you want to query with.
 */

class views_handler_filter_ckan_field extends views_handler_filter_string {

  /**
   * The field for the CKAN resource that this field uses.
   *
   * @var string
   */
  var $ckan_field_name = NULL;

  /**
   * Stores the use taxonomy setting.
   *
   * @var string
   */
  var $use_taxonomy = NULL;

  /**
   * Stores the taxonomy value on which the filter filters.
   *
   * @var string
   */
  var $taxonomy = NULL;

  /**
   * Stores the multiple selection setting.
   *
   * @var string
   */
  var $multiple = NULL;

  public function init(&$view, &$options) {
    parent::init($view, $options);
    $this->ckan_field_name = $this->options['ckan_field_name'];
    $this->use_taxonomy = $this->options['use_taxonomy'];
    $this->taxonomy = $this->options['taxonomy'];
    $this->multiple = $this->options['multiple'];
  }

  /**
   * Define an option for the definition.
   *
   * Updates the option definiton for the filter to include the field name that
   * we will allow view creators to specify.
   *
   * @return array
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['ckan_field_name'] = array('default' => '');
    $options['use_taxonomy'] = array('default' => FALSE);
    $options['taxonomy'] = array('default' => '');
    $options['multiple'] = array('default' => FALSE);
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
    return [
      'contains' => [
        'title' => t('Contains'),
        'short' => t('contains'),
        'method' => 'add_filter',
        'values' => 1,
      ],
    ];
  }

  /**
   * Add the filter to the views query.
   */
  function add_filter() {
    $values = is_array($this->value) ? $this->value : [$this->value];
    foreach ($values as $value) {
      $this->query->add_filter($this->ckan_field_name, $value);
    }
  }

  /**
   * Add the CKAN field name field to the
   *
   * @see parent::value_form
   */
  public function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);

    $form['use_taxonomy'] = [
      '#type' => 'checkbox',
      '#title' => t('Use taxonomy vocabulary'),
      '#description' => t('Use a taxonomy to provide values for this filter'),
      '#default_value' => $this->use_taxonomy,
    ];

    $form['multiple'] = array(
      '#type' => 'checkbox',
      '#title' => t('Allow multiple selections'),
      '#description' => t('Enable to allow users to select multiple items.'),
      '#default_value' => $this->multiple,
    );

    $form['taxonomy'] = [
      '#type' => 'select',
      '#title' => t('Taxonomy vocabulary'),
      '#description' => t('Choose which vocabulary should be used'),
      '#default_value' => $this->taxonomy,
      '#options' => $this->get_vocabs(),
      '#empty_option' => '- Select -',
      '#states' => [
        'invisible' => [
          ':input[name="options[use_taxonomy]"]' => ['checked' => FALSE],
        ],
      ],
    ];

    $form['ckan_field_name'] = [
      '#type' => 'textfield',
      '#title' => t('CKAN Field Name'),
      '#description' => t('The name of the field as it appears in the CKAN dataset.'),
      '#default_value' => $this->ckan_field_name,
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
    unset($form['use_taxonomy']);
    unset($form['taxonomy']);
    unset($form['multiple']);

    if ($this->use_taxonomy) {
      $form[$this->options['id']]['#type'] = 'select';

      if ($this->multiple) {
        $form[$this->options['id']]['#multiple'] = TRUE;
      }

      $form[$this->options['id']]['#options'] = $this->get_field_options();
      $form[$this->options['id']]['#empty_option'] = '- Select -';
      unset($form[$this->options['id']]['#size']);
    }
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
   * Make a request to the configured datasets and retrieve a list
   *
   * @return array
   *   As expected for
   */
  public function get_field_options() {
    $vocab = taxonomy_vocabulary_load($this->options['taxonomy']);
    $tree = taxonomy_get_tree($vocab->vid);
    $options = array();

    foreach ($tree as $term) {
      $options[$term->name] = $term->name;
    }

    return $options;
  }

}
