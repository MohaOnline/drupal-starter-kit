<?php

/**
 * @file
 * Theme callbacks for the file entity module.
 */

/**
 * Copy of theme_file_file_link() for linking to the view file page.
 *
 * @see theme_file_file_link()
 */
function wetkit_bootstrap_file_link($variables) {
  $file = $variables['file'];
  $icon_directory = $variables['icon_directory'];

  $url = 'file/' . $file->fid;
  $icon = theme('file_icon', array('file' => $file, 'icon_directory' => $icon_directory));

  // Set options as per anchor format described at
  // http://microformats.org/wiki/file-format-examples
  $options = array(
    'attributes' => array(
      'type' => $file->filemime . '; length=' . $file->filesize,
    ),
  );

  // Use the description as the link text if available.
  if (empty($file->description)) {
    $link_text = $file->filename;
  }
  else {
    $link_text = $file->description;
    $options['attributes']['title'] = check_plain($file->filename);
  }

  // Check for additional display field.
  if (!empty($file->type)) {
    $field_display_name = 'field_' . $file->type . '_name';
    if (!empty($file->{$field_display_name})) {
      $field_display = field_get_items('file', $file, $field_display_name);
      if (isset($field_display)) {
        $link_text = $field_display[0]['value'];
      }
    }
  }

  return '<span class="file">' . $icon . ' ' . l($link_text, $url, $options) . '</span>';
}
