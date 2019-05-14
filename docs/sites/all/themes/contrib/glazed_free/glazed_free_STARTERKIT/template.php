<?php
/**
 * @file
 * template.php
 */


/**
 * Override or insert variables into the page template for HTML output.
 */
function glazed_free_STARTERKIT_process_html(&$vars) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_html_alter($vars);
  }
}

/**
 * Override or insert variables into the page template.
 */
function glazed_free_STARTERKIT_process_page(&$vars) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_page_alter($vars);
  }
}
