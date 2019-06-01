<?php
/**
 * @file
 * Stub file for theme_admin_page().
 */

/**
 * Returns HTML for an administrative page.
 *
 * @param $variables
 *   An associative array containing:
 *   - blocks: An array of blocks to display. Each array should include a
 *     'title', a 'description', a formatted 'content' and a 'position' which
 *     will control which container it will be in. This is usually 'left' or
 *     'right'.
 *
 * @ingroup themeable
 */
function wetkit_bootstrap_admin_page($variables) {
  $blocks = $variables['blocks'];

  $stripe = 0;
  $container = array();

  foreach ($blocks as $block) {
    if ($block_output = theme('admin_block', array('block' => $block))) {
      if (empty($block['position'])) {
        // perform automatic striping.
        $block['position'] = ++$stripe % 2 ? 'left' : 'right';
      }
      if (!isset($container[$block['position']])) {
        $container[$block['position']] = '';
      }
      $container[$block['position']] .= $block_output;
    }
  }

  $output = '<div class="admin clearfix">';
  $output .= theme('system_compact_link');
  $output .= '<div class="row">';
  foreach ($container as $id => $data) {
    $output .= '<div class="col-md-6 ' . $id . ' clearfix">';
    $output .= $data;
    $output .= '</div>';
  }
  $output .= '</div>';
  $output .= '</div>';

  return $output;
}
