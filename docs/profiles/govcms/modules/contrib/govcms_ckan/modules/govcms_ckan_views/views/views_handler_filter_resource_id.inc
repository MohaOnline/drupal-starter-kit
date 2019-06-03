<?php

/**
 * Class views_handler_filter_ckan.
 *
 * Generic filter for CKAN visulisation views. This will allow you to specify
 * the field that you want to query with.
 */

class views_handler_filter_resource_id extends views_handler_filter_string {

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

  /**
   * Stores the merge resources setting.
   *
   * @var string
   */
  var $merge_resources = NULL;

  public function init(&$view, &$options) {
    parent::init($view, $options);
    $this->use_taxonomy = $this->options['use_taxonomy'];
    $this->taxonomy = $this->options['taxonomy'];
    $this->multiple = $this->options['multiple'];
    $this->merge_resources = $this->options['merge_resources'];
  }

  /**
   * Define an option for the definition.
   *
   * Updates the option definiton for the filter to get values from taxonomy.
   *
   * @return array
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['use_taxonomy'] = array('default' => FALSE);
    $options['taxonomy'] = array('default' => '');
    $options['multiple'] = array('default' => FALSE);
    $options['operator'] = array('default' => 'contains');
    $options['merge_resources'] = array('default' => TRUE);

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
    $values = is_array($this->value) ? $this->value : explode(PHP_EOL, $this->value);

    $values_amount = count($values);
    for ($i = 0; $i < $values_amount; $i++) {
      $resources = explode(',', array_shift($values));
      foreach ($resources as $resource) {
        if ($resource) {
          $this->query->add_resource($resource, $i);
        }
      }
    }
  }

  /**
   * Add the CKAN field name field to the
   *
   * @see parent::value_form
   */
  public function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);

    $form['value']['#type'] = 'textarea';

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
      '#description' => t('Choose which vocabulary should be used. Please make sure term "Description" field is used for storing "Resource ID"'),
      '#default_value' => $this->taxonomy,
      '#options' => $this->get_vocabs(),
      '#empty_option' => '- Select -',
      '#states' => [
        'invisible' => [
          ':input[name="options[use_taxonomy]"]' => ['checked' => FALSE],
        ],
      ],
    ];

    $form['merge_resources'] = [
      '#type' => 'checkbox',
      '#title' => t('Merge resources by dataset type'),
      '#description' => t('Merge multiple resources data retrieved into a single table grouped by dataset type'),
      '#default_value' => $this->merge_resources,
    ];

    return $form;
  }

  /**
   * Alter the exposed form when this filter is used.
   *
   * @see parent::exposed_form
   */
  public function exposed_form(&$form, &$form_state) {
    parent::exposed_form($form, $form_state);
    // Unsetting fields from exposed form.
    unset($form['use_taxonomy']);
    unset($form['taxonomy']);
    unset($form['multiple']);
    unset($form['merge_resources']);

    if ($this->use_taxonomy) {
      $form[$this->options['id']]['#type'] = 'select';

      if ($this->multiple) {
        $form[$this->options['id']]['#multiple'] = TRUE;
      }

      $form[$this->options['id']]['#options'] = $this->get_field_options();
      $form[$this->options['id']]['#empty_option'] = '- Select -';
      $form[$this->options['id']]['#empty_value'] = '';
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
      // Remove html tags because we store resource id in wysiwyg editor.
      $resource_ids = drupal_html_to_text($term->description);
      $resource_id = str_replace(PHP_EOL, ',', $resource_ids);
      $options[$resource_id] = $term->name;
    }

    return $options;
  }

}
