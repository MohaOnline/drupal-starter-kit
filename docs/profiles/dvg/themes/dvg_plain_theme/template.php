<?php

/**
 * Implements template_css_alter().
 *
 * Remove **ALL** Drupal CSS files.
 */
function dvg_plain_theme_css_alter(&$css) {
  $css = array();
}

/**
 * Implements template_js_alter().
 *
 * Remove **ALL** Drupal JS files.
 */
function dvg_plain_theme_js_alter(&$js) {
  $js = array();
}

/**
 * Implements template_template_preprocess_html().
 *
 * Strip all unwanted HTML tags from the page content.
 */
function dvg_plain_theme_preprocess_page(&$vars) {
  $vars['page']['content'] = filter_xss(render($vars['page']['content']), array(
    'h1',
    'h2',
    'h3',
    'h4',
    'h5',
    'h6',
    'p',
    'br',
    'a',
    'ol',
    'ul',
    'li',
    'hr',
  ));

  // Strip all but the main content.
  foreach (element_children($vars['page']) as $region) {
    if (!in_array($region,  array('content', 'content_top', 'content_bottom'))) {
      unset($vars['page'][$region]);
    }
  }
}

/**
 * Implements template_preprocess_field().
 *
 * Hide specific fields.
 */
function dvg_plain_theme_preprocess_field(&$vars) {
  if (in_array($vars['element']['#field_name'], array(
    'field_file_license',
    'field_file_author',
  ))) {
    $vars['label_hidden'] = TRUE;
    $vars['items'] = array();
  }
}
