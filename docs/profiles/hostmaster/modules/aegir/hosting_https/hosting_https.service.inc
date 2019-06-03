<?php
/**
 * @file
 * Define Hosting service class for HTTPS.
 */

class hostingService_https extends hostingService_http_public {

  function load() {
    parent::load();
    $this->mergeData("SELECT https_port FROM {hosting_https_server} WHERE vid=:vid", array(':vid' => $this->server->vid));
    $this->https_enabled = TRUE;
  }

  function insert() {
    parent::insert();
    $id = db_insert('hosting_https_server')
      ->fields(array(
        'vid' => $this->server->vid,
        'nid' => $this->server->nid,
        'https_port' => $this->https_port,
      ))
      ->execute();
  }

  function delete() {
    parent::delete();
    db_delete('hosting_https_server')
      ->condition('nid', $this->server->nid)
      ->execute();
  }

  function delete_revision() {
    parent::delete_revision();
    db_delete('hosting_https_server')
      ->condition('vid', $this->server->vid)
      ->execute();
  }

  function form(&$form) {
    parent::form($form);

    $form['https_port'] = array(
      '#type' => 'textfield',
      '#title' => t('HTTPS Port'),
      '#required' => !empty($this->available),
      '#size' => 40,
      '#default_value' => isset($this->https_port) ? $this->https_port : '443',
      '#description' => t("The port that this service will use for encrypted traffic."),
      '#maxlength' => 255,
      '#weight' => -7,
    );
  }

  function view(&$render) {
    parent::view($render);
    $render['https_port'] = array(
      '#type' => 'item',
      '#title' => t('HTTPS Port'),
      '#markup' => filter_xss($this->https_port),
    );
  }

  public function validate(&$node, &$form, &$form_state) {
    parent::validate($node, $form, $form_state);
    if ((int) $this->https_port <= 0) {
      form_set_error('https_port', t("The port you specify must be a number."));
    }
  }

  public function context_options($task_type, $ref_type, &$task) {
    parent::context_options($task_type, $ref_type, $task);
    $task->context_options[$this->service . '_https_port'] = $this->https_port;
  }

  public function context_import($context) {
    parent::context_import($context);

    $this->https_port = $context->https_port;
  }

}

