<?php
/**
 * @file
 * dvg_file_category_type.file_type.inc
 */

/**
 * Implements hook_file_default_types().
 */
function dvg_file_category_type_file_default_types() {
  $export = array();

  $file_type = new stdClass();
  $file_type->disabled = FALSE; /* Edit this to true to make a default file_type disabled initially */
  $file_type->api_version = 1;
  $file_type->type = 'file_category_item';
  $file_type->label = 'PDF list item';
  $file_type->description = 'Use this type if the file is part of a selected list of files.';
  $file_type->mimetypes = array(
    0 => 'application/pdf',
  );
  $export['file_category_item'] = $file_type;

  return $export;
}
