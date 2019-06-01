<?php
/**
 * @file
 * wetkit_deployment_source.deploy_plans.inc
 */

/**
 * Implements hook_deploy_plans_default().
 */
function wetkit_deployment_source_deploy_plans_default() {
  $export = array();

  $plan = new DeployPlan();
  $plan->disabled = FALSE; /* Edit this to true to make a default plan disabled initially */
  $plan->api_version = 1;
  $plan->name = 'destination_site_plan';
  $plan->title = 'Push to Production (Memory)';
  $plan->description = 'Deploy entities to the Production Environment using the in-memory method.';
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
    'destination_site_batch_endpoint' => 0,
  );
  $plan->dependency_plugin = '';
  $export['destination_site_plan'] = $plan;

  $plan = new DeployPlan();
  $plan->disabled = FALSE; /* Edit this to true to make a default plan disabled initially */
  $plan->api_version = 1;
  $plan->name = 'wetkit_deployment';
  $plan->title = 'Push to Production (Batch)';
  $plan->description = 'Deploy entities to the Production Environment using the Batch method.';
  $plan->debug = 0;
  $plan->aggregator_plugin = 'WetKitDeployAggregatorManaged';
  $plan->aggregator_config = array(
    'delete_post_deploy' => 1,
  );
  $plan->fetch_only = 0;
  $plan->processor_plugin = 'WetKitDeployProcessorQueue';
  $plan->processor_config = array();
  $plan->endpoints = array(
    'destination_site_endpoint' => 0,
    'destination_site_batch_endpoint' => 'destination_site_batch_endpoint',
  );
  $plan->dependency_plugin = '';
  $export['wetkit_deployment'] = $plan;

  return $export;
}
