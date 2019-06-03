<?php
/**
 * @file
 * dvg_ct_block.context.inc
 */

/**
 * Implements hook_context_load_alter().
 */
function dvg_ct_block_context_load_alter(&$context) {
  if ($context->name === 'frontpage' && isset($context->reactions['block'])) {
    $context->reactions['block']['blocks']['dvg_ct_block-dvg_ct_block_contact'] = array(
      'module' => 'dvg_ct_block',
      'delta' => 'dvg_ct_block_contact',
      'region' => 'content',
      'weight' => '-9',
    );
  }
}
