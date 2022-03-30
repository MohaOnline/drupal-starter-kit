<?php
/**
 * @file
 * Features related functions.
 */

/**
 * Implements hook_features_export().
 */
function tb_megamenu_features_export($data, &$export, $module_name = '') {
  $export['dependencies']['tb_megamenu'] = 'tb_megamenu';

  $export['features']['tb_megamenu'] = array();

  foreach ($data as $menu_name) {
    $export['features']['tb_megamenu'][$menu_name] = $menu_name;
  }

  return $export;
}

/**
 * Implements hook_features_export_options().
 */
function tb_megamenu_features_export_options() {
  $menus = array();

  // Query the names of menus configured for tb_megamenu.
  $query = db_select('tb_megamenus', 'tb');
  $query->fields('tb', array('menu_name', 'language'));

  foreach ($query->execute()->fetchAll() as $menu) {
    $menus[$menu->menu_name . '|' . $menu->language] = $menu->menu_name . '|' . $menu->language;
  }

  return $menus;
}

/**
 * Implements hook_features_export_render().
 */
function tb_megamenu_features_export_render($module, $data) {
  $menus = array();

  foreach ($data as $menu_name) {
    $menu_details = explode('|', $menu_name);
    $menu_name_id = $menu_details[0];
    $menu_language = $menu_details[1];

    // Query the configuration of the current menu.
    $menu = db_select('tb_megamenus', 'tb')
      ->fields('tb')
      ->condition('menu_name', $menu_name_id)
      ->condition('language', $menu_language)
      ->execute()
      ->fetchObject();

    if ($menu) {
      $menu_config = json_decode($menu->menu_config, TRUE);

      // Loop all menu items and convert mlid's to identifiers recursively.
      $new_config = array();
      foreach ($menu_config as $mlid => $config) {
        tb_megamenu_export_convert_mlids($config, $mlid);
        $new_config[] = $config;
      }
      $menu->menu_config = $new_config;

      $menu->block_config = json_decode($menu->block_config, TRUE);

      $menus[$menu_name] = $menu;
    }
  }

  // Render the PHP code for the "default hook" function configured in
  // hook_features_api() and created in the MODULENAME.features.tb_megamenu.inc
  // file.
  $code = "  return " . features_var_export($menus, '  ') . ";";

  // Key the code by the "default hook".
  return array('tb_megamenu_default_menus' => $code);
}

/**
 * Implements hook_features_revert().
 */
function tb_megamenu_features_revert($module) {
  tb_megamenu_features_rebuild($module);
}

/**
 * Implements hook_features_rebuild().
 */
function tb_megamenu_features_rebuild($module) {
  // Workaround to make sure that the feature module's file containing the
  // export function is loaded. When drush feature-revert is run with the
  // --force argument, the file is not loaded (for whatever reason), so the
  // export function does not exist and $tb_megamenus below is null. Why does
  // this happen?
  module_load_include('inc', $module, $module . '.' . 'features.tb_megamenu');

  $tb_megamenus = module_invoke($module, 'tb_megamenu_default_menus');

  foreach ($tb_megamenus as $menu_name => $tb_megamenu) {
    $menu_config = $tb_megamenu['menu_config'];
    $new_config = array();

    // Get the menu name
    $menu_details = explode('|', $menu_name);
    $menu_name_id = $menu_details[0];

    // Loop all menu items to recursively convert item identifiers back to
    // mlids.
    foreach ($menu_config as $config) {
      $mlid = tb_megamenu_export_identifier_to_mlid($config['identifier'],
        $menu_name_id);
      $new_config[$mlid] = $config;
      tb_megamenu_export_convert_identifiers($new_config[$mlid], $menu_name_id);
    }

    $tb_megamenu['menu_config'] = json_encode($new_config);
    $tb_megamenu['block_config'] = json_encode($tb_megamenu['block_config']);

    // Create a new record for the current menu or overwrite an existing one.
    db_merge('tb_megamenus')
    ->key(array(
        'menu_name' => $tb_megamenu['menu_name'],
        'language' => $tb_megamenu['language'],
      ))
    ->fields(array(
        'menu_config' => $tb_megamenu['menu_config'],
        'block_config' => $tb_megamenu['block_config'],
      ))
    ->execute();
  }
}

/**
 * Converts the "mlid" fields to system-independent identifiers.
 *
 * Recursively convert the "mlid" fields of the given menu configuration array
 * or subarray to system-independent identifiers.
 *
 * @param array $config
 *   Menu configuration array or subarray.
 * @param int $mlid
 *   mlid for the item represented by the given config. Set this if there is
 *   none set in the config itself.
 */
function tb_megamenu_export_convert_mlids(&$config, $mlid = NULL) {
  if (is_array($config)) {
    foreach ($config as $key => $value) {
      if ($key === 'mlid') {
        $config['identifier'] = tb_megamenu_export_mlid_to_identifier($value);
        unset($config['mlid']);
      }
      // Recursively convert subarrays.
      elseif (is_array($value)) {
        tb_megamenu_export_convert_mlids($config[$key]);
      }
    }

    if ($mlid && !array_key_exists('identifier', $config)) {
      $config['identifier'] = tb_megamenu_export_mlid_to_identifier($mlid);
    }
  }
}

/**
 * Converts the identifier fields back to their respective "mlid".
 *
 * Recursively convert the "identifier" fields of the given menu's configuration
 * back to the respective mlid's.
 *
 * @param array $config
 *   Menu configuration array or subarray.
 * @param string $menu_name
 *   Name of the menu the given config belongs to.
 */
function tb_megamenu_export_convert_identifiers(&$config, $menu_name) {
  if (is_array($config)) {
    foreach ($config as $key => $value) {
      if ($key === 'identifier') {
        $config['mlid'] = tb_megamenu_export_identifier_to_mlid($value,
          $menu_name);
        unset($config['identifier']);
      }
      elseif (is_array($value)) {
        tb_megamenu_export_convert_identifiers($config[$key], $menu_name);
      }
    }
  }
}

/**
 * Convert a mlid to an identifier consisting of urlencoded link title and path.
 *
 * @param int $mlid
 *   Menu item's mlid.
 *
 * @return string
 *   Identifier.
 *
 * @see tb_megamenu_export_identifier_to_mlid()
 */
function tb_megamenu_export_mlid_to_identifier($mlid) {
  // Set the language to the site's default, not 'en'. This is useful when using
  // modules like taxonomy_menu that mess with the translation of the links
  $default_language = language_default('language');
  global $language;
  $language->language = $default_language;

  $link = menu_link_load($mlid);

  // Urlencode title and path to make the separating colon the only one (title
  // and/or path might contain colons, too).
  return urlencode($link['link_title']) . ':' . urlencode($link['link_path']);
}

/**
 * Convert a menu item identifier back to the respective item's mlid.
 *
 * @param string $identifier
 *   Menu item identifier based on the item's title and path.
 * @param string $menu_name
 *   Name of the menu item's menu.
 *
 * @return mixed
 *   Matching mlid or FALSE if none could be determined.
 *
 * @see tb_megamenu_export_mlid_to_identifier()
 */
function tb_megamenu_export_identifier_to_mlid($identifier, $menu_name) {
  list($title, $path) = explode(':', $identifier);
  $title = urldecode($title);
  $path = urldecode($path);

  $query = db_select('menu_links', 'ml');
  $query->fields('ml');
  $query->addField('ml', 'mlid');
  $query->condition('ml.link_path', $path);
  $query->condition('ml.link_title', $title);
  $query->condition('ml.menu_name', $menu_name);
  if ($item = $query->execute()->fetchAssoc()) {
    return $item['mlid'];
  }

  return FALSE;
}
