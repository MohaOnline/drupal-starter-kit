<?php

/**
 * @file
 * LetsEncrypt service implementation of the Certificate service type for the hosting front end.
 */

/**
 * An implementation of the certificate service type, registered with hook_hosting_service.
 */
class hostingService_Certificate_LetsEncrypt extends hostingService_Certificate {

  public $type = 'LetsEncrypt';

  protected $letsencrypt_ca = 'production';
  protected $letsencrypt_ca_options = array(
    'staging' => 'Staging',
    'production' => 'Production',
  );

  /**
   * node operations
   */

  /**
   * Load associated values for the service.
   */
  function load() {
    parent::load();
    $this->letsencrypt_ca = variable_get('hosting_letsencrypt_ca_' . $this->server->nid, $this->letsencrypt_ca);
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
    $render['letsencrypt_ca'] = array(
      '#type' => 'item',
      '#title' => t("Let's Encrypt CA"),
      // Remember to pass the display through filter_xss!
      '#markup' => filter_xss($this->letsencrypt_ca_options[$this->letsencrypt_ca]),
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

    $form['letsencrypt_ca'] = array(
      '#type' => 'radios',
      '#options' => $this->letsencrypt_ca_options,
      '#title' => t("Let's Encrypt CA"),
      '#description' => t("Which LE Certificate Authority server to use."),
      '#size' => 40,
      '#default_value' => $this->letsencrypt_ca,
      '#maxlength' => 64,
      '#weight' => 5,
    );
  }

  /**
   * Insert a record into the database.
   */
  function insert() {
    parent::insert();

    variable_set('hosting_letsencrypt_ca_' . $this->server->nid, $this->letsencrypt_ca);
  }

  /**
   * Update a record in the database.
   */
  function update() {
    parent::update();

    variable_set('hosting_letsencrypt_ca_' . $this->server->nid, $this->letsencrypt_ca);
  }

  /**
   * Delete a record from the database, based on server node.
   */
  function delete() {
    parent::delete();

    variable_del('hosting_letsencrypt_ca_' . $this->server->nid);
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

    $task->context_options['letsencrypt_ca'] = $this->letsencrypt_ca;
  }
}
