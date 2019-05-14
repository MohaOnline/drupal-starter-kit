<?php

 /**
 * Optional Footer Link to SooperThemes
 */
function glazed_free_attribution_link() {
  $key = ord($_SERVER["SERVER_NAME"])%10;
  $links = array(
  '<a href="http://www.sooperthemes.com/">Drupal Themes by SooperThemes</a>',
  '<a href="http://www.sooperthemes.com">Drupal templates</a> by SooperThemes',
  '<a href="http://www.sooperthemes.com/">Drupal Premium Themes</a>',
  'Bootstrap <a href="http://www.sooperthemes.com/">Premium Drupal theme</a> by SooperThemes',
  '<a href="http://www.sooperthemes.com">Drupal templates</a> by SooperThemes',
  'SooperThemes <a href="http://www.sooperthemes.com">Premium Drupal themes</a>',
  'Premium <a href="http://www.sooperthemes.com">Drupal themes</a> by SooperThemes.com',
  '<a href="http://www.sooperthemes.com/">Drupal theme</a> by SooperThemes',
  '<a href="http://www.sooperthemes.com">Drupal Premium Themes</a> by SooperThemes',
  '<a href="http://www.sooperthemes.com/">Premium Drupal themes</a>',
  );
  return '<p class="clear-both sooperthemes-attribution-link glazed-util-text-muted small text-center">' . $links[$key] . '</p>';
}
