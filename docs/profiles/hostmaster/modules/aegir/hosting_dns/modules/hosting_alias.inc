<?php
/**
 * @file
 * Holds the DNS specific hooks for hosting_alias.
 */

/**
 * Implements hook_hosting_dns_default_records().
 */
function hosting_alias_hosting_dns_default_records($site, $domain) {
  if (!empty($site->platform) && ($platform = node_load($site->platform)) && $server = node_load($platform->web_server)) {

    // Fetch the server's ip addresses.
    $ip_addresses = $server->ip_addresses;
    if (empty($ip_addresses)) {
      $ip_addresses = array('127.0.0.1');
    }

    // Prepare the return array.
    $return = array();

    // Automatically create A records for aliases.
    if ($domain != $site->title) {

      foreach ($ip_addresses as $ip) {
        $return += array(
          array(
            'title' => '@',
            'dns_ttl' => variable_get('hosting_dns_default_ttl', 86400),
            'dns_type' => 'A',
            'dns_value' => $ip,
          ),
          array(
            'title' => 'www',
            'dns_ttl' => variable_get('hosting_dns_default_ttl', 86400),
            'dns_type' => 'CNAME',
            'dns_value' => '@',
          ),
        );
      }
    }
    return $return;
  }
}
