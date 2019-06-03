<?php

/**
 * @file
 * Variable defaults for Media: skyfish.
 */

/**
 * Define our constants.
 */
define('MEDIA_SKYFISH_API_BASE_URL', 'https://api.colourbox.com');
define('MEDIA_SKYFISH_API_AUTHENTICATION_SLUG', '/authenticate/userpasshmac');
define('MEDIA_SKYFISH_NAMESPACE', 'media_skyfish__');

/**
 * Get variable for skyfish.
 *
 * @param string $name
 *   String variable name.
 *
 * @return string
 *   Data pulled from variable.
 */
function media_skyfish_variable_get($name) {
  $variable_name = media_skyfish_variable_name($name);
  return variable_get($variable_name, NULL);
}

/**
 * Get skyfish variable name.
 *
 * @param string $name
 *   Variable name without prefix.
 *
 * @return string
 *   Prefixed variable name.
 */
function media_skyfish_variable_name($name) {
  return MEDIA_SKYFISH_NAMESPACE . $name;
}
