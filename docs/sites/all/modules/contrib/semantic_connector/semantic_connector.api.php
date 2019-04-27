<?php

/**
 * @file
 * API documentation for the Semantic Connector.
 */

/**
 * Manipulate the value of PPX API "heartbeat"-method.
 *
 * If you either need a different way to check if the PPX connection is
 * available or have a use a technology built on PoolParty with a different API,
 * this is the hook for you.
 *
 * @param object $ppx_api
 *   An instance of the "SemanticConnectorPPXApi"-class (don't call
 *   SemanticConnectorPPXApi::available() here or enjoy your infinite loop).
 * @param boolean $is_available
 *   The alterable value. (You will always receive this parameter with value
 *   NULL, change it to TRUE or FALSE)
 *
 * @see SemanticConnectorPPXApi::available()
 */
function hook_semantic_connector_ppx_available_alter($ppx_api, &$is_available) {
}

/**
 * Manipulate the value of PPX API "projects"-method.
 *
 * @param object $ppx_api
 *   An instance of the "SemanticConnectorPPXApi"-class (don't call
 *   SemanticConnectorPPXApi::getProjects() here or enjoy your infinite loop).
 * @param array $projects
 *   The alterable list of projects (You will always receive this parameter with
 *   value NULL). If you use this hook make sure that every project in the array
 *   of projects is an object with at least following properties: "label",
 *   "uuid", "defaultLanguage" and "languages".
 *
 * @see SemanticConnectorPPXApi::getProjects()
 */
function hook_semantic_connector_ppx_getProjects_alter($ppx_api, &$projects) {
}

/**
 * Manipulate the value of PPX API "extract"-method.
 *
 * @param object $ppx_api
 *   An instance of the "SemanticConnectorPPXApi"-class (don't call
 *   SemanticConnectorPPXApi::extractConcepts() here or enjoy your infinite
 *   loop).
 * @param object $concepts
 *   The alterable object containing the concepts. (You will always receive this
 *   parameter with value NULL). If you use this hook make sure that this is an
 *   object containing two properties:
 *   - "concepts" => Array of concepts with keys "tid", "uri", "label", "score"
 *   and "type"
 *   - "freeTerms" => Array of freeterms with keys "tid", "uri", "label",
 *   "score" and "type"
 * @param array $context
 *   An array of context parameters with following keys:
 *   - "data" => Can be either a string for normal text-extraction or a
 *     file-object for text extraction of the file content.
 *   - "language" => The iso-code of the language of the data.
 *   - "parameters" => Additional parameters to forward to the API
 *     (e.g., projectId).
 *   - "data type" => Can be one of the following values:
 *      - "text" for text
 *      - "url" for a valid URL
 *      - "file" for a file object with a file ID
 *      - "file direct" for all other files without an ID
 *      - empty if no type was given
 *
 * @see SemanticConnectorPPXApi::extractConcepts()
 * @see powertagging_extract_tags()
 */
function hook_semantic_connector_ppx_extractConcepts_alter($ppx_api, &$concepts, $context) {
}

/**
 * Manipulate the value of PPX API "suggest"-method.
 *
 * @param object $ppx_api
 *   An instance of the "SemanticConnectorPPXApi"-class (don't call
 *   SemanticConnectorPPXApi::suggest() here or enjoy your infinite loop).
 * @param $suggestion
 *   The alterable associative array of concepts and freeterms. (You will always
 *   receive this parameter with value NULL). If you use this hook make sure
 *   that this is an array of objects, each one with properties "uri",
 *   "prefLabel" and "tid" (freeTerms don't have a tid).
 * @param array $context
 *   An array of context parameters with following keys:
 *   - "string" => The string to search matching concepts / freeterms for.
 *   - "language" => The iso-code of the text's language.
 *   - "project_id" => The ID of the PoolParty project to use.
 *   - "parameters" => Additional parameters to forward to the API
 *   (e.g., projectId).
 *
 * @see SemanticConnectorPPXApi::suggest()
 * @see powertagging_autocomplete_tags()
 */
function hook_semantic_connector_ppx_suggest_alter($ppx_api, &$suggestion, $context) {
}
