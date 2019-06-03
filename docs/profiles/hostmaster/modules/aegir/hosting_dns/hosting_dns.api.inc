<?php
/**
 * @file
 * Hooks provided by the hosting_dns module.
 */

/**
 * Register default DNS settings for all new domains.
 *
 * If a user creates a site in aegir, default DNS settings will be applied for
 * the enabled modules. This might be useful in de future if aegir for instance
 * would implement mail or FTP integration.
 *
 * @param $site
 *   The site node the DNS entry will added to, or NULL in the case of the
 *   global settings form.
 *
 * @param $domain
 *   The specific domain to create the record for.
 *
 * @return array
 *   An array of DNS records containing the title, type, ttl and value. The
 *   hosting_dns_default_ttl variable can be used like this:
 *   variable_get('hosting_dns_default_ttl', 86400).
 */
function hook_hosting_dns_default_records($site = NULL, $domain = NULL) {
  return array(
    array(
      'title' => '@',
      'dns_type' => 'A',
      'dns_ttl' => variable_get('hosting_dns_default_ttl', 86400),
      'dns_value' => '127.0.0.1',
    ),
    array(
      'title' => '@',
      'dns_type' => 'MX',
      'dns_ttl' => variable_get('hosting_dns_default_ttl', 86400),
      'dns_value' => '10 mail',
    ),
    array(
      'title' => 'mail',
      'dns_type' => 'CNAME',
      'dns_ttl' => 3600,
      'dns_value' => '@',
    ),
    array(
      'title' => 'ftp',
      'dns_type' => 'CNAME',
      'dns_ttl' => 86400,
      'dns_value' => '@',
    ),
  );
}

/**
 * Implements hook_TYPE_alter()
 *
 * Allows editing of the records before they are processed.
 *
 * @example
 * $records['example.com'] => array(
 *   'www' => array(
 *     'dns_type' => 'A',
 *     'dns_ttl' => 86400,
 *     'dns_value' => '127.0.0.1',
 *   )
 * );
 *
 * @param array $records
 *   An array of records keyed by zone and title, passed by reference.
 *
 * @param string $site
 *   The site node.
 *
 * @param string $domain
 *   The domain in case the site has multiple aliases.
 */
function hook_hosting_dns_records_alter(&$records, $site = NULL, $domain = NULL) {
}
