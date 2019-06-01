<?php
/**
 * @file
 * shared_content_example_source.deploy_plans.inc
 */

/**
 * Implements hook_deploy_plans_default().
 */
function shared_content_example_source_deploy_plans_default() {
  $export = array();

  $plan = new DeployPlan();
  $plan->disabled = FALSE; /* Edit this to true to make a default plan disabled initially */
  $plan->api_version = 1;
  $plan->name = 'destination_site_plan';
  $plan->title = 'Destination Site';
  $plan->description = 'Deploys content to a particular destination site.';
  $plan->debug = 0;
  $plan->aggregator_plugin = 'DeployAggregatorManaged';
  $plan->aggregator_config = array(
    'delete_post_deploy' => 1,
  );
  $plan->fetch_only = 0;
  $plan->processor_plugin = 'DeployProcessorMemory';
  $plan->processor_config = array();
  $plan->endpoints = array(
    'destination_site_endpoint' => 'destination_site_endpoint',
  );
  $export['destination_site_plan'] = $plan;

  return $export;
}
