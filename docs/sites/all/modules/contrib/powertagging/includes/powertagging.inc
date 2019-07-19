<?php

/**
 * @file
 * Callbacks and theme-functions used by the PowerTagging module.
 */

/**
 * Get the data for the Visual Mapper inside a PowerTagging form.
 *
 * @param Object $powertagging_config
 *   The base-path to the glossary of choice.
 * @param boolean $fetch_relations
 *   TRUE if relations (broader, narrower, related) shell be fetched for the
 *   concept, FALSE if not.
 */
function _powertagging_get_visual_mapper_data($powertagging_config, $fetch_relations = TRUE) {
  $root_uri = isset($_GET['uri']) && !empty($_GET['uri']) ? $_GET['uri'] : NULL;
  $project_languages = $powertagging_config->config['projects'][$powertagging_config->project_id]['languages'];
  if (isset($_GET['lang']) && !empty($_GET['lang']) && isset($project_languages[$_GET['lang']]) && !empty($project_languages[$_GET['lang']])) {
    $lang = $project_languages[$_GET['lang']];
    // Get the data for the concept.
    /** @var \SemanticConnectorPPServerConnection $pp_server_connection */
    $pp_server_connection = $powertagging_config->connection;
    $sparql_endpoints = $pp_server_connection->getSparqlEndpoints($powertagging_config->project_id);
    if (count($sparql_endpoints) > 0) {
      /** @var \SemanticConnectorSparqlEndpointConnection $sparql_endpoint */
      $sparql_endpoint = reset($sparql_endpoints);
      /** @var \SemanticConnectorSparqlApi $sparql_api */
      $sparql_api = $sparql_endpoint->getApi();

      if ($fetch_relations) {
        $concept = $sparql_api->getVisualMapperData($root_uri, $lang, (isset($_GET['parent_info']) && $_GET['parent_info'] === 'true'));
      }
      else {
        $concept = $sparql_api->createRootUriObject($root_uri, $lang);
      }

      // Display the data as JSON.
      header("Content-Type: application/json");
      echo json_encode($concept);
    }
  }

  exit;
}
