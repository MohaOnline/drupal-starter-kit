<?php

/**
 * @file
 * Expose the certificate feature to hostmaster.
 */

/**
 * Implements hook_hosting_feature().
 *
 * Register the certificate hosting feature with Aegir, initially this feature
 * will be disabled.
 */
function hosting_certificate_hosting_feature() {
  $features['certificate'] = array(
    'title' => t('Certificate service'),
    'description' => t('Certificate service to generate HTTPS certificates for Aegir-hosted sites.'),
    'status' => HOSTING_FEATURE_DISABLED,
    'module' => 'hosting_certificate',
    'group' => 'experimental'
    );
  return $features;
}
