<?php

/**
 * @file
 * Drush integration of UIkit.
 */

/**
 * Implements hook_drush_command().
 */
function uikit_drush_command() {
  $commands = array();

  $commands['uikit-starterkit'] = array(
    'description' => 'Create a UIkit sub-theme.',
    'arguments' => array(
      'machine_name' => '[optional] A machine-readable name for your theme.',
      'name' => 'A human-readable name for your theme.',
    ),
    'aliases' => array('uikit-sk'),
    'options' => array(
      'name' => 'A human-readable name for your theme.',
      'machine-name' => '[a-z, 0-9, _] A machine-readable name for your theme.',
      'path' => 'The path where your theme will be created. Defaults to: sites/all/themes',
      'description' => 'A description of your theme.',
    ),
    'examples' => array(
      'drush uikit-sk "Amazing name"' => 'Create a sub-theme, using the default options.',
      'drush uikit-sk zomg_amazing "Amazing name"' => 'Create a sub-theme with a specific machine name.',
      'drush uikit-sk "Amazing name" --path=sites/default/themes --description="So amazing."' => 'Create a sub-theme in the specified directory with a custom description.',
    ),
  );

  return $commands;
}

/**
 * Create a UIkit sub-theme using the starter kit.
 */
function drush_uikit_starterkit($machine_name = NULL, $name = NULL) {
  // Determine the theme name and machine name.
  if (!isset($name)) {
    // If only given one argument, it is the $name.
    if (isset($machine_name)) {
      $name = $machine_name;
      unset($machine_name);
    }
    else {
      $name = drush_get_option('name');
    }

    if (!isset($machine_name)) {
      $machine_name = drush_get_option('machine-name');
    }
  }

  if (!$name) {
    if ($machine_name) {
      $name = $machine_name;
    }
    else {
      if (FALSE) {
        // Allow localize.drupal.org to pick up the string to translate.
        t('The name of the theme was not specified.');
      }
      return drush_set_error('UIKIT_SUBTHEME_NAME_NOT_SPECIFIED', dt('The name of the theme was not specified.'));
    }
  }

  if (!$machine_name) {
    $machine_name = $name;
  }

  // Clean up the machine name.
  $machine_name = str_replace(' ', '_', strtolower($machine_name));
  $search = array(
    '/[^a-z0-9_]/', // Remove characters not valid in function names.
    '/^[^a-z]+/',   // Functions must begin with an alpha character.
  );
  $machine_name = preg_replace($search, '', $machine_name);

  // Determine the path to the new sub-theme.
  $subtheme_path = file_exists(DRUPAL_ROOT . '/sites/all/themes/contrib') ? 'sites/all/themes/custom' : 'sites/all/themes';
  $mkdir_subtheme_path = $subtheme_path && !file_exists(DRUPAL_ROOT . '/sites/all/themes/custom');
  if ($mkdir_subtheme_path) {
    // Create the custom themes directory if it does not exist.
    mkdir(DRUPAL_ROOT . '/sites/all/themes/custom');
  }
  if ($path = drush_get_option('path')) {
    $subtheme_path = drush_trim_path($path);
  }
  $subtheme_path = drush_normalize_path(drush_get_context('DRUSH_DRUPAL_ROOT') . '/' . $subtheme_path . '/' . $machine_name);

  // Ensure the destination directory exists.
  if (!is_dir(dirname($subtheme_path))) {
    if (FALSE) {
      // Allow localize.drupal.org to pick up the string to translate.
      t('The directory "!directory" was not found.', array('!directory' => dirname($subtheme_path)));
    }
    return drush_set_error('UIKIT_DESTINATION_NOT_FOUND', dt('The directory "!directory" was not found.', array('!directory' => dirname($subtheme_path))));
  }

  // Ensure the STARTERKIT directory exists.
  $starterkit_path = drush_normalize_path(drush_get_context('DRUSH_DRUPAL_ROOT') . '/' . drupal_get_path('theme', 'uikit') . '/STARTERKIT');
  if (!is_dir($starterkit_path)) {
    if (FALSE) {
      // Allow localize.drupal.org to pick up the string to translate.
      t('The STARTERKIT directory was not found in "!directory"', array('!directory' => dirname($starterkit_path)));
    }
    return drush_set_error('UIKIT_STARTERKIT_NOT_FOUND', dt('The STARTERKIT directory was not found in "!directory"', array('!directory' => dirname($starterkit_path))));
  }

  // Make a fresh copy of the original starter kit.
  drush_op('drush_copy_dir', $starterkit_path, $subtheme_path);

  // Rename the STARTERKIT files.
  $subtheme_info_file = $subtheme_path . '/' . $machine_name . '.info';
  $subtheme_css_base_file = $subtheme_path . '/css/' . $machine_name . '.css';
  $subtheme_js_theme_file = $subtheme_path . '/js/' . $machine_name . '.js';
  drush_op('rename', drush_normalize_path($subtheme_path . '/STARTERKIT.info.text'), drush_normalize_path($subtheme_info_file));
  drush_op('rename', drush_normalize_path($subtheme_path . '/css/STARTERKIT.css'), drush_normalize_path($subtheme_css_base_file));
  drush_op('rename', drush_normalize_path($subtheme_path . '/js/STARTERKIT.js'), drush_normalize_path($subtheme_js_theme_file));

  // Alter the contents of the .info file based on the command options.
  $alterations = array(
    'name = STARTERKIT' => 'name = ' . $name,
    'STARTERKIT.css' => $machine_name . '.css',
    'STARTERKIT.js' => $machine_name . '.js',
  );
  if ($description = drush_get_option('description')) {
    $alterations['UIkit Sub-theme'] = $description;
  }
  drush_op('uikit_file_str_replace', $subtheme_info_file, array_keys($alterations), $alterations);

  // Replace all occurrences of 'STARTERKIT' with the machine name of our sub theme.
  drush_op('uikit_file_str_replace', $subtheme_path . '/theme-settings.php', 'STARTERKIT', $machine_name);
  drush_op('uikit_file_str_replace', $subtheme_path . '/template.php', 'STARTERKIT', $machine_name);
  drush_op('uikit_file_str_replace', $subtheme_path . '/css/' . $machine_name . '.css', 'STARTERKIT', $machine_name);
  drush_op('uikit_file_str_replace', $subtheme_path . '/js/' . $machine_name . '.js', 'Drupal.behaviors.STARTERKIT', str_replace(' ', '', 'Drupal.behaviors.' . $name));
  drush_op('uikit_file_str_replace', $subtheme_path . '/js/' . $machine_name . '.js', 'STARTERKIT', $machine_name);

  // Notify user of the newly created theme.
  if (FALSE) {
    // Allow localize.drupal.org to pick up the string to translate.
    t('Starter kit for "!name" created in: !path', array('!name' => $name, '!path' => $subtheme_path));
  }
  drush_print(dt('Starter kit for "!name" created in: !path', array(
    '!name' => $name,
    '!path' => $subtheme_path,
  )));
}

/**
 * Replace strings in a file.
 */
function uikit_file_str_replace($file_path, $find, $replace) {
  $file_path = drush_normalize_path($file_path);
  $file_contents = file_get_contents($file_path);
  $file_contents = str_replace($find, $replace, $file_contents);
  file_put_contents($file_path, $file_contents);
}
