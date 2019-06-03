<?php

/**
 * @file
 * Provides functions to communicate with moxtra endpoint
 * @see https://developer.moxtra.com/docs/docs-rest-api
 */


/**
 * Retrieve api endpoint from global settings
 *
 * @return string
 */
function opigno_moxtra_app_api_get_gateway_uri() {
  return OPIGNO_MOXTRA_APP_MOXTRA_API_GATEWAY;
}