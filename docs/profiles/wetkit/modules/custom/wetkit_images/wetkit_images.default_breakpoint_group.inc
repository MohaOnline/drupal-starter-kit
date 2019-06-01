<?php
/**
 * @file
 * wetkit_images.default_breakpoint_group.inc
 */

/**
 * Implements hook_default_breakpoint_group().
 */
function wetkit_images_default_breakpoint_group() {
  $export = array();

  $breakpoint_group = new stdClass();
  $breakpoint_group->disabled = FALSE; /* Edit this to true to make a default breakpoint_group disabled initially */
  $breakpoint_group->api_version = 1;
  $breakpoint_group->machine_name = 'wxt_responsive_image';
  $breakpoint_group->name = 'WxT Responsive Image';
  $breakpoint_group->breakpoints = array(
    0 => 'custom.user.mobile',
    1 => 'custom.user.narrow',
    2 => 'custom.user.wide',
  );
  $breakpoint_group->type = 'custom';
  $breakpoint_group->overridden = 0;
  $export['wxt_responsive_image'] = $breakpoint_group;

  return $export;
}
