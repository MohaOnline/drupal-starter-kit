<?php
/**
 * @file
 * Block related code for the Topical feature.
 */

/**
 * Implements hook_preprocess_block().
 */
function dvg_topical_preprocess_block(&$variables) {
  $block = $variables['block'];

  switch ($block->delta) {
    case 'topical-frontpage_block':
      $variables['classes_array'][] = 'topical-block';
      break;
  }
}
