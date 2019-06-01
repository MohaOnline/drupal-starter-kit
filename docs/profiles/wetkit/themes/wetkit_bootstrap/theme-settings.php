<?php
/**
 * @file
 * theme-settings.php
 *
 * Provides theme settings for Bootstrap based themes when admin theme is not.
 *
 * @see templates/settings.inc
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function wetkit_bootstrap_form_system_theme_settings_alter(&$form, $form_state, $form_id = NULL) {
  // Work-around for a core bug affecting admin themes.
  // @see https://drupal.org/node/943212
  if (isset($form_id)) {
    return;
  }

  $wxt_active = variable_get('wetkit_wetboew_theme', 'theme-wet-boew');
  $library_path = libraries_get_path($wxt_active, TRUE);
  $wxt_active = str_replace('-', '_', $wxt_active);

  $form['wetkit_bootstrap'] = array(
    '#type' => 'vertical_tabs',
    '#attached' => array(
      'js'  => array(drupal_get_path('theme', 'bootstrap') . '/js/bootstrap.admin.js'),
    ),
    '#prefix' => '<h2><small>' . t('WxT Bootstrap Settings') . '</small></h2>',
    '#weight' => -10,
  );

  // Accessibility.
  $form['wetkit_accessibility'] = array(
    '#type' => 'fieldset',
    '#title' => t('Accessibility'),
    '#group' => 'wetkit_bootstrap',
  );

  // Skip Navigation.
  $form['wetkit_accessibility']['skip_nav'] = array(
    '#type' => 'fieldset',
    '#title' => t('Skip Navigation'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['wetkit_accessibility']['skip_nav']['wetkit_skip_link_text_1'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Text for the primary “skip link”'),
    '#default_value' => t('Skip to main content'),
    '#description'   => t('For example: <em>Jump to navigation</em>, <em>Skip to content</em>'),
  );
  $form['wetkit_accessibility']['skip_nav']['wetkit_skip_link_id_1'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Anchor ID for the “skip link” 1'),
    '#default_value' => theme_get_setting('wetkit_skip_link_id_1'),
    '#description'   => t('Specify the HTML ID of the element that the accessible-but-hidden “skip link” should link to. (<a href="!link">Read more about skip links</a>.)', array('!link' => 'http://drupal.org/node/467976')),
  );
  $form['wetkit_accessibility']['skip_nav']['wetkit_skip_link_text_2'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Text for secondary “skip link”'),
    '#default_value' => t('Skip to "About this site"'),
    '#description'   => t('For example: <em>Jump to navigation</em>, <em>Skip to content</em>'),
  );
  $form['wetkit_accessibility']['skip_nav']['wetkit_skip_link_id_2'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Anchor ID for the “skip link” 2'),
    '#default_value' => theme_get_setting('wetkit_skip_link_id_2'),
    '#description'   => t('Specify the HTML ID of the element that the accessible-but-hidden “skip link” should link to. (<a href="!link">Read more about skip links</a>.)', array('!link' => 'http://drupal.org/node/467976')),
  );

  // Customization.
  $form['wetkit_customization'] = array(
    '#type' => 'fieldset',
    '#title' => t('Customization'),
    '#group' => 'wetkit_bootstrap',
  );

  // Overrides.
  $form['wetkit_customization']['overrides'] = array(
    '#type' => 'fieldset',
    '#title' => t('Overrides'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['wetkit_customization']['overrides']['wetkit_render_mb_main_link'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Disable rendering of the mb main link inside the mega menu'),
    '#default_value' => theme_get_setting('wetkit_render_mb_main_link'),
    '#description'   => t('Specify whether or not the mega menu should include the main link.'),
  );
  $form['wetkit_customization']['overrides']['wetkit_render_no_link'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Disable rendering of menu items with &lt;nolink&gt; as path'),
    '#default_value' => theme_get_setting('wetkit_render_no_link'),
    '#description'   => t('Specify whether or not menu links with <strong>&lt;nolink&gt;</strong> as path should render an a href. (This can break certain versions of WET)'),
  );
  $form['wetkit_customization']['overrides']['wetkit_sidebar_no_chevron'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Disable rendering of chverons for sidebar menu items'),
    '#default_value' => theme_get_setting('wetkit_sidebar_no_chevron'),
    '#description'   => t('Specify whether or not chevrons appearing beside menu links with children should be rendered.'),
  );
  $form['wetkit_customization']['overrides']['wetkit_sub_site'] = array(
    '#type' => 'textfield',
    '#title' => t('Intranet web site name'),
    '#default_value' => theme_get_setting('wetkit_sub_site'),
    '#description' => t('The display name for the Intranet web site'),
  );
  $form['wetkit_customization']['overrides']['wetkit_sub_site_alt_url'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Alternate Site URL'),
    '#default_value' => theme_get_setting('wetkit_sub_site_alt_url'),
    '#description'   => t('Alternate Site URL to be used with Site Name.'),
  );
  $form['wetkit_customization']['overrides']['wetkit_search_indexing_start'] = array(
    '#type' => 'textfield',
    '#title' => t('Start Indexing tag for search'),
    '#default_value' => theme_get_setting('wetkit_search_indexing_start'),
    '#description' => t('Custom Markup to wrap around content to assist in indexing.'),
  );
  $form['wetkit_customization']['overrides']['wetkit_search_indexing_stop'] = array(
    '#type' => 'textfield',
    '#title' => t('Stop Indexing tag for search'),
    '#default_value' => theme_get_setting('wetkit_search_indexing_stop'),
    '#description' => t('Custom Markup to wrap around content to assist in indexing.'),
  );

  // Search.
  $form['wetkit_search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search'),
    '#group' => 'wetkit_bootstrap',
  );
  $form['wetkit_search']['wetkit_search_box'] = array(
    '#type' => 'textarea',
    '#title' => t('Search box visibility'),
    '#default_value' => theme_get_setting('wetkit_search_box'),
    '#description' => t("Specify pages to exclude by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are <strong>blog</strong> for the blog page and <strong>blog/*</strong> for every personal blog. &lt;front&gt; is the front page."),
  );
  $form['wetkit_search']['canada_search'] = array(
    '#type' => 'checkbox',
    '#title' => t('Toggle Canada.ca Search'),
    '#default_value' => theme_get_setting('canada_search'),
    '#description' => t('If checked the GCWeb theme will use the Canada.ca search'),
  );

  // GCWeb.
  // Support the legacy configurations.
  $gcweb_cdn = theme_get_setting('gcweb_cdn');
  if (!empty($gcweb_cdn)) {
    $gcweb_cdn_megamenu = TRUE;
    $gcweb_cdn_megamenu_url = '//cdn.canada.ca/gcweb-cdn-live/sitemenu/sitemenu-';
    $gcweb_cdn_goc_initiatives = TRUE;
  }
  // Otherwise use the new configurations.
  else {
    $gcweb_cdn_megamenu = theme_get_setting('gcweb_cdn_megamenu');
    $gcweb_cdn_megamenu = !empty($gcweb_cdn_megamenu) ? TRUE : FALSE;

    $gcweb_cdn_megamenu_url = theme_get_setting('gcweb_cdn_megamenu_url');
    $gcweb_cdn_megamenu_url = !empty($gcweb_cdn_megamenu_url) ? $gcweb_cdn_megamenu_url : '//cdn.canada.ca/gcweb-cdn-live/sitemenu/sitemenu-';

    $gcweb_cdn_goc_initiatives = theme_get_setting('gcweb_cdn_goc_initiatives');
    $gcweb_cdn_goc_initiatives = !empty($gcweb_cdn_goc_initiatives) ? TRUE : FALSE;
  }

  if ($wxt_active == 'gcweb_v5') {
    $gcweb_cdn = theme_get_setting('gcweb_cdn');
    if (!empty($gcweb_cdn)) {
      $gcweb_cdn_megamenu = TRUE;
      $gcweb_cdn_megamenu_url = 'https://www.canada.ca/content/dam/canada/sitemenu/sitemenu-v2-';
      $gcweb_cdn_goc_initiatives = TRUE;
    }
    // Otherwise use the new configurations.
    else {
      $gcweb_cdn_megamenu = theme_get_setting('gcweb_cdn_megamenu');
      $gcweb_cdn_megamenu = !empty($gcweb_cdn_megamenu) ? TRUE : FALSE;

      $gcweb_cdn_megamenu_url = theme_get_setting('gcweb_cdn_megamenu_url');
      $gcweb_cdn_megamenu_url = !empty($gcweb_cdn_megamenu_url) ? $gcweb_cdn_megamenu_url : 'https://www.canada.ca/content/dam/canada/sitemenu/sitemenu-v2-';

      $gcweb_cdn_goc_initiatives = theme_get_setting('gcweb_cdn_goc_initiatives');
      $gcweb_cdn_goc_initiatives = !empty($gcweb_cdn_goc_initiatives) ? TRUE : FALSE;
    }
  }

  $form['wetkit_gcweb'] = array(
    '#type' => 'fieldset',
    '#title' => t('GCWeb'),
    '#group' => 'wetkit_bootstrap',
  );
  $form['wetkit_gcweb']['gcweb_cdn_megamenu'] = array(
    '#type' => 'checkbox',
    '#title' => t('Toggle CDN for MegaMenu'),
    '#default_value' => $gcweb_cdn_megamenu,
    '#description' => t('If checked the GCWeb theme will use the CDN for the MegaMenu'),
  );
  $form['wetkit_gcweb']['gcweb_cdn_megamenu_url'] = array(
    '#type' => 'textfield',
    '#title' => t('CDN for MegaMenu'),
    '#prefix' => '<div id="gcweb_cdn_megamenu_url">',
    '#suffix' => '</div>',
    '#default_value' => $gcweb_cdn_megamenu_url,
    '#description' => t('The CDN to use for the MegaMenu. Will be appended with the language and .html (i.e.: en.html or fr.html)'),
    '#states' => array(
      'visible' => array(
        ':input[name="gcweb_cdn_megamenu"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['wetkit_gcweb']['gcweb_cdn_goc_initiatives'] = array(
    '#type' => 'checkbox',
    '#title' => t('Toggle CDN for GoC Initiatives'),
    '#default_value' => $gcweb_cdn_goc_initiatives,
    '#description' => t('If checked the GCWeb theme will use the CDN for GoC Initiatives'),
  );
  $form['wetkit_gcweb']['gcweb_election'] = array(
    '#type' => 'checkbox',
    '#title' => t('Toggle Election mode'),
    '#default_value' => theme_get_setting('gcweb_election'),
    '#description' => t('If checked the GCWeb theme will disable the GCWeb Features section'),
  );
}
