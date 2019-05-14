<?php

$_page_title_animate = theme_get_setting('page_title_animate');

if ($_page_title_animate) {
  $_path_to_css = drupal_get_path('theme', 'glazed_free') . '/js/vendor/animate.css/animate.min.css';
  drupal_add_css($_path_to_css, array('group' => CSS_DEFAULT, 'every_page' => TRUE));
}
