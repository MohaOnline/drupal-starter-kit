<?php

/**
 * The base service type class , registered with hook_hosting_service_type.
 */
class hostingService_remote_import extends hostingService {
  /**
   *   the value stored in the service column of hosting_service table.
   */
  public $service = 'remote_import';
}


/**
 * An implementation of the example service type, registered with hook_hosting_service.
 */
class hostingService_remote_import_hostmaster extends hostingService_remote_import {
  /**
   *   the value stored in the type column of the hosting_service table.
   */
  public $type = 'hostmaster';


  /**
   *   this service needs to have a port specified for it.
   */
  public $has_port = FALSE;

  /**
   * The name displayed to users when creating or editing a server.
   *
   * @var name
   */
  public $name = 'Remote hostmaster';

  /**
   * {@inheritdoc}
   */
  public function load() {
    parent::load();
    $this->remote_user = variable_get('hosting_user_remote_server_' . $this->server->nid, '');
  }

  /**
   * {@inheritdoc}
   */
  public function insert() {
    parent::insert();
    variable_set('hosting_user_remote_server_' . $this->server->nid, $this->remote_user);
  }

  /**
   * {@inheritdoc}
   */
  public function update() {
    parent::update();
    variable_set('hosting_user_remote_server_' . $this->server->nid, $this->remote_user);
  }

  /**
   * {@inheritdoc}
   */
  public function delete() {
    parent::delete();
    variable_del('hosting_user_remote_server_' . $this->server->nid);
  }

  /**
   * {@inheritdoc}
   */
  public function form(&$form) {
    parent::form($form);
    $form['remote_user'] = array(
      '#type' => 'textfield',
      '#title' => 'Remote user',
      '#description' => t('The remote ssh user.'),
      '#default_value' => (isset($this->remote_user)) ? $this->remote_user : '',
      '#size' => 20,
      '#maxlength' => 255,
    );
  }

  /**
   * {@inheritdoc}
   */
  public function view(&$render) {
    parent::view($render);
    $render['remote_user'] = array(
      '#type' => 'item',
      '#title' => t('Remote user'),
      '#markup' => filter_xss($this->remote_user),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function context_options($task_type, $ref_type, &$task) {
    parent::context_options($task_type, $ref_type, $task);
    $task->context_options['remote_user'] = variable_get('hosting_user_remote_server_' . $this->server->nid, '');
  }

}
