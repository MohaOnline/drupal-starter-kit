<?php

/**
 * Implements hook_block_info().
 */
function dvg_global_block_info() {
  $blocks['dvg_global__footer_text'] = array(
    'info' => t('Footer text'),
  );

  $blocks['dvg_global__front_about'] = array(
    'info' => t('Frontpage About menu page'),
  );

  $blocks['dvg_global__front_organization'] = array(
    'info' => t('Frontpage Organization menu page'),
  );

  return $blocks;
}

/**
 * Implements hook_block_info_alter().
 */
function dvg_global_block_info_alter(&$blocks, $theme, $code_blocks) {
  // DVG doesn't have a 'help' region, so we prevent this block from being assigned there.
  $blocks['system']['help']['status'] = 0;
  $blocks['system']['help']['region'] = BLOCK_REGION_NONE;
}

/**
 * Implements hook_block_view().
 */
function dvg_global_block_view($delta = '') {
  $block = array();

  switch ($delta) {
    case 'dvg_global__footer_text':
      $block['subject'] = '';
      $block['content'] = _dvg_global_block_footer_text();
      break;
    case 'dvg_global__front_about':
    case 'dvg_global__front_organization':
      $block['subject'] = '';
      $block['content'] = _dvg_global_block_frontpage_block_page($delta);
      break;
  }

  return $block;
}

/**
 * Helper function that builds the footer text block contents.
 */
function _dvg_global_block_footer_text() {
  $output = array();
  $texts = variable_get('dvg_custom__footer_texts', array());

  foreach ($texts as $text) {
    if (!empty($text['value'])) {
      $output[] = array(
        '#prefix' => '<div class="footer-line">',
        '#suffix' => '</div>',
        '#markup' => check_markup($text['value'], $text['format'])
      );
    }
  }

  if (empty($output) && user_access('dvg_global administer texts')) {
    // Display link to the config page in case no content has been set (yet).
    $output = array(
      '#markup' => l(t('Configure the footer texts'), 'admin/config/content/texts'));
  }

  return render($output);
}

/**
 * Helper function that builds the frontpage introduction page blocks.
 */
function _dvg_global_block_frontpage_block_page($delta) {
  $frontpage_node = functional_content_node($delta . '__nid');
  if ($frontpage_node && node_access('view', $frontpage_node)) {
    $node_view = node_view($frontpage_node, 'frontpage');
    return render($node_view);
  }

  if (user_access('dvg_global administer functional content')) {
    // Display link to the config page in case no rendered node was returned.
    return array(
      '#markup' => l(t('Configure the content placed above this block'), 'admin/config/content/functional-content')
    );
  }
}

/**
 * Implements hook_preprocess_block().
 */
function dvg_global_preprocess_block(&$variables) {
  $block = $variables['block'];

  switch ($block->delta) {
    case 'dvg_global__front_about':
      $variables['classes_array'][] = 'menu-about';
      $variables['classes_array'][] = 'menu-content-block';
      break;

    case 'dvg_global__front_organization':
      $variables['classes_array'][] = 'menu-organization';
      $variables['classes_array'][] = 'menu-content-block';
      break;

    case 'menu-footer-menu':
      $variables['classes_array'][] = 'footer-menu';
      $variables['title_attributes_array']['class'][] = 'element-invisible';
      break;

    case 'menu-social-media-menu':
      $variables['classes_array'][] = 'social-menu';
      $variables['title_attributes_array']['class'][] = 'element-invisible';
      break;

    case '-exp-search-page':
      $variables['classes_array'][] = 'search-block';
      break;
  }

  if ($block->module == 'menu_block') {
    $variables['classes_array'][] = $block->delta;
  }
}

/**
 * Implements hook_block_view_alter().
 */
function dvg_global_block_view_alter(&$data, $block) {
  if ($block->module == 'menu_block') {
    $data['subject'] = FALSE;
  }
}
