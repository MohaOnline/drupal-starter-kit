<?php

namespace Drupal\dvg_authentication;

use Exception;

/**
 * Provide a separate Exception so it can be caught separately.
 *
 * This Exception is thrown on errors specific for the dvg_authentication
 * module and submodules.
 * E.g. on missing or incorrect configuration or login errors.
 */
class DvgAuthenticationException extends Exception {}
