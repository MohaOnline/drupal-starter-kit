<?php

/**
 * @file
 * Contains Batch API callbacks used during installation.
 */

/**
 * BatchAPI callback.
 */
function _dvg_enable_module($module, $module_name, &$context) {
  module_enable(array($module), FALSE);
  $context['message'] = st('Installed %module module', array('%module' => $module_name));
}

/**
 * BatchAPI callback.
 */
function _dvg_uninstall_module($module, &$context) {
  module_disable(array($module), FALSE);
  drupal_uninstall_modules(array($module));
  $context['message'] = st('Uninstalled %module module.', array('%module' => $module));
}

/**
 * BatchAPI callback.
 */
function _dvg_install_set_default_settings($operation, &$context) {
  $context['message'] = t('@operation', array('@operation' => $operation));
  variable_set('i18n_string_source_language', 'en');
  variable_set('date_first_day', 1); // First day of week: monday.

  // Add new date formats and use theme for locale 'nl'.
  db_query("REPLACE INTO {date_formats} (format, type, locked) VALUES ('d-m-Y H:i', 'custom', '0')");
  db_query("REPLACE INTO {date_formats} (format, type, locked) VALUES ('D, d-m-Y - H:i', 'custom', '0')");
  db_query("REPLACE INTO {date_formats} (format, type, locked) VALUES ('j F Y', 'custom', '0');");

  // Set the language negotiation options.
  variable_set('language_negotiation_language', array(
    'locale-user' => array(
      'callbacks' => array(
        'language' => 'locale_language_from_user',
      ),
      'file' => 'includes/locale.inc',
    ),
    'language-default' => array(
      'callbacks' => array(
        'language' => 'language_from_default',
      ),
    ),
  ));
}

/**
 * BatchAPI callback.
 */
function _dvg_install_enable_dvg_theme($operation, &$context) {
  $context['message'] = t('@operation', array('@operation' => $operation));
  theme_disable(array('bartik'));
  theme_enable(array('dvg', 'adminimal', 'dvg_plain_theme'));
  variable_set('theme_default', 'dvg');
  variable_set('admin_theme', 'adminimal');
  variable_set('node_admin_theme', TRUE);
}

/**
 * BatchAPI callback.
 */
function _dvg_install_disable_file_types($operation, &$context) {
  $context['message'] = t('@operation', array('@operation' => $operation));
  if (function_exists('file_type_disable')) {
    $file_types = array('image', 'video', 'audio', 'document');
    foreach ($file_types as $file_type) {
      file_type_disable($file_type);
    }
  }
}

/**
 * BatchAPI callback.
 */
function _dvg_install_permissions($operation, &$context) {
  $context['message'] = t('@operation', array('@operation' => $operation));
  dvg_roles_and_permissions_set_permissions();
}

/**
 * BatchAPI callback.
 */
function _dvg_install_revert_all_features($operation, &$context) {
  $context['message'] = t('@operation', array('@operation' => $operation));
  features_rebuild();
  features_revert();
}

/**
 * BatchAPI callback.
 */
function _dvg_menu_import_file($menu, &$context) {
  $context['message'] = st('Installed menu %menu', array('%menu' => $menu));
  $file = __DIR__ . '/demo-content/' . $menu . '-export.txt';
  menu_import_file($file, $menu, array(
    'link_to_content' => TRUE,
    'create_content' => FALSE,
    'remove_menu_items' => FALSE,
  ));
}

/**
 * BatchAPI callback.
 */
function _dvg_import_node($original_node, &$context) {
  $context['message'] = st('Imported %title', array('%title' => $original_node->title));
  _dvg_import_nodes(array($original_node));
}

/**
 * BatchAPI callback.
 */
function _dvg_install_save_datab_demo_content(&$context) {
  $context['message'] = st('Enable demo mode for the Data B integration');

  // Enable demo mode for the Data B integration.
  $t = get_t();
  variable_set('dvg_datab', array(
    'cid' => '8888',
    'secret' => 'secret',
    'url' => 'https://mc1.datab.nl/docserver',
    'demo_mode' => 1,
    'no_result' => array(
      'value' => $t('There are no documents available for you. Do you think this is a mistake, please contact your municipality.'),
      'format' => 'filtered_html'
    ),
  ));
  variable_set('dvg_datab_no_result', array(
    'value' => $t('There are no documents available for you. Do you think this is a mistake, please contact your municipality.'),
    'format' => 'filtered_html'
  ));
}
/**
 * BatchAPI callback.
 */
function _dvg_install_save_custom_texts_demo_content(&$context) {

  variable_set('dvg_global_503_page_title', 'Website tijdelijk niet beschikbaar');
  variable_set('dvg_global_503_page_body', array(
    'value' => '<p>De website is door een technisch probleem tijdelijk niet beschikbaar (melding: 503 Service Temporary Unavailable).</p>',
    'format' => 'filtered_html',
  ));
  variable_set('dvg_custom__footer_texts', array(
    'dvg_custom__footer_text_1' => array(
      'value' => '<p>Contact:</p><p>Telefoon <strong>(0123) 45 6789</strong></p>',
      'format' => 'filtered_html',
    ),
    'dvg_custom__footer_text_2' => array(
      'value' => '<p>Bezoekadres:</p><p>Straatlaanweg 1</p><p>1234 AB Plaats</p>',
      'format' => 'filtered_html',
    ),
    'dvg_custom__footer_text_3' => array(
      'value' => '<p>Postadres:</p><p>Straatlaanweg 1</p><p>1234 AB Plaats</p>',
      'format' => 'filtered_html',
    ),
  ));

  $context['message'] = st('Saved demo content for custom texts');

}
/**
 * BatchAPI callback.
 */
function _dvg_install_save_taxonomies_demo_content(&$context) {
  $vid = taxonomy_vocabulary_machine_name_load('webform_button_title')->vid;
  $term_names = array('Aanvragen', 'Aanvragen met DigiD', 'Bezwaar maken met DigiD', 'Melden met DigiD', 'Meteen melden');
  foreach ($term_names as $term_name) {
    taxonomy_term_save((object) array(
      'name' => $term_name,
      'vid' => $vid,
    ));
  }
  $vid = taxonomy_vocabulary_machine_name_load('appointment_button_title')->vid;
  $term_names = array('Afspraak maken');
  foreach ($term_names as $term_name) {
    taxonomy_term_save((object) array(
      'name' => $term_name,
      'vid' => $vid,
    ));
  }
  $vid = taxonomy_vocabulary_machine_name_load('image_license')->vid;
  $term_names = array('Copyright', 'Creative Commons');
  foreach ($term_names as $term_name) {
    taxonomy_term_save((object) array(
      'name' => $term_name,
      'vid' => $vid,
    ));
  }
  $vid = taxonomy_vocabulary_machine_name_load('spatial_plan')->vid;
  $term_names = array('Bestemmingsplannen in procedure', 'Geldende bestemmingsplannen', 'Structuurvisies');
  foreach ($term_names as $term_name) {
    taxonomy_term_save((object) array(
      'name' => $term_name,
      'vid' => $vid,
    ));
  }
  $vid = taxonomy_vocabulary_machine_name_load('file_category')->vid;
  $term_names = array('Besluitenlijst B&W');
  foreach ($term_names as $term_name) {
    taxonomy_term_save((object) array(
      'name' => $term_name,
      'vid' => $vid,
    ));
  }
  $vid = taxonomy_vocabulary_machine_name_load('town_council')->vid;
  $term_names = array('College', 'Fracties', 'Oppositie');
  foreach ($term_names as $term_name) {
    taxonomy_term_save((object) array(
      'name' => $term_name,
      'vid' => $vid,
    ));
  }
  $vid = taxonomy_vocabulary_machine_name_load('related_pages_title')->vid;
  $term_names = array('Meer over dit onderwerp', 'Of zoekt u', 'Zie ook');
  foreach ($term_names as $term_name) {
    taxonomy_term_save((object) array(
      'name' => $term_name,
      'vid' => $vid,
    ));
  }
  $context['message'] = st('Imported demo taxonomies', array());
}

/**
 * BatchAPI callback.
 *
 * @see: system_check_directory().
 * @see: system_file_system_settings().
 */
function _dvg_install_file_system($operation, &$context) {
  $context['message'] = t('@operation', array('@operation' => $operation));

  foreach (array(
    'file_public_path' => conf_path() . '/files',
    'file_private_path' => conf_path() . '/files/private',
  ) as $option => $directory) {
    if (!variable_get($option, FALSE)) {
      variable_set($option, $directory);
    }
    else {
      $directory = variable_get($option);
    }
    if (!is_dir($directory)) {
      drupal_mkdir($directory, NULL, TRUE);
    }
    file_create_htaccess($directory, $option == 'file_private_path');
  }
  variable_set('file_temporary_path', file_directory_temp());
}

/**
 * BatchAPI callback.
 *
 * Set 403 and 404 pages.
 */
function _dvg_install_set_40x(&$context) {
  $context['message'] = st('Set 40x pages.');

  $ids = entity_get_id_by_uuid('node', array(
    'b389b6d2-4586-4a86-a75e-d9335b6a2680',
    'bb06002b-7c9e-4a62-bb18-4bbb67094ebc',
  ));
  variable_set('site_403', 'node/' . $ids['b389b6d2-4586-4a86-a75e-d9335b6a2680']);
  variable_set('site_404', 'node/' . $ids['bb06002b-7c9e-4a62-bb18-4bbb67094ebc']);
}
