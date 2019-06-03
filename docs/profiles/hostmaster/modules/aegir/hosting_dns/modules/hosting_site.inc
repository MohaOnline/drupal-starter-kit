<?php
/**
 * @file
 * Holds the DNS specific hooks for hosting_site.
 */

/**
 * Implements hook_hosting_dns_default_records().
 */
function hosting_site_hosting_dns_default_records($site, $domain) {
  if (!empty($site->platform) && ($platform = node_load($site->platform)) && $server = node_load($platform->web_server)) {

    // Fetch the server's ip addresses.
    $ip_addresses = $server->ip_addresses;
    if (empty($ip_addresses)) {
      $ip_addresses = array('127.0.0.1');
    }

    // Prepare the return array.
    $return = array();

    // Only do this for the 'main' domain.
    if ($domain == $site->title) {

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
