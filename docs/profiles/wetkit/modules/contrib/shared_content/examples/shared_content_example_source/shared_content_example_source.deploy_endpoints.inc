<?php
/**
 * @file
 * shared_content_example_source.deploy_endpoints.inc
 */

/**
 * Implements hook_deploy_endpoints_default().
 */
function shared_content_example_source_deploy_endpoints_default() {
  $export = array();

  $endpoint = new DeployEndpoint();
  $endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
  $endpoint->api_version = 1;
  $endpoint->name = 'destination_site_endpoint';
  $endpoint->title = 'Destination Site';
  $endpoint->description = 'Deployment endpoint for a particular destination site.';
  $endpoint->debug = 0;
  $endpoint->authenticator_plugin = 'DeployAuthenticatorSession';
  $endpoint->authenticator_config = array(
    'username' => 'content.deployment.user',
    'password' => 'password',
  );
  $endpoint->service_plugin = 'DeployServiceRestJSON';
  $endpoint->service_config = array(
    'url' => 'http://destination-site.example.com/services/rest',
  );
  $export['destination_site_endpoint'] = $endpoint;

  return $export;
}
