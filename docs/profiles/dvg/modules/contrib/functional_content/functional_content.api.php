<?php

/**
 * @file
 * Hooks provided by Functional Content.
 */

/**
 * Implements hook_functional_content().
 */
function hook_functional_content($reset = FALSE) {
  $items['functional_content_name'] = array(
    'label' => t('My functional content item label'),
    'description' => t('Custom functional Content item for me'),
  );

  return $items;
}
