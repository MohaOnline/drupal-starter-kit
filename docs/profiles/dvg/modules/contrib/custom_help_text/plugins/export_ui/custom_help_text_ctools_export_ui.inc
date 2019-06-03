<?php
/**
 * @file
 * Custom Help Text export_ui plugin.
 */

/**
 * Define this Export UI plugin.
 */
$plugin = array(
  'schema' => 'custom_help_text',
  // As defined in hook_schema().
  'access' => 'administer custom help text',
  // Define a permission users must have to access these pages.
  // Define the menu item.
  'menu' => array(
    'menu prefix' => 'admin/structure',
    'menu item' => 'custom-help-text',
    'menu title' => 'Custom Help Text',
    'menu description' => 'Administer Custom Help Text.',
  ),
  // Define user interface texts.
  'title singular' => t('Custom Help Text'),
  'title plural' => t('Custom Help Texts'),
  'title singular proper' => t('Custom Help Text'),
  'title plural proper' => t('Custom Help Texts'),
  'handler' => 'custom_help_text_export_ui',
);
