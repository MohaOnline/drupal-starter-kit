<?php

/**
 * @file
 * Provision hooks for the delete command
 */

/**
 * Implements drush_hook_post_COMMAND().
 */
function drush_hosting_dns_post_provision_delete() {
  if (d()->type == 'site') {

    // Fetch records.
    $dns_records = d()->dns_records;

    // Delete all zones associated with this site.
    foreach ($dns_records as $zone => $records) {
      d()->service('dns')->delete_zone($zone);
    }
    d()->service('dns')->parse_configs();
  }
}
