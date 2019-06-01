<?php
/**
 * @file
 * Stub file for bootstrap_menu_tree() and suggestion(s).
 */

/**
 * Returns HTML for a wrapper for a menu sub-tree.
 *
 * @param array $variables
 *   An associative array containing:
 *   - tree: An HTML string containing the tree's items.
 *
 * @return string
 *   The constructed HTML.
 *
 * @see template_preprocess_menu_tree()
 * @see theme_menu_tree()
 *
 * @ingroup theme_functions
 */
function wetkit_bootstrap_menu_tree(&$variables) {
  return '<ul class="menu nav">' . $variables['tree'] . '</ul>';
}

/**
 * Overrides theme_menu_tree().
 */
function wetkit_bootstrap_menu_tree__menu_block__main_menu(&$variables) {
  $wxt_active = variable_get('wetkit_wetboew_theme', 'theme-wet-boew');
  $library_path = libraries_get_path($wxt_active, TRUE);
  $wxt_active = str_replace('-', '_', $wxt_active);
  $wxt_active = str_replace('theme_', '', $wxt_active);
  if ($wxt_active == 'gcweb_v5') {
    return '<ul role="menu" aria-orientation="vertical" data-ajax-replace="">' . $variables['tree'] . '</ul>';
  }
  else {
    return '<ul class="list-inline menu" role="menubar">' . $variables['tree'] . '</ul>';
  }
}

/**
 * Overrides theme_menu_tree().
 */
function wetkit_bootstrap_menu_tree__menu_block__sidebar(&$variables) {
  return '<ul class="list-group menu list-unstyled" role="menubar">' . $variables['tree'] . '</ul>';
}

/**
 * Overrides theme_menu_tree().
 */
function wetkit_bootstrap_menu_tree__menu_block__group_list(&$variables) {
  return '<div class="list-group" role="menu">' . $variables['tree'] . '</div>';
}

/**
 * Overrides theme_menu_tree().
 */
function wetkit_bootstrap_menu_tree__menu_block__section(&$variables) {
  return '<ul class="list-group menu list-unstyled" role="menubar">' . $variables['tree'] . '</ul>';
}

/**
 * Overrides theme_menu_tree().
 */
function wetkit_bootstrap_menu_tree__devel($variables) {
  return '<ul class="navbar-menu navbar-menu-devel">' . $variables['tree'] . '</ul>';
}

/**
 * Overrides theme_menu_tree().
 */
function wetkit_bootstrap_menu_tree__shortcut_set_1($variables) {
  return '<ul class="navbar-menu navbar-menu-shortcut">' . $variables['tree'] . '</ul>';
}

/**
 * Overrides theme_menu_tree().
 */
function wetkit_bootstrap_menu_tree__menu_block__mid_footer_menu(&$variables) {
  return $variables['tree'];
}

/**
 * Bootstrap theme wrapper function for the primary menu links.
 */
function wetkit_bootstrap_menu_tree__primary(&$variables) {
  return '<ul class="menu nav navbar-nav">' . $variables['tree'] . '</ul>';
}

/**
 * Bootstrap theme wrapper function for the secondary menu links.
 */
function wetkit_bootstrap_menu_tree__secondary(&$variables) {
  return '<ul class="menu nav navbar-nav secondary">' . $variables['tree'] . '</ul>';
}

/**
 * Overrides theme_menu_tree() for book module.
 */
function wetkit_bootstrap_menu_tree__book_toc(&$variables) {
  $output = '<div class="book-toc btn-group pull-right">';
  $output .= '  <button type="button" class="btn btn-link dropdown-toggle" data-toggle="dropdown">';
  $output .= t('!icon Outline !caret', array(
    '!icon' => _bootstrap_icon('list'),
    '!caret' => '<span class="caret"></span>',
  ));
  $output .= '</button>';
  $output .= '<ul class="dropdown-menu" role="menu">' . $variables['tree'] . '</ul>';
  $output .= '</div>';
  return $output;
}

/**
 * Overrides theme_menu_tree() for book module.
 */
function wetkit_bootstrap_menu_tree__book_toc__sub_menu(&$variables) {
  return '<ul class="dropdown-menu" role="menu">' . $variables['tree'] . '</ul>';
}
