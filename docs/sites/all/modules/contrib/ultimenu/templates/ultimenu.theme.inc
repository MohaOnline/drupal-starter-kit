<?php
/**
 * @file
 * Hooks and preprocess functions for the Ultimenu module.
 */

/**
 * Process variables for ultimenu.tpl.php.
 *
 * @see ultimenu.tpl.php
 */
function template_preprocess_ultimenu(&$variables) {
  $element = $variables['element'];
  $variables['config'] = $element['#config'];
  $variables['delta'] = $element['#delta'];

  // Create the $content variable that templates expect.
  $variables['content'] = $element['#content'];

  // Add orientation classes.
  $orientation = $element['#config']['orientation'];
  $variables['classes_array'][] = strpos($orientation, 'v') !== FALSE ? 'vertical' : 'horizontal';
  $variables['classes_array'][] = $element['#config']['orientation'];
  $variables['classes_array'][] = 'ultimenu-' . $element['#delta'];

  // Add skin classes based on safe CSS file name.
  if (($skin = $element['#config']['skin_name']) !== NULL) {
    $variables['classes_array'][] = drupal_html_class($skin);
  }

  // We don't float list by default, but hence to fix possible list floating.
  $variables['classes_array'][] = 'clearfix';
}

/**
 * HTML for a generic Ultimenu region wrapper.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: A render element containing the properties of the ultimenu
 *     region element, #config, #region and #children.
 *
 * @ingroup themeable
 */
function theme_ultimenu_region($variables) {
  extract($variables);

  $attributes = array();
  $attributes['class'][] = 'ultimenu-flyout';
  $attributes['class'][] = drupal_html_class($element['#region']);
  $attributes['class'][] = 'clearfix';

  // HTML5 section contains Ultimenu region and its blocks.
  $output = '<section ' . drupal_attributes($attributes) . '>';
  $output .= $element['#children'];
  $output .= '</section>';

  return $output;
}

/**
 * HTML for a menu link and ultimenu.
 *
 * We don't use regular menu_link to avoid issue with a theme that provides a
 * fully customized menu_link.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: Structured array data for a menu link.
 *
 * @ingroup themeable
 */
function theme_ultimenu_link(array $variables) {
  extract($variables);

  $goodies  = ultimenu_get_settings('goodies');
  $ultimenu = '';

  $title_stripped = isset($element['#title']) ? strip_tags($element['#title']) : '';

  // Having a sane class on multilingual site is fairly expensive.
  // Only use it if transliteration and locale (i18n dependent) are installed.
  // @todo add option if any request. function_exists('locale') &&
  // if (function_exists('transliteration_clean_filename')) {
  // $title_class = transliteration_clean_filename($title_stripped);
  // }
  $is_description = isset($element['#localized_options']['attributes']) && !empty($element['#localized_options']['attributes']['title']);

  // Render description, and strip the title.
  if (!empty($goodies['menu-desc']) && $is_description) {
    // Manual HTML is required to display wrapper for description, but we'll
    // close the hole again, leaving only manual SMALL tag.
    // If you are overriding this, please take relevant measure with less
    // limited options to say: em and strong:
    // filter_xss($string, array('em', 'strong'))
    $element['#localized_options']['html'] = TRUE;
    $menu_description = '<small>' . strip_tags($element['#localized_options']['attributes']['title']) . '</small>';
    $element['#title'] = !empty($goodies['desc-top']) ? $menu_description . $title_stripped : $title_stripped . $menu_description;
  }

  // Add LI counter class based on counter if so configured.
  if (!empty($goodies['counter-class'])) {
    static $item_id = 0;
    $element['#attributes']['class'][] = 'menu-list-' . (++$item_id);
  }

  // Add LI title class based on title if so configured.
  if (!empty($goodies['title-class'])) {
    $element['#attributes']['class'][] = drupal_html_class('menu-list-' . $title_stripped);
  }

  // Add LI mlid class based on mlid if so configured.
  if (!empty($goodies['mlid-class'])) {
    $element['#attributes']['class'][] = 'menu-' . $element['#original_link']['mlid'];
  }

  // Remove browser tooltip if so configured.
  if (!empty($goodies['no-tooltip'])) {
    $element['#localized_options']['attributes']['title'] = '';
  }

  // Render our Ultimenu region and its blocks if any.
  if ($element['#below']) {
    $element['#attributes']['class'][] = 'has-ultimenu';
    $ultimenu = drupal_render($element['#below']);
  }

  $element['#localized_options']['attributes']['class'][] = 'ultimenu-item';
  $output = l($element['#title'], $element['#href'], $element['#localized_options']);

  // LI attributes. This check should prevent empty class attributes from print.
  $list_attributes = !empty($element['#attributes']['class']) ? drupal_attributes($element['#attributes']) : '';

  return '<li' . $list_attributes . '>' . $output . $ultimenu . "</li>\n";
}
