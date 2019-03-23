<?php

/**
 * @file
 * Documents hooks provided by this module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Allow other modules to change the status of the AMP page. This allow you to
 * turn AMP on/off for any node or page.
 *
 * @param bool $result
 *   TRUE if its an AMP page, otherwise FALSE
 */
function hook_is_amp_request_alter(&$result) {

}

/**
 * Allow other modules to change the metadata before it's outputted to the page.
 *
 * @param $metadata_json
 *    The json array with key/values
 * @param $node
 *    The node object
 * @param $type
 *    The type
 */
function hook_amp_metadata_alter(&$metadata_json, $node, $type) {

}

/**
 * Allow other modules to alter the amp converter.
 *
 * @param Lullabot\AMP\AMP
 */
function hook_amp_converter_alter($amp) {

}

/**
 * Modify the list of AMP JS components.
 *
 * @param array $js_list
 *
 */
function hook_amp_js_list_alter($js_list) {
  $js_list['amp-some-component'] = 'https://cdn.ampproject.org/v0/amp-some-component-0.1.js';
}
