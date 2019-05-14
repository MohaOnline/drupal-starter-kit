<?php

/**
 * Implements template_preprocess_block()
 */
function glazed_free_preprocess_block(&$vars) {
  if ((theme_get_setting('block_design_regions'))
    && in_array($vars['block']->region, array_filter(theme_get_setting('block_design_regions')))) {
    if ($block_well = theme_get_setting('block_well')) {
      $vars['classes_array'][] = $block_well;
    }
    if ($title_well = theme_get_setting('title_well')) {
      $vars['title_attributes_array']['class'][] = $title_well;
    }
    if (theme_get_setting('title_sticker')) {
      $vars['title_prefix']['wrap_1']['#markup'] = '<div class="wrap-block-title">';
      $vars['title_suffix']['wrap_2']['#markup'] = '</div>';
    }
    if ((theme_get_setting('block_preset'))
      && (theme_get_setting('block_divider'))
      && (theme_get_setting('block_divider_thickness'))) {
      $vars['title_suffix']['hr']['#markup'] = '<hr class="block-hr">';
    }
  }
}
