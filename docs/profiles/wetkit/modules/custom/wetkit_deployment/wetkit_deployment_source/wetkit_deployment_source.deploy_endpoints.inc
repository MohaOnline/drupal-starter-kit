<?php
/**
 * @file
 * wetkit_deployment_source.deploy_endpoints.inc
 */

/**
 * Implements hook_deploy_endpoints_default().
 */
function wetkit_deployment_source_deploy_endpoints_default() {
  $export = array();

  $endpoint = new DeployEndpoint();
  $endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
  $endpoint->api_version = 1;
  $endpoint->name = 'destination_site_batch_endpoint';
  $endpoint->title = 'WetKit Deployment Production Server';
  $endpoint->description = 'Deployment endpoint for a particular destination site.';
  $endpoint->debug = 0;
  $endpoint->authenticator_plugin = 'DeployAuthenticatorSession';
  $endpoint->authenticator_config = array(
    'username' => 'admin',
    'password' => 'WetKit2015',
  );
  $endpoint->service_plugin = 'WetKitServiceRestJSON';
  $endpoint->service_config = array(
    'url' => 'http://wxt.prod:8000/services/rest',
  );
  $export['destination_site_batch_endpoint'] = $endpoint;

  $endpoint = new DeployEndpoint();
  $endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
  $endpoint->api_version = 1;
  $endpoint->name = 'destination_site_endpoint';
  $endpoint->title = 'Destination Site';
  $endpoint->description = 'Deployment endpoint for a particular destination site.';
  $endpoint->debug = 0;
  $endpoint->authenticator_plugin = 'DeployAuthenticatorSession';
  $endpoint->authenticator_config = array(
    'username' => 'admin',
    'password' => 'WetKit2015',
  );
  $endpoint->service_plugin = 'WetKitServiceRestJSON';
  $endpoint->service_config = array(
    'url' => 'http://wxt.prod:8000/services/rest',
  );
  $export['destination_site_endpoint'] = $endpoint;

  return $export;
}
