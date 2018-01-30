<?php

/**
 * @file
 * Easychart helpers.
 */

/**
 *  Update the csv data from the url stored in the database.
 */
function _easychart_update_csv_from_url() {

  $fields = field_read_fields(array('type' => 'easychart'));
  $clear_caches = FALSE;

  foreach ($fields as $field) {

    // Get all Easycharts with a csv url.
    $table_name = _field_sql_storage_tablename($field);
    $prefix = str_replace('field_data_','', $table_name);
    $query = db_select($table_name, 'ec');
    $query->fields('ec',array($prefix. '_csv_url', $prefix. '_csv', 'delta', 'entity_id', 'revision_id'));
    $query->condition('ec.' .$prefix. '_csv_url', '', '!=');

    $results = $query->execute();

    foreach ($results as $result) {
      $url = $result->easychart_csv_url;

      // Parse the csv to an array.
      $csv_data = file_get_contents($url);
      if (!empty($csv_data)) {
        $delimiter = _find_csv_delimiter($csv_data);
        $csv = json_encode(_parse_csv($csv_data, $delimiter));

        // Save the new data if it has changed.
        if ($csv != $result->easychart_csv) {
          $update = db_update($table_name);
          $update->fields(array($prefix . '_csv' => $csv));
          $update->condition('entity_id', $result->entity_id, '=');
          $update->condition('delta', $result->delta, '=');
          $update->execute();

          // Also overwrite the revision.
          $revision_table_name = str_replace('_data_', '_revision_', $table_name);
          $update = db_update($revision_table_name);
          $update->fields(array($prefix . '_csv' => $csv));
          $update->condition('entity_id', $result->entity_id, '=');
          $update->condition('revision_id', $result->revision_id, '=');
          $update->condition('delta', $result->delta, '=');
          $update->execute();

          $clear_caches = TRUE;
        }
      }
    }
  }

  // Clear the caches if anything has changed.
  if ($clear_caches) {
    drupal_flush_all_caches();
  }
}

/**
 * Helper function to parse the csv data into an array.
 */
function _parse_csv($csv_string, $delimiter = ",", $skip_empty_lines = true, $trim_fields = true) {
  $enc = preg_replace('/(?<!")""/', '!!Q!!', $csv_string);
  $enc = preg_replace_callback(
    '/"(.*?)"/s',
    function ($field) {
      return urlencode(utf8_encode($field[1]));
    },
    $enc
  );
  $lines = preg_split($skip_empty_lines ? ($trim_fields ? '/( *\R)+/s' : '/\R+/s') : '/\R/s', $enc);
  return array_map(
    function ($line) use ($delimiter, $trim_fields) {
      $fields = $trim_fields ? array_map('trim', explode($delimiter, $line)) : explode($delimiter, $line);
      return array_map(
        function ($field) {
          return str_replace('!!Q!!', '"', utf8_decode(urldecode($field)));
        },
        $fields
      );
    },
    $lines
  );
}

/**
 * Helper function to find the delimiter in a csv file.
 */
function _find_csv_delimiter($data) {
  // possible delimiters
  $delimiters = array(
    'tab'       => "\t",
    'comma'     => ",",
    'semicolon' => ";"
  );

  // Count how much a possible delimiter appears.
  $delimiters_found = array();
  foreach ($delimiters as $key => $value ){
    $delimiters_found[$key] = count(explode($value, $data)) - 1;
  }

  // Get the highest appearance score.
  arsort($delimiters_found);
  reset($delimiters_found);
  $delimiter = key($delimiters_found);

  return $delimiters[$delimiter];
}
