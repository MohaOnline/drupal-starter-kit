<?php

/**
 * @file
 * Provision hooks for the verify command.
 */

/**
 * Implements drush_hook_post_COMMAND().
 */
function drush_hosting_dns_post_provision_verify() {
  if (d()->type == 'site') {
    // Todo: This does not need to happen on every verify.
    d()->service('dns')->create_host();
    d()->service('dns')->parse_configs();
  }
}
