<?php
/**
 * @file
 * wetkit_apachesolr.apachesolr_environments.inc
 */

/**
 * Implements hook_apachesolr_environments().
 */
function wetkit_apachesolr_apachesolr_environments() {
  $export = array();

  $environment = new stdClass();
  $environment->api_version = 1;
  $environment->env_id = 'solr';
  $environment->name = 'localhost server';
  $environment->url = 'http://localhost:8983/solr';
  $environment->service_class = '';
  $environment->conf = array(
    'apachesolr_index_last' => array(),
  );
  $environment->index_bundles = array(
    'node' => array(
      0 => 'wetkit_documentation',
      1 => 'wetkit_page',
    ),
  );
  $export['solr'] = $environment;

  return $export;
}
