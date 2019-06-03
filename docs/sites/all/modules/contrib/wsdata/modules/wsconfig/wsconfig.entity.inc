<?php

/**
 * @file
 * Entity classes
 */

/**
 * Exception thrown by WsConfig() on unsupported calls
 */
class WsConfigException extends Exception {}

/**
 * The class used for wsconfig entities
 */
class WsConfig extends Entity {

  public $connector;

  public function __construct($values = array()) {
    parent::__construct($values, 'wsconfig');
    $this->wsconfig_type = wsconfig_type_load($this->type);

    if (isset($this->wsconfig_type->data['connector']) and class_exists($this->wsconfig_type->data['connector'])) {
      $this->connector = new $this->wsconfig_type->data['connector']($this->wsconfig_type->getEndpoint());
      // Configure connector with caching settings.
      if ($this->connector->supportsCaching()) {
        if (is_string($this->data)) {
          $data = unserialize($this->data);
        } else {
          $data = $this->data;
        }
        
        $cache_default_time = isset($data['cache_default_time']) ? $data['cache_default_time'] : 0;
        $cache_default_override = isset($data['cache_default_override']) ? $data['cache_default_override'] : FALSE;
        $stale_cache = isset($data['stale_cache']) ? $data['stale_cache'] : FALSE; 
        $this->connector->defaultCache($cache_default_time, $cache_default_override, $stale_cache);
      }
    }
  }


  /**
   * Method wsconfig->setEndpoint().
   *  Overide's the wsconfig_type's default endpoint
   */
  public function setEndpoint($endpoint) {
    if (isset($this->wsconfig_type->data['connector']) and class_exists($this->wsconfig_type->data['connector'])) {
      $this->wsconfig_type->setEndpoint($endpoint);
      $this->connector = new $this->wsconfig_type->data['connector']($endpoint);
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Method wsconfig->getEndpoint().
   */
  public function getEndpoint() {
    if (isset($this->connector) and is_object($this->connector)) {
      return $this->connector->getEndpoint();
    }
    return FALSE;
  }

  /**
   * Get the currently configured language plugin and its settings
   */
  public function getLanguagePlugin() {
    $plugin = $this->wsconfig_type->getEnabledLanguagePlugin();
    if (!empty($plugin)) {
      return $plugin;
    }
    return FALSE;
  }

  /**
   * Method for calling a webservice method.
   *
   * @param string $type
   *  Name of the type of call. Generally "CRUD" but could include other methods
   * @param array $replacements [optional]
   *  Replacements values for placeholders in the request URI
   * @param array $argument [optional]
   *  Payload data for POST requests. Ex: body => 'body data'
   * @param array $options [optional]
   *  Options to pass to the connector. Ex: header data, special triggers.
   *  See the documentation for your given connector
   * @see http://drupal.org/project/restclient
   * @param string $string [reference]
   *  Reference to the URL which was called
   * @return array
   *  Returns the result of the method call.
   */
  public function call($type, $replacement = array(), $argument = array(), $options = array(), &$method = '') {
    if ($this->wsconfig_type->isDisabled()) {
      return FALSE;
    }

    if (isset($this->data['options'][$this->getMethodKey($type)]) and is_array($this->data['options'][$this->getMethodKey($type)])) {
      $options += $this->data['options'][$this->getMethodKey($type)];
    }

    // Pass a reference to the config to the connector
    $options['wsconfig'] = $this;

    if (isset($this->wsconfig_type->data['language always'])
      and $this->wsconfig_type->data['language always']
      and empty($options['language'])) {

      global $language;
      $options['language'] = $language->language;
    }

    // Add the language handling if a language was requested
    if (!empty($options['language'])) {
      $plugin = $this->getLanguagePlugin();
      $options['language plugin'] = $plugin;
    }

    $replacements = $this->getReplacements($type);
    $method = $this->getMethod($type, $replacement);

    $matches = array();
    preg_match_all("/(%[a-zA-Z0-9]+)/", $method, $matches);

    // Compare the tokens extracted to see if some haven't been replaced.
    if (sizeof($matches[0])) {
      foreach ($matches[0] as $match) {
        if (in_array($match, $replacements)) {
          throw new WsConfigException(t('Replacement tokens not all replaced before wscall: @tokens', array('@tokens' => implode(',', $matches[0]))));
        }
      }
    }
    $start_time = microtime(true);

    $result = FALSE;
    if (isset($this->connector)) {
      $result = $this->connector->wscall($type, $method, $argument, $options);
    }

    if ($result === FALSE and isset($this->connector) and $this->connector->isDegraded()) {
      $this->wsconfig_type->disable(TRUE);
    }

    if (module_exists('ws_performance')) {
      $run_time = round((microtime(true) - $start_time) * 1000);
      ws_performance_record_performance($this, $type, $method, $run_time, array('replacement' => $replacement, 'arguments' => $argument, 'options' => $options), $result);
    }
    return $result;
  }

  /**
   * Get a list of defined methods.
   */
  public function getMethod($type, $replacement = array()) {
    if (!isset($this->data[$type . '_data_method'])) {
      return FALSE;
    }
    $method = $this->data[$type . '_data_method'];
    foreach ($replacement as $token => $replace) {
       $method = str_replace($token, $replace, $method);
    }
    return $method;
  }

  public function getReplacements($type) {
    if (!isset($this->data[$type . '_data_method'])) {
      return FALSE;
    }
    $method = $this->data[$type . '_data_method'];
    $matches = array();
    preg_match_all("/(%\w+)/", $method, $matches);
    return $matches[0];
  }

  public function getOperations() {
    $ops = array();
    foreach ($this->data as $key => $val) {
      if (drupal_substr($key, -1 * drupal_strlen('_data_method')) == '_data_method') {
         $ops[] = drupal_substr($key, 0, -1 * drupal_strlen('_data_method'));
      }
    }
    if (empty($ops)) {
      return array();
    }
    return $ops;
  }

  public function addMethod($type, $name = '') {
    $methods = $this->getPossibleMethods();
    if (!isset($methods[$type])) {
      return FALSE;
    }

    $methodname = $type;

    $supported = $this->connector->getMethods();
    if (isset($supported['multiple'][$type])) {
      $name = drupal_strtolower(preg_replace('/\W/', '', $name));
      if (empty($name)) {
        return FALSE;
      }

      $methodname .= '_' . $name;
    }

    $this->data[$this->getMethodKey($methodname)] = '';
    return TRUE;
  }

  public function getPossibleMethods() {
    $supported = $this->connector->getMethods();
    $methods = array_merge($supported['single'], $supported['multiple']);
    foreach ($this->getOperations() as $op) {
      if (isset($supported['single'][$op])) {
        unset($methods[$op]);
      }
    }
    return $methods;
  }

  public function getMethodKey($operation) {
    return $operation . '_data_method';
  }

  public function getMethodName($operation) {
    $supported = $this->connector->getMethods();
    foreach ($supported['multiple'] as $key => $val) {
      if (drupal_substr($operation, 0, drupal_strlen($key) + 1) == $key . '_') {
         $operation = ucfirst($key) . ': ' . ucfirst(drupal_substr($operation, drupal_strlen($key) + 1));
         return $operation;
      }
    }
    return ucfirst($operation);
  }

  protected function defaultLabel() {
    return $this->title;
  }

  protected function defaultUri() {
    return array('path' => 'wsconfig/' . $this->name);
  }
}

/**
 * The Controller for WsConfig entities
 */
class WsConfigController extends EntityAPIControllerExportable {
  public function __construct($entityType) {
    parent::__construct($entityType);
  }

  /**
   * Create a wsconfig - we first set up the values that are specific
   * to our wsconfig schema but then also go through the EntityAPIController
   * function.
   *
   * @param $type
   *   The machine-readable type of the wsconfig.
   *
   * @return
   *   A wsconfig object with all default fields initialized.
   */
  public function create(array $values = array()) {
    // Add values that are specific to our WsConfig
    $values += array(
      'wsconfig_id' => '',
      'name' => '',
      'is_new' => TRUE,
      'title' => '',
      'created' => '',
      'changed' => '',
      'data' => array(),
    );

    $wsconfig = parent::create($values);
    return $wsconfig;
  }

  /**
   * Overriding the buildContent function to add entity specific fields
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $content = parent::buildContent($entity, $view_mode, $langcode, $content);
//    $content['wsconfig_sample_data'] =  array(
//      '#markup' => theme('wsconfig_sample_data', array('wsconfig_sample_data' => check_plain($entity->data['sample_data']), 'wsconfig' => $entity)),
//    );

    return $content;
  }

  public function export($entity, $prefix = '') {
    $entity = clone $entity;
    unset($entity->wsconfig_type);
    unset($entity->connector);
    unset($entity->created);
    unset($entity->changed);
    return parent::export($entity, $prefix);
  }
}

/**
 * UI controller
 */
class WsConfigUIController extends EntityDefaultUIController {
  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu() {

    $items = parent::hook_menu();
    $id_count = count(explode('/', $this->path));

    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    // Change the overview menu type for the list of web service configurations.
    $items[$this->path]['type'] = MENU_NORMAL_ITEM;

    // Change the add page menu to multiple types of entities
    $items[$this->path . '/add'] = array(
      'title' => t('Add a wsconfig'),
      'description' => t('Add a new Web Service Configuration'),
      'page callback'  => 'wsconfig_add_page',
      'access callback'  => 'wsconfig_access',
      'access arguments' => array('edit'),
      'type' => MENU_LOCAL_ACTION,
      'weight' => -20,
      'file' => 'wsconfig.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );

    // Add menu items to add each different type of entity.
    foreach (wsconfig_get_types() as $type) {
      $items[$this->path . '/add/' . $type->type] = array(
        'title' => 'Add ' . $type->label,
        'page callback' => 'wsconfig_form_wrapper',
        'page arguments' => array(wsconfig_create(array('type' => $type->type))),
        'access callback' => 'wsconfig_access',
        'access arguments' => array('edit', 'edit ' . $type->type),
        'file' => 'wsconfig.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module'])
      );
    }

    // Loading and editing wsconfig entities

    // Menu item for viewing web service configurations
    $items[$this->path . '/' . $wildcard] = array(
      'title callback' => 'wsconfig_page_title',
      'title arguments' => array($id_count),
      'page callback' => 'wsconfig_view_form_wrapper',
      'page arguments' => array($id_count),
      'access callback' => 'wsconfig_access',
      'access arguments' => array('view', $id_count),
      'type' => MENU_CALLBACK,
      'file' => 'wsconfig.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module']),
    );
    return $items;
  }


  /**
   * Create the markup for the add Web Service Configuration Entities page within the class
   * so it can easily be extended/overriden.
   */
  public function addPage() {
    return theme('wsconfig_add_list', array('content' => wsconfig_get_types()));
  }
}


/**
 * The class used for wsconfig type entities
 */
class WsConfigType extends Entity {
  public $type;
  public $label;

  public function __construct($values = array()) {
    parent::__construct($values, 'wsconfig_type');

    if (is_array($this->data) and !isset($this->data['degraded_backoff'])) {
      $this->data['degraded_backoff'] = WSCONFIG_DEFAULT_DEGRADED_BACKOFF;
    }
  }

  /**
   * Gets the enabled language plugin
   *
   * @return array|boolean
   *  Returns the language plugin and settings, FALSE otherwise.
   */
  public function getEnabledLanguagePlugin() {
    $plugin = FALSE;
    if (!empty($this->data['language plugin'])) {
      $plugin = array(
        $this->data['language plugin'] => isset($this->data['language plugin settings']) ? $this->data['language plugin settings'] : array(),
      );
    }

    return $plugin;
  }

  /*
   * API function to get endpoint from the WSConfig Type
   */
  public function setEndpoint($endpoint) {
    $this->data['endpoint'] = $endpoint;
    return true; 
  }

  /*
   * API function to set the endpoint in the WSConfig Type.
   */
  public function getEndpoint() {
    $endpoint = $this->data['endpoint'];
    $matches = array();
    preg_match_all('/\$\{(.+?)(:.+){0,1}\}/', $endpoint, $matches);
    if (!empty($matches[0])) {
      for ($n = 0; $n < count($matches[0]); $n++) {
        $default = '';
        if (isset($matches[2][$n])) {
          $default = drupal_substr($matches[2][$n], 1);
        }
        $replacements[] = (string)variable_get($matches[1][$n], $default);
      }
      $endpoint = str_replace($matches[0], $replacements, $endpoint);
    }
    return $endpoint;
  }

  /**
   * API function to disabled this wsconfig type.
   */
  public function disable($degraded = FALSE) {
    $reason = '';

    if ($degraded) {
      if (!isset($this->data['degraded_backoff'])) {
        $this->data['degraded_backoff'] = WSCONFIG_DEFAULT_DEGRADED_BACKOFF;
      }
      if ($this->data['degraded_backoff'] == 0) {
        return;
      }

      $reason = '  ' . t('Automatically disabled due to degrated service.');
      $this->data['degraded'] = time();
    }

    $this->data['disabled'] = TRUE;
    watchdog('wsconfig', t('WSConfig Type %label (%type) was disabled.', array('%label' => $this->label, '%type' => $this->type)) . $reason);
    $this->save();
  }

  /**
   * API function to enabled this wsconfig type.
   */
  public function enable($degraded = FALSE) {
    unset($this->data['degraded']);
    unset($this->data['disabled']);

    $reason = '';
    if ($degraded) {
      $reason = '  ' . t('Automatically re-enabling previously degrated service.');
    }

    watchdog('wsconfig', t('WSConfig Type %label (%type) was enabled.', array('%label' => $this->label, '%type' => $this->type)) . $reason);
    $this->save();
  }

  /**
   * API function to check if this wsconfig type is disabled.
   */
  public function isDisabled() {

    if (!isset($this->data['degraded_backoff'])) {
      $this->data['degraded_backoff'] = WSCONFIG_DEFAULT_DEGRADED_BACKOFF;
    }

    if (isset($this->data['degraded']) and $this->data['degraded'] < time() - $this->data['degraded_backoff']) {
      $this->enable(TRUE);
      return FALSE;
    }

    return isset($this->data['disabled']) ? $this->data['disabled'] : FALSE;
  }

  public function getDegraded() {
    if (!isset($this->data['degraded_backoff'])) {
      $this->data['degraded_backoff'] = WSCONFIG_DEFAULT_DEGRADED_BACKOFF;
    }

    if (isset($this->data['degraded'])) {
      return $this->data['degraded'] - time() + $this->data['degraded_backoff'];
    }

    return 0;
  }
}

/**
 * The Controller for wsconfig type entities
 */
class WsConfigTypeController extends EntityAPIControllerExportable {
  public function __construct($entityType) {
    parent::__construct($entityType);
  }

  /**
   * Create a wsconfig type - we first set up the values that are specific
   * to our wsconfig type schema but then also go through the EntityAPIController
   * function.
   *
   * @param $type
   *   The machine-readable type of the wsconfig.
   *
   * @return
   *   A wsconfig type object with all default fields initialized.
   */
  public function create(array $values = array()) {
    // Add values that are specific to our Web Service Configuration
    $values += array(
      'id' => '',
      'is_new' => TRUE,
      'data' => array(),
    );
    $wsconfig_type = parent::create($values);
    return $wsconfig_type;
  }
}

/**
 * UI controller
 */
class WsConfigTypeUIController extends EntityDefaultUIController {
  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage wsconfig entity types, including changing endpoints and connectors.';
    return $items;
  }
}
