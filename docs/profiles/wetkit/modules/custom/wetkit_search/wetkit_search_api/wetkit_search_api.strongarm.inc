<?php
/**
 * @file
 * wetkit_search_api.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function wetkit_search_api_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'facetapi:block_cache:search_api@database_node_index';
  $strongarm->value = '-1';
  $export['facetapi:block_cache:search_api@database_node_index'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'facetapi:block_cache:search_api@node_index';
  $strongarm->value = -1;
  $export['facetapi:block_cache:search_api@node_index'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'search_active_modules';
  $strongarm->value = array(
    'node' => 'node',
    'user' => 0,
  );
  $export['search_active_modules'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'search_api_facets_search_ids';
  $strongarm->value = array(
    'node_index' => array(
      'search_api_views:wetkit_search_api:search_solr_results' => 'search_api_views:wetkit_search_api:search_solr_results',
      'search_api_views:wetkit_search_api:page_1' => 'search_api_views:wetkit_search_api:page_1',
    ),
    'database_node_index' => array(
      'search_api_views:wetkit_database_search:default' => 'search_api_views:wetkit_database_search:default',
      'search_api_views:wetkit_database_search:panel_pane_1' => 'search_api_views:wetkit_database_search:panel_pane_1',
      'search_api_views:wetkit_database_search:search_database_results' => 'search_api_views:wetkit_database_search:search_database_results',
      'search_api_views:wetkit_database_search:page_1' => 'search_api_views:wetkit_database_search:page_1',
    ),
  );
  $export['search_api_facets_search_ids'] = $strongarm;

  return $export;
}
