<?php
/**
 * @file
 * wetkit_language.features.defaultconfig.inc
 */

/**
 * Implements hook_defaultconfig_features().
 */
function wetkit_language_defaultconfig_features() {
  return array(
    'wetkit_language' => array(
      'strongarm' => 'strongarm',
    ),
  );
}
