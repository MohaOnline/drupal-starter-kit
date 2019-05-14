<?php

/**
 * Override or insert variables into the page template.
 */
function glazed_free_process_page(&$vars) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_page_alter($vars);
  }
}
