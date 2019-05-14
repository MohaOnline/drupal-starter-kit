<?php
if ($breakpoint = theme_get_setting('header_mobile_breakpoint')) {
  if ($breakpoint > 4099) {
    $breakpoint = 99999;
  }
}
else {
  $breakpoint = 1200;
}
$nav_init = <<<EOT
var glazedNavBreakpoint = {$breakpoint};
var glazedWindowWidth = window.innerWidth;
if (glazedWindowWidth > glazedNavBreakpoint) {
  document.getElementsByTagName('body')[0].className += ' body--glazed-nav-desktop';
}
else {
  document.getElementsByTagName('body')[0].className += ' body--glazed-nav-mobile';
}
EOT;
drupal_add_js($nav_init, array(
  'type' => 'inline',
  'scope' => 'footer',
  )
);
