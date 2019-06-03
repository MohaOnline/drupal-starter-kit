<?php
/**
 * @file
 * Context related code for the Topical feature.
 */

/**
 * Implements hook_context_load_alter().
 */
function dvg_topical_context_load_alter(&$context) {
  if ($context->name === 'frontpage' && isset($context->reactions['block'])) {
    $context->reactions['block']['blocks']['views-topical-frontpage_block'] = array(
      'module' => 'views',
      'delta' => 'topical-frontpage_block',
      'region' => 'content',
      'weight' => '-8',
    );
  }
}
