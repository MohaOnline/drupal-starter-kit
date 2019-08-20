<?php

/**
 * @file
 * Provides a generic OpenID Connect client plugin.
 *
 * Used to login to Drupal sites powered by Windows
 * sites powered by oauth2-server-php.
 */

$plugin = array(
  'title' => t('Windows Azure AD'),
  'class' => 'OpenidConnectWindowsAadClient',
);
