<?php

// Script will localize Taxonomy
// Run command: drush scr translate.drush.inc or
// use devel at admin/devel/php to localize

if (($handle = fopen("taxonomy_localize.csv", "r")) !== FALSE) {
 print "Starting loading french strings: " . date('h:i:s A') ."\n";
 while (($data = fgetcsv($handle, 1000, ',','"')) !== FALSE) {
    $vocab = "wetkit_categories";
    $result = db_query('SELECT n.uuid FROM {taxonomy_term_data} n, {taxonomy_vocabulary} v WHERE v.machine_name = :vocab and n.name = :name AND n.vid = v.vid', array(':name' => $data[0], ':vocab' => $vocab));

    // Iterate over each row of the CSV.
    foreach ($result as $row) {
      $source = $data[0];
      $translation = $data[1];
      $langcode = 'fr';
      $string_context = array('taxonomy', 'term', $row->uuid, 'name');
      $result_translation = i18n_string_translation_update($string_context, $translation, $langcode, $source);
      cache_clear_all('locale:', 'cache', TRUE);
    }
  }
}
fclose($handle);
