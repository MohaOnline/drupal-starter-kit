<?php

/**
 * @file
 * The PoolParty Thesaurus (PPT) API class. PoolParty >= 4.6 is supported.
 */

/**
 * Abstract Class SemanticConnectorPPTApi
 *
 * API Class for the PoolParty Thesaurus.
 */
abstract class SemanticConnectorPPTApi {

  protected $connection;
  protected $apiVersion;

  /**
   * The constructor of the PoolParty Thesaurus class.
   *
   * @param string $endpoint
   *   URL of the endpoint of the PoolParty-server.
   * @param string $credentials
   *   Username and password if required (format: "username:password").
   */
  public function __construct($endpoint, $credentials = '') {
    $this->connection = new SemanticConnectorCurlConnection($endpoint, $credentials);
    $this->apiVersion = str_replace(array('SemanticConnectorPPTApi_', '_'), array('', '.'), get_class($this));
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
   * Get the path to the PPT API.
   * 
   * @return string
   *   The path to the PPT API.
   */
  public function getApiPath() {
    // Use API versioning for version 6.1+.
    return '/PoolParty/api/' . (version_compare($this->apiVersion, '6.1', '>=') ? $this->apiVersion . '/' : '');
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
   * Get the version of the installed PoolParty web service.
   *
   * @return string
   *   The PoolParty version formatted like '4.1.6'
   */
  public function getVersion() {
    return '';
  }

  /**
   * Get a list of available projects of a PoolParty server.
   *
   * @return array
   *   A list of projects.
   */
  public function getProjects() {
    return array();
  }

  /**
   * Create a new project on a PoolParty server.
   *
   * @param string $title
   *   The title of the project.
   * @param string $language_default
   *   The default language of the project.
   * @param array $user_groups
   *   An arary of PoolParty user groups to make this project available for.
   * @param array $settings
   *   An associative array of optional settings. Possible values are:
   *   - author: (string) Project author
   *   - availableLanguages: (string[]) Additional languages
   *   - baseURL: (string) Base URL of the project
   *   - contributor: (string) Project contributor
   *   - description: (string) Project description
   *   - enableSkosXl: (bool) Enable SKOS-XL
   *   - enableWorkflow: (bool) Enable Workflow
   *   - idGeneration: (string) ID-Generation Method. Possible Values: increment, prefLabel, uuid, manual
   *   - incrementStart: (int) Increment start only needed when ID-Generation is 'increment'
   *   - license: (string) Project licensce. Must be a valid URI
   *   - projectIdentifier: (string) Project Identifier
   *   - publisher: (string) Project publisher
   *   - qualitySetting: (string) Quality Setting. Possible Values:default, autoIndexing, classification, documentsuggestion, restricted, disabled
   *   - repositoryType: (string) Repository Type. Possible Values: memory, native
   *   - snapshotInterval: (int) SnapshotInterval. Possible Values:5,10,15,30,-1:disabled
   *   - subject: (string) Primary subject of the project
   *   - workflowAssignee: (string) URI of the user that will get assigned to the workflow. Only used when workflow enabled
   *   - workflowState: (string) State of the workflow. Possible values: DRAFT, APPROVED
   *
   * @return string|bool
   *   A list of projects.
   */
  public function createProject($title, $language_default = 'en', $user_groups = array('Public'), $settings = array()) {
    return FALSE;
  }

  /**
   * Export data of a project as a file and store it on the server.
   *
   * @param string $project_id
   *   The ID of the PoolParty project to export and store.
   * @param string $format
   *   The returned RDF format.
   *   Possible values are: TriG, N3, Turtle, N-Triples, RDF/XML, TriX
   * @param string $export_modules
   *   A list of the export modules for the data that should be exported.
   *   Possible values are:
   *   - concepts - includes concept schemes, concepts, collections and notes
   *   - workflow - workflow status for all concepts
   *   - history - all history events
   *   - freeConcepts - all free concepts
   *   - void - the project VoiD graph
   *   - adms - the project ADMS graph
   *
   * @return string
   *   The URL of the stored file or an empty string if an error occurred.
   */
  public function storeProject($project_id, $format = 'RDF/XML', $export_modules = 'concepts') {
    return '';
  }

  /**
   * Gets information about a concept scheme.
   *
   * @param string $project_id
   *   The ID of the PoolParty project of the concepts.
   * @param string $language
   *   Only concepts with labels in this language will be displayed. If no
   *   language is given, the default language of the project will be used.
   *
   * @return stdClass[]
   *   An array of concept scheme objects within the respective PoolParty project.
   */
  public function getConceptSchemes($project_id, $language = '') {
    return array();
  }

  /**
   * Gets a list of all top concepts of a specific concept scheme.
   *
   * @param string $project_id
   *   The ID of the PoolParty project.
   * @param string $scheme_uri
   *   The URI of the concept scheme.
   * @param array $properties
   *   A list of additional properties to fetch (e.g. skos:altLabel, skso:hiddenLabel).
   * @param string $language
   *   Only concepts with labels in this language will be displayed. If no
   *   language is given, the default language of the project will be used.
   *
   * @return array
   *   A list of top concepts.
   */
  public function getTopConcepts($project_id, $scheme_uri, array $properties = array(), $language = '') {
    return array();
  }

  /**
   * Get a list of all concepts under a specified concept in a tree format.
   *
   * @param int $project_id
   *   The ID of the PoolParty project.
   * @param string $uri
   *   A concept URI.
   * @param array $properties
   *   A list of additional properties to fetch (e.g. skos:altLabel, skso:hiddenLabel).
   * @param string $language
   *   Only concepts with labels in this language will be displayed. If no
   *   language is given, the default language of the project will be used.
   *
   * @return array
   *   A list of concept objects in a tree format.
   */
  public function getSubTree($project_id, $uri, array $properties = array(), $language = '') {
    return array();
  }

  /**
   * Get information about concepts.
   *
   * @param string $project_id
   *   The ID of the PoolParty project of the concepts.
   * @param array $concept_uris
   *   An array of concept URIs to get information for.
   * @param array $properties
   *   Array of additional concept properties that will be fetched (only
   *   properties uri and prefLabel are included by default). Possible values
   *   are:
   *   - skos:altLabel
   *   - skos:hiddenLabel
   *   - skos:definition
   *   - skos:broader
   *   - skos:narrower
   *   - skos:related
   *   - skos:ConceptScheme
   * @param string $language
   *   Only concepts with labels in this language will be displayed. If no
   *   language is given, the default language of the project will be used.
   *
   * @return array
   *   Array of concept objects within the respective PoolParty project with
   *   following properties:
   *   - uri --> URI of the concept
   *   - prefLabel --> Preferred label
   *   - altLabels --> Alternative labels
   *   - hiddenLabels --> Hidden labels
   *   - definitions --> Definitions
   *   - broaders --> Broader concepts
   *   - narrowers --> Narrower concepts
   *   - relateds --> Related concepts
   *   - conceptSchemes --> Concept schemes
   */
  public function getConcepts($project_id, array $concept_uris, array $properties = array(), $language = NULL) {
    return array();
  }

  /**
   * Get information about a concept.
   *
   * @param string $project_id
   *   The ID of the PoolParty project of the concepts.
   * @param string $concept_uri
   *   The concept URI, from which the data should be retrieved.
   * @param string[] $properties
   *   Array of additional concept properties that will be fetched (only
   *   properties uri and prefLabel are included by default). Possible values
   *   are:
   *   - skos:altLabel
   *   - skos:hiddenLabel
   *   - skos:definition
   *   - skos:broader
   *   - skos:narrower
   *   - skos:related
   *   - skos:ConceptScheme
   * @param string $language
   *   Only concepts with labels in this language will be displayed. If no
   *   language is given, the default language of the project will be used.
   *
   * @return stdClass
   *   A concept object within the respective PoolParty project with
   *   following properties:
   *   - uri --> URI of the concept
   *   - prefLabel --> Preferred label
   *   - altLabels --> Alternative labels
   *   - hiddenLabels --> Hidden labels
   *   - definitions --> Definitions
   *   - broaders --> Broader concepts
   *   - narrowers --> Narrower concepts
   *   - relateds --> Related concepts
   *   - conceptSchemes --> Concept schemes
   */
  public function getConcept($project_id, $concept_uri, array $properties = array(), $language = '') {
    return new stdClass();
  }

  /**
   * Creates a concept scheme in a specific project.
   *
   * @param string $project_id
   *   The ID of the PoolParty project in which the concept scheme should be created.
   * @param string $title
   *   The title of the new concept scheme.
   * @param string $description
   *   A description for the new concept scheme.
   * @param string $creator
   *   The name of the creator of the new concept scheme.
   *
   * @return string
   *   The URI of the new concept scheme.
   */
  public function createConceptScheme($project_id, $title, $description, $creator = 'Drupal') {
    return '';
  }

  /**
   * Creates a new concept in a specific project.
   *
   * @param string $project_id
   *   The ID of the PoolParty project in which the concept should be created.
   * @param string $parent
   *   The URI of the parent concept or concept scheme of the new concept.
   * @param string $prefLabel
   *   The label in the default language of the project.
   *
   * @return string
   *   The URI of the new concept.
   */
  public function createConcept($project_id, $parent, $prefLabel) {
    return '';
  }

  /**
   * Adds a SKOS relation between two existing concepts
   *
   * @param string $project_id
   *   The ID of the PoolParty project.
   * @param string $source
   *   The URI of the source concept.
   * @param string $target
   *   The URI of the target concept.
   * @param string $property
   *   The relation property. Possible values are:
   *   - broader
   *   - narrower
   *   - related
   *   - hasTopConcept
   *   - topConceptOf
   *
   * @return mixed
   *  Status: 200 - OK
   */
  public function addRelation($project_id, $source, $target, $property = 'broader') {
    return '';
  }

  /**
   * Adds a literal to an existing concept
   *
   * @param string $project_id
   *  The ID of the PoolParty project.
   * @param string $concept_uri
   *  The URI of the Concept.
   * @param string $property
   *  The SKOS property. Possible values are:
   *  - preferredLabel
   *  - alternativeLabel
   *  - hiddenLabel
   *  - definition
   *  - scopeNote
   *  - example
   *  - notation
   * @param string $label
   *  The RDF literal to add.
   * @param null $language
   *  The attribute language.
   *
   * @return mixed
   *  Status: 200 - OK
   */
  public function addLiteral($project_id, $concept_uri, $property, $label, $language = NULL) {
    return '';
  }

  /**
   * Adds a literal to an existing concept
   *
   * @param string $project_id
   *  The ID of the PoolParty project.
   * @param string $concept_uri
   *  The URI of the Concept to add the property to.
   * @param string $attribute_uri
   *  The URI of the custom attribute property.
   * @param string $value
   *  The attribute value that should be added
   * @param null $language
   *  The attribute language.
   *
   * @return mixed
   *  Status: 200 - OK
   */
  public function addCustomAttribute($project_id, $concept_uri, $attribute_uri, $value, $language = NULL) {
    return '';
  }

  /**
   * Get all history items of a PoolParty project.
   *
   * @param string $project_id
   *   The ID of the project to get history items for.
   * @param int $from_time
   *   Optional; Only history items after this time will be included.
   * @param int $to_time
   *   Optional; Only history items before this time will be included.
   * @param string[] $events
   *   Optional; Filter by event type.
   *   Possible values: resourceChangeAddition, resourceChangeRemoval,
   *     resourceChangeUpdate, addRelation, removeRelation, addLiteral,
   *     removeLiteral, updateLiteral, addCollectionMember,
   *     removeCollectionMember, createCollection, deleteCollection,
   *     importConcept, resourceChangeAddition, addCustomAttributeLiteral,
   *     removeCustomAttributeLiteral ,updateCustomAttributeLiteral,
   *     addCustomRelation, removeCustomRelation, addCustomClass,
   *     removeCustomClass
   *
   * @return array
   *   An array of history items.
   */
  public function getHistory($project_id, $from_time = NULL, $to_time = NULL, $events = array()) {
    return array();
  }

  /**
   * Returns a list of PoolParty user group names
   *
   * @return string[]
   *   Array of PoolParty user groups.
   */
  public function getUserGroups() {
    return array();
  }

  /**
   * Get the languages available in the PoolParty.
   *
   * @return array
   *   An associative array of available languages (iso-code --> label).
   */
  public function getLanguages() {
    return array();
  }

  /**
   * Get information about the extraction model for a PP project.
   *
   * @param string $project_id
   *   The ID of the PP project to get the extraction model info for.
   *
   * @return array|bool
   *   Associative array of extraction model info or FALSE in case of an error.
   *   Following keys are included:
   *   - lastBuildTime (string) --> Last extraction model build time
   *   - lastChangeTime (string) --> Last thesaurus change
   *   - upToDate (bool) --> Whether the extraction model is up-to-date or not
   */
  public function getExtractionModelInfo($project_id) {
    return FALSE;
  }

  /**
   * Refresh the extraction model for a PP project
   *
   * @param string $project_id
   *   The ID of the PP project to refresh the extraction model for.
   *
   * @return array
   *   An associative array informing about the success of the refreshing
   *   containing following keys:
   *   - success (bool) --> TRUE if the refreshing worked, FALSE if not
   *   - message (string) --> This property is optional, but if it exists it
   *       includes more details about why the connection could not be
   *       established.
   *   - since PP 6.0 also "plainMessage" and "reportable"
   */
  public function refreshExtractionModel($project_id) {
    return array('success' => FALSE);
  }

  /**
   * Get the corpora available for a PoolParty project.
   *
   * @param string $project_id
   *   The ID of the PP project to refresh the extraction model for.
   *
   * @return array
   *   An array of associative corpus arrays containing following properties:
   *   - corpusId (string) --> Corpus id
   *   - corpusName (string) --> Corpus name
   *   - language (string) --> Language of corpus (en|de|es|fr|...)
   *   - upToDate (boolean) --> Up to date flag
   */
  public function getCorpora($project_id) {
    return array();
  }

  /**
   * Push text into a PoolParty corpus.
   *
   * @param string $project_id
   *   The ID of the PP project to use.
   * @param string $corpus_id
   *   The ID of the corpus to add the text to.
   * @param string $title
   *   The title of the document to push into the corpus.
   * @param mixed $data
   *   The text to push into the corpus.
   * @param string $data_type
   *   The type of the data. Can be one of the following values:
   *   - "text" for a text
   *   - "file" for a file object with a file ID
   *   - "file direct" for all other files without an ID
   * @param boolean $check_language
   *   Checks if the language of the uploaded file and the language of the
   *   corpus are the same.
   *
   * @return mixed
   *  Status: 200 - OK
   */
  public function addDataToCorpus($project_id, $corpus_id, $title, $data, $data_type, $check_language = TRUE) {
    return '';
  }

  /**
   * Get the metadata (additional information) of a corpus.
   *
   * @param string $project_id
   *   The ID of the PP project to get the corpus metadata for.
   * @param string $corpus_id
   *   The ID of the corpus to get the metadata for.
   *
   * @return array
   *   An array of associative corpus arrays containing following properties:
   *   - corpusName (string) --> Corpus name
   *   - corpusId (string) --> Corpus id
   *   - language (string) --> Language of corpus (en|de|es|fr|...)
   *   - createdBy (string) --> Corpus created by user
   *   - created (string) --> Time of creation of the corpus
   *   - extractedTerms (int) --> Number of unique free terms
   *   - concepts (int) --> Number of unique concepts
   *   - suggestedTermOccurrences (int) --> Number of free terms
   *   - conceptOccurrences (int) --> Number of concepts
   *   - quality (string) --> Quality of the corpus
   *     Possible values are "good", "moderate" and "poor"
   *   - overallFileSize (string) --> Overall file size of documents in the corpus
   *   - lastModified (string) --> Last modification date of the corpus
   *   - storedDocuments (int) --> Number of stored documents in the corpus
   */
  public function getCorpusMetadata($project_id, $corpus_id) {
    return NULL;
  }

  /**
   * Check if a corpus is up to date (or has to by analysed in case it is not).
   *
   * @param string $project_id
   *   The ID of the PP project of the corpus to check.
   * @param string $corpus_id
   *   The ID of the corpus to check.
   *
   * @return boolean
   *   TRUE if the corpus is up to date, FALSE if not
   */
  public function isCorpusUpToDate($project_id, $corpus_id) {
    return FALSE;
  }

  /**
   * Start the analysis of a corpus.
   *
   * @param string $project_id
   *   The ID of the PP project of the corpus.
   * @param string $corpus_id
   *   The ID of the corpus to start the analysis for.
   *
   * @return array
   *   An associative array informing about the success of the analysis
   *   containing following keys:
   *   - success (bool) --> TRUE if the analysis worked, FALSE if not
   *   - message (string) --> This property is optional, but if it exists it
   *       includes more details about why the connection could not be
   *       established.
   *   - since PP 6.0 also "plainMessage" and "reportable"
   */
  public function analyzeCorpus($project_id, $corpus_id) {
    return array('success' => FALSE);
  }

  /**
   * Check if a corpus analysis is running for a project (only one analysis can
   * run per project at a time).
   *
   * @param string $project_id
   *   The ID of the PP project of the corpus to check.
   *
   * @return boolean
   *   TRUE if a corpus is running for that project, FALSE if not
   */
  public function isCorpusAnalysisRunning($project_id) {
    return FALSE;
  }

  /**
   * Returns the license of this PoolParty installation.
   *
   * @return array|boolean
   *   An associative array of information about the license or FALSE in case
   *   of an error. The array includes following keys:
   *   - expiryDate (String) --> e.g. "2049-12-31T00:00:00.000+01:00"
   *   - expiryDateInMillis (long) --> e.g. 2524518000000
   *   - features (Array of String) --> e.g. ["some feature", "other feature"]
   *   - ipConstraint (String) --> e.g. "*"
   *   - issueDate (String) --> e.g. "2018-01-01T00:00:00.000+01:00"
   *   - issueDateInMillis (long) --> e.g. 1514761200000
   *   - licensee (String) --> text describing the licensee
   *   - macConstraint (String) --> e.g. "*"
   *   - maximumVersion (String) --> "6.0.1"
   *   - minimumVersion (String) --> "7.0.1"
   *   - type (String) --> e.g. "https://semantic-web.com/api/type#15230"
   *   - uid (String) --> User ID of the license creator
   *   - valid (boolean) --> if the license is valid
   */
  public function getLicense() {
    return FALSE;
  }

  /**
   * Suggest a set of concepts.
   *
   * @param string $project_id
   *   The ID of the project to suggest the concept for.
   * @param array $concepts
   *   An array of information about the concept to suggest. Following keys are
   *   supported:
   *   - prefLabels (Array of LanguageLiteral) --> Suggested preferred labels of
   *     the new concept - at least one must be in the project default language
   *   - broaderConcept (Array of IRI) --> Suggested broader concepts of the new
   *     concept. Optional
   *   - checkForDuplicates (boolean) --> If another Concept with the same
   *     preferred label already exists, an error will be thrown. (Default: true)
   *   - definition (Array of LanguageLiteral) --> Textual definitions of the
   *     new concept. Optional
   *   - note (String) --> Notes describing the new concept. Optional
   *   - relatedConcept (Array of IRI) --> Suggested related concepts of the new
   *     concept. Optional
   *   - score (float) --> Custom score for the new concept between 0 and 1.
   *     Optional
   *   - suffix (String) --> Suffix for manual uri creation. Optional
   *   - topConceptOf (Array of IRI) --> Schemes of which the suggested concept
   *     is a top concept. Optional
   *
   * @return array|bool
   *   An array or concept-arrays containing a uri-key or FALSE in case of an error.
   */
  public function suggestConcepts($project_id, array $concepts) {
    return FALSE;
  }

  /**
   * Get the suggested concepts for a PoolParty project.
   *
   * @param string $project_id
   *   The ID of the project to get the suggested concepts for.
   * @param int $offset
   *   Start index from where the suggested concepts results should be returned.
   * @param int $number
   *   The number of retrieved suggested concepts. Use 0 to get all suggested
   *   concepts.
   *
   * @return array
   *   An array of suggested concept arrays containing following keys:
   *   - broaderConcepts (Array of IRI) --> Broader Concepts of the Suggested Concept
   *   - date (String) --> Creation date of the Suggested Concept
   *   - definitions (Array of Literal) --> Definitions of the Suggested Concept
   *   - note (String) --> Note of the Suggested Concept
   *   - prefLabels (Array of Literal) --> Preferred Labels URI of the Suggested Concept
   *   - relatedConcepts (Array of IRI) --> Related Concepts of the Suggested Concept
   *   - score (float) --> Score of the Suggested Concept
   *   - topConceptOf (Array of IRI) --> Schemes in which the Suggested Concept is a top concept.
   *   - uri (IRI) --> URI of the Suggested Concept
   */
  public function getSuggestedConcepts($project_id, $offset = 0, $number = 0) {
    return array();
  }

  /**
   * Get all classifiers for a specific PoolParty project.
   *
   * @param string $project_id
   *   The project UUID to get the classifiers for.
   *
   * @return array
   *   An array of classifiers, each one is an associative array including
   *   following keys:
   *   - isOnline (boolean) --> Online status
   *   - language (String) --> Language of classifier (en|de|es|fr|...)
   *   - name (String) --> Classifier name
   *   - status (String) --> Status
   *   - uri (String) --> Classifier id
   */
  public function getClassifiers($project_id) {
    return [];
  }
}

/**
 * Class SemanticConnectorPPTApi_4_6
 *
 * API Class for the version 4.6
 */
class SemanticConnectorPPTApi_4_6 extends SemanticConnectorPPTApi {

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
    $is_available = array(
      'success' => FALSE,
      'message' => '',
    );
    $resource_path = $this->getApiPath() . 'version';
    $result = $this->connection->get($resource_path, array('headers' => array('Accept' => 'text/plain')));

    if (is_string($result) && !empty($result)) {
      $is_available['success'] = TRUE;
    }
    else {
      $is_available['message'] = 'PoolParty server is not available';
    }

    return $is_available;
  }

  /**
   * Get the version of the installed PoolParty web service.
   *
   * @return string
   *   The PoolParty version formatted like '4.6'
   */
  public function getVersion() {
    $resource_path = $this->getApiPath() . 'version';
    return $this->connection->get($resource_path, array('headers' => array('Accept' => 'text/plain')));
  }

  /**
   * Get a list of available projects of a PoolParty server.
   *
   * @return array
   *   A list of projects.
   */
  public function getProjects() {
    $resource_path = $this->getApiPath() . 'projects';
    $result = $this->connection->get($resource_path);
    $projects = json_decode($result);

    if (is_array($projects)) {
      foreach ($projects as &$project) {
        if (property_exists($project, 'uriSupplement')) {
          $project->sparql_endpoint_url = $this->connection->getEndpoint() . '/PoolParty/sparql/' . $project->uriSupplement;
        }
      }
    }
    else {
      $projects = array();
    }

    return $projects;
  }

  /**
   * Export data of a project as a file and store it on the server.
   *
   * @param string $project_id
   *   The ID of the PoolParty project to export and store.
   * @param string $format
   *   The returned RDF format.
   *   Possible values are: TriG, N3, Turtle, N-Triples, RDF/XML, TriX
   * @param string $export_modules
   *   A list of the export modules for the data that should be exported.
   *   Possible values are:
   *   - concepts - includes concept schemes, concepts, collections and notes
   *   - workflow - workflow status for all concepts
   *   - history - all history events
   *   - freeConcepts - all free concepts
   *   - void - the project VoiD graph
   *   - adms - the project ADMS graph
   *
   * @return string
   *   The URL of the stored file or an empty string if an error occurred.
   */
  public function storeProject($project_id, $format = 'RDF/XML', $export_modules = 'concepts') {
    $resource_path = $this->getApiPath() . 'projects/' . $project_id . '/store';
    $get_parameters = array(
      'format' => $format,
      'exportModules' => $export_modules,
    );
    $file_path = $this->connection->get($resource_path, array(
      'query' => $get_parameters,
      'headers' => array('Accept' => 'text/plain'),
    ));

    return (filter_var($file_path, FILTER_VALIDATE_URL) !== FALSE) ? $file_path : '';
  }

  /**
   * Gets information about a concept scheme.
   *
   * @param string $project_id
   *   The ID of the PoolParty project of the concepts.
   * @param string $language
   *   Only concepts with labels in this language will be displayed. If no
   *   language is given, the default language of the project will be used.
   *
   * @return stdClass[]
   *   An array of concept scheme objects within the respective PoolParty project.
   */
  public function getConceptSchemes($project_id, $language = '') {
    $resource_path = $this->getApiPath() . 'thesaurus/' . $project_id . '/schemes';
    $get_parameters = array(
      'language' => $language,
    );
    $result = $this->connection->get($resource_path, array(
      'query' => $get_parameters,
    ));
    $concept_schemes = json_decode($result);

    return $concept_schemes;
  }

  /**
   * Gets a list of all top concepts of a specific concept scheme.
   *
   * @param string $project_id
   *   The ID of the PoolParty project.
   * @param string $scheme_uri
   *   The URI of the concept scheme.
   * @param array $properties
   *   A list of additional properties to fetch (e.g. skos:altLabel, skso:hiddenLabel).
   * @param string $language
   *   Only concepts with labels in this language will be displayed. If no
   *   language is given, the default language of the project will be used.
   *
   * @return array
   *   A list of top concepts.
   */
  public function getTopConcepts($project_id, $scheme_uri, array $properties = array(), $language = '') {
    $resource_path = $this->getApiPath() . 'thesaurus/' . $project_id . '/topconcepts';
    $get_parameters = array(
      'scheme' => $scheme_uri,
      'properties' => implode(',', $properties),
      'language' => $language,
    );
    $result = $this->connection->get($resource_path, array(
      'query' => $get_parameters,
    ));
    $concept = json_decode($result);

    return $concept;
  }

  /**
   * Get a list of all concepts under a specified concept in a tree format.
   *
   * @param int $project_id
   *   The ID of the PoolParty project.
   * @param string $uri
   *   A concept URI.
   * @param array $properties
   *   A list of additional properties to fetch (e.g. skos:altLabel, skso:hiddenLabel).
   * @param string $language
   *   Only concepts with labels in this language will be displayed. If no
   *   language is given, the default language of the project will be used.
   *
   * @return array
   *   A list of concept objects in a tree format.
   */
  public function getSubTree($project_id, $uri, array $properties = array(), $language = '') {
    // PoolParty Thesaurus API Bug (version 5.3.1):
    // At least the prefLabel proberty must be indicated.
    if (!in_array('skos:prefLabel', $properties)) {
      $properties[] = 'skos:prefLabel';
    }

    $resource_path = $this->getApiPath() . 'thesaurus/' . $project_id . '/subtree';
    $get_parameters = array(
      'uri' => $uri,
      'properties' => implode(',', $properties),
      'language' => $language,
    );
    $result = $this->connection->get($resource_path, array(
      'query' => $get_parameters,
      'timeout' => 120 // Allowing up to 2 minutes for the process.
    ));
    $concept = json_decode($result);

    return $concept;
  }

  /**
   * Get information about concepts.
   *
   * @param string $project_id
   *   The ID of the PoolParty project of the concepts.
   * @param array $concept_uris
   *   An array of concept URIs to get information for.
   * @param array $properties
   *   Array of additional concept properties that will be fetched (only
   *   properties uri and prefLabel are included by default). Possible values
   *   are:
   *   - skos:altLabel
   *   - skos:hiddenLabel
   *   - skos:definition
   *   - skos:broader
   *   - skos:narrower
   *   - skos:related
   *   - skos:ConceptScheme
   * @param string $language
   *   Only concepts with labels in this language will be displayed. If no
   *   language is given, the default language of the project will be used.
   *
   * @return array
   *   Array of concept objects within the respective PoolParty project with
   *   following properties:
   *   - uri --> URI of the concept
   *   - prefLabel --> Preferred label
   *   - altLabels --> Alternative labels
   *   - hiddenLabels --> Hidden labels
   *   - definitions --> Definitions
   *   - broaders --> Broader concepts
   *   - narrowers --> Narrower concepts
   *   - relateds --> Related concepts
   *   - conceptSchemes --> Concept schemes
   */
  public function getConcepts($project_id, array $concept_uris, array $properties = array(), $language = '') {
    if (empty($concept_uris)) {
      return array();
    }

    $resource_path = $this->getApiPath() . 'thesaurus/' . $project_id . '/concepts';
    foreach ($concept_uris as $uri_count => $concept_uri) {
      if ($uri_count == 0) {
        $resource_path .= '?';
      }
      else {
        $resource_path .= '&';
      }
      $resource_path .= 'concepts=' . urlencode($concept_uri);
    }
    foreach ($properties as $property) {
      $resource_path .= '&properties=' . urlencode($property);
    }
    if (!is_null($language)) {
      $resource_path .= '&language=' . urlencode($language);
    }

    $result = $this->connection->get($resource_path);
    $concepts = json_decode($result);

    return $concepts;
  }

  /**
   * Get information about a concept.
   *
   * @param string $project_id
   *   The ID of the PoolParty project of the concepts.
   * @param string $concept_uri
   *   The concept URI, from which the data should be retrieved.
   * @param string[] $properties
   *   Array of additional concept properties that will be fetched (only
   *   properties uri and prefLabel are included by default). Possible values
   *   are:
   *   - skos:prefLabel
   *   - skos:altLabel
   *   - skos:hiddenLabel
   *   - skos:definition
   *   - skos:broader
   *   - skos:narrower
   *   - skos:related
   *   - skos:ConceptSchemes
   *   - all
   * @param string $language
   *   Only concepts with labels in this language will be displayed. If no
   *   language is given, the default language of the project will be used.
   *
   * @return stdClass
   *   A concept object within the respective PoolParty project with
   *   following properties:
   *   - uri --> URI of the concept
   *   - prefLabel --> Preferred label
   *   - altLabels --> Alternative labels
   *   - hiddenLabels --> Hidden labels
   *   - definitions --> Definitions
   *   - broaders --> Broader concepts
   *   - narrowers --> Narrower concepts
   *   - relateds --> Related concepts
   *   - conceptSchemes --> Concept schemes
   */
  public function getConcept($project_id, $concept_uri, array $properties = array(), $language = '') {
    // PoolParty Thesaurus API Bug (version 5.3.1):
    // At least the prefLabel proberty must be indicated.

    if (!in_array('skos:prefLabel', $properties)) {
      $properties[] = 'skos:prefLabel';
    }

    $resource_path = $this->getApiPath() . 'thesaurus/' . $project_id . '/concept';
    $get_parameters = array(
      'concept' => $concept_uri,
      'properties' => implode(',', $properties),
      'language' => $language,
    );
    $result = $this->connection->get($resource_path, array(
      'query' => $get_parameters,
    ));
    $concept = json_decode($result);

    return $concept;
  }

  /**
   * Creates a concept scheme in a specific project.
   *
   * @param string $project_id
   *   The ID of the PoolParty project in which the concept scheme should be created.
   * @param string $title
   *   The title of the new concept scheme.
   * @param string $description
   *   A description for the new concept scheme.
   * @param string $creator
   *   The name of the creator of the new concept scheme.
   *
   * @return string
   *   The URI of the new concept scheme.
   */
  public function createConceptScheme($project_id, $title, $description, $creator = 'Drupal') {
    $resource_path = $this->getApiPath() . 'thesaurus/' . $project_id . '/createConceptScheme';
    $post_parameters = array(
      'title' => $title,
      'description' => $description,
      'creator' => $creator,
    );
    $result = $this->connection->post($resource_path, array(
      'data' => $post_parameters,
      'headers' => array('Accept' => 'text/plain'),
    ));

    return $result;
  }

  /**
   * Creates a new concept in a specific project.
   *
   * @param string $project_id
   *   The ID of the PoolParty project in which the concept should be created.
   * @param string $prefLabel
   *   The label in the default language of the project.
   * @param string $parent
   *   The URI of the parent concept or concept scheme of the new concept.
   *
   * @return string
   *   The URI of the new concept.
   */
  public function createConcept($project_id, $prefLabel, $parent) {
    $resource_path = $this->getApiPath() . 'thesaurus/' . $project_id . '/createConcept';
    $post_parameters = array(
      'prefLabel' => $prefLabel,
      'parent' => $parent,
    );
    $result = $this->connection->post($resource_path, array(
      'data' => $post_parameters,
      'headers' => array('Accept' => 'text/plain'),
    ));

    return $result;
  }

  /**
   * Adds a SKOS relation between two existing concepts
   *
   * @param string $project_id
   *   The ID of the PoolParty project.
   * @param string $source
   *   The URI of the source concept.
   * @param string $target
   *   The URI of the target concept.
   * @param string $property
   *   The relation property. Possible values are:
   *   - broader
   *   - narrower
   *   - related
   *   - hasTopConcept
   *   - topConceptOf
   *
   * @return mixed
   *  Status: 200 - OK
   */
  public function addRelation($project_id, $source, $target, $property = 'broader') {
    $resource_path = $this->getApiPath() . 'thesaurus/' . $project_id . '/addRelation';
    $post_parameters = array(
      'sourceConcept' => $source,
      'targetConcept' => $target,
      'property' => $property,
    );
    $result = $this->connection->post($resource_path, array(
      'data' => $post_parameters,
    ));

    return $result;
  }

  /**
   * Adds a literal to an existing concept
   *
   * @param string $project_id
   *  The ID of the PoolParty project.
   * @param string $concept_uri
   *  The URI of the Concept.
   * @param string $property
   *  The SKOS property. Possible values are:
   *  - preferredLabel
   *  - alternativeLabel
   *  - hiddenLabel
   *  - definition
   *  - scopeNote
   *  - example
   *  - notation
   * @param string $label
   *  The RDF literal to add.
   * @param null $language
   *  The attribute language.
   *
   * @return mixed
   *  Status: 200 - OK
   */
  public function addLiteral($project_id, $concept_uri, $property, $label, $language = NULL) {
    $resource_path = $this->getApiPath() . 'thesaurus/' . $project_id . '/addLiteral';
    $post_parameters = array(
      'concept' => $concept_uri,
      'label' => $label,
      'property' => $property,
    );

    if (!is_null($language) && !empty($language)) {
      $post_parameters['language'] = $language;
    }

    $result = $this->connection->post($resource_path, array(
      'data' => $post_parameters,
    ));

    return $result;
  }

  /**
   * Adds a literal to an existing concept
   *
   * @param string $project_id
   *  The ID of the PoolParty project.
   * @param string $concept_uri
   *  The URI of the Concept to add the property to.
   * @param string $attribute_uri
   *  The URI of the custom attribute property.
   * @param string $value
   *  The attribute value that should be added
   * @param null $language
   *  The attribute language.
   *
   * @return mixed
   *  Status: 200 - OK
   */
  public function addCustomAttribute($project_id, $concept_uri, $attribute_uri, $value, $language = NULL) {
    $resource_path = $this->getApiPath() . 'thesaurus/' . $project_id . '/addCustomAttribute';
    $post_parameters = array(
      'resource' => $concept_uri,
      'property' => $attribute_uri,
      'value' => $value,
    );

    if (!is_null($language) && !empty($language)) {
      $post_parameters['language'] = $language;
    }

    $result = $this->connection->post($resource_path, array(
      'data' => $post_parameters,
    ));

    return $result;
  }

  /**
   * Get the languages available in the PoolParty.
   *
   * @return array
   *   An associative array of available languages (iso-code --> label).
   */
  public function getLanguages() {
    // Return a static list of all the PoolParty languages and their labels.
    $languages = array(
      'sq' => 'Albanian [sq]',
      'sq-AL' => 'Albanian (Albania) [sq-AL]',
      'ar' => 'Arabic [ar]',
      'ar-DZ' => 'Arabic (Algeria) [ar-DZ]',
      'ar-BH' => 'Arabic (Bahrain) [ar-BH]',
      'ar-EG' => 'Arabic (Egypt) [ar-EG]',
      'ar-IQ' => 'Arabic (Iraq) [ar-IQ]',
      'ar-JO' => 'Arabic (Jordan) [ar-JO]',
      'ar-KW' => 'Arabic (Kuwait) [ar-KW]',
      'ar-LB' => 'Arabic (Lebanon) [ar-LB]',
      'ar-LY' => 'Arabic (Libya) [ar-LY]',
      'ar-MA' => 'Arabic (Morocco) [ar-MA]',
      'ar-OM' => 'Arabic (Oman) [ar-OM]',
      'ar-QA' => 'Arabic (Qatar) [ar-QA]',
      'ar-SA' => 'Arabic (Saudi Arabia) [ar-SA]',
      'ar-SD' => 'Arabic (Sudan) [ar-SD]',
      'ar-SY' => 'Arabic (Syria) [ar-SY]',
      'ar-TN' => 'Arabic (Tunisia) [ar-TN]',
      'ar-AE' => 'Arabic (United Arab Emirates) [ar-AE]',
      'ar-YE' => 'Arabic (Yemen) [ar-YE]',
      'be' => 'Belarusian [be]',
      'be-BY' => 'Belarusian (Belarus) [be-BY]',
      'bg' => 'Bulgarian [bg]',
      'bg-BG' => 'Bulgarian (Bulgaria) [bg-BG]',
      'ca' => 'Catalan [ca]',
      'ca-ES' => 'Catalan (Spain) [ca-ES]',
      'zh' => 'Chinese [zh]',
      'zh-CN' => 'Chinese (China) [zh-CN]',
      'zh-HK' => 'Chinese (Hong Kong) [zh-HK]',
      'zh-SG' => 'Chinese (Singapore) [zh-SG]',
      'zh-TW' => 'Chinese (Taiwan) [zh-TW]',
      'hr' => 'Croatian [hr]',
      'hr-HR' => 'Croatian (Croatia) [hr-HR]',
      'cs' => 'Czech [cs]',
      'cs-CZ' => 'Czech (Czech Republic) [cs-CZ]',
      'da' => 'Danish [da]',
      'da-DK' => 'Danish (Denmark) [da-DK]',
      'nl' => 'Dutch [nl]',
      'nl-BE' => 'Dutch (Belgium) [nl-BE]',
      'nl-NL' => 'Dutch (Netherlands) [nl-NL]',
      'en' => 'English [en]',
      'en-AU' => 'English (Australia) [en-AU]',
      'en-CA' => 'English (Canada) [en-CA]',
      'en-IN' => 'English (India) [en-IN]',
      'en-IE' => 'English (Ireland) [en-IE]',
      'en-MT' => 'English (Malta) [en-MT]',
      'en-NZ' => 'English (New Zealand) [en-NZ]',
      'en-PH' => 'English (Philippines) [en-PH]',
      'en-SG' => 'English (Singapore) [en-SG]',
      'en-ZA' => 'English (South Africa) [en-ZA]',
      'en-GB' => 'English (United Kingdom) [en-GB]',
      'en-US' => 'English (United States) [en-US]',
      'et' => 'Estonian [et]',
      'et-EE' => 'Estonian (Estonia) [et-EE]',
      'fi' => 'Finnish [fi]',
      'fi-FI' => 'Finnish (Finland) [fi-FI]',
      'fr' => 'French [fr]',
      'fr-BE' => 'French (Belgium) [fr-BE]',
      'fr-CA' => 'French (Canada) [fr-CA]',
      'fr-FR' => 'French (France) [fr-FR]',
      'fr-LU' => 'French (Luxembourg) [fr-LU]',
      'fr-CH' => 'French (Switzerland) [fr-CH]',
      'de' => 'German [de]',
      'de-AT' => 'German (Austria) [de-AT]',
      'de-DE' => 'German (Germany) [de-DE]',
      'de-GR' => 'German (Greece) [de-GR]',
      'de-LU' => 'German (Luxembourg) [de-LU]',
      'de-CH' => 'German (Switzerland) [de-CH]',
      'el' => 'Greek [el]',
      'el-CY' => 'Greek (Cyprus) [el-CY]',
      'el-GR' => 'Greek (Greece) [el-GR]',
      'he' => 'Hebrew [he]',
      'he-IL' => 'Hebrew (Israel) [he-IL]',
      'hi' => 'Hindi [hi]',
      'hi-IN' => 'Hindi (India) [hi-IN]',
      'hu' => 'Hungarian [hu]',
      'hu-HU' => 'Hungarian (Hungary) [hu-HU]',
      'is' => 'Icelandic [is]',
      'is-IS' => 'Icelandic (Iceland) [is-IS]',
      'id' => 'Indonesian [id]',
      'id-ID' => 'Indonesian (Indonesia) [id-ID]',
      'ga' => 'Irish [ga]',
      'ga-IE' => 'Irish (Ireland) [ga-IE]',
      'it' => 'Italian [it]',
      'it-IT' => 'Italian (Italy) [it-IT]',
      'it-CH' => 'Italian (Switzerland) [it-CH]',
      'ja' => 'Japanese [ja]',
      'ja-JP' => 'Japanese (Japan) [ja-JP]',
      'ja-JP-u-ca-japanese-x-lvariant-JP' => 'Japanese (Japan,JP) [ja-JP-u-ca-japanese-x-lvariant-JP]',
      'ko' => 'Korean [ko]',
      'ko-KR' => 'Korean (South Korea) [ko-KR]',
      'lv' => 'Latvian [lv]',
      'lv-LV' => 'Latvian (Latvia) [lv-LV]',
      'lt' => 'Lithuanian [lt]',
      'lt-LT' => 'Lithuanian (Lithuania) [lt-LT]',
      'mk' => 'Macedonian [mk]',
      'mk-MK' => 'Macedonian (Macedonia) [mk-MK]',
      'ms' => 'Malay [ms]',
      'ms-MY' => 'Malay (Malaysia) [ms-MY]',
      'mt' => 'Maltese [mt]',
      'mt-MT' => 'Maltese (Malta) [mt-MT]',
      'no' => 'Norwegian [no]',
      'no-NO' => 'Norwegian (Norway) [no-NO]',
      'nn-NO' => 'Norwegian (Norway,Nynorsk) [nn-NO]',
      'pl' => 'Polish [pl]',
      'pl-PL' => 'Polish (Poland) [pl-PL]',
      'pt' => 'Portuguese [pt]',
      'pt-BR' => 'Portuguese (Brazil) [pt-BR]',
      'pt-PT' => 'Portuguese (Portugal) [pt-PT]',
      'ro' => 'Romanian [ro]',
      'ro-RO' => 'Romanian (Romania) [ro-RO]',
      'ru' => 'Russian [ru]',
      'ru-RU' => 'Russian (Russia) [ru-RU]',
      'sr' => 'Serbian [sr]',
      'sr-BA' => 'Serbian (Bosnia and Herzegovina) [sr-BA]',
      'sr-Latn' => 'Serbian (Latin) [sr-Latn]',
      'sr-Latn-BA' => 'Serbian (Latin,Bosnia and Herzegovina) [sr-Latn-BA]',
      'sr-Latn-ME' => 'Serbian (Latin,Montenegro) [sr-Latn-ME]',
      'sr-Latn-RS' => 'Serbian (Latin,Serbia) [sr-Latn-RS]',
      'sr-ME' => 'Serbian (Montenegro) [sr-ME]',
      'sr-CS' => 'Serbian (Serbia and Montenegro) [sr-CS]',
      'sr-RS' => 'Serbian (Serbia) [sr-RS]',
      'sk' => 'Slovak [sk]',
      'sk-SK' => 'Slovak (Slovakia) [sk-SK]',
      'sl' => 'Slovenian [sl]',
      'sl-SI' => 'Slovenian (Slovenia) [sl-SI]',
      'es' => 'Spanish [es]',
      'es-AR' => 'Spanish (Argentina) [es-AR]',
      'es-BO' => 'Spanish (Bolivia) [es-BO]',
      'es-CL' => 'Spanish (Chile) [es-CL]',
      'es-CO' => 'Spanish (Colombia) [es-CO]',
      'es-CR' => 'Spanish (Costa Rica) [es-CR]',
      'es-CU' => 'Spanish (Cuba) [es-CU]',
      'es-DO' => 'Spanish (Dominican Republic) [es-DO]',
      'es-EC' => 'Spanish (Ecuador) [es-EC]',
      'es-SV' => 'Spanish (El Salvador) [es-SV]',
      'es-GT' => 'Spanish (Guatemala) [es-GT]',
      'es-HN' => 'Spanish (Honduras) [es-HN]',
      'es-MX' => 'Spanish (Mexico) [es-MX]',
      'es-NI' => 'Spanish (Nicaragua) [es-NI]',
      'es-PA' => 'Spanish (Panama) [es-PA]',
      'es-PY' => 'Spanish (Paraguay) [es-PY]',
      'es-PE' => 'Spanish (Peru) [es-PE]',
      'es-PR' => 'Spanish (Puerto Rico) [es-PR]',
      'es-ES' => 'Spanish (Spain) [es-ES]',
      'es-US' => 'Spanish (United States) [es-US]',
      'es-UY' => 'Spanish (Uruguay) [es-UY]',
      'es-VE' => 'Spanish (Venezuela) [es-VE]',
      'sv' => 'Swedish [sv]',
      'sv-SE' => 'Swedish (Sweden) [sv-SE]',
      'th' => 'Thai [th]',
      'th-TH' => 'Thai (Thailand) [th-TH]',
      'th-TH-u-nu-thai-x-lvariant-TH' => 'Thai (Thailand,TH) [th-TH-u-nu-thai-x-lvariant-TH]',
      'tr' => 'Turkish [tr]',
      'tr-TR' => 'Turkish (Turkey) [tr-TR]',
      'uk' => 'Ukrainian [uk]',
      'uk-UA' => 'Ukrainian (Ukraine) [uk-UA]',
      'vi' => 'Vietnamese [vi]',
      'vi-VN' => 'Vietnamese (Vietnam) [vi-VN]',
    );
    return $languages;
  }
}

/**
 * Class SemanticConnectorPPTApi_5_3
 *
 * API Class for the version 5.3
 */
class SemanticConnectorPPTApi_5_3 extends SemanticConnectorPPTApi_4_6 {

  /**
   * Get all history items of a PoolParty project.
   *
   * @param string $project_id
   *   The ID of the project to get history items for.
   * @param int $from_time
   *   Optional; Only history items after this time will be included.
   * @param int $to_time
   *   Optional; Only history items before this time will be included.
   * @param string[] $events
   *   Optional; Filter by event type.
   *   Possible values: resourceChangeAddition, resourceChangeRemoval,
   *     resourceChangeUpdate, addRelation, removeRelation, addLiteral,
   *     removeLiteral, updateLiteral, addCollectionMember,
   *     removeCollectionMember, createCollection, deleteCollection,
   *     importConcept, resourceChangeAddition, addCustomAttributeLiteral,
   *     removeCustomAttributeLiteral ,updateCustomAttributeLiteral,
   *     addCustomRelation, removeCustomRelation, addCustomClass,
   *     removeCustomClass
   *
   * @return array
   *   An array of history items.
   */
  public function getHistory($project_id, $from_time = NULL, $to_time = NULL, $events = array()) {
    $resource_path = $this->getApiPath() . 'history/' . $project_id;
    $get_parameters = array();

    if (!is_null($from_time)) {
      $get_parameters['fromTime'] = date('c', $from_time);
    }
    if (!is_null($to_time)) {
      $get_parameters['toTime'] = date('c', $to_time);
    }
    if (!empty($events)) {
      $get_parameters['events'] = $events;
    }

    $result = $this->connection->get($resource_path, array(
      'query' => $get_parameters,
    ));
    $history_items = json_decode($result);

    return $history_items;
  }

  /**
   * Returns a list of PoolParty user group names
   *
   * @return string[]
   *   Array of PoolParty user groups.
   */
  public function getUserGroups() {
    $resource_path = $this->getApiPath() . 'user/groups';
    $result = $this->connection->get($resource_path);
    $groups = json_decode($result);

    return $groups;
  }

  /**
   * Get information about the extraction model for a PP project.
   *
   * @param string $project_id
   *   The ID of the PP project to get the extraction model info for.
   *
   * @return array|bool
   *   Associative array of extraction model info or FALSE in case of an error.
   *   Following keys are included:
   *   - lastBuildTime (string) --> Last extraction model build time
   *   - lastChangeTime (string) --> Last thesaurus change
   *   - upToDate (bool) --> Whether the extraction model is up-to-date or not
   */
  public function getExtractionModelInfo($project_id) {
    $resource_path = $this->getApiPath() . 'indexbuilder/' . $project_id;
    $result = $this->connection->get($resource_path);
    $extraction_model_info = json_decode($result, TRUE);

    return $extraction_model_info;
  }

  /**
   * Refresh the extraction model for a PP project
   *
   * @param string $project_id
   *   The ID of the PP project to refresh the extraction model for.
   *
   * @return array
   *   An associative array informing about the success of the refreshing
   *   containing following keys:
   *   - success (bool) --> TRUE if the refreshing worked, FALSE if not
   *   - message (string) --> This property is optional, but if it exists it
   *       includes more details about why the connection could not be
   *       established.
   *   - since PP 6.0 also "plainMessage" and "reportable"
   */
  public function refreshExtractionModel($project_id) {
    $refresh_info = array(
      'success' => FALSE,
      'message' => '',
    );

    $resource_path = $this->getApiPath() . 'indexbuilder/' . $project_id . '/refresh';
    $variables = array(
      'timeout' => 600 // Allowing up to 10 minutes for the process.
    );
    $result = $this->connection->get($resource_path, $variables);
    $result = json_decode($result, TRUE);
    if (is_array($result)) {
      $refresh_info = $result;
    }

    return $refresh_info;
  }
}

/**
 * Class SemanticConnectorPPTApi_5_6
 *
 * API Class for the version 5.6
 */
class SemanticConnectorPPTApi_5_6 extends SemanticConnectorPPTApi_5_3 {

  /**
   * Export data of a project as a file and store it on the server.
   *
   * @param string $project_id
   *   The ID of the PoolParty project to export and store.
   * @param string $format
   *   The returned RDF format.
   *   Possible values are: TriG, N3, Turtle, N-Triples, RDF/XML, TriX
   * @param string $export_modules
   *   A list of the export modules for the data that should be exported.
   *   Possible values are:
   *   - concepts - includes concept schemes, concepts, collections and notes
   *   - workflow - workflow status for all concepts
   *   - history - all history events
   *   - freeConcepts - all free concepts
   *   - void - the project VoiD graph
   *   - adms - the project ADMS graph
   *
   * @return string
   *   The URL of the stored file or an empty string if an error occurred.
   */
  public function storeProject($project_id, $format = 'RDF/XML', $export_modules = 'concepts') {
    $resource_path = $this->getApiPath() . 'projects/' . $project_id . '/store';
    $get_parameters = array(
      'format' => $format,
      'exportModules' => $export_modules,
    );
    $file_path = $this->connection->get($resource_path, array(
      'query' => $get_parameters,
    ));
    $file_path = json_decode($file_path);

    return (filter_var($file_path, FILTER_VALIDATE_URL) !== FALSE) ? $file_path : '';
  }

  /**
   * Get a list of all concepts under a specified concept in a tree format.
   *
   * @param int $project_id
   *   The ID of the PoolParty project.
   * @param string $uri
   *   A concept URI.
   * @param array $properties
   *   A list of additional properties to fetch (e.g. skos:altLabel, skso:hiddenLabel).
   * @param string $language
   *   Only concepts with labels in this language will be displayed. If no
   *   language is given, the default language of the project will be used.
   *
   * @return array
   *   A list of concept objects in a tree format.
   */
  public function getSubTree($project_id, $uri, array $properties = array(), $language = '') {
    // PoolParty Thesaurus API Bug (version 5.3.1):
    // At least the prefLabel proberty must be indicated.
    if (!in_array('skos:prefLabel', $properties)) {
      $properties[] = 'skos:prefLabel';
    }

    $resource_path = $this->getApiPath() . 'thesaurus/' . $project_id . '/subtree';
    $get_parameters = array(
      'root' => $uri,
      'properties' => implode(',', $properties),
      'language' => $language,
    );
    $result = $this->connection->get($resource_path, array(
      'query' => $get_parameters,
      'timeout' => 120 // Allowing up to 2 minutes for the process.
    ));
    $concept = json_decode($result);

    return $concept;
  }
  
  /**
   * Creates a concept scheme in a specific project.
   *
   * @param string $project_id
   *   The ID of the PoolParty project in which the concept scheme should be created.
   * @param string $title
   *   The title of the new concept scheme.
   * @param string $description
   *   A description for the new concept scheme.
   * @param string $creator
   *   The name of the creator of the new concept scheme.
   *
   * @return string
   *   The URI of the new concept scheme.
   */
  public function createConceptScheme($project_id, $title, $description, $creator = 'Drupal') {
    $resource_path = $this->getApiPath() . 'thesaurus/' . $project_id . '/createConceptScheme';
    $post_parameters = array(
      'title' => $title,
      'description' => $description,
      'creator' => $creator,
    );
    $result = $this->connection->post($resource_path, array(
      'data' => $post_parameters,
    ));

    return json_decode($result);
  }

  /**
   * Creates a new concept in a specific project.
   *
   * @param string $project_id
   *   The ID of the PoolParty project in which the concept should be created.
   * @param string $prefLabel
   *   The label in the default language of the project.
   * @param string $parent
   *   The URI of the parent concept or concept scheme of the new concept.
   *
   * @return string
   *   The URI of the new concept.
   */
  public function createConcept($project_id, $prefLabel, $parent) {
    $resource_path = $this->getApiPath() . 'thesaurus/' . $project_id . '/createConcept';
    $post_parameters = array(
      'prefLabel' => $prefLabel,
      'parent' => $parent,
    );
    $result = $this->connection->post($resource_path, array(
      'data' => $post_parameters,
    ));

    return json_decode($result);
  }

  /**
   * Get the corpora available for a PoolParty project.
   *
   * @param string $project_id
   *   The ID of the PP project to get the corpora for.
   *
   * @return array
   *   An array of associative corpus arrays containing following properties:
   *   - corpusId (string) --> Corpus id
   *   - corpusName (string) --> Corpus name
   *   - language (string) --> Language of corpus (en|de|es|fr|...)
   *   - upToDate (boolean) --> Up to date flag
   */
  public function getCorpora($project_id) {
    $corpora = array();

    $resource_path = $this->getApiPath() . 'corpusmanagement/' . $project_id . '/corpora';
    $result = $this->connection->get($resource_path);
    $corpus_data = json_decode($result, TRUE);
    if (!is_null($corpus_data) && isset($corpus_data['jsonCorpusList'])) {
      $corpora = $corpus_data['jsonCorpusList'];
    }

    return $corpora;
  }

  /**
   * Push text into a PoolParty corpus.
   *
   * @param string $project_id
   *   The ID of the PP project to use.
   * @param string $corpus_id
   *   The ID of the corpus to add the text to.
   * @param string $title
   *   The title of the document to push into the corpus.
   * @param mixed $data
   *   The text to push into the corpus.
   * @param string $data_type
   *   The type of the data. Can be one of the following values:
   *   - "text" for a text
   *   - "file" for a file object with a file ID
   *   - "file direct" for all other files without an ID
   * @param boolean $check_language
   *   Checks if the language of the uploaded file and the language of the
   *   corpus are the same.
   *
   * @return mixed
   *  Status: 200 - OK
   */
  public function addDataToCorpus($project_id, $corpus_id, $title, $data, $data_type, $check_language = TRUE) {
    $resource_path = $this->getApiPath() . 'corpusmanagement/' . $project_id . '/add';
    $post_parameters = array(
      'corpusId' => $corpus_id,
      'checkLanguage' => $check_language,
    );

    $result = FALSE;
    switch ($data_type) {
      // Extract concepts from a given text.
      case 'text':
        $post_parameters = array_merge(array(
          'text' => $data,
          'title' => $title,
        ), $post_parameters);
        $result = $this->connection->post($resource_path, array(
          'data' => $post_parameters,
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
              'filename' => $title,
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
          'filename' => $title,
        ), $post_parameters);
        $result = $this->connection->post($resource_path, array(
          'data' => $post_parameters,
          'headers' => array('Content-Type' => 'multipart/form-data'),
        ));
        break;

      default:
        SemanticConnectorWatchdog::message('PPT API', 'The type of the data to push content into a corpus is not supported.');
        break;
    }

    return $result;
  }

  /**
   * Get the metadata (additional information) of a corpus.
   *
   * @param string $project_id
   *   The ID of the PP project to get the corpus metadata for.
   * @param string $corpus_id
   *   The ID of the corpus to get the metadata for.
   *
   * @return array
   *   An array of associative corpus arrays containing following properties:
   *   - corpusName (string) --> Corpus name
   *   - corpusId (string) --> Corpus id
   *   - language (string) --> Language of corpus (en|de|es|fr|...)
   *   - createdBy (string) --> Corpus created by user
   *   - created (string) --> Time of creation of the corpus
   *   - extractedTerms (int) --> Number of unique free terms
   *   - concepts (int) --> Number of unique concepts
   *   - suggestedTermOccurrences (int) --> Number of free terms
   *   - conceptOccurrences (int) --> Number of concepts
   *   - quality (string) --> Quality of the corpus
   *     Possible values are "good", "moderate" and "poor"
   *   - overallFileSize (string) --> Overall file size of documents in the corpus
   *   - lastModified (string) --> Last modification date of the corpus
   *   - storedDocuments (int) --> Number of stored documents in the corpus
   */
  public function getCorpusMetadata($project_id, $corpus_id) {
    $resource_path = $this->getApiPath() . 'corpusmanagement/' . $project_id . '/metadata';

    $get_parameters = array(
      'corpusId' => $corpus_id,
    );
    $result = $this->connection->get($resource_path, array(
      'query' => $get_parameters,
    ));
    $corpus_data = json_decode($result, TRUE);

    return $corpus_data;
  }

  /**
   * Check if a corpus is up to date (or has to by analysed in case it is not).
   *
   * @param string $project_id
   *   The ID of the PP project of the corpus to check.
   * @param string $corpus_id
   *   The ID of the corpus to check.
   *
   * @return boolean
   *   TRUE if the corpus is up to date, FALSE if not
   */
  public function isCorpusUpToDate($project_id, $corpus_id) {
    $resource_path = $this->getApiPath() . 'corpusmanagement/' . $project_id . '/uptodate';

    $get_parameters = array(
      'corpusId' => $corpus_id,
    );
    $result = $this->connection->get($resource_path, array(
      'query' => $get_parameters,
    ));
    $corpus_check = json_decode($result, TRUE);
    if (is_array($corpus_check) && isset($corpus_check['upToDate'])) {
      return $corpus_check['upToDate'];
    }
    else {
      return FALSE;
    }
  }

  /**
   * Start the analysis of a corpus.
   *
   * @param string $project_id
   *   The ID of the PP project of the corpus.
   * @param string $corpus_id
   *   The ID of the corpus to start the analysis for.
   *
   * @return array
   *   An associative array informing about the success of the analysis
   *   containing following keys:
   *   - success (bool) --> TRUE if the analysis worked, FALSE if not
   *   - message (string) --> This property is optional, but if it exists it
   *       includes more details about why the connection could not be
   *       established.
   *   - since PP 6.0 also "plainMessage" and "reportable"
   */
  public function analyzeCorpus($project_id, $corpus_id) {
    $analysis_info = array(
      'success' => FALSE,
      'message' => '',
    );

    $resource_path = $this->getApiPath() . 'corpusmanagement/' . $project_id . '/analyze';

    $post_parameters = array(
      'corpusId' => $corpus_id,
    );
    $variables = array(
      'data' => $post_parameters,
      'timeout' => 600 // Allowing up to 10 minutes for the process.
    );
    $result = $this->connection->post($resource_path, $variables);

    $result = json_decode($result, TRUE);
    if (is_array($result)) {
      $analysis_info = $result;
    }

    return $analysis_info;
  }

  /**
   * Suggest a set of concepts.
   *
   * @param string $project_id
   *   The ID of the project to suggest the concept for.
   * @param array $concepts
   *   An array of information about the concept to suggest. Following keys are
   *   supported:
   *   - prefLabels (Array of LanguageLiteral) --> Suggested preferred labels of
   *     the new concept - at least one must be in the project default language
   *   - broaderConcept (Array of IRI) --> Suggested broader concepts of the new
   *     concept. Optional
   *   - checkForDuplicates (boolean) --> If another Concept with the same
   *     preferred label already exists, an error will be thrown. (Default: true)
   *   - definition (Array of LanguageLiteral) --> Textual definitions of the
   *     new concept. Optional
   *   - note (String) --> Notes describing the new concept. Optional
   *   - relatedConcept (Array of IRI) --> Suggested related concepts of the new
   *     concept. Optional
   *   - score (float) --> Custom score for the new concept between 0 and 1.
   *     Optional
   *   - suffix (String) --> Suffix for manual uri creation. Optional
   *   - topConceptOf (Array of IRI) --> Schemes of which the suggested concept
   *     is a top concept. Optional
   *
   * @return array|bool
   *   An array or concept-arrays containing a uri-key or FALSE in case of an error.
   */
  public function suggestConcepts($project_id, array $concepts) {
    $resource_path = $this->getApiPath() . 'thesaurus/' . $project_id . '/suggestConcepts';

    $result = $this->connection->post($resource_path, array(
      'data' => json_encode(array(
        'conceptSuggestRequests' => $concepts,
      )),
    ));

    return json_decode($result, TRUE);
  }

  /**
   * Get the suggested concepts for a PoolParty project.
   *
   * @param string $project_id
   *   The ID of the project to get the suggested concepts for.
   * @param int $offset
   *   Start index from where the suggested concepts results should be returned.
   * @param int $number
   *   The number of retrieved suggested concepts. Use 0 to get all suggested
   *   concepts.
   *
   * @return array
   *   An array of suggested concept arrays containing following keys:
   *   - broaderConcepts (Array of IRI) --> Broader Concepts of the Suggested Concept
   *   - date (String) --> Creation date of the Suggested Concept
   *   - definitions (Array of Literal) --> Definitions of the Suggested Concept
   *   - note (String) --> Note of the Suggested Concept
   *   - prefLabels (Array of Literal) --> Preferred Labels URI of the Suggested Concept
   *   - relatedConcepts (Array of IRI) --> Related Concepts of the Suggested Concept
   *   - score (float) --> Score of the Suggested Concept
   *   - topConceptOf (Array of IRI) --> Schemes in which the Suggested Concept is a top concept.
   *   - uri (IRI) --> URI of the Suggested Concept
   */
  public function getSuggestedConcepts($project_id, $offset = 0, $number = 0) {
    $resource_path = $this->getApiPath() . 'thesaurus/' . $project_id . '/suggestedConcepts';

    $get_parameters = [];
    if ($offset > 0) {
      $get_parameters['offset'] = $offset;
    }
    if ($number > 0) {
      $get_parameters['noOfConcepts'] = $number;
    }

    $result = $this->connection->get($resource_path, array(
      'query' => $get_parameters,
    ));

    return json_decode($result, TRUE);
  }
}

/**
 * Class SemanticConnectorPPTApi_6_0
 *
 * API Class for the version 6.0
 */
class SemanticConnectorPPTApi_6_0 extends SemanticConnectorPPTApi_5_6 {

  /**
   * @inheritdoc
   */
  public function createProject($title, $language_default = 'en', $user_groups = array('Public'), $settings = array()) {
    $resource_path = $this->getApiPath() . 'projects/create';
    $post_parameters = $settings;
    $post_parameters += array(
      'title' => $title,
      'defaultLanguage' => $language_default,
      'userGroups' => $user_groups,
    );

    $result = $this->connection->post($resource_path, array(
      'data' => json_encode($post_parameters),
    ));

    $result = json_decode($result);
    if (isset($result->id)) {
      return $result->id;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Check if a corpus analysis is running for a project (only one analysis can
   * run per project at a time).
   *
   * @param string $project_id
   *   The ID of the PP project of the corpus to check.
   *
   * @return boolean
   *   TRUE if a corpus is running for that project, FALSE if not
   */
  public function isCorpusAnalysisRunning($project_id) {
    $resource_path = $this->getApiPath() . 'corpusmanagement/' . $project_id . '/analysisRunning';

    $result = $this->connection->get($resource_path);
    $analysis_running = json_decode($result, TRUE);
    return $analysis_running;
  }

  /**
   * Returns the license of this PoolParty installation.
   *
   * @return array|boolean
   *   An associative array of information about the license or FALSE in case
   *   of an error. The array includes following keys:
   *   - expiryDate (String) --> e.g. "2049-12-31T00:00:00.000+01:00"
   *   - expiryDateInMillis (long) --> e.g. 2524518000000
   *   - features (Array of String) --> e.g. ["some feature", "other feature"]
   *   - ipConstraint (String) --> e.g. "*"
   *   - issueDate (String) --> e.g. "2018-01-01T00:00:00.000+01:00"
   *   - issueDateInMillis (long) --> e.g. 1514761200000
   *   - licensee (String) --> text describing the licensee
   *   - macConstraint (String) --> e.g. "*"
   *   - maximumVersion (String) --> "6.0.1"
   *   - minimumVersion (String) --> "7.0.1"
   *   - type (String) --> e.g. "https://semantic-web.com/api/type#15230"
   *   - uid (String) --> User ID of the license creator
   *   - valid (boolean) --> if the license is valid
   */
  public function getLicense() {
    $resource_path = $this->getApiPath() . 'license';

    $result = $this->connection->get($resource_path);
    $license = json_decode($result, TRUE);

    return $license;
  }
}

/**
 * Class SemanticConnectorPPTApi_6_2
 *
 * API Class for the version 6.2. 6.1 is the first version that supports PPT API
 * versioning and 6.2 is the first version available for both PPX and PPT,
 * that's why the class is required even though there was no API change
 * interesting for the Drupal integration.
 */
class SemanticConnectorPPTApi_6_2 extends SemanticConnectorPPTApi_6_0 {
  /**
   * Get all classifiers for a specific PoolParty project.
   *
   * @param string $project_id
   *   The project UUID to get the classifiers for.
   *
   * @return array
   *   An array of classifiers, each one is an associative array including
   *   following keys:
   *   - isOnline (boolean) --> Online status
   *   - language (String) --> Language of classifier (en|de|es|fr|...)
   *   - name (String) --> Classifier name
   *   - status (String) --> Status
   *   - uri (String) --> Classifier id
   */
  public function getClassifiers($project_id) {
    $resource_path = $this->getApiPath() . 'classification/' . $project_id . '/classifiers';

    $result = $this->connection->get($resource_path);
    $classifier_data = json_decode($result, TRUE);

    $classifiers = [];
    if (is_array($classifier_data) && isset($classifier_data['jsonClassifierList'])) {
      $classifiers = $classifier_data['jsonClassifierList'];
    }
    return $classifiers;
  }
}

/**
 * Class SemanticConnectorPPTApi_7_0
 *
 * API Class for the version 7.0.
 */
class SemanticConnectorPPTApi_7_0 extends SemanticConnectorPPTApi_6_2 {
  /**
   * Adds a literal to an existing concept
   *
   * @param string $project_id
   *  The ID of the PoolParty project.
   * @param string $concept_uri
   *  The URI of the Concept.
   * @param string $property
   *  The SKOS property. Possible values are:
   *  - preferredLabel
   *  - alternativeLabel
   *  - hiddenLabel
   *  - definition
   *  - scopeNote
   *  - example
   *  - notation
   * @param string $label
   *  The RDF literal to add.
   * @param null $language
   *  The attribute language.
   *
   * @return mixed
   *  Status: 200 - OK
   */
  public function addLiteral($project_id, $concept_uri, $property, $label, $language = NULL) {
    $resource_path = $this->getApiPath() . 'thesaurus/' . $project_id . '/addLiteral';
    $post_parameters = array(
      'resourceUri' => $concept_uri,
      'label' => $label,
      'property' => $property,
    );

    if (!is_null($language) && !empty($language)) {
      $post_parameters['language'] = $language;
    }

    $result = $this->connection->post($resource_path, array(
      'data' => $post_parameters,
    ));

    return $result;
  }
}