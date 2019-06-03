<?php

/**
 * @file
 * Self-signed service implementation of the Certificate service type for the hosting front end.
 */

/**
 * An implementation of the certificate service type, registered with hook_hosting_service.
 */
class hostingService_Certificate_SelfSigned extends hostingService_Certificate {

  public $type = 'SelfSigned';
  public $name = 'Self Signed';

  protected $self_signed_field = 'default';

  /**
   * node operations
   */

  /**
   * Load associated values for the service.
   *
   * In this certificate we will use the variable system to retrieve values.
   */
  function load() {
    parent::load();
    $this->self_signed_field = variable_get('hosting_self_signed_field_' . $this->server->nid, $this->self_signed_field);

    /**
     * Although this certificate does not have it's own tables, we provide some utitilty functions
     * for use in this method.
     *
     * If this certificate used it's own tables, we could use the mergeData method below to merge in the
     * results automatically, instead of iterating through the results ourself.
     */

    // $this->mergeData("SELECT self_signed_field FROM {hosting_certificate} WHERE vid = :vid", array(':vid' => $this->server->vid));
  }

  /**
   * Display settings on the server node page.
   *
   * @param
   *   A reference to the associative array of the subsection of the page
   *   reserved for this service implementation.
   */
  function view(&$render) {
    parent::view($render);
    $render['self_signed_field'] = array(
      '#type' => 'item',
      '#title' => t('Self-signed field'),
      // Remember to pass the display through filter_xss!
      '#markup' => filter_xss($this->self_signed_field),
    );
  }

  /**
   * Extend the server node form.
   *
   * @param
   *   A reference to the associative array of the subsection of the form
   *   reserved for this service implementation.
   */
  function form(&$form) {
    parent::form($form);

    $form['self_signed_field'] = array(
      '#type' => 'textfield',
      '#title' => t('SelfSigned field'),
      '#description' => t('An self_signed field for the user to fill in.'),
      '#size' => 40,
      '#default_value' => $this->self_signed_field,
      '#maxlength' => 64,
      '#weight' => 5,
    );
  }

  /**
   * Validate a form submission.
   */
  function validate(&$node, &$form, &$form_state) {
    parent::validate($node, $form, $form_state);

    if (strlen($this->self_signed_field) > 30) {
      form_set_error('self_signed_field', t("The certificate string must not be longer than 30 characters"));
    }
  }

  /**
   * Insert a record into the database.
   */
  function insert() {
    parent::insert();

    variable_set('hosting_self_signed_field_' . $this->server->nid, $this->self_signed_field);
  }

  /**
   * Update a record in the database.
   */
  function update() {
    parent::update();

    variable_set('hosting_self_signed_field_' . $this->server->nid, $this->self_signed_field);
  }

  /**
   * Delete a record from the database, based on server node.
   */
  function delete() {
    parent::delete();

    variable_del('hosting_self_signed_field_' . $this->server->nid);
  }

  /**
   * Delete a specific revision from the database.
   */
  function delete_revision() {
    parent::delete_revision();
  }

  /**
   * Pass values to the provision backend when we call provision-save.
   */
  public function context_options($task_type, $ref_type, &$task) {
    parent::context_options($task_type, $ref_type, $task);

    $task->context_options['self_signed_field'] = $this->self_signed_field;
  }
}
