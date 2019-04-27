<?php
/**
 * @file
 * The PoolParty GraphSearch API class; Server version >= 4.6 is supported.
 */

/**
 * Abstract Class SemanticConnectorSonrApi
 *
 * API Class for the PoolParty GraphSearch server.
 */
abstract class SemanticConnectorSonrApi {

  const ATTR_SOURCE = 'dyn_lit_source';
  const ATTR_AUTHOR = 'dyn_lit_author';
  const ATTR_SENTIMENT = 'dyn_flt_sentiment';
  const ATTR_ALL_CONCEPTS = 'dyn_uri_all_concepts';
  const ATTR_CONTENT_TYPE = 'dyn_lit_content_type';
  const ATTR_CONTENT = 'dyn_txt_content';
  const ATTR_REGIONS = 'dyn_uri_Regions';
  const ATTR_SPACE = 'dyn_lit_space';

  protected $id;
  protected $customAttributes;
  protected $connection;
  protected $graphSearchPath = 'GraphSearch';
  protected $apiVersion;

  /**
   * The constructor of the SemanticConnectorSonrApi-class.
   *
   * @param string $endpoint
   *   URL of the endpoint of the PoolParty-server.
   * @param string $credentials
   *   Username and password if required (format: "username:password").
   * @param string $custom_graphsearch_path
   *   The customizable path to the GraphSearch instance
   */
  public function __construct($endpoint, $credentials = '', $custom_graphsearch_path = '') {
    $this->connection = new SemanticConnectorCurlConnection($endpoint, $credentials);
    $this->customAttributes = [
      self::ATTR_SOURCE,
      self::ATTR_AUTHOR,
    ];
    if (!empty($custom_graphsearch_path)) {
      $this->graphSearchPath = $custom_graphsearch_path;
    }
    $this->apiVersion = str_replace(array('SemanticConnectorSonrApi_', '_'), array('', '.'), get_class($this));
  }

  /**
   * Returns the cUrl connection object.
   *
   * @return SemanticConnectorCurlConnection
   *   The cUrl connection object.
   */
  public function getConnection() {
    return $this->connection;
  }

  /**
   * Sets the PoolParty GraphSearch configuration id.
   *
   * @param int $id
   *   The id of the PoolParty GraphSearch configuration.
   */
  public function setId($id) {
    $this->id = $id;
  }

  /**
   * Returns the ID.
   *
   * @return int
   *   The id of the PoolParty GraphSearch configuration.
   */
  public function getId() {
    return $this->id;
  }

  /**
   * Adds a specific attribute to the search property "customAttributes".
   *
   * @param string $attribute
   *   The name of the attribute
   */
  public function addCustomAttribute($attribute) {
    if (!in_array($attribute, $this->customAttributes)) {
      $this->customAttributes[] = $attribute;
    }
  }

  /**
   * Sets/replaces all the attributes for the search propery "customAttributes".
   *
   * @param array $attributes
   *   The array with the specific attributes
   */
  public function setCustomAttributes($attributes) {
    $this->customAttributes = $attributes;
  }

  /**
   * Returns all the attributes from the search property "customAttributes".
   *
   * @return array
   *   An array of custom attributes.
   */
  public function getCustomAttributes() {
    return $this->customAttributes;
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
   * Get the path to the GraphSearch API.
   *
   * @return string
   *   The path to the GraphSearch API.
   */
  public function getApiPath() {
    // GraphSearch
    if (version_compare($this->apiVersion, '5.6', '>=')) {
      // Use API versioning for version 6.1+.
      return '/' . $this->graphSearchPath . '/' . (version_compare($this->apiVersion, '6.1', '>=') ? $this->apiVersion . '/' : '') . 'api/';
    }
    // Old sOnr-support.
    else {
      return '/sonr-backend/api/';
    }
  }

  /**
   * This method checks if the GraphSearch service exists and is running.
   *
   * @return bool
   *   TRUE if the service is available, FALSE if not
   */
  public function available() {
    return FALSE;
  }

  /**
   * Get the version of the installed PoolParty GraphSearch service.
   *
   * Important: This method supports GraphSearch as well as GraphSearch version
   * and therefore must NOT be overwritten by one of the sub-classes!
   *
   * @return string
   *   The GraphSearch version formatted like '4.1.6'
   */
  public function getVersion() {
    $version = '';

    // Disable error logging.
    $this->connection->setErrorLogging(FALSE);

    // Try to find the version of GraphSearch inside the HTML code of the website.
    $resource_path = '/sonr-backend/';
    $sonr_website_html = $this->connection->get($resource_path, [
      'headers' => ['Accept' => 'text/html'],
    ]);

    // PoolParty GraphSearch was found, check the HTML code for its version.
    if ($sonr_website_html !== FALSE) {
      $version_search_pattern = '/<b>Version:<\/b> +([0-9.]+)/';
      if (preg_match($version_search_pattern, $sonr_website_html, $matches)) {
        $version = $matches[1];
      }
    }
    // If no traditional GraphSearch connection could be established, try checking for
    // GraphSearch instead.
    else {
      // Remove error messages from the failed GET request.
      drupal_get_messages('error');

      $resource_path = '/' . $this->graphSearchPath . '/admin';
      $sonr_website_html = $this->connection->get($resource_path, [
        'headers' => ['Accept' => 'text/html'],
      ]);
      $version_search_pattern = '/<a id="ppgs-brand" class="navbar-brand" href="\/' . $this->graphSearchPath . '\/">GraphSearch +([0-9.]+)/';
      if (preg_match($version_search_pattern, $sonr_website_html, $matches)) {
        $version = $matches[1];
      }
    }

    // Enable error logging again.
    $this->connection->setErrorLogging(TRUE);

    return $version;
  }

  /**
   * This method gets the server configuration of the PoolParty GraphSearch
   * server.
   *
   * @return array|boolean
   *   pp-server => The URL to the PoolParty server used for the extraction
   *   project => The PoolParty project used for the extraction
   *   language => The configured language of the content.
   */
  public function getConfig() {
    return FALSE;
  }

  /**
   * This method gets the field configuration of the PoolParty GraphSearch
   * server.
   *
   * @param string $search_space_id
   *   The ID of the search space to get the field config for.
   *
   * @return boolean|array
   *   searchFields -> Search field setup
   *   fieldNameMap -> Search field map
   *   fieldTypeMap -> Search field type map
   *   uriFields -> Search uri fields.
   */
  public function getFieldConfig($search_space_id = '') {
    return FALSE;
  }

  /**
   * This method searches in the GraphSearch index.
   *
   * @param string $search_space_id
   *   The search space to use for the search.
   * @param array $facets
   *   A list of facet objects that should be used for faceting the
   *   search. [optional]
   * @param array $filters
   *   A list of filter object parameters that define the query. [optional]
   *    array(
   *      object(
   *        'field'    => (string)  facedID   | 'date-to' | 'date-from',
   *        'value'    => (int)     conceptID | timestamp | timestamp,
   *        'optional' => (boolean) TRUE, (default: TRUE)
   *      ),
   *      ...
   *    )
   * @param array $parameters
   *   A list of key value pairs [optional]
   *    array(
   *      'facetMinCount' => (int)    1,     (default:    1)
   *      'locale'        => (string) 'en',  (default: 'en')
   *      'start'         => (int)    0,     (default:    0)
   *      'count'         => (int)    10,    (default:   10)
   *      'sort'          => object(
   *        'field'     => (string) facetID | 'date',
   *        'direction' => (string) 'DESC' | 'ASC',
   *      ),   (default: object('field' => 'date', 'direction' => 'DESC')
   *    )
   *
   * @return boolean|array
   *   List of items or FALSE in case of an error
   */
  public function search($search_space_id = '', $facets = [], $filters = [], $parameters = []) {
    return FALSE;
  }

  /**
   * Get concept suggestions from a given search string.
   *
   * @param string $search_string
   *   The string to get suggestions for
   * @param string $search_space_id
   *   The ID of the search space to use for the suggestions.
   * @param array $parameters
   *   array(
   *    'locale' => (string) 'en',  (default: 'en')
   *    'count'  => (int)    10,    (default:   10)
   *  )
   *
   * @return boolean|array
   *   Array of concepts
   *   array(
   *    'id'      => (string) URI of concept
   *    'label'   => (string) prefLabel of concept
   *    'context' => (string) label of conceptScheme
   *    'field'   => (string) URI of conceptScheme
   *  )
   */
  public function suggest($search_string, $search_space_id = '', $parameters = []) {
    return FALSE;
  }

  /**
   * Get all project dependent facets.
   *
   * @param string $search_space_id
   *   The ID of the search space to get the facets for.
   *
   * @return array
   *   A key value pair list of facets
   */
  public function getFacets($search_space_id = '') {
    return [];
  }

  /**
   * Get all custom facets.
   *
   * @param string $search_space_id
   *   The ID of the search space to get the custom facets for.
   *
   * @return array
   *   A key value pair list of custom facets
   */
  public function getCustomFacets($search_space_id = '') {
    return [];
  }

  /**
   * Get similar content.
   *
   * @param mixed $data
   *   The ID of the document or a text depending on $type.
   * @param string $search_space_id
   *   The ID of the search to use to get similar content.
   * @param array $parameters
   *   Array of the parameters
   * @param string $type
   *   The type of data [id | text]
   *
   * @return boolean|array
   *   A key value pair list of facets or FALSE in case of an error
   */
  public function getSimilar($data, $search_space_id = '', $parameters = [], $type = 'id') {
    return FALSE;
  }

  /**
   * Get the concepts, free terms and recommended content for a given text.
   *
   * @param string $text
   *   The text for the recommendation.
   * @param string $search_space_id
   *   The ID of the search space to use for the recommendation.
   * @param array $parameters
   *   array(
   *     'language' => (string) 'en', (default: 'en')
   *   )
   *
   * @return boolean|array
   *   List of concepts, free terms and recommended content or FALSE in case of
   *   an error
   */
  public function getRecommendation($text, $search_space_id = '', $parameters = []) {
    return FALSE;
  }

  /**
   * Returns the link to a file collected from GraphSearch.
   *
   * @param string $file_path
   *   Relative path to a file in the collection
   *
   * @return string
   *   Link to the file in the collection or FALSE in case of an error
   */
  public function getLinkToFile($file_path) {
    return FALSE;
  }

  /**
   * Get all agents with their configuration and status.
   *
   * @param string $search_space_id
   *   The ID of the search to get the agents for.
   *
   * @return boolean|array
   *   A list of agents with their configuration and status
   */
  public function getAgents($search_space_id = '') {
    return [];
  }

  /**
   * Get all agents that have feed items stored in the search index.
   *
   * @param string $search_space_id
   *   The ID of the search space to get the agents for
   *
   * @return array
   *   A list of agents
   */
  public function getIndexedAgents($search_space_id = '') {
    return [];
  }

  /**
   * Get one agent with his configuration.
   *
   * @param int $agent_id
   *   The ID of the agent
   * @param string $search_space_id
   *   The ID of the search to get the agent for.
   *
   * @return boolean|array
   *   List of agents with their configuration or FALSE in case of an error
   */
  public function getAgent($agent_id, $search_space_id = '') {
    return FALSE;
  }

  /**
   * Add a new agent.
   *
   * @param array $config
   *   array(
   *    'source'          => (string) 'My Source',
   *    'url'             => (string) 'http://example.com/rss.xml'
   *    'username'        => (string) 'admin',
   *    'privateContent'  => (boolean) FALSE,
   *    'periodMillis'    => (int) 3600000,
   *    'spaceKey'        => (string) 'extern',
   *   )
   * @param string $search_space_id
   *   The ID of the search to create the agent for.
   *
   * @return bool
   *   TRUE on success, FALSE on error
   */
  public function addAgent($config, $search_space_id = '') {
    return FALSE;
  }

  /**
   * Update an agent.
   *
   * @param int $agent_id
   *   The ID of the agent.
   * @param array $config
   *   array(
   *    'source'          => (string) 'My Source',
   *    'url'             => (string) 'http://example.com/rss.xml'
   *    'username'        => (string) 'admin',
   *    'privateContent'  => (boolean) FALSE,
   *    'periodMillis'    => (int) 3600000,
   *    'spaceKey'        => (string) 'extern',
   *   )
   * @param string $search_space_id
   *   The ID of the search space the agent was created for.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function updateAgent($agent_id, $config, $search_space_id = '') {
    return FALSE;
  }

  /**
   * Delete an agent.
   *
   * @param int $agent_id
   *   The ID of the agent.
   * @param string $search_space_id
   *   The ID of the search space the agent was created for.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function deleteAgent($agent_id, $search_space_id = '') {
    return FALSE;
  }

  /**
   * Run an agent.
   *
   * @param int $agent_id
   *   The ID of the agent.
   * @param string $search_space_id
   *   The ID of the search space the agent was created for.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function runAgent($agent_id, $search_space_id = '') {
    return FALSE;
  }

  /**
   * Create a single ping.
   *
   * @param array $ping
   *   array(
   *    'title'         => (string) Title of the ping
   *    'text'          => (string) Content of the ping
   *    'username'      => (string) 'admin',
   *    'creationDate'  => (int) unix timestamp,
   *    'pageUrl'       => (string) node URL --> will become the ID,
   *    'spaceKey'      => (string) 'extern', ... not relevant for Drupal.
   *    'dynUris'{      => (object) Tags of the content
   *      'dyn_uri_all_concepts": [
   *        'http://server.com/Project/Concept1',
   *        'http://server.com/Project/Concept2',
   *        'http://server.com/Project/Concept3'
   *      ]
   *    }
   *  )
   * @param string $search_space_id
   *   The ID of the search space to create the ping in.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function createPing(array $ping, $search_space_id = '') {
    return FALSE;
  }

  /**
   * Update an existing ping.
   *
   * @param array $ping
   *   array(
   *    'title'         => (string) Title of the ping
   *    'text'          => (string) Content of the ping
   *    'username'      => (string) 'admin',
   *    'creationDate'  => (int) unix timestamp,
   *    'pageUrl'       => (string) node URL --> will become the ID,
   *    'spaceKey'      => (string) 'extern', ... not relevant for Drupal.
   *    'dynUris'{      => (object) Tags of the content
   *      'dyn_uri_all_concepts": [
   *        'http://server.com/Project/Concept1',
   *        'http://server.com/Project/Concept2',
   *        'http://server.com/Project/Concept3'
   *        ]
   *    }
   *  )
   * @param string $search_space_id
   *   The ID of the search space to update the ping in.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function updatePing(array $ping, $search_space_id = '') {
    return FALSE;
  }

  /**
   * Delete an existing ping.
   *
   * @param string $page
   *   The URL of the page (= ID of the ping).
   * @param string $search_space_id
   *   The ID of the search space to delete the ping in.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function deletePing($page, $search_space_id = '') {
    return FALSE;
  }

  /**
   * Delete all indexed documents from an agent.
   *
   * @param string $source
   *   The name of the source.
   * @param string $search_space_id
   *   The ID of the search space to delete documents from.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function deleteIndex($source, $search_space_id = '') {
    return FALSE;
  }

  /**
   * Get trends from a list of concepts.
   *
   * @param array $uris
   *   A list of uris of concepts.
   * @param string $search_space_id
   *   The search space to get the trends for.
   *
   * @return array
   *   List of trends.
   */
  public function getTrends($uris, $search_space_id = '') {
    return [];
  }

  /**
   * Adds a new custom search field for the suggestion call.
   *
   * @param string $label
   *   The label of the custom search field.
   * @param string $field
   *   The name of the custom search field.
   *   Must start with 'dyn_lit_', e.g. 'dyn_lit_content_type'.
   * @param string $search_space_id
   *   The ID of the search space to add the custom search field for.
   *
   * @return boolean
   *   TRUE if field is added, otherwise FALSE.
   */
  public function addCustomSearchField($label, $field, $search_space_id = '') {
    return TRUE;
  }

  /**
   * Deletes a custom search field for the suggestion call.
   *
   * @param string $field
   *   The name of the custom search field.
   *   Must start with 'dyn_lit_', e.g. 'dyn_lit_content_type'.
   * @param string $search_space_id
   *   The ID of the search space to delete the custom search field for.
   *
   * @return boolean
   *   TRUE if field is deleted, otherwise FALSE.
   */
  public function deleteCustomSearchField($field, $search_space_id = '') {
    return TRUE;
  }

  /**
   * Converts facet list into a list of object parameters for the GraphSearch.
   *
   * @param array $facets
   *   The list of facet objects.
   *
   * @return array
   *   Array of facet objects.
   */
  protected function prepareFacets($facets) {
    return [];
  }

  /**
   * Maps filters into the defined filters or the GraphSearch.
   *
   * @param array $filters
   *   The list of filters.
   *
   * @return array
   *   Array of filter object parameters.
   */
  protected function prepareFilters($filters) {
    return [];
  }

  /**
   * Sort agends by their source.
   *
   * @param array $a
   *   The first agent.
   * @param array $b
   *   The second agent.
   *
   * @return int
   *   The sort-comparison-value.
   */
  protected function sortAgents($a, $b) {
    return strcasecmp($a->configuration['source'], $b->configuration['source']);
  }
}

/**
 * Class SemanticConnectorSonrApi_4_6
 *
 * API Class for the version 4.6
 */
class SemanticConnectorSonrApi_4_6 extends SemanticConnectorSonrApi {

  /**
   * This method checks if the GraphSearch service exists and is running.
   *
   * @return bool
   *   TRUE if the service is available, FALSE if not
   */
  public function available() {
    $resource_path = $this->getApiPath() . 'heartbeat';
    $result = $this->connection->get($resource_path);

    return $result === '' ? TRUE : FALSE;
  }

  /**
   * This method gets the configuration of the PoolParty GraphSearch server.
   *
   * @return array|boolean
   *   project => The PoolParty project used for the extraction
   *   language => The configured language of the content.
   */
  public function getConfig() {
    $resource_path = $this->getApiPath() . 'admin/config';
    $result = $this->connection->get($resource_path);

    if ($result === FALSE) {
      return FALSE;
    }

    $data = drupal_json_decode($result);
    // Create a project with a fake search space.
    $projects = array(
      $data['project'] => array(
        'id' => $data['project'],
        'search_spaces' => array(
          $data['project'] => array(
            'id' => $data['project'],
            'name' => '',
            'language' => $data['language'],
          ),
        ),
      ),
    );

    return array(
      'projects' => $projects
    );
  }

  /**
   * This method gets the field configuration of the PoolParty GraphSearch
   * server.
   *
   * @param string $search_space_id
   *   The ID of the search space to get the field config for.
   *
   * @return boolean|array
   *   searchFields -> Search field setup
   *   fieldNameMap -> Search field map
   *   fieldTypeMap -> Search field type map
   *   uriFields -> Search uri fields.
   */
  public function getFieldConfig($search_space_id = '') {
    $resource_path = $this->getApiPath() . 'config';
    $result = $this->connection->get($resource_path);
    $facet_list = drupal_json_decode($result);

    if (!is_array($facet_list)) {
      return FALSE;
    }

    return $facet_list;
  }

  /**
   * This method searches in the GraphSearch index.
   *
   * @param string $search_space_id
   *   The search space to use for the search.
   * @param array $facets
   *   A list of facet objects that should be used for faceting the
   *   search. [optional]
   * @param array $filters
   *   A list of filter object parameters that define the query. [optional]
   *    array(
   *      object(
   *        'field'    => (string)  facedID   | 'date-to' | 'date-from',
   *        'value'    => (int)     conceptID | timestamp | timestamp,
   *        'optional' => (boolean) TRUE, (default: TRUE)
   *      ),
   *      ...
   *    )
   * @param array $parameters
   *   A list of key value pairs [optional]
   *    array(
   *      'facetMinCount' => (int)    1,     (default:    1)
   *      'locale'        => (string) 'en',  (default: 'en')
   *      'start'         => (int)    0,     (default:    0)
   *      'count'         => (int)    10,    (default:   10)
   *      'sort'          => object(
   *        'field'     => (string) facetID | 'date',
   *        'direction' => (string) 'DESC' | 'ASC',
   *      ),   (default: object('field' => 'date', 'direction' => 'DESC')
   *    )
   *
   * @return boolean|array
   *   List of items or FALSE in case of an error
   */
  public function search($search_space_id = '', $facets = [], $filters = [], $parameters = []) {

    $resource_path = $this->getApiPath() . 'search';

    $sort = new stdClass();
    $sort->field = 'date';
    $sort->direction = 'DESC';

    $post_parameters = [
      'facets' => $this->prepareFacets($facets),
      'filters' => $this->prepareFilters($filters),
      'customAttributes' => $this->customAttributes,
      'facetMinCount' => isset($parameters['facetMinCount']) ? $parameters['facetMinCount'] : 1,
      'maxFacetCount' => isset($parameters['maxFacetCount']) ? $parameters['maxFacetCount'] : 10,
      'locale' => isset($parameters['locale']) ? $parameters['locale'] : 'en',
      'start' => isset($parameters['start']) ? $parameters['start'] : 0,
      'count' => isset($parameters['count']) ? $parameters['count'] : 10,
      'format' => 'json',
      'sort' => isset($parameters['sort']) ? $parameters['sort'] : $sort,
    ];

    $result = $this->connection->post($resource_path, [
      'data' => drupal_json_encode($post_parameters),
    ]);

    $items = drupal_json_decode($result);

    if (!is_array($items)) {
      return FALSE;
    }

    return $items;
  }

  /**
   * Get concept suggestions from a given search string.
   *
   * @param string $search_string
   *   The string to get suggestions for
   * @param string $search_space_id
   *   The ID of the search space to use for the suggestions.
   * @param array $parameters
   *   array(
   *    'locale' => (string) 'en',  (default: 'en')
   *    'count'  => (int)    10,    (default:   10)
   *  )
   *
   * @return boolean|array
   *   Array of concepts
   *   array(
   *    'id'      => (string) URI of concept
   *    'label'   => (string) prefLabel of concept
   *    'context' => (string) label of conceptScheme
   *    'field'   => (string) URI of conceptScheme
   *  )
   */
  public function suggest($search_string, $search_space_id = '', $parameters = []) {
    $resource_path = $this->getApiPath() . 'suggest';

    $get_parameters = array_merge([
      'searchString' => $search_string,
      'locale' => 'en',
      'count' => 10,
      'format' => 'json',
    ], $parameters);

    $result = $this->connection->get($resource_path, [
      'query' => $get_parameters,
    ]);

    $concepts = drupal_json_decode($result);

    if (!is_array($concepts)) {
      return FALSE;
    }

    return $concepts;
  }

  /**
   * Get all project dependent facets.
   *
   * @param string $search_space_id
   *   The ID of the search space to get the facets for.
   *
   * @return boolean|array
   *   A key value pair list of facets
   */
  public function getFacets($search_space_id = '') {
    // Get the fields for the facets.
    $resource_path = $this->getApiPath() . 'config';
    $result = $this->connection->get($resource_path);
    $facet_list = drupal_json_decode($result);

    if (!is_array($facet_list)) {
      return FALSE;
    }

    // Add project dependent facets.
    $facets = [];
    $all_fields = [];
    foreach ($facet_list['searchFields'] as $field) {
      $all_fields[] = $field['name'];
      if ($field['type'] == 'URI' && $field['defaultFacet'] == TRUE) {
        $facets[$field['name']] = $field['label'];
      }
    }

    // Add project independent facets.
    $custom_facets = $this->getCustomFacets($search_space_id);
    if (!empty($custom_facets)) {
      foreach ($custom_facets as $field_name => $field_label) {
        if ((!in_array($field_name, $all_fields) || $field_name == self::ATTR_SOURCE) && $field_name != self::ATTR_SPACE) {
          $facets[$field_name] = $field_label;
        }
      }
    }

    return $facets;
  }

  /**
   * Get all custom facets.
   *
   * @param string $search_space_id
   *   The ID of the search space to get the custom facets for.
   *
   * @return array
   *   A key value pair list of custom facets
   */
  public function getCustomFacets($search_space_id = '') {
    // Get the custom fields.
    $resource_path = $this->getApiPath() . 'config/custom';
    $result = $this->connection->get($resource_path);
    $custom_facet_list = drupal_json_decode($result);

    $facets = [];
    if (isset($custom_facet_list['customSearchFields']) && !empty($custom_facet_list['customSearchFields'])) {
      foreach ($custom_facet_list['customSearchFields'] as $field) {
        $facets[$field['name']] = $field['label'];
      }
    }

    return $facets;
  }

  /**
   * Get similar content.
   *
   * @param mixed $data
   *   The ID of the document or a text depending on $type.
   * @param string $search_space_id
   *   The ID of the search to use to get similar content.
   * @param array $parameters
   *   Array of the parameters
   * @param string $type
   *   The type of data [id | text]
   *
   * @return boolean|array
   *   A key value pair list of facets or FALSE in case of an error
   */
  public function getSimilar($data, $search_space_id = '', $parameters = [], $type = 'id') {
    $resource_path = $this->getApiPath() . 'similar';
    $type = ($type == 'id') ? 'id' : 'text';
    $post_parameters = array_merge([
      $type => $data,
      'locale' => 'en',
      'start' => 0,
      'count' => 10,
      'format' => 'json',
      'fields' => ['dyn_uri_all_concepts', 'title', 'description'],
      'customAttributes' => $this->customAttributes,
    ], $parameters);

    $result = $this->connection->post($resource_path, [
      'data' => drupal_json_encode($post_parameters),
    ]);

    $similar = drupal_json_decode($result);

    if (!is_array($similar)) {
      return FALSE;
    }

    return $similar;
  }

  /**
   * Get the concepts, free terms and recommended content for a given text.
   *
   * @param string $text
   *   The text for the recommendation.
   * @param string $search_space_id
   *   The ID of the search space to use for the recommendation.
   * @param array $parameters
   *   array(
   *     'language' => (string) 'en', (default: 'en')
   *   )
   *
   * @return boolean|array
   *   List of concepts, free terms and recommended content or FALSE in case of
   *   an error
   */
  public function getRecommendation($text, $search_space_id = '', $parameters = []) {
    $resource_path = $this->getApiPath() . 'recommend';
    $post_parameters = array_merge([
      'text' => $text,
      'language' => isset($parameters['locale']) ? $parameters['locale'] : 'en',
      'start' => 0,
      'count' => 10,
      'numberOfConcepts' => 10,
      'numberOfTerms' => 5,
      'fields' => ['dyn_uri_all_concepts', 'title', 'description'],
      'nativeQuery' => '',
      'customAttributes' => $this->customAttributes,
    ], $parameters);

    $result = $this->connection->post($resource_path, [
      'data' => drupal_json_encode($post_parameters),
    ]);

    $recommendations = json_decode($result);

    if (!is_object($recommendations)) {
      return FALSE;
    }

    return $recommendations;
  }

  /**
   * Returns the link to a file collected from GraphSearch.
   *
   * @param string $file_path
   *   Relative path to a file in the collection
   *
   * @return string
   *   Link to the file in the collection or FALSE in case of an error
   */
  public function getLinkToFile($file_path) {
    $resource_path = $this->getApiPath() . 'collector/';
    return $this->connection->getEndpoint() . $resource_path . $file_path;
  }

  /**
   * Get all agents with their configuration and status.
   *
   * @param string $search_space_id
   *   The ID of the search to get the agents for.
   *
   * @return boolean|array
   *   A list of agents with their configuration and status
   */
  public function getAgents($search_space_id = '') {
    $resource_path = $this->getApiPath() . 'agents/status';
    $result = $this->connection->get($resource_path);

    $agent_list = drupal_json_decode($result);

    if (!is_array($agent_list)) {
      return FALSE;
    }

    $agents = [];
    if (!is_null($agent_list)) {
      foreach ($agent_list as $id => $agent) {
        $agents[$id] = new stdClass();
        $agents[$id]->id = $agent['agent']['id'];
        $agents[$id]->configuration = $agent['agent']['configuration'];
        $agents[$id]->status = $agent['status'];
      }
      usort($agents, [$this, 'sortAgents']);
    }

    return $agents;
  }

  /**
   * Get all agents that have feed items stored in the search index.
   *
   * @param string $search_space_id
   *   The ID of the search space to get the agents for
   *
   * @return array
   *   A list of agents
   */
  public function getIndexedAgents($search_space_id = '') {
    $agents = [];

    // Make a simple search call with the source facet only.
    // TODO: This hardcoded config should be from the saved configuration.
    $facets = [
      self::ATTR_SOURCE => [
        'name' => 'Source',
        'selected' => 1,
        'facet_mode' => 'list',
        'max-items' => 1,
        'tree_depth' => 1,
        'facet_id' => self::ATTR_SOURCE,
      ],
    ];
    $parameters = [
      'count' => 1,
      'start' => 0,
      'maxFacetCount' => 10000,
    ];
    $search = $this->search($search_space_id, $facets, [], $parameters);
    if ($search == FALSE) {
      return [];
    }

    // Get the agents from the facet list.
    if (isset($search['facetList']) && !empty($search['facetList'])) {
      $source_facet = $search['facetList'][0]['facets'];
      foreach ($source_facet as $source) {
        $agents[$source['label']] = $source['value'];
      }
    }

    return $agents;
  }

  /**
   * Get one agent with his configuration.
   *
   * @param int $agent_id
   *   The ID of the agent
   * @param string $search_space_id
   *   The ID of the search to get the agent for.
   *
   * @return boolean|array
   *   The configuration of a given agent or FALSE in case of an error
   */
  public function getAgent($agent_id, $search_space_id = '') {
    $resource_path = $this->getApiPath() . 'agents/%id';

    $result = $this->connection->get($resource_path, [
      'parameters' => ['%id' => $agent_id],
    ]);

    $agent = drupal_json_decode($result);

    if (!is_array($agent)) {
      return FALSE;
    }

    $agent['id'] = $agent_id;

    return $agent;
  }

  /**
   * Add a new agent.
   *
   * @param array $config
   *   array(
   *    'source'          => (string) 'My Source',
   *    'url'             => (string) 'http://example.com/rss.xml'
   *    'username'        => (string) 'admin',
   *    'periodMillis'    => (int) 3600000,
   *   )
   * @param string $search_space_id
   *   The ID of the search to create the agent for.
   *
   * @return bool
   *   TRUE on success, FALSE on error
   */
  public function addAgent($config, $search_space_id = '') {
    $config['privateContent'] = FALSE;
    $config['spaceKey'] = '';

    $resource_path = $this->getApiPath() . 'agents';

    $result = $this->connection->post($resource_path, [
      'data' => drupal_json_encode($config),
    ]);

    return $result === FALSE ? FALSE : TRUE;
  }

  /**
   * Update an agent.
   *
   * @param int $agent_id
   *   The ID of the agent.
   * @param array $config
   *   array(
   *    'source'          => (string) 'My Source',
   *    'url'             => (string) 'http://example.com/rss.xml'
   *    'username'        => (string) 'admin',
   *    'periodMillis'    => (int) 3600000,
   *   )
   * @param string $search_space_id
   *   The ID of the search space the agent was created for.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function updateAgent($agent_id, $config, $search_space_id = '') {
    $config['privateContent'] = FALSE;
    $config['spaceKey'] = '';

    $resource_path = $this->getApiPath() . 'agents/%id';

    $result = $this->connection->put($resource_path, [
      'parameters' => ['%id' => $agent_id],
      'data' => drupal_json_encode($config),
    ]);

    return $result === FALSE ? FALSE : TRUE;
  }

  /**
   * Delete an agent.
   *
   * @param int $agent_id
   *   The ID of the agent.
   * @param string $search_space_id
   *   The ID of the search space the agent was created for.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function deleteAgent($agent_id, $search_space_id = '') {
    $resource_path = $this->getApiPath() . 'agents/%id';

    $result = $this->connection->delete($resource_path, [
      'parameters' => ['%id' => $agent_id],
    ]);

    return $result === FALSE ? FALSE : TRUE;
  }

  /**
   * Run an agent.
   *
   * @param int $agent_id
   *   The ID of the agent.
   * @param string $search_space_id
   *   The ID of the search space the agent was created for.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function runAgent($agent_id, $search_space_id = '') {
    $resource_path = $this->getApiPath() . 'agents/runAgent';

    $result = $this->connection->get($resource_path, [
      'query' => ['id' => $agent_id],
      'timeout' => 120,
    ]);

    return $result === FALSE ? FALSE : TRUE;
  }

  /**
   * Create a single ping.
   *
   * @param array $ping
   *   array(
   *    'title'         => (string) Title of the ping
   *    'text'          => (string) Content of the ping
   *    'username'      => (string) 'admin',
   *    'creationDate'  => (int) unix timestamp,
   *    'pageUrl'       => (string) node URL --> will become the ID,
   *    'spaceKey'      => (string) 'extern', ... not relevant for Drupal.
   *    'dynUris'{      => (object) Tags of the content
   *      'dyn_uri_all_concepts": [
   *        'http://server.com/Project/Concept1',
   *        'http://server.com/Project/Concept2',
   *        'http://server.com/Project/Concept3'
   *      ]
   *    }
   *  )
   * @param string $search_space_id
   *   The ID of the search space to create the ping in.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function createPing(array $ping, $search_space_id = '') {
    $resource_path = $this->getApiPath() . 'pings/create';
    $ping['text'] = substr($ping['text'], 0, 12000);

    $result = $this->connection->post($resource_path, [
      'data' => drupal_json_encode($ping),
    ]);

    return $result === FALSE ? FALSE : TRUE;
  }

  /**
   * Update an existing ping.
   *
   * @param array $ping
   *   array(
   *    'title'         => (string) Title of the ping
   *    'text'          => (string) Content of the ping
   *    'username'      => (string) 'admin',
   *    'creationDate'  => (int) unix timestamp,
   *    'pageUrl'       => (string) node URL --> will become the ID,
   *    'spaceKey'      => (string) 'extern', ... not relevant for Drupal.
   *    'dynUris'{      => (object) Tags of the content and term references
   *      'dyn_uri_all_concepts": [
   *        'http://server.com/Project/Concept1',
   *        'http://server.com/Project/Concept2',
   *        'http://server.com/Project/Concept3'
   *        ]
   *    }
   *  )
   * @param string $search_space_id
   *   The ID of the search space to update the ping in.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function updatePing(array $ping, $search_space_id = '') {
    $resource_path = $this->getApiPath() . 'pings/update';
    $ping['text'] = substr($ping['text'], 0, 12000);

    $result = $this->connection->post($resource_path, [
      'data' => drupal_json_encode($ping),
    ]);

    return $result === FALSE ? FALSE : TRUE;
  }

  /**
   * Delete an existing ping.
   *
   * @param string $page
   *   The URL of the page (= ID of the ping).
   * @param string $search_space_id
   *   The ID of the search space to delete the ping in.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function deletePing($page, $search_space_id = '') {
    $resource_path = $this->getApiPath() . 'pings/delete';

    $result = $this->connection->post($resource_path, [
      'data' => drupal_json_encode(['pageUrl' => $page]),
    ]);

    return $result === FALSE ? FALSE : TRUE;
  }

  /**
   * Delete all indexed documents from an agent.
   *
   * @param string $source
   *   The name of the source.
   * @param string $search_space_id
   *   The ID of the search space to delete documents from.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function deleteIndex($source, $search_space_id = '') {
    $resource_path = $this->getApiPath() . 'pings/deleteAll';

    $result = $this->connection->get($resource_path, [
      'query' => ['source' => $source],
    ]);

    return $result === FALSE ? FALSE : TRUE;
  }

  /**
   * Get trends from a list of concepts.
   *
   * @param array $uris
   *   A list of uris of concepts.
   * @param string $search_space_id
   *   The search space to get the trends for.
   *
   * @return boolean|array
   *   List of trends.
   */
  public function getTrends($uris, $search_space_id = '') {
    $resource_path = $this->getApiPath() . 'trend/histories';

    if (is_string($uris)) {
      $uris = [$uris];
    }

    $result = $this->connection->get($resource_path, [
      'query' => ['concepts' => implode(',', $uris)],
    ]);

    $trends = drupal_json_decode($result);

    if (!is_array($trends)) {
      return FALSE;
    }

    return $trends;
  }

  /**
   * Converts facet list into a list of object parameters for the GraphSearch.
   *
   * @param array $facets
   *   The list of facet objects.
   *
   * @return array
   *   Array of facet objects.
   */
  protected function prepareFacets($facets) {
    $facet_parameters = [];
    foreach ($facets as $facet) {
      $facet_parameters[] = [
        'field' => $facet['facet_id'],
        'facetMode' => $facet['facet_mode'],
      ];
    }

    return $facet_parameters;
  }

  /**
   * Maps filters into the defined filters or the GraphSearch.
   *
   * @param array $filters
   *   The list of filters.
   *
   * @return array
   *   Array of filter object parameters.
   */
  protected function prepareFilters($filters) {
    $dates = [];
    foreach ($filters as $key => $filter) {
      if ($filter->field == 'date-from') {
        $dates['from'] = $filter->value . 'T00:00:00Z';
        unset($filters[$key]);
      }
      if ($filter->field == 'date-to') {
        $dates['to'] = $filter->value . 'T23:59:59Z';
        unset($filters[$key]);
      }
    }
    $value = '';
    $value .= isset($dates['from']) ? $dates['from'] : '*';
    $value .= ' TO ';
    $value .= isset($dates['to']) ? $dates['to'] : '*';

    $item = new stdClass();
    $item->field = 'date';
    $item->value = '[' . $value . ']';

    $filters[] = $item;
    $filters = array_values($filters);

    return $filters;
  }
}

/**
 * Class SemanticConnectorSonrApi_5_3
 *
 * API Class for the version 5.3
 */
class SemanticConnectorSonrApi_5_3 extends SemanticConnectorSonrApi_4_6 {

  /**
   * Get concept suggestions from a given search string.
   *
   * @param string $search_string
   *   The string to get suggestions for
   * @param string $search_space_id
   *   The ID of the search space to use for the suggestions.
   * @param array $parameters
   *   array(
   *    'locale' => (string) 'en',  (default: 'en')
   *    'count'  => (int)    10,    (default:   10)
   *  )
   *
   * @return boolean|array
   *   Array of concepts
   *   array(
   *    'id'      => (string) URI of concept
   *    'label'   => (string) prefLabel of concept
   *    'context' => (string) prefLabel of the broader concept
   *    'field'   => (string) URI of conceptScheme
   *  )
   */
  public function suggest($search_string, $search_space_id = '', $parameters = []) {
    $resource_path = $this->getApiPath() . 'suggest/multi';
    $get_parameters = array_merge([
      'searchString' => $search_string,
      'locale' => 'en',
      'count' => 10,
      'format' => 'json',
    ], $parameters);

    $result = $this->connection->get($resource_path, [
      'query' => $get_parameters,
    ]);

    $concepts = drupal_json_decode($result);

    if (!is_array($concepts)) {
      return FALSE;
    }

    return $concepts;
  }

  /**
   * Adds a new custom search field for the suggestion call.
   *
   * @param string $label
   *   The label of the custom search field.
   * @param string $field
   *   The name of the custom search field, e.g. 'content_type'.
   * @param string $search_space_id
   *   The ID of the search space to add the custom search field for.
   *
   * @return boolean
   *   TRUE if field is added, otherwise FALSE.
   */
  public function addCustomSearchField($label, $field, $search_space_id = '') {
    $resource_path = $this->getApiPath() . 'config/search/add';
    $field = 'dyn_lit_' . str_replace('-', '_', $field);
    $post_parameters = [
      'label' => $label,
      'name' => $field,
    ];

    $result = $this->connection->post($resource_path, [
      'data' => $post_parameters,
    ]);

    if ($result !== FALSE) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Deletes a custom search field for the suggestion call.
   *
   * @param string $field
   *   The name of the custom search field.
   *   Must start with 'dyn_lit_', e.g. 'dyn_lit_content_type'.
   * @param string $search_space_id
   *   The ID of the search space to delete the custom search field for.
   *
   * @return boolean
   *   TRUE if field is deleted, otherwise FALSE.
   */
  public function deleteCustomSearchField($field, $search_space_id = '') {
    $resource_path = $this->getApiPath() . 'config/search/delete';
    $field = 'dyn_lit_' . str_replace('-', '_', $field);
    $post_parameters = [
      'name' => $field,
    ];

    $result = $this->connection->post($resource_path, [
      'data' => $post_parameters,
    ]);

    if ($result !== FALSE) {
      return TRUE;
    }

    return FALSE;
  }

}

/**
 * Class SemanticConnectorSonrApi_5_6
 *
 * API Class for the version 5.6 = the first GraphSearch version.
 */
class SemanticConnectorSonrApi_5_6 extends SemanticConnectorSonrApi_5_3 {

  /**
   * This method checks if the GraphSearch service exists and is running.
   *
   * @return bool
   *   TRUE if the service is available, FALSE if not
   */
  public function available() {
    $resource_path = $this->getApiPath() . 'admin/heartbeat';
    $result = $this->connection->get($resource_path);

    return $result === '' ? TRUE : FALSE;
  }

  /**
   * Get all custom facets.
   *
   * @param string $search_space_id
   *   The ID of the search space to get the custom facets for.
   *
   * @return array
   *   A key value pair list of custom facets
   */
  public function getCustomFacets($search_space_id = '') {
    // Get the custom fields.
    $resource_path = $this->getApiPath() . 'config/custom';
    $result = $this->connection->get($resource_path);
    $custom_facet_list = drupal_json_decode($result);

    $facets = [];
    if (isset($custom_facet_list['searchFields']) && !empty($custom_facet_list['searchFields'])) {
      foreach ($custom_facet_list['searchFields'] as $field) {
        $facets[$field['name']] = $field['label'];
      }
    }

    return $facets;
  }

  /**
   * Create a single ping.
   *
   * @param array $ping
   *   array(
   *    'title'         => (string) Title of the ping
   *    'text'          => (string) Content of the ping
   *    'username'      => (string) 'admin',
   *    'creationDate'  => (int) unix timestamp,
   *    'pageUrl'       => (string) node URL --> will become the ID,
   *    'spaceKey'      => (string) 'extern', ... not relevant for Drupal.
   *    'dynUris'{      => (object) Tags of the content
   *      'dyn_uri_all_concepts": [
   *        'http://server.com/Project/Concept1',
   *        'http://server.com/Project/Concept2',
   *        'http://server.com/Project/Concept3'
   *      ]
   *    }
   *  )
   * @param string $search_space_id
   *   The ID of the search space to create the ping in.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function createPing(array $ping, $search_space_id = '') {
    $resource_path = $this->getApiPath() . 'content/create';
    $ping['text'] = substr($ping['text'], 0, 12000);

    $result = $this->connection->post($resource_path, [
      'data' => drupal_json_encode($ping),
    ]);

    return $result === FALSE ? FALSE : TRUE;
  }

  /**
   * Update an existing ping.
   *
   * @param array $ping
   *   array(
   *    'title'         => (string) Title of the ping
   *    'text'          => (string) Content of the ping
   *    'username'      => (string) 'admin',
   *    'creationDate'  => (int) unix timestamp,
   *    'pageUrl'       => (string) node URL --> will become the ID,
   *    'spaceKey'      => (string) 'extern', ... not relevant for Drupal.
   *    'dynUris'{      => (object) Tags of the content and term references
   *      'dyn_uri_all_concepts": [
   *        'http://server.com/Project/Concept1',
   *        'http://server.com/Project/Concept2',
   *        'http://server.com/Project/Concept3'
   *        ]
   *    }
   *  )
   * @param string $search_space_id
   *   The ID of the search space to update the ping in.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function updatePing(array $ping, $search_space_id = '') {
    $resource_path = $this->getApiPath() . 'content/update';
    $ping['text'] = substr($ping['text'], 0, 12000);

    $result = $this->connection->post($resource_path, [
      'data' => drupal_json_encode($ping),
    ]);

    return $result === FALSE ? FALSE : TRUE;
  }

  /**
   * Delete an existing ping.
   *
   * @param string $page
   *   The URL of the page (= ID of the ping).
   * @param string $search_space_id
   *   The ID of the search space to delete the ping in.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function deletePing($page, $search_space_id = '') {
    $resource_path = $this->getApiPath() . 'content/delete';

    $result = $this->connection->post($resource_path, [
      'data' => drupal_json_encode(['pageUrl' => $page]),
    ]);

    return $result === FALSE ? FALSE : TRUE;
  }

  /**
   * Delete all indexed documents from an agent.
   *
   * @param string $source
   *   The name of the source.
   * @param string $search_space_id
   *   The ID of the search space to delete documents from.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function deleteIndex($source, $search_space_id = '') {
    $resource_path = $this->getApiPath() . 'content/delete/all';

    $result = $this->connection->get($resource_path, [
      'query' => ['source' => $source],
    ]);

    return $result === FALSE ? FALSE : TRUE;
  }

  /**
   * Get trends from a list of concepts.
   *
   * @param array $uris
   *   A list of uris of concepts.
   * @param string $search_space_id
   *   The search space to get the trends for.
   *
   * @return boolean|array
   *   List of trends.
   */
  public function getTrends($uris, $search_space_id = '') {
    $resource_path = $this->getApiPath() . 'trend/histories';

    if (is_string($uris)) {
      $uris = [$uris];
    }

    $result = $this->connection->get($resource_path, [
      'query' => ['concepts' => implode(',', $uris)],
    ]);

    $trends = drupal_json_decode($result);

    if (!is_array($trends)) {
      return FALSE;
    }

    return $trends;
  }

}

/**
 * Class SemanticConnectorSonrApi_5_7
 *
 * API Class for the version 5.7.
 */
class SemanticConnectorSonrApi_5_7 extends SemanticConnectorSonrApi_5_6 {

  /**
   * Update an agent.
   *
   * @param int $agent_id
   *   The ID of the agent.
   * @param array $config
   *   array(
   *    'source'          => (string) 'My Source',
   *    'url'             => (string) 'http://example.com/rss.xml'
   *    'username'        => (string) 'admin',
   *    'periodMillis'    => (int) 3600000,
   *   )
   * @param string $search_space_id
   *   The ID of the search space the agent was created for.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function updateAgent($agent_id, $config, $search_space_id = '') {
    $config['privateContent'] = FALSE;
    $config['spaceKey'] = '';

    $resource_path = $this->getApiPath() . 'agents/%id';

    $result = $this->connection->post($resource_path, [
      'parameters' => ['%id' => $agent_id],
      'data' => drupal_json_encode($config),
    ]);

    return $result === FALSE ? FALSE : TRUE;
  }

  /**
   * Delete an agent.
   *
   * @param int $agent_id
   *   The ID of the agent.
   * @param string $search_space_id
   *   The ID of the search space the agent was created for.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function deleteAgent($agent_id, $search_space_id = '') {
    $resource_path = $this->getApiPath() . 'agents/%id/delete';

    $result = $this->connection->post($resource_path, [
      'parameters' => ['%id' => $agent_id],
      'data' => [],
    ]);

    return $result === FALSE ? FALSE : TRUE;
  }

  /**
   * Run an agent.
   *
   * @param int $agent_id
   *   The ID of the agent.
   * @param string $search_space_id
   *   The ID of the search space the agent was created for.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function runAgent($agent_id, $search_space_id = '') {
    $resource_path = $this->getApiPath() . 'agents/runAgent';

    $result = $this->connection->post($resource_path, [
      'data' => ['id' => $agent_id],
      'timeout' => 120,
    ]);

    return $result === FALSE ? FALSE : TRUE;
  }

  /**
   * Delete all indexed documents from an agent.
   *
   * @param string $source
   *   The name of the source.
   * @param string $search_space_id
   *   The ID of the search space to delete documents from.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function deleteIndex($source, $search_space_id = '') {
    $resource_path = $this->getApiPath() . 'content/delete/all';

    $result = $this->connection->post($resource_path, [
      'data' => ['source' => $source],
    ]);

    return $result === FALSE ? FALSE : TRUE;
  }

}

/**
 * Class SemanticConnectorSonrApi_6_0
 *
 * API Class for the version 6.0.
 */
class SemanticConnectorSonrApi_6_0 extends SemanticConnectorSonrApi_5_7 {

  /**
   * This method checks if the GraphSearch service exists and is running.
   *
   * @return bool
   *   TRUE if the service is available, FALSE if not
   */
  public function available() {
    $resource_path = '/' . $this->graphSearchPath . 'admin/heartbeat';
    $result = $this->connection->get($resource_path);

    return $result === '' ? TRUE : FALSE;
  }

  /**
   * This method gets the configuration of the PoolParty GraphSearch server.
   *
   * @return boolean|array
   *   project => The PoolParty project used for the extraction
   *   language => The configured language of the content.
   */
  public function getConfig() {
    $resource_path = '/' . $this->graphSearchPath . '/admin/config/server';
    $result = $this->connection->get($resource_path);

    if ($result === FALSE) {
      return FALSE;
    }

    $data = drupal_json_decode($result);
    // Create a project with a fake search space.
    $projects = array(
      $data['facet'] => array(
        'id' => $data['facet'],
        'search_spaces' => array(
          $data['facet'] => array(
            'id' => $data['facet'],
            'name' => '',
            'language' => $data['defaultLanguage'],
          ),
        ),
      ),
    );

    return array(
      'projects' => $projects
    );
  }

  /**
   * This method gets the field configuration of the PoolParty GraphSearch
   * server.
   *
   * @param string $search_space_id
   *   The ID of the search space to get the field config for.
   *
   * @return boolean|array
   *   searchFields -> all available fields in a list
   *   fieldNameMap -> all fields grouped by name
   *   fieldTypeMap -> all fields grouped by type
   *   contextSearchFields -> all fields grouped by field context.
   */
  public function getFieldConfig($search_space_id = '') {
    $resource_path = '/' . $this->graphSearchPath . '/admin/config/fields';
    $result = $this->connection->get($resource_path);
    $facet_list = drupal_json_decode($result);

    if (!is_array($facet_list)) {
      return FALSE;
    }

    // Make compatible with older version
    if (!empty($facet_list['searchFields'])) {
      foreach ($facet_list['searchFields'] as &$search_fields) {
        $search_fields['name'] = $search_fields['field'];
      }
    }

    return $facet_list;
  }

  /**
   * This method searches in the GraphSearch index.
   *
   * @param string $search_space_id
   *   The search space to use for the search.
   * @param array $facets
   *   A list of facet objects that should be used for faceting the
   *   search. [optional]
   * @param array $filters
   *   A list of filter object parameters that define the query. [optional]
   *    array(
   *      object(
   *        'field'    => (string)  facedID   | 'date-to' | 'date-from',
   *        'value'    => (int)     conceptID | timestamp | timestamp,
   *        'optional' => (boolean) TRUE, (default: TRUE)
   *      ),
   *      ...
   *    )
   * @param array $parameters
   *   A list of key value pairs [optional]
   *    array(
   *      'facetMinCount' => (int)    1,     (default:    1)
   *      'locale'        => (string) 'en',  (default: 'en')
   *      'start'         => (int)    0,     (default:    0)
   *      'count'         => (int)    10,    (default:   10)
   *      'sort'          => object(
   *        'field'     => (string) facetID | 'date',
   *        'direction' => (string) 'DESC' | 'ASC',
   *      ),   (default: object('field' => 'date', 'direction' => 'DESC')
   *    )
   *
   * @return boolean|array
   *   List of items or FALSE in case of an error
   */
  public function search($search_space_id = '', $facets = [], $filters = [], $parameters = []) {

    $resource_path = $this->getApiPath() . 'search';

    $sort = new stdClass();
    $sort->field = 'date';
    $sort->direction = 'DESC';

    $post_parameters = [
      'searchFacets' => $this->prepareFacets($facets),
      'searchFilters' => $this->prepareFilters($filters),
      'documentFacets' => $this->customAttributes,
      'facetMinCount' => isset($parameters['facetMinCount']) ? $parameters['facetMinCount'] : 1,
      'maxFacetCount' => isset($parameters['maxFacetCount']) ? $parameters['maxFacetCount'] : 10,
      'locale' => isset($parameters['locale']) ? $parameters['locale'] : 'en',
      'start' => isset($parameters['start']) ? $parameters['start'] : 0,
      'count' => isset($parameters['count']) ? $parameters['count'] : 10,
      'sort' => isset($parameters['sort']) ? $parameters['sort'] : $sort,
    ];

    $result = $this->connection->post($resource_path, [
      'data' => drupal_json_encode($post_parameters),
    ]);

    $items = drupal_json_decode($result);

    if (!is_array($items)) {
      return FALSE;
    }

    // Make compatible with older version
    $items = $this->makeSearchCompatible($items);

    return $items;
  }

  /**
   * Get concept suggestions from a given search string.
   *
   * @param string $search_string
   *   The string to get suggestions for
   * @param string $search_space_id
   *   The ID of the search space to use for the suggestions.
   * @param array $parameters
   *   array(
   *    'locale' => (string) 'en',  (default: 'en')
   *    'count'  => (int)    10,    (default:   10)
   *  )
   *
   * @return boolean|array
   *   Array of concepts
   *   array(
   *    'id'      => (string) URI of concept
   *    'label'   => (string) prefLabel of concept
   *    'context' => (string) prefLabel of the broader concept
   *    'field'   => (string) URI of conceptScheme
   *  )
   */
  public function suggest($search_string, $search_space_id = '', $parameters = []) {
    $resource_path = $this->getApiPath() . 'suggest/multi';

    $get_parameters = array_merge([
      'searchString' => $search_string,
      'locale' => 'en',
      'count' => 10,
    ], $parameters);

    $result = $this->connection->get($resource_path, [
      'query' => $get_parameters,
    ]);

    $concepts = drupal_json_decode($result);

    if (!is_array($concepts)) {
      return FALSE;
    }

    // Make compatible with older version
    $concepts = $this->makeSuggestCompatible($concepts);

    return $concepts;
  }

  /**
   * Get all project dependent facets.
   *
   * @param string $search_space_id
   *   The ID of the search space to get the facets for.
   *
   * @return boolean|array
   *   A key value pair list of facets
   */
  public function getFacets($search_space_id = '') {
    // Get the fields for the facets.
    $facet_list = $this->getFieldConfig($search_space_id);

    if (!is_array($facet_list)) {
      return FALSE;
    }

    // Make compatible with older version

    // Add custom facets.
    $facets = [];
    if (!empty($facet_list['contextSearchFields']['CustomSearchFieldContext'])) {
      foreach ($facet_list['contextSearchFields']['CustomSearchFieldContext'] as $field) {
        $facets[$field['field']] = $field['label'];
      }
    }

    // Add GraphSearch facets.
    if (!empty($facet_list['contextSearchFields']['GraphSearchFieldContext'])) {
      foreach ($facet_list['contextSearchFields']['GraphSearchFieldContext'] as $field) {
        $facets[$field['field']] = $field['label'];
      }
    }

    return $facets;
  }

  /**
   * Get all custom facets.
   *
   * @param string $search_space_id
   *   The ID of the search space to get the custom facets for.
   *
   * @return boolean|array
   *   A key value pair list of custom facets
   */
  public function getCustomFacets($search_space_id = '') {
    // Get the fields for the facets.
    $facet_list = $this->getFieldConfig($search_space_id);

    if (!is_array($facet_list)) {
      return FALSE;
    }

    // Make compatible with older version

    // Add custom facets.
    $facets = [];
    if (!empty($facet_list['contextSearchFields']['CustomSearchFieldContext'])) {
      foreach ($facet_list['contextSearchFields']['CustomSearchFieldContext'] as $field) {
        $facets[$field['field']] = $field['label'];
      }
    }

    return $facets;
  }

  /**
   * Get similar content.
   *
   * @param mixed $data
   *   The ID of the document or a text depending on $type.
   * @param string $search_space_id
   *   The ID of the search to use to get similar content.
   * @param array $parameters
   *   Array of the parameters
   * @param string $type
   *   The type of data [id | text]
   *
   * @return boolean|array
   *   A key value pair list of facets or FALSE in case of an error
   */
  public function getSimilar($data, $search_space_id = '', $parameters = [], $type = 'id') {
    $resource_path = $this->getApiPath() . 'similar';
    $type = ($type == 'id') ? 'id' : 'text';
    $post_parameters = array_merge([
      $type => $data,
      'locale' => 'en',
      'start' => 0,
      'count' => 10,
      'fields' => ['dyn_uri_all_concepts', 'title', 'description'],
      'documentFacets' => $this->customAttributes,
    ], $parameters);

    $result = $this->connection->post($resource_path, [
      'data' => drupal_json_encode($post_parameters),
    ]);

    $similar = drupal_json_decode($result);
    if (!is_array($similar)) {
      return FALSE;
    }

    // Make compatible with older version
    $similar = $this->makeSearchCompatible($similar);

    return $similar;
  }

  /**
   * Get the concepts, free terms and recommended content for a given text.
   *
   * @param string $text
   *   The text for the recommendation.
   * @param string $search_space_id
   *   The ID of the search space to use for the recommendation.
   * @param array $parameters
   *   array(
   *     'language' => (string) 'en', (default: 'en')
   *   )
   *
   * @return boolean|array
   *   List of concepts, free terms and recommended content or FALSE in case of
   *   an error
   */
  public function getRecommendation($text, $search_space_id = '', $parameters = []) {
    $resource_path = $this->getApiPath() . 'recommend';
    $post_parameters = array_merge([
      'text' => $text,
      'language' => isset($parameters['locale']) ? $parameters['locale'] : 'en',
      'start' => 0,
      'count' => 10,
      'numberOfConcepts' => 10,
      'numberOfTerms' => 5,
      'fields' => ['dyn_uri_all_concepts', 'title', 'description'],
    ], $parameters);

    $result = $this->connection->post($resource_path, [
      'data' => drupal_json_encode($post_parameters),
    ]);

    $recommendations = json_decode($result);

    if (!is_object($recommendations)) {
      return FALSE;
    }
    else {
      // property 'prefLabel' is named 'label' now --> add prefLabel for
      // backwards compatibility.
      if (isset($recommendations->concepts)) {
        foreach ($recommendations->concepts as &$annotation) {
          if (!isset($annotation->prefLabel)) {
            $annotation->prefLabel = $annotation->label;
          }
          unset($annotation);
        }
      }
    }

    return $recommendations;
  }

  /**
   * Returns the link to a file collected from GraphSearch.
   *
   * INFO: This method does not exists any more.
   *
   * @param string $file_path
   *   Relative path to a file in the collection
   *
   * @return string
   *   Link to the file in the collection or FALSE in case of an error
   */
  public function getLinkToFile($file_path) {
    return '';
  }

  /**
   * Get all agents that have feed items stored in the search index.
   *
   * @param string $search_space_id
   *   The ID of the search space to get the agents for
   *
   * @return array
   *   A list of agents
   */
  public function getIndexedAgents($search_space_id = '') {
    $agents = [];

    $parameters = [
      'count' => 1,
      'start' => 0,
      'maxFacetCount' => 10000,
    ];
    $search = $this->search($search_space_id, [], [], $parameters);
    if ($search == FALSE) {
      return [];
    }

    // Get the agents from the facet list.
    if (isset($search['facetList']) && !empty($search['facetList'])) {
      $source_facet = $search['facetList'][0]['facets'];
      foreach ($source_facet as $source) {
        $agents[$source['label']] = $source['value'];
      }
    }

    return $agents;
  }

  /**
   * Add a new agent.
   *
   * @param array $config
   *   array(
   *    'source'          => (string) 'My Source',
   *    'url'             => (string) 'http://example.com/rss.xml'
   *    'username'        => (string) 'admin',
   *    'periodMillis'    => (int) 3600000,
   *   )
   * @param string $search_space_id
   *   The ID of the search to create the agent for.
   *
   * @return bool
   *   TRUE on success, FALSE on error
   */
  public function addAgent($config, $search_space_id = '') {
    $config['privateContent'] = FALSE;
    $config['context'] = '';

    $resource_path = $this->getApiPath() . 'agents';

    $result = $this->connection->post($resource_path, [
      'data' => drupal_json_encode($config),
    ]);

    return $result === FALSE ? FALSE : TRUE;
  }

  /**
   * Update an agent.
   *
   * @param int $agent_id
   *   The ID of the agent.
   * @param array $config
   *   array(
   *    'source'          => (string) 'My Source',
   *    'url'             => (string) 'http://example.com/rss.xml'
   *    'username'        => (string) 'admin',
   *    'periodMillis'    => (int) 3600000,
   *   )
   * @param string $search_space_id
   *   The ID of the search space the agent was created for.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function updateAgent($agent_id, $config, $search_space_id = '') {
    $config['privateContent'] = FALSE;
    $config['context'] = '';

    $resource_path = $this->getApiPath() . 'agents/%id';

    $result = $this->connection->post($resource_path, [
      'parameters' => ['%id' => $agent_id],
      'data' => drupal_json_encode($config),
    ]);

    return $result === FALSE ? FALSE : TRUE;
  }

  /**
   * Create a single ping.
   *
   * @param array $ping
   *   array(
   *    'title'         => (string) Title of the ping
   *    'text'          => (string) Content of the ping
   *    'username'      => (string) 'admin',
   *    'creationDate'  => (int) unix timestamp,
   *    'pageUrl'       => (string) node URL --> will become the ID,
   *    'spaceKey'      => (string) 'extern', ... not relevant for Drupal.
   *    'dynUris'{      => (object) Tags of the content
   *      'dyn_uri_all_concepts": [
   *        'http://server.com/Project/Concept1',
   *        'http://server.com/Project/Concept2',
   *        'http://server.com/Project/Concept3'
   *      ]
   *    }
   *  )
   * @param string $search_space_id
   *   The ID of the search space to create the ping in.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function createPing(array $ping, $search_space_id = '') {
    return $this->updatePing($ping, $search_space_id);
  }

  /**
   * Update an existing ping.
   *
   * @param array $ping
   *   array(
   *    'title'         => (string) Title of the ping
   *    'text'          => (string) Content of the ping
   *    'username'      => (string) 'admin',
   *    'creationDate'  => (int) unix timestamp,
   *    'pageUrl'       => (string) node URL --> will become the ID,
   *    'spaceKey'      => (string) 'extern', ... not relevant for Drupal.
   *    'dynUris'{      => (object) Tags of the content and term references
   *      'dyn_uri_all_concepts": [
   *        'http://server.com/Project/Concept1',
   *        'http://server.com/Project/Concept2',
   *        'http://server.com/Project/Concept3'
   *        ]
   *    }
   *  )
   * @param string $search_space_id
   *   The ID of the search space to update the ping in.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function updatePing(array $ping, $search_space_id = '') {
    $resource_path = $this->getApiPath() . 'content/update';
    $ping['identifier'] = $ping['pageUrl'];
    $ping['text'] = substr($ping['text'], 0, 12000);
    $ping['author'] = $ping['username'];
    $ping['date'] = $ping['creationDate'];
    $ping['facets'] = array_merge($ping['customAttributes'], $ping['dynUris']);
    $ping['useExtraction'] = empty($ping['dynUris']);
    $ping['context'] = $ping['spaceKey'];

    unset($ping['pageUrl']);
    unset($ping['username']);
    unset($ping['creationDate']);
    unset($ping['customAttributes']);
    unset($ping['dynUris']);
    unset($ping['spaceKey']);

    $result = $this->connection->post($resource_path, [
      'data' => drupal_json_encode($ping),
    ]);

    return $result === FALSE ? FALSE : TRUE;
  }

  /**
   * Delete an existing ping.
   *
   * @param string $page
   *   The URL of the page (= ID of the ping).
   * @param string $search_space_id
   *   The ID of the search space to delete the ping in.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function deletePing($page, $search_space_id = '') {
    $resource_path = $this->getApiPath() . 'content/delete/id';

    $result = $this->connection->post($resource_path, [
      'data' => drupal_json_encode(['identifier' => $page]),
    ]);

    return $result === FALSE ? FALSE : TRUE;
  }

  /**
   * Adds a new custom search field for the suggestion call.
   *
   * @param string $label
   *   The label of the custom search field.
   * @param string $field
   *   The name of the custom search field, e.g. 'content_type'.
   * @param string $search_space_id
   *   The ID of the search space to add the custom search field for.
   *
   * @return boolean
   *   TRUE if field is added, otherwise FALSE.
   */
  public function addCustomSearchField($label, $field, $search_space_id = '') {
    $resource_path = '/' . $this->graphSearchPath . '/admin/suggest/add';
    $field = 'dyn_lit_' . str_replace('-', '_', $field);
    $post_parameters = [
      'field' => $field,
      'label' => $label,
    ];

    $result = $this->connection->post($resource_path, [
      'data' => $post_parameters,
    ]);

    if ($result !== FALSE) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Deletes a custom search field for the suggestion call.
   *
   * @param string $field
   *   The name of the custom search field, e.g. 'content_type'.
   * @param string $search_space_id
   *   The ID of the search space to delete the custom search field for.
   *
   * @return boolean
   *   TRUE if field is deleted, otherwise FALSE.
   */
  public function deleteCustomSearchField($field, $search_space_id = '') {
    $resource_path = '/' . $this->graphSearchPath . '/admin/suggest/delete';
    $field = 'dyn_lit_' . str_replace('-', '_', $field);
    $post_parameters = [
      'field' => $field,
    ];

    $result = $this->connection->post($resource_path, [
      'data' => $post_parameters,
    ]);

    if ($result !== FALSE) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Changes the result array so that it is compatible with older version.
   *
   * @param array $result
   *   The result of the search API call.
   *
   * @return array
   *   The compatible result for older version.
   */
  protected function makeSearchCompatible($result) {
    if (empty($result['results'])) {
      return $result;
    }

    foreach ($result['results'] as &$item) {
      $item['customAttributes'] = [];
      if (empty($item['facetList'])) {
        unset($item['facetList']);
        continue;
      }

      foreach ($item['facetList'] as $facet) {
        switch ($facet['field']) {
          case self::ATTR_ALL_CONCEPTS:
            foreach ($facet['facets'] as $concept) {
              $item['customAttributes'][$facet['field']][] = [
                'prefLabel' => $concept['value'],
                'uri' => $concept['label'],
              ];
            }
            break;

          case self::ATTR_CONTENT_TYPE:
            $item['customAttributes'][$facet['field']][0] = $facet['facets'][0]['label'];
            break;

          case self::ATTR_AUTHOR:
          case self::ATTR_SENTIMENT:
          case self::ATTR_SOURCE:
            $item['customAttributes'][$facet['field']] = $facet['facets'][0]['label'];
            break;
        }
      }
      unset($item['facetList']);
    }

    return $result;
  }

  /**
   * Changes the result array so that it is compatible with older version.
   *
   * @param array $result
   *   The result of the search API call.
   *
   * @return array
   *   The compatible result for older version.
   */
  protected function makeSuggestCompatible($result) {
    if (!empty($result['results'])) {
      foreach ($result['results'] as &$concept) {
        $concept['id'] = $concept['value'];
      }
    }

    return $result;
  }

}

/**
 * Class SemanticConnectorSonrApi_6_1
 *
 * API Class for the version 6.1.
 */
class SemanticConnectorSonrApi_6_1 extends SemanticConnectorSonrApi_6_0 {

  /**
   * This method checks if the GraphSearch service exists and is running.
   *
   * @return bool
   *   TRUE if the service is available, FALSE if not
   */
  public function available() {
    $resource_path = $this->getApiPath() . 'heartbeat';
    $result = $this->connection->get($resource_path);

    return $result === '' ? TRUE : FALSE;
  }

  /**
   * This method gets the configuration of the PoolParty GraphSearch server.
   *
   * @return boolean|array
   *   project => The PoolParty project used for the extraction
   *   language => The configured language of the content.
   */
  public function getConfig() {
    $resource_path = $this->getApiPath() . 'context/all';
    $result = $this->connection->get($resource_path);

    if ($result === FALSE) {
      return FALSE;
    }

    $data = drupal_json_decode($result);

    $projects = array();
    foreach ($data as $searchSpace) {
      if (strtolower($searchSpace['integrationType']) != 'drupal') {
        continue;
      }
      foreach ($searchSpace['facetIds'] as $facetId) {
        if ($facetId['thesaurus']) {
          if (!isset($projects[$facetId['facetId']])) {
            $projects[$facetId['facetId']] = array(
              'id' => $facetId['facetId'],
              'search_spaces' => array(),
            );
          }
          $projects[$facetId['facetId']]['search_spaces'][$searchSpace['searchSpaceId']] = array(
            'id' => $searchSpace['searchSpaceId'],
            'name' => $searchSpace['searchSpaceName'],
            'language' => $searchSpace['defaultLanguage'],
          );
        }
      }
    }

    return array(
      'projects' => $projects
    );
  }

  /**
   * This method gets the field configuration of the PoolParty GraphSearch
   * server.
   *
   * @param string $search_space_id
   *   The ID of the search space to get the field config for.
   *
   * @return boolean|array
   *   searchFields -> all available fields in a list
   *   contextSearchFields -> all fields grouped by field context.
   */
  public function getFieldConfig($search_space_id = '') {
    $resource_path = $this->getApiPath() . 'context/fields';

    $get_parameters = array();
    if (!empty($search_space_id)) {
      $get_parameters['searchSpaceId'] = $search_space_id;
    }

    $result = $this->connection->get($resource_path, [
      'query' => $get_parameters,
    ]);
    $facet_list = drupal_json_decode($result);

    if (!is_array($facet_list)) {
      return FALSE;
    }

    // Make compatible with older version
    if (!empty($facet_list['searchFields'])) {
      foreach ($facet_list['searchFields'] as &$search_fields) {
        $search_fields['name'] = $search_fields['field'];
        $search_fields['type'] = $search_fields['searchFieldType'];
        if (isset($search_fields['sortDirection'])) {
          $search_fields['defaultSortDirection'] = $search_fields['sortDirection'];
        }
      }
    }

    return $facet_list;
  }

  /**
   * This method searches in the GraphSearch index.
   *
   * @param string $search_space_id
   *   The search space to use for the search.
   * @param array $facets
   *   A list of facet objects that should be used for faceting the
   *   search. [optional]
   * @param array $filters
   *   A list of filter object parameters that define the query. [optional]
   *    array(
   *      object(
   *        'field'    => (string)  facedID   | 'date-to' | 'date-from',
   *        'value'    => (int)     conceptID | timestamp | timestamp,
   *        'optional' => (boolean) TRUE, (default: TRUE)
   *      ),
   *      ...
   *    )
   * @param array $parameters
   *   A list of key value pairs [optional]
   *    array(
   *      'facetMinCount' => (int)    1,     (default:    1)
   *      'locale'        => (string) 'en',  (default: 'en')
   *      'start'         => (int)    0,     (default:    0)
   *      'count'         => (int)    10,    (default:   10)
   *      'sort'          => object(
   *        'field'     => (string) facetID | 'date',
   *        'direction' => (string) 'DESC' | 'ASC',
   *      ),   (default: object('field' => 'date', 'direction' => 'DESC')
   *    )
   *
   * @return boolean|array
   *   List of items or FALSE in case of an error
   */
  public function search($search_space_id = '', $facets = [], $filters = [], $parameters = []) {

    $resource_path = $this->getApiPath() . 'search';

    $sort = new stdClass();
    $sort->field = 'date';
    $sort->direction = 'DESC';

    $post_parameters = [
      'searchFacets' => $this->prepareFacets($facets),
      'searchFilters' => $this->prepareFilters($filters),
      'documentFacets' => $this->customAttributes,
      'facetMinCount' => isset($parameters['facetMinCount']) ? $parameters['facetMinCount'] : 1,
      'maxFacetCount' => isset($parameters['maxFacetCount']) ? $parameters['maxFacetCount'] : 10,
      'locale' => isset($parameters['locale']) ? $parameters['locale'] : 'en',
      'start' => isset($parameters['start']) ? $parameters['start'] : 0,
      'count' => isset($parameters['count']) ? $parameters['count'] : 10,
      'sort' => isset($parameters['sort']) ? $parameters['sort'] : $sort,
    ];

    if (!empty($search_space_id)) {
      $post_parameters['searchSpaceId'] = $search_space_id;
    }

    $result = $this->connection->post($resource_path, [
      'data' => drupal_json_encode($post_parameters),
    ]);

    $items = drupal_json_decode($result);

    if (!is_array($items)) {
      return FALSE;
    }

    // Make compatible with older version
    $items = $this->makeSearchCompatible($items);

    return $items;
  }

  /**
   * Get concept suggestions from a given search string.
   *
   * @param string $search_string
   *   The string to get suggestions for
   * @param string $search_space_id
   *   The ID of the search space to use for the suggestions.
   * @param array $parameters
   *   array(
   *    'locale' => (string) 'en',  (default: 'en')
   *    'count'  => (int)    10,    (default:   10)
   *  )
   *
   * @return boolean|array
   *   Array of concepts
   *   array(
   *    'id'      => (string) URI of concept
   *    'label'   => (string) prefLabel of concept
   *    'context' => (string) prefLabel of the broader concept
   *    'field'   => (string) URI of conceptScheme
   *  )
   */
  public function suggest($search_string, $search_space_id = '', $parameters = []) {
    $resource_path = $this->getApiPath() . 'suggest/multi';

    $get_parameters = array_merge([
      'searchString' => $search_string,
      'locale' => 'en',
      'count' => 10,
    ], $parameters);

    if (!empty($search_space_id)) {
      $get_parameters['searchSpaceId'] = $search_space_id;
    }

    $result = $this->connection->get($resource_path, [
      'query' => $get_parameters,
    ]);

    $concepts = drupal_json_decode($result);

    if (!is_array($concepts)) {
      return FALSE;
    }

    // Make compatible with older version
    $concepts = $this->makeSuggestCompatible($concepts);

    return $concepts;
  }

  /**
   * Get similar content.
   *
   * @param mixed $data
   *   The ID of the document or a text depending on $type.
   * @param string $search_space_id
   *   The ID of the search to use to get similar content.
   * @param array $parameters
   *   Array of the parameters
   * @param string $type
   *   The type of data [id | text]
   *
   * @return boolean|array
   *   A key value pair list of facets or FALSE in case of an error
   */
  public function getSimilar($data, $search_space_id = '', $parameters = [], $type = 'id') {
    $resource_path = $this->getApiPath() . 'similar';
    $type = ($type == 'id') ? 'id' : 'text';
    $post_parameters = array_merge([
      $type => $data,
      'locale' => 'en',
      'start' => 0,
      'count' => 10,
      'fields' => ['dyn_uri_all_concepts', 'title', 'description'],
      'documentFacets' => $this->customAttributes,
    ], $parameters);

    if (!empty($search_space_id)) {
      $post_parameters['searchSpaceId'] = $search_space_id;
    }

    $result = $this->connection->post($resource_path, [
      'data' => drupal_json_encode($post_parameters),
    ]);

    $similar = drupal_json_decode($result);

    if (!is_array($similar)) {
      return FALSE;
    }

    // Make compatible with older version
    $similar = $this->makeSearchCompatible($similar);

    return $similar;
  }

  /**
   * Get the concepts, free terms and recommended content for a given text.
   *
   * @param string $text
   *   The text for the recommendation.
   * @param string $search_space_id
   *   The ID of the search space to use for the recommendation.
   * @param array $parameters
   *   array(
   *     'language' => (string) 'en', (default: 'en')
   *   )
   *
   * @return boolean|array
   *   List of concepts, free terms and recommended content or FALSE in case of
   *   an error
   */
  public function getRecommendation($text, $search_space_id = '', $parameters = []) {
    $resource_path = $this->getApiPath() . 'recommend';
    $post_parameters = array_merge([
      'text' => $text,
      'locale' => 'en',
      'start' => 0,
      'count' => 10,
      'fields' => ['dyn_uri_all_concepts', 'title', 'description'],
      'documentFacets' => $this->customAttributes,
    ], $parameters);

    if (!empty($search_space_id)) {
      $post_parameters['searchSpaceId'] = $search_space_id;
    }

    $result = $this->connection->post($resource_path, [
      'data' => drupal_json_encode($post_parameters),
    ]);

    $recommendations = json_decode($result);

    if (!is_object($recommendations)) {
      return FALSE;
    }

    // property 'prefLabel' is named 'label' now --> add prefLabel for
    // backwards compatibility.
    if (isset($recommendations->concepts)) {
      foreach ($recommendations->concepts as &$annotation) {
        if (!isset($annotation->prefLabel)) {
          $annotation->prefLabel = $annotation->label;
        }
        unset($annotation);
      }
    }

    return $recommendations;
  }

  /**
   * Get all agents with their configuration and status.
   *
   * @param string $search_space_id
   *   The ID of the search to get the agents for.
   *
   * @return boolean|array
   *   A list of agents with their configuration and status
   */
  public function getAgents($search_space_id = '') {
    $resource_path = $this->getApiPath() . 'agents/status';

    $get_parameters = [];
    if (!empty($search_space_id)) {
      $get_parameters['searchSpaceId'] = $search_space_id;
    }
    $result = $this->connection->get($resource_path, [
      'query' => $get_parameters,
    ]);

    $agent_list = drupal_json_decode($result);

    if (!is_array($agent_list)) {
      return FALSE;
    }

    $agents = [];
    if (!is_null($agent_list)) {
      foreach ($agent_list as $id => $agent) {
        $agents[$id] = new stdClass();
        $agents[$id]->id = $agent['agent']['id'];
        $agents[$id]->configuration = $agent['agent']['configuration'];
        $agents[$id]->status = $agent['status'];
      }
      usort($agents, [$this, 'sortAgents']);
    }

    return $agents;
  }

  /**
   * Get one agent with his configuration.
   *
   * @param int $agent_id
   *   The ID of the agent
   * @param string $search_space_id
   *   The ID of the search to get the agent for.
   *
   * @return boolean|array
   *   The configuration of a given agent or FALSE in case of an error
   */
  public function getAgent($agent_id, $search_space_id = '') {
    $resource_path = $this->getApiPath() . 'agents/config';

    $get_parameters = [
      'id' => $agent_id,
    ];
    if (!empty($search_space_id)) {
      $get_parameters['searchSpaceId'] = $search_space_id;
    }
    $result = $this->connection->get($resource_path, [
      'query' => $get_parameters,
    ]);

    $agent = drupal_json_decode($result);

    if (!is_array($agent)) {
      return FALSE;
    }

    $agent['id'] = $agent_id;

    return $agent;
  }

  /**
   * Add a new agent.
   *
   * @param array $config
   *   array(
   *    'source'          => (string) 'My Source',
   *    'url'             => (string) 'http://example.com/rss.xml'
   *    'username'        => (string) 'admin',
   *    'periodMillis'    => (int) 3600000,
   *   )
   * @param string $search_space_id
   *   The ID of the search to create the agent for.
   *
   * @return bool
   *   TRUE on success, FALSE on error
   */
  public function addAgent($config, $search_space_id = '') {
    $config['privateContent'] = FALSE;
    $config['context'] = '';

    if (!empty($search_space_id)) {
      $config['searchSpaceId'] = $search_space_id;
    }

    $resource_path = $this->getApiPath() . 'agents/create';

    $result = $this->connection->post($resource_path, [
      'data' => drupal_json_encode($config),
    ]);

    return $result === FALSE ? FALSE : TRUE;
  }

  /**
   * Update an agent.
   *
   * @param int $agent_id
   *   The ID of the agent.
   * @param array $config
   *   array(
   *    'source'          => (string) 'My Source',
   *    'url'             => (string) 'http://example.com/rss.xml'
   *    'username'        => (string) 'admin',
   *    'periodMillis'    => (int) 3600000,
   *   )
   * @param string $search_space_id
   *   The ID of the search space the agent was created for.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function updateAgent($agent_id, $config, $search_space_id = '') {
    $config['privateContent'] = FALSE;
    $config['context'] = '';

    if (!empty($search_space_id)) {
      $config['searchSpaceId'] = $search_space_id;
    }

    $resource_path = $this->getApiPath() . 'agents/update';

    $result = $this->connection->post($resource_path, [
      'data' => drupal_json_encode($config),
      'query' => ['id' => $agent_id],
    ]);

    return $result === FALSE ? FALSE : TRUE;
  }

  /**
   * Delete an agent.
   *
   * @param int $agent_id
   *   The ID of the agent.
   * @param string $search_space_id
   *   The ID of the search space the agent was created for.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function deleteAgent($agent_id, $search_space_id = '') {
    $resource_path = $this->getApiPath() . 'agents/delete';

    $query_parameters = [
      'id' => $agent_id,
    ];
    if (!empty($search_space_id)) {
      $query_parameters['searchSpaceId'] = $search_space_id;
    }

    $result = $this->connection->post($resource_path, [
      'query' => $query_parameters,
      'data' => '',
    ]);

    return $result === FALSE ? FALSE : TRUE;
  }

  /**
   * Run an agent.
   *
   * @param int $agent_id
   *   The ID of the agent.
   * @param string $search_space_id
   *   The ID of the search space the agent was created for.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function runAgent($agent_id, $search_space_id = '') {
    $resource_path = $this->getApiPath() . 'agents/runAgent';

    $query_parameters = [
      'id' => $agent_id,
    ];
    if (!empty($search_space_id)) {
      $query_parameters['searchSpaceId'] = $search_space_id;
    }

    $result = $this->connection->post($resource_path, [
      'query' => $query_parameters,
      'data' => '',
      'timeout' => 120,
    ]);

    return $result === FALSE ? FALSE : TRUE;
  }

  /**
   * Update an existing ping.
   *
   * @param array $ping
   *   array(
   *    'title'         => (string) Title of the ping
   *    'text'          => (string) Content of the ping
   *    'username'      => (string) 'admin',
   *    'creationDate'  => (int) unix timestamp,
   *    'pageUrl'       => (string) node URL --> will become the ID,
   *    'spaceKey'      => (string) 'extern', ... not relevant for Drupal.
   *    'dynUris'{      => (object) Tags of the content and term references
   *      'dyn_uri_all_concepts": [
   *        'http://server.com/Project/Concept1',
   *        'http://server.com/Project/Concept2',
   *        'http://server.com/Project/Concept3'
   *        ]
   *    }
   *  )
   * @param string $search_space_id
   *   The ID of the search space to update the ping in.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function updatePing(array $ping, $search_space_id = '') {
    $resource_path = $this->getApiPath() . 'content/update';
    $ping['identifier'] = $ping['pageUrl'];
    $ping['text'] = substr($ping['text'], 0, 12000);
    $ping['author'] = $ping['username'];
    $ping['date'] = $ping['creationDate'];
    $ping['facets'] = array_merge($ping['customAttributes'], $ping['dynUris']);
    $ping['useExtraction'] = empty($ping['dynUris']);
    $ping['context'] = $ping['spaceKey'];

    if (!empty($search_space_id)) {
      $ping['searchSpaceId'] = $search_space_id;
    }

    unset($ping['pageUrl']);
    unset($ping['username']);
    unset($ping['creationDate']);
    unset($ping['customAttributes']);
    unset($ping['dynUris']);
    unset($ping['spaceKey']);

    $result = $this->connection->post($resource_path, [
      'data' => drupal_json_encode($ping),
    ]);

    return $result === FALSE ? FALSE : TRUE;
  }

  /**
   * Delete an existing ping.
   *
   * @param string $page
   *   The URL of the page (= ID of the ping).
   * @param string $search_space_id
   *   The ID of the search space to delete the ping in.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function deletePing($page, $search_space_id = '') {
    $resource_path = $this->getApiPath() . 'content/delete/id';

    $post_parameters = [
      'identifier' => $page
    ];
    if (!empty($search_space_id)) {
      $post_parameters['searchSpaceId'] = $search_space_id;
    }

    $result = $this->connection->post($resource_path, [
      'data' => drupal_json_encode($post_parameters),
    ]);

    return $result === FALSE ? FALSE : TRUE;
  }

  /**
   * Delete all indexed documents from an agent.
   *
   * @param string $source
   *   The name of the source.
   * @param string $search_space_id
   *   The ID of the search space to delete documents from.
   *
   * @return bool
   *   TRUE on success, FALSE on error.
   */
  public function deleteIndex($source, $search_space_id = '') {
    $resource_path = $this->getApiPath() . 'content/delete/source';

    $post_parameters = [
      'source' => $source
    ];
    if (!empty($search_space_id)) {
      $post_parameters['searchSpaceId'] = $search_space_id;
    }

    $result = $this->connection->post($resource_path, [
      'data' => $post_parameters,
    ]);

    return $result === FALSE ? FALSE : TRUE;
  }

  /**
   * Get trends from a list of concepts.
   *
   * @param array $uris
   *   A list of uris of concepts.
   * @param string $search_space_id
   *   The search space to get the trends for.
   *
   * @return boolean|array
   *   List of trends.
   */
  public function getTrends($uris, $search_space_id = '') {
    $resource_path = $this->getApiPath() . 'trend/histories';

    if (is_string($uris)) {
      $uris = [$uris];
    }

    $get_parameters = [
      'concepts' => implode(',', $uris)
    ];
    if (!empty($search_space_id)) {
      $get_parameters['searchSpaceId'] = $search_space_id;
    }

    $result = $this->connection->get($resource_path, [
      'query' => $get_parameters,
    ]);

    $trends = drupal_json_decode($result);

    if (!is_array($trends)) {
      return FALSE;
    }

    return $trends;
  }

  /**
   * Adds a new custom search field for the suggestion call.
   *
   * @param string $label
   *   The label of the custom search field.
   * @param string $field
   *   The name of the custom search field, e.g. 'content_type'.
   * @param string $search_space_id
   *   The ID of the search space to add the custom search field for.
   *
   * @return boolean
   *   TRUE if field is added, otherwise FALSE.
   */
  public function addCustomSearchField($label, $field, $search_space_id = '') {
    $resource_path = '/' . $this->graphSearchPath . '/admin/suggest/add';
    $field = 'dyn_lit_' . str_replace('-', '_', $field);
    $post_parameters = [
      'field' => $field,
      'label' => $label,
    ];
    if (!empty($search_space_id)) {
      $post_parameters['searchSpaceId'] = $search_space_id;
    }

    $result = $this->connection->post($resource_path, [
      'data' => $post_parameters,
    ]);

    if ($result !== FALSE) {
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Deletes a custom search field for the suggestion call.
   *
   * @param string $field
   *   The name of the custom search field, e.g. 'content_type'.
   * @param string $search_space_id
   *   The ID of the search space to delete the custom search field for.
   *
   * @return boolean
   *   TRUE if field is deleted, otherwise FALSE.
   */
  public function deleteCustomSearchField($field, $search_space_id = '') {
    $resource_path = '/' . $this->graphSearchPath . '/admin/suggest/delete';
    $field = 'dyn_lit_' . str_replace('-', '_', $field);
    $post_parameters = [
      'field' => $field,
    ];
    if (!empty($search_space_id)) {
      $post_parameters['searchSpaceId'] = $search_space_id;
    }

    $result = $this->connection->post($resource_path, [
      'data' => $post_parameters,
    ]);

    if ($result !== FALSE) {
      return TRUE;
    }

    return FALSE;
  }

}

/**
 * Class SemanticConnectorSonrApi_7_0
 *
 * API Class for the version 7.0.
 */
class SemanticConnectorSonrApi_7_0 extends SemanticConnectorSonrApi_6_1 {

  /**
   * Changes the result array so that it is compatible with older version.
   *
   * @param array $result
   *   The result of the search API call.
   *
   * @return array
   *   The compatible result for older version.
   */
  protected function makeSearchCompatible($result) {
    $result['results'] = $result['result'];
    unset($result['result']);

    return parent::makeSearchCompatible($result);
  }

  /**
   * Changes the result array so that it is compatible with older version.
   *
   * @param array $result
   *   The result of the search API call.
   *
   * @return array
   *   The compatible result for older version.
   */
  protected function makeSuggestCompatible($result) {
    $result['results'] = $result['result'];
    unset($result['result']);

    return parent::makeSuggestCompatible($result);
  }

}