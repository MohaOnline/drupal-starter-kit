<?php

/**
 * @file
 * Administrative page callbacks for the AMP module.
 */

/**
 * Form constructor for the AMP administration form.
 *
 * @ingroup forms
 * @see amp_admin_form_validate()
 * @see amp_admin_form_submit()
 */
function amp_admin_form($form, &$form_state) {
  $form = array();

  if (!module_exists('token')) {
    // Provide message in case somebody has upgraded AMP module but has not
    // installed Token.
    drupal_set_message(t('The AMP module requires the <a href="@module">Token</a> module as a dependency. Please download and install Token to prevent errors with AMP.', array('@module' => 'https://www.drupal.org/project/token')), 'warning');
  }

  if (!module_exists('ctools')) {
    // Provide message in case somebody has upgraded AMP module but has not
    // installed ctools.
    drupal_set_message(t('The AMP module requires the <a href="@module">ctools</a> module as a dependency. Please download and install ctools to prevent errors with AMP.', array('@module' => 'https://www.drupal.org/project/ctools')), 'warning');
  }

  if (module_exists('field_ui')) {
    $form['amp_content_amp_status'] = array(
      '#title' => t('AMP Status by Content Type'),
      '#theme' => 'item_list',
      '#items' => amp_get_formatted_status_list(),
    );
  }
  else {
    $form['amp_content_amp_status'] = array(
      '#type' => 'item',
      '#title' => t('AMP Status by Content Type'),
      '#markup' => t('(In order to enable and disable AMP content types in the UI, the Field UI module must be enabled.)'),
    );
  }

  // AMP theme settings.
  $form['amp_theme'] = array(
    '#type' => 'select',
    '#options' => _amp_get_theme_options(),
    '#title' => t('AMP theme'),
    '#description' => t('Choose a theme to use for AMP pages.'),
    '#default_value' => variable_get('amp_theme', 'ampsubtheme_example'),
  );

  if (!module_exists('amp_adsense')) {
    $form['amp_google_adsense_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Google AdSense Publisher ID'),
      '#default_value' => variable_get('amp_google_adsense_id'),
      '#maxlength' => 25,
      '#size' => 20,
      '#description' => t('This is the Google AdSense Publisher ID for the site owner. Get this in your Google Adsense account. It should be similar to pub-9999999999999'),
    );
  }

  if (!module_exists('amp_dfp')) {
    $form['amp_google_doubleclick_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Google DoubleClick for Publishers Network ID'),
      '#default_value' => variable_get('amp_google_doubleclick_id', '/'),
      '#maxlength' => 25,
      '#size' => 20,
      '#description' => t('The Network ID to use on all tags. This value should begin with a /.'),
    );
  }

  $form['pixel_group'] = array(
    '#type' => 'fieldset',
    '#title' => t('amp-pixel'),
  );
  $form['pixel_group']['amp_pixel'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable amp-pixel'),
    '#default_value' => variable_get('amp_pixel'),
    '#description' => t('The amp-pixel element is meant to be used as a typical tracking pixel -- to count page views. Find more information in the <a href="https://www.ampproject.org/docs/reference/amp-pixel.html">amp-pixel documentation</a>.'),
  );
  $form['pixel_group']['amp_pixel_domain_name'] = array(
    '#type' => 'textfield',
    '#title' => t('amp-pixel domain name'),
    '#default_value' => variable_get('amp_pixel_domain_name'),
    '#description' => t('The domain name where the tracking pixel will be loaded: do not include http or https.'),
    '#states' => array('visible' => array(
      ':input[name="amp_pixel"]' => array('checked' => TRUE))
    ),
  );
  $form['pixel_group']['amp_pixel_query_string'] = array(
    '#type' => 'textfield',
    '#title' => t('amp-pixel query path'),
    '#default_value' => variable_get('amp_pixel_query_string'),
    '#description' => t('The path at the domain where the GET request will be received, e.g. "pixel" in example.com/pixel?RANDOM.'),
    '#states' => array('visible' => array(
      ':input[name="amp_pixel"]' => array('checked' => TRUE))
    ),
  );
  $form['pixel_group']['amp_pixel_random_number'] = array(
    '#type' => 'checkbox',
    '#title' => t('Random number'),
    '#default_value' => variable_get('amp_pixel_random_number'),
    '#description' => t('Use the special string RANDOM to add a random number to the URL if required. Find more information in the <a href="https://github.com/ampproject/amphtml/blob/master/spec/amp-var-substitutions.md#random">amp-pixel documentation</a>.'),
    '#states' => array('visible' => array(
      ':input[name="amp_pixel"]' => array('checked' => TRUE))
    ),
  );

  $form['amp_library_group'] = array(
    '#type' => 'fieldset',
    '#title' => t('AMP Library Configuration <a href="https://github.com/Lullabot/amp-library">(GitHub Home and Documentation)</a>'),
  );

  $form['amp_library_group']['amp_test_page'] = array(
    '#type' => 'item',
    '#markup' => t('<a href="@url">Test that AMP is configured properly</a>', array('@url' => '/admin/config/amp/library/test')),
  );

  $form['amp_library_group']['amp_library_warnings_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('<em>Debugging</em>: Show AMP Library warnings in <em>all</em> AMP text formatters for <em>all</em> users'),
    '#default_value' => variable_get('amp_library_warnings_display', false),
    '#description' => t('<ul><li>If you only want to see amp formatter specific warning for one node add '
            . 'query "warnfix" at end of a node url. e.g. <strong>node/12345?amp&warnfix</strong></li>'
            . '<li>If the Debugging checkbox value is changed upon save, the Drupal cache will be cleared</li>'),
  );

  $form['amp_library_group']['amp_library_process_full_html'] = array(
      '#type' => 'checkbox',
      '#title' => t('<strong><em>Power User:</em> Run the whole HTML page through the AMP library</strong>'),
      '#default_value' => variable_get('amp_library_process_full_html', false),
      '#description' => t('The AMP PHP library will fix many AMP HTML standard non-compliance issues by ' .
          'removing illegal or disallowed attributes, tags and property value pairs. This is useful for processing the output of modules that ' .
          'generate AMP unfriendly HTML. Please test when enabling on your site as some modules may depend on ' .
          'the HTML removed by the library and thus break in possibly subtle ways.')
  );

  $form['amp_library_group']['amp_library_process_statistics'] = array(
      '#type' => 'checkbox',
      '#title' => t('<em>Statistics:</em> Add an <a href="https://www.drupal.org/files/issues/time_taken.png">HTML comment</a> at the end of Drupal page output indicating various performance statistics like time taken, number of tags processed etc.'),
      '#default_value' => variable_get('amp_library_process_statistics', false),
      '#states' => array('visible' => array(
          ':input[name="amp_library_process_full_html"]' => array('checked' => TRUE))
      ),
  );

  $form['amp_library_group']['amp_library_process_full_html_warnings'] = array(
      '#type' => 'checkbox',
      '#title' => t('<em>Debugging:</em> Add a notice in the Drupal log for each processed AMP page showing the AMP warnings (and fixes) generated'),
      '#default_value' => variable_get('amp_library_process_full_html_warnings', false),
      '#description' => t('A Drupal log entry will be generated for <em>each</em> non-anonymous AMP request. ' .
          'However <em>anonymous</em> page requests will be cached by Drupal and will not repeatedly call the AMP library.'),
      '#states' => array('visible' => array(
          ':input[name="amp_library_process_full_html"]' => array('checked' => TRUE))
      ),
  );

  $form['#submit'] = array('amp_admin_form_submit');
  return system_settings_form($form);
}
/**
 * Submit handler for the amp_admin_form
 * Clears the Drupal cache if the AMP Library warnings display checkbox value is changed
 *
 * @param type $form
 * @param type $form_state
 */
function amp_admin_form_submit($form, &$form_state) {
  if (variable_get('amp_library_warnings_display', false) !== $form_state['values']['amp_library_warnings_display']) {
    cache_clear_all();
    drupal_set_message(t('AMP Library debugging checkbox changed. Cleared cache.'));
  }
}

/**
 * Form validation handler for amp_admin_form().
 *
 * @see amp_admin_form_submit()
 */
function amp_admin_form_validate($form, &$form_state) {
  // Validate the Google Adsense ID.
  if (!empty($form_state['values']['amp_google_adsense_id'])) {
    if (!preg_match('/^pub-[0-9]+$/', $form_state['values']['amp_google_adsense_id'])) {
      form_set_error('amp_google_adsense_id', t('A valid Google AdSense Publisher ID is case sensitive and formatted like pub-9999999999999'));
    }
  }
}

/**
 * Helper function to get available theme options.
 */
function _amp_get_theme_options() {
  // Get system_sort_theme_by_info_name from system.admin.inc
  module_load_include('inc', 'system', 'system.admin');

  // Get all available themes.
  $themes = system_rebuild_theme_data();
  uasort($themes, 'system_sort_modules_by_info_name');
  $theme_options = array();

  foreach ($themes as &$theme) {
    // Do not show hidden themes.
    if (!empty($theme->info['hidden'])) {
      continue;
    }

    // Do not show disabled themes.
    if (!empty($theme->status)) {
      $theme_options[$theme->name] = $theme->info['name'];
    }
  }

  return $theme_options;
}

/**
 * Helper function to retrieve an array of AMP-enabled content types.
 *
 */
function amp_get_enabled_types() {
  $enabled_types = &drupal_static(__FUNCTION__, array());
  if (empty($enabled_types)) {
    if ($cache = cache_get('amp_enabled_types')) {
      $enabled_types = $cache->data;
    }
    else {
      $node_types = array_keys(node_type_get_names());
      foreach ($node_types as $node_type) {
        $view_mode_settings = field_view_mode_settings('node', $node_type);
        if (isset($view_mode_settings['amp']) && $view_mode_settings['amp']['custom_settings']) {
          $enabled_types[] = $node_type;
        }
      }
      cache_set('amp_enabled_types', $enabled_types, 'cache');
    }
  }
  return $enabled_types;
}

/**
 * Helper function to retrieve a formatted list of AMP-enabled content types.
 */
function amp_get_formatted_status_list() {
  $enabled_types = amp_get_enabled_types();
  $node_types = node_type_get_names();
  $node_status_list = array();
  foreach ($node_types as $bundle => $label) {
    $configure = t('/admin/structure/types/manage/@bundle/display/amp?destination=/admin/config/content/amp', array('@bundle' => $bundle));
    $enable_disable = t('/admin/structure/types/manage/@bundle/display?destination=/admin/config/content/amp', array('@bundle' => $bundle));
    if (in_array($bundle, $enabled_types)) {
      $node_status_list[] = $label .
        t(' is <em>enabled</em>: <a href="@configure">Configure AMP view mode</a> or <a href="@enable_disable">Disable AMP display</a>', array(
          '@configure' => $configure,
          '@enable_disable' => $enable_disable,
        ));
    }
    else {
      $node_status_list[] = $label .
        t(' is <em>disabled</em>: <a href="@enable_disable">Enable AMP in Custom Display Settings</a>', array(
          '@enable_disable' => $enable_disable,
        ));
    }
  }
  return $node_status_list;
}

/**
 * Form constructor for the AMP metadata form.
 */
function amp_admin_metadata_form($form, &$form_state) {
  if (!module_exists('token')) {
    // Provide message in case somebody has upgraded AMP module but has not
    // installed Token.
    drupal_set_message(t('The AMP module requires the <a href="@module">Token</a> module as a dependency. Please download and install Token to prevent errors with AMP.', array('@module' => 'https://www.drupal.org/project/token')), 'warning');
  }

  if (!module_exists('ctools')) {
    // Provide message in case somebody has upgraded AMP module but has not
    // installed ctools.
    drupal_set_message(t('The AMP module requires the <a href="@module">ctools</a> module as a dependency. Please download and install ctools to prevent errors with AMP.', array('@module' => 'https://www.drupal.org/project/ctools')), 'warning');
  }


  $form['amp_metadata_organization'] = array(
    '#type' => 'fieldset',
    '#title' => t('Organization information'),
  );

  $form['amp_metadata_organization']['description'] = array(
    '#type' => 'item',
    '#description' => t('Provide information about your organization for use in search metadata.'),
  );

  $form['amp_metadata_organization']['amp_metadata_token_tree'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('site'),
    '#dialog' => TRUE,
  );

  $form['amp_metadata_organization']['amp_metadata_organization_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Organization name'),
    '#description' => t('
      <p>Name of the publisher of the content. Typically, this can be the same as the site name.</p>
      <p>Suggested token: [site:name]</p>'),
    '#required' => TRUE,
    '#attributes' => ['placeholder' => '[site:name]'],
    '#default_value' => variable_get('amp_metadata_organization_name', NULL)
  );

  $form['amp_metadata_organization']['amp_metadata_organization_logo'] = array(
    '#type' => 'managed_file',
    '#title' => t('Organization logo'),
    '#description' => t('
      <p>Upload a logo for your organization.</p>
      <p>This logo must have a height of 60px and a width less than 600px. SVG logos are not allowed: please provide a JPG, JPEG, GIF or PNG.</p>
      <p>See the AMP <a href="@logo_guidelines">logo guidelines</a>.</p>', array('@logo_guidelines' => 'https://developers.google.com/search/docs/data-types/articles#amp-logo-guidelines')),
    '#upload_location' => 'public://',
    '#upload_validators' => array(
      'file_validate_extensions' => array('png jpg'),
    ),
    '#required' => TRUE,
    '#default_value' => variable_get('amp_metadata_organization_logo', FALSE),
  );

  $form['amp_metadata_organization']['amp_metadata_organization_logo_image_style_id'] = array(
    '#type' => 'select',
    '#title' => t('Organization logo image style'),
    '#options' => image_style_options(TRUE),
    '#description' => t('<p>The image style to use for the organization logo.</p>'),
    '#default_value' => variable_get('amp_metadata_organization_logo_image_style_id', NULL)
  );

  $enabled_types = amp_get_enabled_types();
  if (!empty($enabled_types)) {
    $node_types = node_type_get_names();
    $enabled_type_list = array();
    foreach ($enabled_types as $type) {
      $enabled_type_list[] = $node_types[$type] . t(': <a href="@configure">Edit AMP metadata settings</a>', array(
          '@configure' => '/admin/structure/types/manage/' . $type . '?destination=/admin/config/content/amp/metadata',
        ));
    }

    $form['amp_metadata_content_types'] = array(
      '#title' => 'Content information from AMP-enabled content types',
      '#theme' => 'item_list',
      '#items' => $enabled_type_list,
    );
  }
  else {
    $form['amp_metadata_content_types'] = array(
      '#markup' => t('No content types are currently enabled for AMP. <a href="@configure">Enable them here</a>.', array(
        '@configure' => '/admin/config/content/amp',
      )),
    );
  }

  $form['#submit'] = array('amp_admin_metadata_form_submit');
  return system_settings_form($form);
}

/**
 * Submit handler for the amp_admin_metadata_form.
 * Sets the logo file to permanent.
 */
function amp_admin_metadata_form_submit($form, &$form_state) {
  $old_fid = variable_get('amp_metadata_organization_logo', 0);
  if ($form_state['values']['amp_metadata_organization_logo'] != $old_fid) {

    $file = file_load($form_state['values']['amp_metadata_organization_logo']);

    // If this is a new file...
    if (!$file->status) {
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      file_usage_add($file, 'amp', 'logo', 1);
    }

    // Delete the old file
    $old_file = $old_fid ? file_load($old_fid) : FALSE;
    if ($old_file) {
      file_usage_delete($old_file, 'amp', 'logo', 1);
      // Only delete a file if it is the only place it is in use.
      if (empty(file_usage_list($old_file))) {
        file_delete($old_file);
      }
    }
  }
}
