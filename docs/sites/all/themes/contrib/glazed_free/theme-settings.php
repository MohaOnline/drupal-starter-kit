<?php

/**
 * Implements hook_form_system_theme_settings_alter().
 *
 * @param $form
 *   The form.
 * @param $form_state
 *   The form state.
 */

function glazed_free_form_system_theme_settings_alter(&$form, &$form_state) {
  /**
   * @ code
   * a bug in D7 causes the theme to load twice, if this file is loaded a
   * second time we return immediately to prevent further complications.
   */
  global $glazed_free_altered, $base_path, $theme_chain;
  if ($glazed_free_altered) return;
  $glazed_free_altered = TRUE;

  // Wrap global and jQuery setting fieldsets in vertical tabs.
  $form['global'] = array(
    '#type' => 'vertical_tabs',
    '#prefix' => '<h2><small>' . t('Override Global Settings') . '</small></h2>',
    '#weight' => -9,
  );
  $form['theme_settings']['#group'] = 'global';
  $form['logo']['#group'] = 'global';
  $form['favicon']['#group'] = 'global';
  $form['favicon']['#group'] = 'global';

  // $subject_theme = arg(count(arg()) - 1); old way
  $subject_theme = $form_state['build_info']['args'][0];
  $glazed_free_theme_path = drupal_get_path('theme', 'glazed_free') . '/';
  $theme_path = drupal_get_path('theme', $subject_theme) . '/';
  $themes = list_themes();
  $theme_chain = array($subject_theme);
  foreach ($themes[$subject_theme]->base_themes as $base_theme => $base_theme_name) {
    $theme_chain[] = $base_theme;
  }

  /**
   * Glazed cache builder
   * Cannot run as submit function because  it will set outdated values by
   * using theme_get_setting to retrieve settings from database before the db is
   * updated. Cannot put cache builder in form scope and use $form_state because
   * it also needs to initialize default settings by reading the .info file.
   * By calling the cache builder here it will run twice: once before the
   * settings are saved and once after the redirect with the updated settings.
   * @todo come up with a less 'icky' solution
   */

  if (!isset($files_path)) { // in case admin theme is used
    global $files_path;
    $files_path = variable_get('file_public_path', conf_path() . '/files');
  }
  require_once(drupal_get_path('theme', 'glazed_free') . '/glazed_free_callbacks.inc');
  glazed_free_css_cache_build(arg(count(arg()) - 1));

  if ($GLOBALS['theme'] == 'seven') {
    drupal_set_message(t('Install the Glazed Theme Tools helper module to have a better theme settings experience'), 'warning');
    drupal_add_css('https://cdn.jsdelivr.net/bootstrap/3.3.6/css/bootstrap.min.css', 'external');
    drupal_add_js('https://cdn.jsdelivr.net/bootstrap/3.3.6/js/bootstrap.min.js', 'external');
    drupal_add_css('html body { font-size: 14px; }', 'inline');
  }

  // drupal_add_css('themes/seven/vertical-tabs.css');
  drupal_add_library('system', 'ui.slider'); // If this isn't loaded the bootstrapSlider won't exist anymore
  // drupal_add_library('system', 'ui.tabs');
  drupal_add_css($glazed_free_theme_path . 'js/vendor/bootstrap-switch/bootstrap-switch.min.css');
  drupal_add_css($glazed_free_theme_path . 'js/vendor/bootstrap-slider/bootstrap-slider.min.css');
  drupal_add_css($glazed_free_theme_path . 'css/glazed.admin.themesettings.css');
  drupal_add_js($glazed_free_theme_path . 'js/vendor/bootstrap-switch/bootstrap-switch.min.js', 'file');
  drupal_add_js($glazed_free_theme_path . 'js/vendor/bootstrap-slider/bootstrap-slider.min.js', 'file');
  drupal_add_js($glazed_free_theme_path . 'js/glazed-settings.admin.js', 'file');
  drupal_add_js($glazed_free_theme_path . 'js/color.js', 'file');
  drupal_add_js('jQuery(function () {Drupal.behaviors.formUpdated = null;});', 'inline');
  // Decoy function to fix erros resulting from missing preview.js
  drupal_add_js('Drupal.color = { callback: function() {} }', 'inline');

  drupal_add_js(array('glazed_free' => array('palette' => theme_get_setting('palette', 'glazed_free'))), 'setting');

  if (!isset($themes['bootstrap']->info['version'])) {
    $themes['bootstrap']->info['version'] = 'dev';
  }

  // $header  = '<div class="settings-header">';
  // $header .= '  <h2>' . ucfirst($subject_theme) . ' ' . $themes[$subject_theme]->info['version'] . ' <span class="lead">(Bootstrap ' . $themes['bootstrap']->info['version'] . ')</span></h2>';
  // $header .= '</div>';
  $img = '<img style="width:35px;margin-right:5px;" src="' . $base_path . $glazed_free_theme_path . 'logo-white.png" />';
  $form['glazed_free_settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => -20,
    '#prefix' => '<h2><small>' . $img . ' ' . ucfirst($subject_theme) . ' ' . $themes[$subject_theme]->info['version'] . ' <span class="lead">(Bootstrap ' . $themes['bootstrap']->info['version'] . ')</span>' . '</small></h2>',
  );

  // Load Sooper Features
  foreach ($theme_chain as $theme) {
    foreach (file_scan_directory(drupal_get_path('theme', $theme) . '/features', '/settings.inc/i') as $file) {
      include($file->uri);
    }
  }

  // Adding submit handler requires some extra code due to buggy theme settings system
  // http://ghosty.co.uk/2014/03/managed-file-upload-in-drupal-theme-settings/
  $form['#submit'][] = 'glazed_free_settings_form_submit';
  // Get all themes.
  $themes = list_themes();
  // Get the current theme
  $active_theme = $GLOBALS['theme_key'];
  $form_state['build_info']['files'][] = str_replace("/$active_theme.info", '', $themes[$active_theme]->filename) . '/theme-settings.php';

    // $form['glazed_free_settings']['drupal']['theme_settings'] = $form['theme_settings'];
    // $form['glazed_free_settings']['drupal']['logo'] = $form['logo'];
    // $form['glazed_free_settings']['drupal']['favicon'] = $form['favicon'];
    // unset($form['theme_settings']);
    // unset($form['logo']);
    // unset($form['favicon']);
  // Return the additional form widgets
  return $form;
}

/**
 * Retrieves the Color module information for a particular theme.
 */
function _glazed_free_get_color_names($theme = NULL) {
  static $theme_info = array();
  if (!isset($theme)) {
    $theme = variable_get('theme_default', NULL);
  }

  if (isset($theme_info[$theme])) {
    return $theme_info[$theme];
  }

  $path = drupal_get_path('theme', $theme);
  $file = DRUPAL_ROOT . '/' . $path . '/color/color.inc';
  if ($path && file_exists($file)) {
    include $file;
    $theme_info[$theme] = $info['fields'];
    return $info['fields'];
  } else {
    return array();
  }
}

/**
 * Color options for theme settings
 */
function _glazed_free_color_options($theme) {
  $colors = array(
    '' => t('None (Theme Default)'),
    'white' => t('White'),
    'custom' => t('Custom Color'),
  );
  $colors = array_merge($colors, array(t('Glazed Colors') => _glazed_free_get_color_names()));
  return $colors;
}

function _glazed_free_is_demo_content ($feature) {
  return ((strpos($feature->name, '_content') OR (strpos($feature->name, '_theme_settings')))
    && isset($feature->info['features']['uuid_node']));
}

function _glazed_free_is_demo_content_exclude_subtheme ($feature) {
  return (strpos($feature->name, '_content')
    && isset($feature->info['features']['uuid_node']));
}

function _glazed_free_is_subtheme ($feature) {
  return (strpos($feature->name, '_theme_settings')
    && isset($feature->info['features']['uuid_node']));
}

function _glazed_free_node_types_options() {
  $types = array();
  foreach (node_type_get_types() as $key => $value) {
    $types[$key] = $value->name;
  }
  return $types;
}

/**
 * Implements hook_form_FORM_ID_alter().
 * We hijack the function that is reserved for the user module in order
 * to get the full monty of $form stuff. The module cache is cleared to make sure
 * our hook implementation is known before this point. Yes this is a dirty hack.
 */
if (module_exists('color')) {
  registry_rebuild();
  function user_form_system_theme_settings_alter(&$form, &$form_state) {
    if (isset($form['color'])) {
      $form['glazed_free_settings']['color'] = $form['color'];
      unset($form['color']);
      $form['glazed_free_settings']['color']['#title'] = 'Colors';
      $form['glazed_free_settings']['color']['#weight'] = 1;
    }
  }
}

function _glazed_free_type_preview() {
  $output = <<<EOT
<div class="type-preview">
  <div class="type-container type-title-container">
    <h1>Beautiful Typography</h1>
  </div>

  <div class="type-container">
    <h2>Typewriter delectus cred. Thundercats, sed scenester before they sold out et aesthetic</h2>
    <hr>
    <p class="lead">Lead Text Direct trade gluten-free blog, fanny pack cray labore skateboard before they sold out adipisicing non magna id Helvetica freegan. Disrupt aliqua Brooklyn church-key lo-fi dreamcatcher.</p>


    <h3>Truffaut disrupt sartorial deserunt</h3>

    <p>Cosby sweater plaid shabby chic kitsch pour-over ex. Try-hard fanny pack mumblecore cornhole cray scenester. Assumenda narwhal occupy, Blue Bottle nihil culpa fingerstache. Meggings kogi vinyl meh, food truck banh mi Etsy magna 90's duis typewriter banjo organic leggings Vice.</p>

    <ul>
      <li>Roof party put a bird on it incididunt sed umami craft beer cred.</li>
      <li>Carles literally normcore, Williamsburg Echo Park fingerstache photo booth twee keffiyeh chambray whatever.</li>
      <li>Scenester High Life Banksy, proident master cleanse tousled squid sriracha ad chillwave post-ironic retro.</li>
    </ul>

    <h4>Fingerstache nesciunt lomo nostrud hoodie</h4>

    <blockquote>
      <p>Cosby sweater plaid shabby chic kitsch pour-over ex. Try-hard fanny pack mumblecore cornhole cray scenester. Assumenda narwhal occupy, Blue Bottle nihil culpa fingerstache. Meggings kogi vinyl meh, food truck banh mi Etsy magna 90's duis typewriter banjo organic leggings Vice.</p>
      <footer>Someone famous in <cite title="Source Title">Source Title</cite></footer>
    </blockquote>
  </div>
</div>
EOT;
  return $output;
}

/**
 * Submit callback for theme settings form
 * Import Demo content.
 */
function glazed_free_settings_form_submit(&$form, &$form_state) {
  if ($import = $form_state['values']['settings_import_box']) {
    $import_settings = drupal_parse_info_format($import);
    if (is_array($import_settings) && isset($import_settings['settings'])) {
      $form_state['values'] = array_merge($form_state['values'], $import_settings['settings']);
    }
  }

  // If requested import additional demo content
  if (module_exists('uuid_features')) {
    module_load_include('module', 'uuid');
    module_load_include('inc', 'features', 'features.admin');
    $demo_content_modules = array_filter(_features_get_features_list(), "_glazed_free_is_demo_content");
    if (!empty($demo_content_modules)) {
      usort($demo_content_modules, function($a, $b) {
        $return = (count($a->info['features']['uuid_node']) < count($b->info['features']['uuid_node'])) ? 1 : -1;
        return $return;
      });
      foreach ($demo_content_modules as $module) {
        if (isset($module->info['features']) && isset($module->info['features']['uuid_node'])) {
          $node_sample = $module->info['features']['uuid_node'][0];
          if ($form_state['values'][$module->name] && !entity_get_id_by_uuid('node', array($node_sample))) {
            drupal_set_message($module->name . ' ' . t('installed'));
            module_enable(array($module->name));
            module_disable(array($module->name), FALSE);
          }
        }
      }
    }
  }

}
