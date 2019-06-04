<?php
/**
 * @file
 * The PoolParty Extractor (PPX) API class. PoolParty >= 4.6 is supported.
 */

/**
 * Abstract Class SemanticConnectorPPXApi
 *
 * API Class for the PoolParty Extractor.
 */
abstract class SemanticConnectorPPXApi {

  protected $connection;
  protected $apiVersion;

  /**
   * The constructor of the PoolParty Extractor class.
   *
   * @param string $endpoint
   *   URL of the endpoint of the PoolParty-server.
   * @param string $credentials
   *   Username and password if required (format: "username:password").
   */
  public function __construct($endpoint, $credentials = '') {
    $this->connection = new SemanticConnectorCurlConnection($endpoint, $credentials);
    $this->apiVersion = str_replace(array('SemanticConnectorPPXApi_', '_'), array('', '.'), get_class($this));
  }

  /**
   * Get the configured cURL-connection.
   *
   * @return SemanticConnectorCurlConnection
   *   The connection object.
   */
  public function getConnection() {
    return $this->connection;
  }

  /**
   * Get the configured used API version.
   *
   * @return string
   *   The API version.
   */
  public function getApiVersion() {
    return $this->apiVersion;
  }

  /**
   * Get the path to the PPX API.
   *
   * @return string
   *   The path to the PPX API.
   */
  public function getApiPath() {
    // Use API versioning for version 6.1+.
    return '/extractor/' . (version_compare($this->apiVersion, '6.1', '>=') ? $this->apiVersion . '/' : '') . 'api/';
  }

  /**
   * This method checks if the PoolParty server exists and is running.
   *
   * @return array
   *   Associative array which following properties:
   *   - success (boolean): TRUE if a connection to the server can be
   *     established.
   *   - message (string): This property is optional, but if it exists it
   *     includes more details about why the connection could not be
   *     established.
   */
  public function available() {
    return array('success' => FALSE);
  }

  /**
   * Get a list of available projects of a PoolParty server.
   *
   * @return array
   *   An array of projects found on the PoolParty available for the current
   *   PoolParty user.
   */
  public function getProjects() {
    return array();
  }

  /**
   * Extract concepts from given data.
   *
   * @param mixed $data
   *   Can be either a string for normal text-extraction of a file-object for
   *   text extraction of the file content.
   * @param string $language
   *   The iso-code of the text's language.
   * @param array $parameters
   *   Additional parameters to forward to the API (e.g., projectId).
   * @param string $data_type
   *   The type of the data. Can be one of the following values:
   *   - "text" for a text
   *   - "url" for a valid URL
   *   - "file" for a file object with a file ID
   *   - "file direct" for all other files without an ID
   * @param boolean $categorize
   *   TRUE if categories should also be returned, FALSE if not.
   *
   * @return object
   *   Object of concepts.
   */
  public function extractConcepts($data, $language, array $parameters = array(), $data_type = '', $categorize = FALSE) {
    return NULL;
  }

  /**
   * Get a list of of concepts / free terms matching a string.
   *
   * @param string $string
   *   The string to search matching concepts / freeterms for.
   * @param string $language
   *   The iso-code of the text's language.
   * @param string $project_id
   *   The ID of the PoolParty project to use.
   * @param array $parameters
   *   Additional parameters to forward to the API (e.g. projectId).
   *
   * @return array
   *   An associative array of concepts and freeterms.
   */
  public function suggest($string, $language, $project_id, array $parameters = array()) {
    return array();
  }

  /**
   * Extract categories from given data.
   *
   * @param mixed $data
   *   Can be either a string for normal text-extraction of a file-object for
   *   text extraction of the file content.
   * @param string $language
   *   The iso-code of the language of the data.
   * @param array $parameters
   *   Additional parameters to forward to the API (e.g., projectId).
   * @param string $data_type
   *   The type of the data. Can be one of the following values:
   *   - "text" for a text
   *   - "url" for a valid URL
   *   - "file" for a file object with a file ID
   *   - "file direct" for all other files without an ID
   *
   * @return object
   *   Object of categories.
   */
  public function extractCategories($data, $language, array $parameters = array(), $data_type = '') {
    return NULL;
  }

  /**
   * Get the type of $data.
   *
   * @param mixed $data
   *   The data.
   *
   * @return string
   *   The type of the data. Can be one of the following values:
   *   - "text" for a text
   *   - "url" for a valid URL
   *   - "file" for a file object with a file ID
   *   - "file direct" for all other files without an ID
   *   - empty if no type was identified
   */
  protected function getTypeOfData($data) {
    $data_type = '';

    if (is_string($data) && valid_url($data)) {
      $data_type = 'url';
    }
    elseif (is_string($data)) {
      $data_type = 'text';
    }
    elseif (is_object($data) && property_exists($data, 'fid')) {
      $data_type = 'file';
    }
    elseif (is_object($data) && property_exists($data, 'file_path')) {
      $data_type = 'file direct';
    }

    return $data_type;
  }

  /**
   * Extract categories from given data.
   *
   * @param mixed $data
   *   Can be either a string for normal text-extraction of a file-object for
   *   text extraction of the file content.
   * @param string $language
   *   The iso-code of the language of the data.
   * @param array $types
   *   The types of entities to extract.
   * @param array $parameters
   *   Additional parameters to forward to the API.
   * @param string $data_type
   *   The type of the data. Can be one of the following values:
   *   - "text" for a text
   *   - "url" for a valid URL
   *   - "file" for a file object with a file ID
   *   - "file direct" for all other files without an ID
   *
   * @return array
   *   An array of named entity objects.
   */
  public function extractNamedEntities($data, $language, array $types, $parameters = [], $data_type = '') {
    return [];
  }
}

/**
 * Class SemanticConnectorPPXApi_4_6
 *
 * API Class for the version 4.6.
 */
class SemanticConnectorPPXApi_4_6 extends SemanticConnectorPPXApi {

  /**
   * This method checks if the PoolParty server exists and is running.
   *
   * @return array
   *   Associative array which following properties:
   *   - success (boolean): TRUE if a connection to the server can be
   *     established.
   *   - message (string): This property is optional, but if it exists it
   *     includes more details about why the connection could not be
   *     established.
   */
  public function available() {
    $is_available = NULL;
    drupal_alter('semantic_connector_ppx_available', $this, $is_available);

    if (is_null($is_available)) {
      $is_available = array(
        'success' => FALSE,
        'message' => '',
      );
      $resource_path = $this->getApiPath() . 'heartbeat';
      $result = json_decode($this->connection->get($resource_path));

      if (is_object($result) && property_exists($result, 'success')) {
        $is_available['success'] = $result->success;
        if (property_exists($result, 'message')) {
          $is_available['message'] = $result->message;
        }
      }
    }

    return $is_available;
  }

  /**
   * Get a list of available projects of a PoolParty server.
   *
   * @return array
   *   An array of projects found on the PoolParty available for the current
   *   PoolParty user.
   */
  public function getProjects() {
    // Offer the possibility to support a different value for this function.
    $projects = NULL;
    drupal_alter('semantic_connector_ppx_getProjects', $this, $projects);

    if (is_null($projects)) {
      $resource_path = $this->getApiPath() . 'projects';
      $result = $this->connection->get($resource_path);

      $projects = json_decode($result);
      if (is_object($projects) && property_exists($projects, 'projects')) {
        $projects = $projects->projects;
      }
    }

    $default_project = array(
      'label' => 'Default project',
      'uuid' => '',
      'defaultLanguage' => 'en',
      'languages' => array('en'),
    );

    if (is_array($projects)) {
      foreach ($projects as &$project) {
        $project = (object) (array_merge($default_project, (array) $project));
      }
    }
    else {
      $projects = array();
    }

    return $projects;
  }

  /**
   * Extract concepts from given data.
   *
   * @param mixed $data
   *   Can be either a string for normal text-extraction of a file-object for
   *   text extraction of the file content.
   * @param string $language
   *   The iso-code of the language of the data.
   * @param array $parameters
   *   Additional parameters to forward to the API (e.g., projectId).
   * @param string $data_type
   *   The type of the data. Can be one of the following values:
   *   - "text" for a text
   *   - "url" for a valid URL
   *   - "file" for a file object with a file ID
   *   - "file direct" for all other files without an ID
   * @param boolean $categorize
   *   TRUE if categories should also be returned, FALSE if not.
   *
   * @return object
   *   Object of concepts.
   */
  public function extractConcepts($data, $language, array $parameters = array(), $data_type = '', $categorize = FALSE) {
    // Offer the possibility to support a different value for this function.
    $concepts = NULL;

    $input = array(
      'data' => $data,
      'language' => $language,
      'parameters' => $parameters,
      'data type' => $data_type,
      'categorize' => $categorize,
    );
    drupal_alter('semantic_connector_ppx_extractConcepts', $this, $concepts, $input);

    $result = NULL;
    if (is_null($concepts)) {
      $resource_path = $this->getApiPath() . 'extract';
      if (empty($data_type)) {
        $data_type = $this->getTypeOfData($data);
      }

      // Add categorization if required.
      if ($categorize) {
        $parameters['categorize'] = TRUE;
        $parameters['disambiguate'] = TRUE;
      }

      switch ($data_type) {
        // Extract concepts from a given text.
        case 'text':
          $post_parameters = array_merge(array(
            'text' => $data,
            'language' => $language,
          ), $parameters);
          $result = $this->connection->post($resource_path, array(
            'data' => $post_parameters,
          ));
          break;

        // Extract concepts from a given URL.
        case 'url':
          $post_parameters = array_merge(array(
            'url' => $data,
            'language' => $language,
          ), $parameters);
          $result = $this->connection->post($resource_path, array(
            'data' => $post_parameters
          ));
          break;

        // Extract concepts from a given file uploaded via file field.
        case 'file':
          // Check if the file is in the public folder
          // and the PoolParty GraphSearch server can read it.
          if ($wrapper = file_stream_wrapper_get_instance_by_uri('public://')) {
            $public_path = $wrapper->realpath();
            $file_path = drupal_realpath($data->uri);
            if (strpos($file_path, $public_path) !== FALSE) {
              $post_parameters = array_merge(array(
                'file' => '@' . $file_path,
                'language' => $language,
              ), $parameters);
              $result = $this->connection->post($resource_path, array(
                'data' => $post_parameters,
                'headers' => array('Content-Type' => 'multipart/form-data'),
              ));
            }
          }
          break;

        // Extract concepts from a given file
        case 'file direct':
          $post_parameters = array_merge(array(
            'file' => '@' . $data->file_path,
            'language' => $language,
          ), $parameters);
          $result = $this->connection->post($resource_path, array(
            'data' => $post_parameters,
            'headers' => array('Content-Type' => 'multipart/form-data'),
          ));
          break;

        default:
          SemanticConnectorWatchdog::message('PPX API', 'The type of the data to extract concepts is not supported.');
          break;
      }

      $concepts = json_decode($result);
    }

    // Files have additional information we don't need --> remove it.
    if (is_object($concepts) && property_exists($concepts, 'document')) {
      $concepts = $concepts->document;
    }
    if (is_object($concepts) && property_exists($concepts, 'text')) {
      $concepts = $concepts->text;
    }

    return $concepts;
  }

  /**
   * Get a list of of concepts / free terms matching a string.
   *
   * @param string $string
   *   The string to search matching concepts / freeterms for.
   * @param string $language
   *   The iso-code of the text's language.
   * @param string $project_id
   *   The ID of the PoolParty project to use.
   * @param array $parameters
   *   Additional parameters to forward to the API (e.g., projectId).
   *
   * @return array
   *   An array of objects (every object can be an object or a freeterm,
   *   detectable by the tid-property).
   */
  public function suggest($string, $language, $project_id, array $parameters = array()) {
    $suggestion = NULL;

    $input = array(
      'string' => $string,
      'language' => $language,
      'project_id' => $project_id,
      'parameters' => $parameters,
    );
    // Offer the possibility to support a different value for this function.
    drupal_alter('semantic_connector_ppx_suggest', $this, $suggestion, $input);

    if (is_null($suggestion)) {
      $resource_path = $this->getApiPath() . 'suggest';
      $post_parameters = array_merge(array(
        'searchString' => $string,
        'language' => $language,
        'projectId' => $project_id,
      ), $parameters);

      $result = $this->connection->post($resource_path, array(
        'data' => $post_parameters,
      ));

      $suggestion = json_decode($result);
    }

    if (is_object($suggestion) && property_exists($suggestion, 'suggestedConcepts') && is_array($suggestion->suggestedConcepts)) {
      return $suggestion->suggestedConcepts;
    }

    return array();
  }
}

/**
 * Class SemanticConnectorPPXApi_5_3
 *
 * API Class for the version 5.3
 */
class SemanticConnectorPPXApi_5_3 extends SemanticConnectorPPXApi_4_6 {

}

/**
 * Class SemanticConnectorPPXApi_5_6
 *
 * API Class for the version 5.6
 */
class SemanticConnectorPPXApi_5_6 extends SemanticConnectorPPXApi_5_3 {
  /**
   * Extract categories from given data.
   *
   * @param mixed $data
   *   Can be either a string for normal text-extraction of a file-object for
   *   text extraction of the file content.
   * @param string $language
   *   The iso-code of the language of the data.
   * @param array $parameters
   *   Additional parameters to forward to the API (e.g., projectId).
   * @param string $data_type
   *   The type of the data. Can be one of the following values:
   *   - "text" for a text
   *   - "url" for a valid URL
   *   - "file" for a file object with a file ID
   *   - "file direct" for all other files without an ID
   *
   * @return object
   *   Object of categories.
   */
  public function extractCategories($data, $language, array $parameters = array(), $data_type = '') {
    // Offer the possibility to support a different value for this function.
    $categories = NULL;

    $input = array(
      'data' => $data,
      'language' => $language,
      'parameters' => $parameters,
      'data type' => $data_type,
    );
    drupal_alter('semantic_connector_ppx_extractCategories', $this, $categories, $input);

    $result = NULL;
    if (is_null($categories)) {
      $resource_path = $this->getApiPath() . 'categorization';
      if (empty($data_type)) {
        $data_type = $this->getTypeOfData($data);
      }
      $parameters['disambiguation'] = TRUE;

      switch ($data_type) {
        // Extract categories from a given text.
        case 'text':
          $post_parameters = array_merge(array(
            'text' => $data,
            'language' => $language,
          ), $parameters);
          $result = $this->connection->post($resource_path, array(
            'data' => $post_parameters,
          ));
          break;

        // Extract categories from a given URL.
        case 'url':
          $post_parameters = array_merge(array(
            'url' => $data,
            'language' => $language,
          ), $parameters);
          $result = $this->connection->post($resource_path, array(
            'data' => $post_parameters
          ));
          break;

        // Extract categories from a given file uploaded via file field.
        case 'file':
          // Check if the file is in the public folder
          // and the PoolParty GraphSearch server can read it.
          if ($wrapper = file_stream_wrapper_get_instance_by_uri('public://')) {
            $public_path = $wrapper->realpath();
            $file_path = drupal_realpath($data->uri);
            if (strpos($file_path, $public_path) !== FALSE) {
              $post_parameters = array_merge(array(
                'file' => '@' . $file_path,
                'language' => $language,
              ), $parameters);
              $result = $this->connection->post($resource_path, array(
                'data' => $post_parameters,
                'headers' => array('Content-Type' => 'multipart/form-data'),
              ));
            }
          }
          break;

        // Extract categories from a given file
        case 'file direct':
          $post_parameters = array_merge(array(
            'file' => '@' . $data->file_path,
            'language' => $language,
          ), $parameters);
          $result = $this->connection->post($resource_path, array(
            'data' => $post_parameters,
            'headers' => array('Content-Type' => 'multipart/form-data'),
          ));
          break;

        default:
          SemanticConnectorWatchdog::message('PPX API', 'The type of the data to extract categories is not supported.');
          break;
      }

      $categories = json_decode($result);
    }

    // Files have additional information we don't need --> remove it.
    if (is_object($categories) && property_exists($categories, 'title')) {
      $categories = $categories->document;
    }
    if (is_object($categories) && property_exists($categories, 'text')) {
      $categories = $categories->text;
    }

    return $categories;
  }
}

/**
 * Class SemanticConnectorPPXApi_6_0
 *
 * API Class for the version 6.0
 */
class SemanticConnectorPPXApi_6_0 extends SemanticConnectorPPXApi_5_6 {

  /**
   * {@inheritdoc}
   */
  public function extractConcepts($data, $language, array $parameters = array(), $data_type = '', $categorize = FALSE) {
    $concepts = parent::extractConcepts($data, $language, $parameters, $data_type, $categorize);

    // Rename the 'extractedTerms' property to the old 'freeTerms' property.
    if (is_object($concepts) && property_exists($concepts, 'extractedTerms')) {
      $concepts->freeTerms = $concepts->extractedTerms;
      unset($concepts->extractedTerms);
    }

    return $concepts;
  }
}

/**
 * Class SemanticConnectorPPXApi_6_2
 *
 * API Class for the version 6.2. 6.2 is the first version that supports PPX API
 * versioning, that's why the class is required even though there was no API
 * change interesting for the Drupal integration.
 */
class SemanticConnectorPPXApi_6_2 extends SemanticConnectorPPXApi_6_0 {
}

/**
 * Class SemanticConnectorPPXApi_7_0
 *
 * API Class for the version 7.0.
 */
class SemanticConnectorPPXApi_7_0 extends SemanticConnectorPPXApi_6_2 {
  /**
   * Extract categories from given data.
   *
   * @param mixed $data
   *   Can be either a string for normal text-extraction of a file-object for
   *   text extraction of the file content.
   * @param string $language
   *   The iso-code of the language of the data.
   * @param array $types
   *   The types of entities to extract.
   * @param array $parameters
   *   Additional parameters to forward to the API.
   * @param string $data_type
   *   The type of the data. Can be one of the following values:
   *   - "text" for a text
   *   - "url" for a valid URL
   *   - "file" for a file object with a file ID
   *   - "file direct" for all other files without an ID
   *
   * @return array
   *   An array of named entity objects.
   */
  public function extractNamedEntities($data, $language, array $types, $parameters = [], $data_type = '') {
    $resource_path = $this->getApiPath() . 'extract';

    $post_parameters = array_merge(array(
      'language' => $language,
      'numberOfConcepts' => 0,
      'numberOfTerms' => 0,
    ), $parameters);

    // Add the NER parameters.
    for ($typecount = 0; $typecount < count($types); $typecount++) {
      $post_parameters['nerParameters[' . $typecount . '].type'] = $types[$typecount];
      $post_parameters['nerParameters[' . $typecount . '].method'] = 'MAXIMUM_ENTROPY';
    }

    if (empty($data_type)) {
      $data_type = $this->getTypeOfData($data);
    }

    $result = NULL;
    switch ($data_type) {
      // Extract concepts from a given text.
      case 'text':
        $post_parameters = array_merge(array(
          'text' => $data,
        ), $post_parameters);
        $result = $this->connection->post($resource_path, array(
          'data' => $post_parameters,
        ));
        break;

      // Extract concepts from a given URL.
      case 'url':
        $post_parameters = array_merge(array(
          'url' => $data,
        ), $post_parameters);
        $result = $this->connection->post($resource_path, array(
          'data' => $post_parameters
        ));
        break;

      // Extract concepts from a given file uploaded via file field.
      case 'file':
        // Check if the file is in the public folder
        // and the PoolParty GraphSearch server can read it.
        if ($wrapper = file_stream_wrapper_get_instance_by_uri('public://')) {
          $public_path = $wrapper->realpath();
          $file_path = drupal_realpath($data->uri);
          if (strpos($file_path, $public_path) !== FALSE) {
            $post_parameters = array_merge(array(
              'file' => '@' . $file_path,
            ), $post_parameters);
            $result = $this->connection->post($resource_path, array(
              'data' => $post_parameters,
              'headers' => array('Content-Type' => 'multipart/form-data'),
            ));
          }
        }
        break;

      // Extract concepts from a given file
      case 'file direct':
        $post_parameters = array_merge(array(
          'file' => '@' . $data->file_path,
        ), $post_parameters);
        $result = $this->connection->post($resource_path, array(
          'data' => $post_parameters,
          'headers' => array('Content-Type' => 'multipart/form-data'),
        ));
        break;

      default:
        SemanticConnectorWatchdog::message('PPX API', 'The type of the data to extract named entities is not supported.');
        break;
    }

    $entities = json_decode($result, TRUE);

    if (!empty($entities) && isset($entities['namedEntities'])) {
      return $entities['namedEntities'];
    }
    return [];
  }
}

/**
 * Class SemanticConnectorPPXApi_7_1
 *
 * API Class for the version 7.1.
 */
class SemanticConnectorPPXApi_7_1 extends SemanticConnectorPPXApi_7_0 {
  /**
   * Get a list of of concepts / free terms matching a string.
   *
   * @param string $string
   *   The string to search matching concepts / freeterms for.
   * @param string $language
   *   The iso-code of the text's language.
   * @param string $project_id
   *   The ID of the PoolParty project to use.
   * @param array $parameters
   *   Additional parameters to forward to the API (e.g., projectId).
   *
   * @return array
   *   An array of objects (every object can be an object or a freeterm,
   *   detectable by the tid-property).
   */
  public function suggest($string, $language, $project_id, array $parameters = array()) {
    $suggestion = parent::suggest($string, $language, $project_id, $parameters);
    // prefLabel was replaced by prefLabels including all languages.
    foreach ($suggestion as &$suggest_item) {
      if (isset($suggest_item->prefLabels) && property_exists($suggest_item->prefLabels, $language)) {
        $suggest_item->prefLabel = $suggest_item->prefLabels->{$language};
      }
    }

    return $suggestion;
  }

  /**
   * {@inheritdoc}
   */
  public function extractConcepts($data, $language, array $parameters = array(), $data_type = '', $categorize = FALSE) {
    $concepts = parent::extractConcepts($data, $language, $parameters, $data_type, $categorize);

    // prefLabel was replaced by prefLabels including all languages.
    if (is_object($concepts) && property_exists($concepts, 'concepts')) {
      foreach ($concepts->concepts as &$concept) {
        if (property_exists($concept, 'prefLabels') && property_exists($concept->prefLabels, $language)) {
          $concept->prefLabel = $concept->prefLabels->{$language};
        }
      }
    }

    return $concepts;
  }
}
