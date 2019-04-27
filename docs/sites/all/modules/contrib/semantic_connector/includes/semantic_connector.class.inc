<?php

/**
 * @file
 * The PoolParty PPX API-class.
 */

/**
 * A collection of static functions offered by the PoolParty Semantic Connector.
 */
class SemanticConnector {
  /**
   * Get a connection of the PoolParty Semantic Connector by its ID.
   *
   * @param string $type
   *   The type of the connection to receive. Possible values: 'pp_server',
   *   and 'sparql_endpoint'.
   * @param int $connection_id
   *   The ID of the Semantic Connector Connection.
   *
   * @return SemanticConnectorConnection
   *   The connection object, depending on the provided $type.
   */
  public static function getConnection($type, $connection_id = 0) {
    switch ($type) {
      case 'pp_server':
        return new SemanticConnectorPPServerConnection($connection_id);

      case 'sparql_endpoint':
        return new SemanticConnectorSparqlEndpointConnection($connection_id);

      default:
        return NULL;
    }
  }

  /**
   * Get all connection of the PoolParty Semantic Connector by connection-type.
   *
   * @param string $type
   *   The type of the connections to receive. Possible values: 'pp_server',
   *   'sonr_server' and 'sparql_endpoint'.
   *
   * @return array
   *   Array of SemanticConnectorConnection-objects of the give type.
   */
  public static function getConnectionsByType($type) {
    $connection_ids = db_select('semantic_connector_connections', 'c')
      ->fields('c', array('connection_id'))
      ->condition('type', $type)
      ->execute()
      ->fetchCol();

    $connections = array();
    foreach ($connection_ids as $connection_id) {
      switch ($type) {
        case 'pp_server':
          $connections[$connection_id] = new SemanticConnectorPPServerConnection($connection_id);
          break;

        case 'sparql_endpoint':
          $connections[$connection_id] = new SemanticConnectorSparqlEndpointConnection($connection_id);
          break;
      }
    }

    return $connections;
  }

  /**
   * Search for connections matching a set of search filters.
   *
   * @param array $search_filters
   *   An associative array of search filters, where the key is the name of the
   *   database field to search in and the value is a string, which will
   *   filtered for on "Exact match"-basis.
   *   Possible keys are: 'type', 'url', 'title', 'username', 'password' and
   *   'config' (config needs to be a serialized array)
   *
   * @return array
   *   Array of SemanticConnectorConnection-objects matching the search filters.
   */
  public static function searchConnections(array $search_filters) {
    $connections_query = db_select('semantic_connector_connections', 'c')
      ->fields('c', array('connection_id', 'type'));

    $allowed_filter_keys = array('type', 'url', 'title', 'username', 'password', 'config');
    foreach ($search_filters as $search_filter_key => $search_filter_value) {
      if (in_array($search_filter_key, $allowed_filter_keys) && (is_string($search_filter_value) || is_array($search_filter_value))) {
        $connections_query->condition($search_filter_key, $search_filter_value);
      }
    }

    $connections_found = $connections_query->execute()
      ->fetchAllAssoc('connection_id');

    $connections = array();
    foreach ($connections_found as $connection_id => $connection_found) {
      switch ($connection_found->type) {
        case 'pp_server':
          $connections[$connection_id] = new SemanticConnectorPPServerConnection($connection_id);
          break;

        case 'sparql_endpoint':
          $connections[$connection_id] = new SemanticConnectorSparqlEndpointConnection($connection_id);
          break;
      }
    }

    return $connections;
  }

  /**
   * Create a new connection for the PoolParty Semantic Connector.
   *
   * @param string $type
   *   The type of the connection to receive. Possible values: 'pp_server',
   *   'sonr_server' and 'sparql_endpoint'.
   * @param string $url
   *   The URL of the connection.
   * @param string $title
   *   The title of the connection.
   * @param array $credentials
   *   The credentials required for the connection in the format
   *   "username:password" if required.
   * @param array $config
   *   The config of the Semantic Connector Connection as an array.
   * @param int $fixed_id
   *   A fixed connection id to use instead of the one with the given $type and
   *   $url.
   *   WARNING: Using an ID that does not exist will result in an error.
   *
   * @return SemanticConnectorConnection
   *   The connection object, depending on the provided $type.
   */
  public static function createConnection($type, $url, $title, array $credentials = array('username' => '', 'password' => ''), array $config = array(), $fixed_id = 0) {
    $connection = NULL;
    $allowed_types = array('pp_server', 'sparql_endpoint');

    if (!in_array($type, $allowed_types)) {
      drupal_set_message(t('The type (%type) of the connection %title is wrong.', array('%type' => $type, '%title' => $title)), 'error');
      return NULL;
    }

    // Remove trailing slashes from the URL.
    $url = rtrim($url,"/");

    if ($fixed_id <= 0) {
      $old_connection_id = db_select('semantic_connector_connections', 'c')
        ->fields('c', array('connection_id'))
        ->condition('type', $type)
        ->condition('url', $url)
        ->execute()
        ->fetchField();
    }
    else {
      $old_connection_id = $fixed_id;
    }

    // If there is a connection available with the url, load it.
    if ($old_connection_id !== FALSE) {
      switch ($type) {
        case 'pp_server':
          $connection = new SemanticConnectorPPServerConnection($old_connection_id);
          break;

        case 'sparql_endpoint':
          $connection = new SemanticConnectorSparqlEndpointConnection($old_connection_id);
          break;
      }

      // If there already is a connection available, change if data has changed.
      $has_changed = FALSE;
      if ($connection->getTitle() != $title) {
        $connection->setTitle($title);
        $has_changed = TRUE;
      }
      if ($connection->getUrl() != $url) {
        $connection->setUrl($url);
        $has_changed = TRUE;
      }
      if ($connection->getCredentials() != $credentials) {
        $connection->setCredentials($credentials);
        $has_changed = TRUE;
      }
      if (!empty($config) && $connection->getConfig() != $config) {
        $connection->setConfig(array_merge($connection->getConfig(), $config));
        $has_changed = TRUE;
      }

      // Save the connection if its data has changed.
      if ($has_changed) {
        $connection->save();
      }
    }
    // Data was not found in the DB --> Really create a new Connection.
    else {
      switch ($type) {
        case 'pp_server':
          $connection = new SemanticConnectorPPServerConnection();
          break;

        case 'sparql_endpoint':
          $connection = new SemanticConnectorSparqlEndpointConnection();
          break;
      }

      // Set all the required variables and save the connection.
      $connection->setTitle($title);
      $connection->setUrl($url);
      $connection->setCredentials($credentials);
      $connection->setConfig(array_merge($connection->getDefaultConfig(), $config));
      $connection->save();
    }

    return $connection;
  }

  /**
   * Delete one or multiple Semantic Connector connections.
   *
   * @param array $connection_ids
   *   A single connection_id or an array of connection_ids to remove.
   */
  public static function deleteConnections(array $connection_ids) {
    if (!empty($connection_ids)) {
      db_delete('semantic_connector_connections')
        ->condition('connection_id', $connection_ids)
        ->execute();
    }
  }
}

/**
 * An abstract class for all connections used by PoolParty Semantic Connector.
 */
abstract class SemanticConnectorConnection {
  protected $id;
  protected $type;
  protected $url;
  protected $credentials;
  protected $title;
  protected $config;

  /**
   * Get an already configured API of a connection.
   *
   * @param string $api_type
   *   Additional information on what API to get, if a connection supports more
   *   than one APIs.
   *
   * @return object
   *   An API-object, depending on the $api_type.
   */
  abstract public function getApi($api_type = '');

  /**
   * Get the default config of the Semantic Connector Connection.
   *
   * @return array
   *   The default config of the Semantic Connector Connection.
   */
  abstract public function getDefaultConfig();

  /**
   * Check if a connection is available.
   *
   * @return bool
   *   TRUE if the connection is available, FALSE if not.
   */
  public abstract function available();

  /**
   * The constructor of the SemanticConnectorConnection class.
   *
   * @param int $connection_id
   *   The ID of the Semantic Connector Connection.
   */
  public function __construct($connection_id = 0) {
    $this->id = $connection_id;

    // Set the default values.
    $this->credentials = array(
      'username' => '',
      'password' => '',
    );
    $this->config = $this->getDefaultConfig();
    $this->title = '';
    $this->url = '';

    if ($connection_id > 0) {
      $connection = db_select('semantic_connector_connections', 'c')
        ->fields('c')
        ->condition('connection_id', $connection_id)
        ->execute()
        ->fetchAssoc();

      if ($connection !== FALSE) {
        $this->type = $connection['type'];
        $this->url = $connection['url'];
        $this->credentials['username'] = $connection['username'];
        $this->credentials['password'] = $connection['password'];
        $this->title = $connection['title'];
        
        $connection_overrides = variable_get('semantic_connector_override_connections', array());
        if (isset($connection_overrides[$connection_id])) {
          $overrides = $connection_overrides[$connection_id];
          if (isset($overrides['url'])) {
            $this->url = $overrides['url'];
          }
          if (isset($overrides['username'])) {
            $this->credentials['username'] = $overrides['username'];
          }
          if (isset($overrides['password'])) {
            $this->credentials['password'] = $overrides['password'];
          }
          if (isset($overrides['title'])) {
            $this->title = $overrides['title'];
          }
        } 

        // Merge the config with the default values in case there was a new
        // property added.
        $this->config = array_merge($this->config, unserialize($connection['config']));
      }
      else {
        // todo: throw an error.
        $this->id = 0;
      }
    }
  }

  /**
   * Create / update the current Semantic Connector connection in the database.
   */
  public function save() {
    // Update an existing connection.
    if ($this->id > 0) {
      db_update('semantic_connector_connections')
        ->fields(array(
          'type' => $this->type,
          'url' => $this->url,
          'title' => $this->title,
          'username' => $this->credentials['username'],
          'password' => $this->credentials['password'],
          'config' => serialize($this->config),
        ))
        ->condition('connection_id', $this->id)
        ->execute();
    }
    // Create a new connection.
    else {
      if (!isset($this->url)) {
        // todo: throw an error.
        return;
      }

      $id = db_insert('semantic_connector_connections')
        ->fields(array(
          'type' => $this->type,
          'url' => $this->url,
          'title' => $this->title,
          'username' => $this->credentials['username'],
          'password' => $this->credentials['password'],
          'config' => serialize($this->config),
        ))
        ->execute();

      $this->id = $id;
    }
  }

  /**
   * Deletes the current connection to the PoolParty.
   */
  public function delete() {
    SemanticConnector::deleteConnections(array($this->id));
  }

  /**
   * Get the ID of the Semantic Connector Connection.
   *
   * @return int
   *   The ID of the Semantic Connector Connection.
   */
  public function getId() {
    return $this->id;
  }

  /**
   * Get the type of the Semantic Connector Connection.
   *
   * @return string
   *   The type of the Semantic Connector Connection.
   */
  public function getType() {
    return $this->type;
  }

  /**
   * Set the type of the Semantic Connector Connection.
   *
   * @param string $type
   *   The type of the Semantic Connector Connection.
   */
  public function setType($type) {
    $this->type = $type;
  }

  /**
   * Get the URL of the Semantic Connector Connection.
   *
   * @return string
   *   The URL of the Semantic Connector Connection.
   */
  public function getUrl() {
    return $this->url;
  }

  /**
   * Set the URL of the Semantic Connector Connection.
   *
   * @param string $url
   *   The URL of the Semantic Connector Connection.
   */
  public function setUrl($url) {
    // Remove trailing slashes.
    $this->url = rtrim($url,"/");
  }

  /**
   * Get the credentials of the Semantic Connector Connection.
   *
   * @return array
   *   An array with the credentials of the Semantic Connector Connection,
   *   including properties "username" and "password" .
   */
  public function getCredentials() {
    return $this->credentials;
  }

  /**
   * Set the credentials of the Semantic Connector Connection.
   *
   * @param array $credentials
   *   An array with the credentials of the Semantic Connector Connection,
   *   including properties "username" and "password" .
   */
  public function setCredentials(array $credentials) {
    if (!isset($credentials['username']) || !isset($credentials['password'])) {
      // todo: throw an error.
    }
    $this->credentials = $credentials;
  }

  /**
   * Get the title of the Semantic Connector Connection.
   *
   * @return string
   *   The title of the Semantic Connector Connection.
   */
  public function getTitle() {
    return $this->title;
  }

  /**
   * Set the title of the Semantic Connector Connection.
   *
   * @param string $title
   *   The title of the Semantic Connector Connection.
   */
  public function setTitle($title) {
    $this->title = $title;
  }

  /**
   * Get the config of the Semantic Connector Connection.
   *
   * @return array
   *   The config of the Semantic Connector Connection.
   */
  public function getConfig() {
    return $this->config;
  }

  /**
   * Set the config of the Semantic Connector Connection.
   *
   * @param array $config
   *   The config of the Semantic Connector Connection as an array.
   */
  public function setConfig(array $config) {
    $this->config = array_merge($this->getDefaultConfig(), $config);
  }
}

/**
 * A class for a connection to a PoolParty-server.
 */
class SemanticConnectorPPServerConnection extends SemanticConnectorConnection {

  /**
   * The constructor of the SemanticConnectorPPServerConnection class.
   *
   * @param int $connection_id
   *   The ID of the Semantic Connector Connection.
   */
  public function __construct($connection_id = 0) {
    $this->type = 'pp_server';
    parent::__construct($connection_id);

    // Incorrect connection was loaded --> Throw an error.
    if ($this->type != 'pp_server') {
      // todo: throw an error.
    }
  }

  /**
   * Check if a connection is available.
   *
   * @return bool
   *   TRUE if the connection is available, FALSE if not.
   */
  public function available() {
    $availability = $this->getApi('PPX')->available();
    return $availability['success'];
  }

  /**
   * Adds PoolParty projects and SPARQL endpoints before saving it.
   */
  public function save() {
    // Update the PoolParty version.
    $ppt_api = $this->getApi('PPT');
    $this->config['version'] = $ppt_api->getVersion();

    // Get all the projects from the PoolParty server connection.
    $projects = $ppt_api->getProjects();

    // Get the Sparql-Endpoints for each project.
    $sparql_endpoints = array();
    foreach ($projects as $project) {
      if (property_exists($project, 'sparql_endpoint_url')) {
        $sparql_endpoints[] = $project->sparql_endpoint_url;
      }
    }

    // Get all not existing Sparql-Endpoints for removing.
    $sparql_endpoints_to_remove = array();
    if (isset($this->config['projects'])) {
      foreach ($this->config['projects'] as $project) {
        if (property_exists($project, 'sparql_endpoint_url') && !in_array($project->sparql_endpoint_url, $sparql_endpoints)) {
          $sparql_endpoints_to_remove[] = $project->sparql_endpoint_url;
        }
      }
    }

    // Add the projects to the configuration.
    $this->config['projects'] = $projects;

    // Update the PoolParty GraphSearch configuration.
    $sonr_config = array();
    /** @var \SemanticConnectorSonrApi $sonr_api */
    $sonr_api = $this->getApi('sonr');
    // Get the version of the PoolParty GraphSearch web service.
    $sonr_version = $sonr_api->getVersion();

    // Get the appropriate API for the correct version.
    $this->config['sonr_configuration'] = array(
      'version' => $sonr_version,
    );
    $sonr_api = $this->getApi('sonr');

    // If a PoolParty GraphSearch server exists, create a config.
    if (!empty($sonr_version)) {
      // Get the server-side configuration and save it also to the database.
      $sonr_config = $sonr_api->getConfig();
      $sonr_config['version'] = $sonr_version;
    }
    $this->config['sonr_configuration'] = $sonr_config;

    // Save the configuration.
    parent::save();

    // Add a SPARQL-endpoint connection for every project.
    foreach ($this->config['projects'] as $project) {
      if (property_exists($project, 'sparql_endpoint_url')) {
        SemanticConnector::createConnection('sparql_endpoint', $project->sparql_endpoint_url, $project->title, $this->credentials, array('pp_server_id' => $this->id));
      }
    }

    // Remove SPARQL-endpoints, that do not exist anymore.
    if (!empty($sparql_endpoints_to_remove)) {
      $delete_connection_ids = db_select('semantic_connector_connections', 'c')
        ->fields('c', array('connection_id'))
        ->condition('type', 'sparql_endpoint')
        ->condition('url', $sparql_endpoints_to_remove)
        ->execute()
        ->fetchCol();

      SemanticConnector::deleteConnections($delete_connection_ids);
    }
  }

  /**
   * Deletes the current connection to the PoolParty with its Sparql-Endpoints.
   */
  public function delete() {
    // Delete the Sparql-Endpoint for each project.
    if (isset($this->config['projects'])) {
      $sparql_endpoints_to_remove = array();
      foreach ($this->config['projects'] as $project) {
        if (property_exists($project, 'sparql_endpoint_url')) {
          $sparql_endpoints_to_remove[] = $project->sparql_endpoint_url;
        }
      }

      if (!empty($sparql_endpoints_to_remove)) {
        $delete_connection_ids = db_select('semantic_connector_connections', 'c')
          ->fields('c', array('connection_id'))
          ->condition('type', 'sparql_endpoint')
          ->condition('url', $sparql_endpoints_to_remove)
          ->execute()
          ->fetchCol();
        SemanticConnector::deleteConnections($delete_connection_ids);
      }
    }

    parent::delete();
  }

  /**
   * Returns the default configuration.
   *
   * @return array
   *   An empty array.
   */
  public function getDefaultConfig() {
    return array();
  }

  /**
   * Returns the API to a specific type.
   *
   * @param string $api_type
   *   The desired API type. Possible values are:
   *   - "PPX": The PoolParty Extraction service API
   *   - "PPT": The PoolParty Thesaurus API
   *   - "sonr": The PoolParty GraphSearch server API
   *
   * @return null|SemanticConnectorPPTApi|SemanticConnectorPPXApi
   *   The specific API.
   */
  public function getApi($api_type = 'PPX') {
    if (in_array($api_type, array('PPX', 'PPT', 'sonr'))) {
      $api_version_info = $this->getVersionInfo($api_type);
      $credentials = !empty($this->credentials['username']) ? $this->credentials['username'] . ':' . $this->credentials['password'] : '';

      // PPX or PPT API.
      if ($api_type != 'sonr') {
        return new $api_version_info['api_class_name']($this->url, $credentials);
      }
      // PoolParty GraphSearch API.
      else {
        // Use an overridden GraphSearch path if available.
        $connection_overrides = variable_get('semantic_connector_override_connections', array());
        $custom_graphsearch_path = '';
        if (isset($connection_overrides[$this->id]) && isset($connection_overrides[$this->id]['graphsearch_path'])) {
          $custom_graphsearch_path = $connection_overrides[$this->id]['graphsearch_path'];
        }

        $sonr_api = new $api_version_info['api_class_name']($this->url, $credentials, $custom_graphsearch_path);
        $sonr_api->setId($this->id);
        return $sonr_api;
      }
    }
    else {
      return NULL;
    }
  }

  /**
   * Get all information about the version of a API available on the PP server.
   *
   * @param string $api_type
   *   The desired API type. Possible values are:
   *   - "PPX": The PoolParty Extraction service API
   *   - "PPT": The PoolParty Thesaurus API
   *   - "sonr": The PoolParty GraphSearch server API
   *
   * @return array
   *   An associative array containing following keys:
   *   - "installed_version": The current version of the API service
   *   - "latest_version": The latest API implementation for the service
   *   - "api_class_name": The class name of the appropriate API class to use
   */
  public function getVersionInfo($api_type) {
    // List of finished API version implementations. Only add versions to this
    // list when they are fully functional. The order of the versions is not
    // important.
    $available_api_versions = array(
      'pp_server' => array('4.6', '5.3', '5.6', '6.0', '6.2', '7.0'),
      'sonr' => array('4.6', '5.3', '5.6', '5.7', '6.0', '6.1', '7.0'),
    );

    $version_infos = array(
      'installed_version' => '',
      'latest_version' => '',
      'api_class_name' => '',
    );

    // PPX or PPT API.
    if ($api_type != 'sonr') {
      $api_versions = $available_api_versions['pp_server'];
      usort($api_versions, 'version_compare');
      $class_prefix = 'SemanticConnector' . $api_type . 'Api_';
      if (!isset($this->config['version']) || empty($this->config['version'])) {
        // Check with the lowest API version, which supports getVersion for all
        // API versions.
        $version_check_class_name = 'SemanticConnectorPPTApi_' . str_replace('.', '_', $api_versions[0]);
        $credentials = !empty($this->credentials['username']) ? $this->credentials['username'] . ':' . $this->credentials['password'] : '';

        /** @var \SemanticConnectorPPTApi $ppt_api */
        $ppt_api = new $version_check_class_name($this->url, $credentials);
        $this->config['version'] = $ppt_api->getVersion();
      }
      $version_infos['installed_version'] = $this->config['version'];
    }
    // PoolParty GraphSearch API.
    else {
      $api_versions = $available_api_versions['sonr'];
      usort($api_versions, 'version_compare');
      $class_prefix = 'SemanticConnectorSonrApi_';
      if (!isset($this->config['sonr_configuration']) || !isset($this->config['sonr_configuration']['version']) || empty($this->config['sonr_configuration']['version'])) {
        // Check with the lowest API version, which supports getVersion for all
        // API versions.
        $version_check_class_name = $class_prefix . str_replace('.', '_', $api_versions[0]);
        $credentials = !empty($this->credentials['username']) ? $this->credentials['username'] . ':' . $this->credentials['password'] : '';

        $custom_graphsearch_path = $this->getGraphSearchPath();
        /** @var \SemanticConnectorSonrApi $sonr_api */
        $sonr_api = new $version_check_class_name($this->url, $credentials, ($custom_graphsearch_path != 'sonr-backend' ? $custom_graphsearch_path : ''));
        $this->config['sonr_configuration']['version'] = $sonr_api->getVersion();
      }
      $version_infos['installed_version'] = $this->config['sonr_configuration']['version'];
    }

    // To get the newest compatible API version, we have to reverse the array
    // and check every single version.
    $api_versions = array_reverse($api_versions);
    $version_infos['latest_version'] = $api_versions[0];
    foreach ($api_versions as $current_api_version) {
      if (version_compare($version_infos['installed_version'], $current_api_version, '>=')) {
        $class_version = $current_api_version;
        break;
      }
    }
    if (!isset($class_version)) {
      $class_version = $api_versions[count($api_versions) - 1];
    }
    $version_infos['api_class_name'] = $class_prefix . str_replace('.', '_', $class_version);

    return $version_infos;
  }

  /**
   * Get the path to the GraphSearch connected to the PP server.
   *
   * @return string
   *   The path to the GraphSearch instance
   */
  public function getGraphSearchPath() {
    $graphsearch_path = '';
    if (isset($this->config['sonr_configuration']) && isset($this->config['sonr_configuration']['version']) && !empty($this->config['sonr_configuration']['version'])) {
      $graphsearch_path = 'sonr-backend';
      if (version_compare($this->config['sonr_configuration']['version'], '5.6.0', '>=')) {
        $graphsearch_path = 'GraphSearch';
        // Use an overridden GraphSearch path if available.
        $connection_overrides = variable_get('semantic_connector_override_connections', array());
        if (isset($connection_overrides[$this->id]) && isset($connection_overrides[$this->id]['graphsearch_path'])) {
          $graphsearch_path = $connection_overrides[$this->id]['graphsearch_path'];
        }
      }
    }

    return $graphsearch_path;
  }

  /**
   * Get the sparql endpoints for this PP Server connection.
   *
   * @param string $project_id
   *   Optional; If given only SPARQL endpoints for that project will be returned.
   *
   * @return array
   *   An array of connection IDs of SPARQL endpoint connections.
   */
  public function getSparqlEndpoints($project_id = '') {
    $sparql_endpoints = [];

    $server_config = $this->getConfig();
    $sparql_endpoint_urls = [];
    if (isset($server_config['projects']) && !empty($server_config['projects'])) {
      foreach ($server_config['projects'] as $project) {
        if ((empty($project_id) || $project->id === $project_id) && isset($project->sparql_endpoint_url)) {
          $sparql_endpoint_urls[] = $project->sparql_endpoint_url;
        }
      }
    }

    if (!empty($sparql_endpoint_urls)) {
      $sparql_endpoints = SemanticConnector::searchConnections(
        [
          'type' => 'sparql_endpoint',
          'url' => $sparql_endpoint_urls
        ]
      );
    }

    return $sparql_endpoints;
  }
}

/**
 * A class for a connection to a SPARQL-endpoint.
 */
class SemanticConnectorSparqlEndpointConnection extends SemanticConnectorConnection {

  /**
   * The constructor of the SemanticConnectorSparqlEndpointConnection class.
   *
   * @param int $connection_id
   *   The ID of the Semantic Connector Connection.
   */
  public function __construct($connection_id = 0) {
    $this->type = 'sparql_endpoint';
    parent::__construct($connection_id);

    // Incorrect connection was loaded --> Throw an error.
    if ($this->type != 'sparql_endpoint') {
      // todo: throw an error.
    }
  }

  /**
   * Check if a connection is available.
   *
   * @return bool
   *   TRUE if the connection is available, FALSE if not.
   */
  public function available() {
    $sparql_client = $this->getApi();
    $query = "
      SELECT *
      WHERE {
        ?s ?p ?o.
      }
      LIMIT 1";

    try {
      $row = $sparql_client->query($query);
    }
    catch (Exception $e) {
      return FALSE;
    }

    return ($row->numRows() == 0) ? FALSE : TRUE;
  }

  /**
   * Returns the default configuration.
   *
   * @return array
   *   The default configuration.
   */
  public function getDefaultConfig() {
    return array(
      'pp_server_id' => 0,
    );
  }

  /**
   * Returns the EasyRdf SPARQL client.
   *
   * @param string $api_type
   *   This parameter is not used.
   *
   * @return EasyRdf_Sparql_Client
   *   The EasyRdf SPARQL client.
   */
  public function getApi($api_type = '') {
    if (!class_exists('EasyRdf_Sparql_Client')) {
      $easyrdf_path = 'sites/all/libraries/easyrdf';
      if (module_exists('libraries')) {
        $easyrdf_path = libraries_get_path('easyrdf');
      }

      // Include the EasyRDF library.
      require_once $easyrdf_path . '/EasyRdf.php';

      if (!empty($this->credentials['username'])) {
        $http_client = EasyRdf_Http::getDefaultHttpClient();

        // Use basic authentication, Digest is not supported by the way EasyRDF
        // currently works.
        $http_client->setHeaders('Authorization', 'Basic ' . base64_encode($this->credentials['username'] . ':' . $this->credentials['password']));
        EasyRdf_Http::setDefaultHttpClient($http_client);
      }

    }

    return new SemanticConnectorSparqlApi($this->url);
  }
}
