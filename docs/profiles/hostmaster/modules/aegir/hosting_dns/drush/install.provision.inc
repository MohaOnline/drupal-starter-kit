<?php

/**
 * @file
 * Provision hooks for the install command.
 */

/**
 * Implements drush_hook_post_COMMAND().
 */
function drush_hosting_dns_post_provision_install() {
  // We create this post-install because we need the ip's to be assigned already.
  d()->service('dns')->create_host();
  d()->service('dns')->parse_configs();
}
