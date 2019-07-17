<?php

/**
 * @file
 * The main mappings page.
 */

/**
 * Main mappings page.
 */
function exif_custom_mappings() {
  $mappings = _exif_custom_get_maps();
  if ($mappings == FALSE) {
    $output = '<p>No mappings have been created yet.</p>';
  }
  else {
    $site_default_mid = variable_get('exif_custom_default', -1);
    $output = '<table>';
    while ($result = $mappings->fetchObject()) {
      if (exif_custom_get_user_default() == $result->mid) {
        $default = '<strong>User Default</strong>';
      }
      elseif ($site_default_mid == $result->mid) {
        $default = 'Site Default';
      }
      else {
        $default = '';
      }
      $output .= '<tr><td>' . $result->name . '</td>';
      $output .= '<td>' . l('edit', 'admin/config/media/exif_custom/map/' . $result->mid) . '</td>';
      $output .= '<td>' . $default . '</td></tr>';
    }
    $output .= '</table>';
  }

  return $output;
}
