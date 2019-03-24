<?php

/**
 * @file
 * Modify structured content arrays.
 *
 * These hooks are called after the content has been assembled in a structured
 * array and may be used for doing processing which requires that the complete
 * content structure has been built.
 *
 * If the theme wishes to act on the rendered HTML of the content rather than
 * the structured content array, it may use this hook to add a #post_render
 * callback. Alternatively, it could also implement hook_preprocess_HOOK().
 *
 * @see drupal_render()
 * @see theme()
 */

use Drupal\uikit\UIkit;

/**
 * Implements hook_css_alter().
 */
function uikit_css_alter(&$css) {
  $theme = drupal_get_path('theme', 'uikit');

  // Stop Drupal core stylesheets from being loaded.
  unset($css[drupal_get_path('module', 'system') . '/system.messages.css']);
  unset($css[drupal_get_path('module', 'system') . '/system.theme.css']);
  unset($css[drupal_get_path('module', 'system') . '/system.menus.css']);

  // Replace the book module's book.css with a custom version.
  $book_css = drupal_get_path('module', 'book') . '/book.css';
  if (isset($css[$book_css])) {
    $css[$book_css]['data'] = $theme . '/css/book.css';
  }

  // Replace the field module's field.css with a custom version.
  $field_css = drupal_get_path('module', 'field') . '/theme/field.css';
  if (isset($css[$field_css])) {
    $css[$field_css]['data'] = $theme . '/css/field.css';
  }
}

/**
 * Implements hook_html_head_alter().
 */
function uikit_html_head_alter(&$head_elements) {
  global $theme_key;

  // Add a charset meta tag to the head elements.
  if (isset($head_elements['system_meta_content_type'])) {
    $head_elements['system_meta_content_type']['#attributes'] = array(
      'charset' => UIkit::getThemeSetting('meta_charset', $theme_key),
    );
    $head_elements['system_meta_content_type']['#weight'] = -9999;
  }

  // Some modules, such as the Adminimal Admin menu, add a viewport meta tag.
  // Remove this so the theme can define the tag.
  if (isset($head_elements['viewport'])) {
    unset($head_elements['viewport']);
  }
}

/**
 * Implements hook_menu_local_tasks_alter().
 */
function uikit_menu_local_tasks_alter(&$data, $router_item, $root_path) {
  // Define various icons.
  $plus = '<i uk-icon="icon:plus"></i> ';

  foreach ($data['actions']['output'] as $key => $action) {
    // Add icon based on link path.
    if (isset($action['#link']['path'])) {
      switch ($action['#link']['path']) {
        case 'node/add':
        case 'admin/structure/block/add':
        case 'admin/structure/types/add':
        case 'admin/structure/menu/add':
        case 'admin/structure/taxonomy/add':
        case 'admin/structure/taxonomy/%/add':
        case 'admin/appearance/install':
        case 'admin/people/create':
        case 'admin/modules/install':
        case 'admin/config/content/formats/add':
        case 'admin/config/media/image-styles/add':
        case 'admin/config/search/path/add':
        case 'admin/config/regional/date-time/types/add':
        case 'admin/config/user-interface/shortcut/add-set':
          $title = $plus . $data['actions']['output'][$key]['#link']['title'];
          $data['actions']['output'][$key]['#link']['title'] = $title;
          $data['actions']['output'][$key]['#link']['localized_options']['html'] = TRUE;
          break;
      }
    }
    // Some actions use the href key instead of the path key.
    elseif (isset($action['#link']['href'])) {
      switch ($action['#link']['href']) {
        case 'node/add/blog':
          $title = $plus . $data['actions']['output'][$key]['#link']['title'];
          $data['actions']['output'][$key]['#link']['title'] = $title;
          $data['actions']['output'][$key]['#link']['localized_options']['html'] = TRUE;
          break;
      }
    }
  }
}
