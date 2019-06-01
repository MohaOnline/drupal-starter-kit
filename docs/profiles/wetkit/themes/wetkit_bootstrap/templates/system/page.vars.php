<?php
/**
 * @file
 * Stub file for "page" theme hook [pre]process functions.
 */

/**
 * Pre-processes variables for the "page" theme hook.
 *
 * See template for list of available variables.
 *
 * @see page.tpl.php
 *
 * @ingroup theme_preprocess
 */
function wetkit_bootstrap_preprocess_page(&$variables) {

  global $base_url;

  // Internationalization Settings.
  global $language;
  $is_multilingual = 0;
  if (module_exists('i18n_menu') && drupal_multilingual()) {
    $is_multilingual = 1;
  }

  // WxT Settings.
  $theme_prefix = 'wb';
  $theme_menu_prefix = 'wet-fullhd-lang';
  $wxt_active = variable_get('wetkit_wetboew_theme', 'theme-wet-boew');
  $library_path = libraries_get_path($wxt_active, TRUE);
  $wxt_active = str_replace('-', '_', $wxt_active);
  $wxt_active = str_replace('theme_', '', $wxt_active);
  $wxt_role_main = 'role="main"';

  // Extra variables to pass to templates.
  $variables['library_path'] = $library_path;
  $variables['language'] = $language->language;
  $variables['language_prefix'] = $language->prefix;
  $variables['language_prefix_alt'] = ($language->prefix == 'en') ? 'fr' : 'fra';

  // Site Name.
  if (!empty($variables['site_name'])) {
    $variables['site_name_title'] = filter_xss(variable_get('site_name', 'Drupal'));
    $variables['site_name_unlinked'] = $variables['site_name_title'];
    $variables['site_name_url'] = url(variable_get('site_frontpage', 'node'));
    $variables['site_name'] = trim($variables['site_name_title']);
  }

  // Logo settings.
  $default_logo = theme_get_setting('default_logo');
  $default_svg_logo = theme_get_setting('wetkit_theme_svg_default_logo');
  $default_logo_path = $variables['logo'];
  $default_svg_logo_path = theme_get_setting('wetkit_theme_svg_logo_path');
  $toggle_logo = theme_get_setting('toggle_logo');
  $variables['logo_class'] = '';
  $variables['logo_svg'] = '';

  // Toggle Logo off/on.
  if ($toggle_logo == 0) {
    $variables['logo'] = '';
    $variables['logo_svg'] = '';
    $variables['logo_class'] = drupal_attributes(array('class' => 'no-logo'));
  }

  // Default Logo.
  if ($default_logo == 1) {
    $variables['logo'] = $library_path . '/assets/logo.png';
    $variables['logo_svg'] = $library_path . '/assets/logo.svg';

    // GCWeb or GC Intranet.
    if ($wxt_active == 'gcweb' || $wxt_active == 'gc_intranet') {
      $variables['logo'] = $library_path . '/assets/sig-blk-' . $language->language . '.png';
      $variables['logo_svg'] = $library_path . '/assets/sig-blk-' . $language->language . '.svg';
    }
    if ($wxt_active == 'gcweb_v5') {
      $variables['logo'] = $library_path . '/assets/sig-blk-' . $language->language . '.png';
      $variables['logo_svg'] = $library_path . '/assets/sig-blk-' . $language->language . '.svg';
    }
    elseif ($wxt_active == 'gcwu_fegc') {
      $variables['logo'] = $library_path . '/assets/sig-' . $language->language . '.png';
      $variables['logo_svg'] = $library_path . '/assets/sig-' . $language->language . '.svg';
    }
  }

  // Custom Logo.
  if ($default_logo == 0) {
    if ($default_svg_logo == 1) {
      $variables['logo_svg'] = $base_url . '/' . $default_svg_logo_path;
    }
  }

  // Default GCWeb misc.
  if ($wxt_active == 'gcweb') {
    $variables['logo_bottom'] = $library_path . '/assets/wmms-blk' . '.png';
    $variables['logo_bottom_svg'] = $library_path . '/assets/wmms-blk' . '.svg';
  }

  // Default GCWeb v5 misc.
  if ($wxt_active == 'gcweb_v5') {
    $variables['logo_bottom'] = $library_path . '/assets/wmms-blk' . '.png';
    $variables['logo_bottom_svg'] = $library_path . '/assets/wmms-blk' . '.svg';
  }

  // Add information about the number of sidebars.
  if (!empty($variables['page']['sidebar_first']) && !empty($variables['page']['sidebar_second'])) {
    $variables['content_column_class'] = ' class="col-sm-6"';
  }
  elseif (!empty($variables['page']['sidebar_first']) || !empty($variables['page']['sidebar_second'])) {
    $variables['content_column_class'] = ' class="col-sm-9"';
  }
  else {
    $variables['content_column_class'] = '';
  }

  // Fluid container.
  if(bootstrap_setting('fluid_container') == 1) {
    $variables['container_class'] = 'container-fluid';
  }
  else {
    $variables['container_class'] = 'container';
  }

  $i18n = module_exists('i18n_menu');

  // Primary menu.
  $variables['primary_nav'] = FALSE;
  if ($variables['main_menu']) {
    // Load the tree
    $tree = menu_tree_page_data(variable_get('menu_main_links_source', 'main-menu'));

    // Localize the tree.
    if ($i18n) {
      $tree = i18n_menu_localize_tree($tree);
    }

    // Build links.
    $variables['primary_nav'] = menu_tree_output($tree);

    // Provide default theme wrapper function.
    $variables['primary_nav']['#theme_wrappers'] = array('menu_tree__primary');
  }

  // Secondary nav.
  $variables['secondary_nav'] = FALSE;
  if ($variables['secondary_menu']) {
    // Load the tree
    $tree = menu_tree_page_data(variable_get('menu_secondary_links_source', 'user-menu'));

    // Localize the tree.
    if ($i18n) {
      $tree = i18n_menu_localize_tree($tree);
    }

    // Build links.
    $variables['secondary_nav'] = menu_tree_output($tree);

    // Provide default theme wrapper function.
    $variables['secondary_nav']['#theme_wrappers'] = array('menu_tree__secondary');
  }

  // Navbar.
  $variables['navbar_classes_array'] = array('navbar');

  if (bootstrap_setting('navbar_position') !== '') {
    $variables['navbar_classes_array'][] = 'navbar-' . bootstrap_setting('navbar_position');
  }
  elseif(bootstrap_setting('fluid_container') == 1) {
    $variables['navbar_classes_array'][] = 'container-fluid';
  }
  else {
    $variables['navbar_classes_array'][] = 'container';
  }
  if (bootstrap_setting('navbar_inverse')) {
    $variables['navbar_classes_array'][] = 'navbar-inverse';
  }
  else {
    $variables['navbar_classes_array'][] = 'navbar-default';
  }

  // Navbar override.
  $variables['navbar_classes_array'] = array('');

  // Mega Menu Region.
  if (module_exists('menu_block') && empty($variables['mega_menu'])) {
    $menu_name = 'main_menu';
    $data = array(
      '#pre_render' => array('_wetkit_menu_tree_build_prerender'),
      '#cache' => array(
        'keys' => array('wetkit_bootstrap', 'menu', 'mega_menu', $menu_name),
        'expire' => CACHE_TEMPORARY,
        'granularity' => DRUPAL_CACHE_PER_ROLE
      ),
      '#menu_name' => $menu_name,
    );
    $variables['page']['mega_menu'] = $data;
  }

  // Splash Page.
  if (current_path() == 'splashify-splash') {
    // GCWeb Theme.
    if ($wxt_active == 'gcweb') {
      $variables['background'] = base_path() . drupal_get_path('theme', 'wetkit_bootstrap') . '/images/sp-bg-2.jpg';
    }
  }

  // Panels Integration.
  if (module_exists('page_manager')) {
    $panels_wxt_active = str_replace('theme_', '', $wxt_active);

    // Page template suggestions for Panels pages.
    $panel_page = page_manager_get_current_page();

    $panel_page_display = panels_get_current_page_display();
    if (isset($panel_page_display)) {
      $panel_page_display_renderer = panels_get_renderer_handler($panel_page_display->renderer, $panel_page_display);
      if (isset($panel_page_display_renderer->plugins['layout']['main']) && $panel_page_display_renderer->plugins['layout']['main'] == TRUE) {
        $wxt_role_main = '';
      }
    }

    if (!empty($panel_page)) {
      // Add the active WxT theme machine name to the template suggestions.
      $suggestions[] = 'page__panels__' . $panels_wxt_active;

      if (drupal_is_front_page()) {
        $suggestions[] = 'page__panels__' . $panels_wxt_active . '__front';
      }

      // Add the panel page machine name to the template suggestions.
      $suggestions[] = 'page__' . $panel_page['name'];
      // Merge the suggestions in to the existing suggestions
      // (as more specific than the existing suggestions).
      $variables['theme_hook_suggestions'] = array_merge($variables['theme_hook_suggestions'], $suggestions);
      $variables['panels_layout'] = TRUE;
    }
    // Page template suggestions for normal pages.
    else {
      $suggestions[] = 'page__' . $panels_wxt_active;

      // Splash Page.
      if (current_path() == 'splashify-splash') {
        $suggestions[] = 'page__splash__' . $panels_wxt_active;
      }

      // Merge the suggestions in to the existing suggestions (as more specific
      // than the existing suggestions).
      $variables['theme_hook_suggestions'] = array_merge($variables['theme_hook_suggestions'], $suggestions);
    }
  }

  // Fix for role main use in panels
  $variables['wxt_role_main'] = $wxt_role_main;

  // Header Navigation + Language Switcher.
  $menu = ($is_multilingual) ? i18n_menu_navigation_links('menu-wet-header') : menu_navigation_links('menu-wet-header');
  $nav_bar_markup = theme('links__menu_menu_wet_header', array(
    'links' => $menu,
    'attributes' => array(
      'id' => 'menu',
      'class' => array('links', 'clearfix'),
    ),
    'heading' => array(
      'text' => 'Language Selection',
      'level' => 'h2',
    ),
  ));
  $nav_bar_markup = strip_tags($nav_bar_markup, '<h2><li><a>');

  if (module_exists('wetkit_language')) {
    if ($wxt_active == 'gcwu_fegc') {
      $language_link_markup = '<li id="wb-lng"><ul class="list-inline"><li>' . strip_tags($variables['menu_lang_bar'], '<a><span>') . '</li></ul></li>';
    }
    else {
      $language_link_markup = '<li id="' . $theme_menu_prefix . '">' . strip_tags($variables['menu_lang_bar'], '<a><span>') . '</li>';
    }

    if ($wxt_active == 'gcweb') {
      $variables['menu_bar'] = '<ul class="list-inline margin-bottom-none">' . $language_link_markup . '</ul>';
    }
    else if ($wxt_active == 'gcweb_v5') {
      $variables['menu_bar'] = '<ul class="links list-inline mrgn-bttm-none">' . $language_link_markup . '</ul>';
    }
    else if ($wxt_active == 'gcwu_fegc') {
      $variables['menu_bar'] = '<ul id="gc-bar" class="list-inline">' . preg_replace("/<h([1-6]{1})>.*?<\/h\\1>/si", '', $nav_bar_markup) . $language_link_markup . '</ul>';
    }
    else if ($wxt_active == 'gc_intranet') {
      $variables['menu_bar'] = '<ul id="gc-bar" class="list-inline">' . $language_link_markup . '</ul>';
    }
    else {
      $variables['menu_bar'] = '<ul class="text-right">' . preg_replace("/<h([1-6]{1})>.*?<\/h\\1>/si", '', $nav_bar_markup) . $language_link_markup . '</ul>';
    }
  }
  else {
    $variables['menu_bar'] = '<ul class="text-right">' . preg_replace("/<h([1-6]{1})>.*?<\/h\\1>/si", '', $nav_bar_markup) . '</ul>';
  }

  // Custom Search Box.
  if (module_exists('custom_search')) {
    $custom_search_form_name = 'custom_search_blocks_form_1';
    $custom_search = array(
      '#pre_render' => array('_wetkit_custom_search_prerender'),
      '#cache' => array(
        'keys' => array('wetkit_bootstrap', 'custom_search', $custom_search_form_name),
        'expire' => CACHE_TEMPORARY,
        'granularity' => DRUPAL_CACHE_PER_USER
      ),
      '#custom_search_form_name' => $custom_search_form_name,
      '#wxt_active' => $wxt_active,
    );
    $variables['custom_search'] = $custom_search;

    // CDN Support.
    if ($wxt_active == 'gcweb') {
      // Setup the CDN variables.
      // Support the legacy configurations.
      $gcweb_cdn = theme_get_setting('gcweb_cdn');
      if (!empty($gcweb_cdn)) {
        $variables['gcweb_cdn_megamenu'] = TRUE;
        $variables['gcweb_cdn_megamenu_url'] = '//cdn.canada.ca/gcweb-cdn-live/sitemenu/sitemenu-';
        $variables['gcweb_cdn_goc_initiatives'] = TRUE;
      }
      // Otherwise use the new configurations.
      else {
        $gcweb_cdn_megamenu = theme_get_setting('gcweb_cdn_megamenu');
        $variables['gcweb_cdn_megamenu'] = !empty($gcweb_cdn_megamenu) ? TRUE : FALSE;

        $gcweb_cdn_megamenu_url = theme_get_setting('gcweb_cdn_megamenu_url');
        $variables['gcweb_cdn_megamenu_url'] = !empty($gcweb_cdn_megamenu_url)? $gcweb_cdn_megamenu_url : '//cdn.canada.ca/gcweb-cdn-live/sitemenu/sitemenu-';

        $gcweb_cdn_goc_initiatives = theme_get_setting('gcweb_cdn_goc_initiatives');
        $variables['gcweb_cdn_goc_initiatives'] = !empty($gcweb_cdn_goc_initiatives) ? TRUE : FALSE;
      }

      $gcweb_election = theme_get_setting('gcweb_election');
      $variables['gcweb_election'] = (!empty($gcweb_election)) ? TRUE : FALSE;
    }

    if ($wxt_active == 'gcweb_v5') {
      // Setup the CDN variables.
      // Support the legacy configurations.
      $gcweb_cdn = theme_get_setting('gcweb_cdn');
      if (!empty($gcweb_cdn)) {
        $variables['gcweb_cdn_megamenu'] = TRUE;
        $variables['gcweb_cdn_megamenu_url'] = 'https://www.canada.ca/content/dam/canada/sitemenu/sitemenu-v2-';
        $variables['gcweb_cdn_goc_initiatives'] = TRUE;
      }
      // Otherwise use the new configurations.
      else {
        $gcweb_cdn_megamenu = theme_get_setting('gcweb_cdn_megamenu');
        $variables['gcweb_cdn_megamenu'] = !empty($gcweb_cdn_megamenu) ? TRUE : FALSE;

        $gcweb_cdn_megamenu_url = theme_get_setting('gcweb_cdn_megamenu_url');
        $variables['gcweb_cdn_megamenu_url'] = !empty($gcweb_cdn_megamenu_url)? $gcweb_cdn_megamenu_url : 'https://www.canada.ca/content/dam/canada/sitemenu/sitemenu-v2-';

        $gcweb_cdn_goc_initiatives = theme_get_setting('gcweb_cdn_goc_initiatives');
        $variables['gcweb_cdn_goc_initiatives'] = !empty($gcweb_cdn_goc_initiatives) ? TRUE : FALSE;
      }

      $gcweb_election = theme_get_setting('gcweb_election');
      $variables['gcweb_election'] = (!empty($gcweb_election)) ? TRUE : FALSE;
    }

    // Visibility settings.
    $pages = drupal_strtolower(theme_get_setting('wetkit_search_box'));
    // Convert the Drupal path to lowercase.
    $path = drupal_strtolower(drupal_get_path_alias($_GET['q']));
    // Compare the lowercase internal and lowercase path alias (if any).
    $page_match = drupal_match_path($path, $pages);
    if ($path != $_GET['q']) {
      $page_match = $page_match || drupal_match_path($_GET['q'], $pages);
    }
    // When $visibility has a value of 0 (VISIBILITY_NOTLISTED),
    // the block is displayed on all pages except those listed in $pages.
    // When set to 1 (VISIBILITY_LISTED), it is displayed only on those
    // pages listed in $pages.
    $visibility = 0;
    $page_match = !(0 xor $page_match);
    if ($page_match) {
      $variables['search_box'] = render($variables['custom_search']);
      $variables['search_box'] = str_replace('type="text"', 'type="search"', $variables['search_box']);
    }
    else {
      $variables['search_box'] = '';
    }
  }

  // Terms Navigation.
  $menu = ($is_multilingual) ? i18n_menu_navigation_links('menu-wet-terms') : menu_navigation_links('menu-wet-terms');
  $class = ($wxt_active == 'gcwu_fegc' || $wxt_active == 'gc_intranet') ? array('list-inline') : array('links', 'clearfix');
  $terms_class = 'gc-tctr';
  if ($wxt_active == 'gcweb') {
    $terms_bar_markup = theme('links__menu_menu_wet_terms', array(
      'links' => $menu,
      'heading' => array(),
    ));
  }
  else {
    $terms_bar_markup = theme('links__menu_menu_wet_terms', array(
      'links' => $menu,
      'attributes' => array(
        'id' => $terms_class,
        'class' => $class,
      ),
      'heading' => array(),
    ));
  }
  $variables['page']['menu_terms_bar'] = $terms_bar_markup;

  // Mid Footer Region.
  if (module_exists('menu_block')) {
    $menu_name = 'mid_footer_menu';
    $data = array(
      '#pre_render' => array('_wetkit_menu_tree_build_prerender'),
      '#cache' => array(
        'keys' => array('wetkit_bootstrap', 'menu', 'footer', $menu_name),
        'expire' => CACHE_TEMPORARY,
        'granularity' => DRUPAL_CACHE_PER_ROLE
      ),
      '#menu_name' => $menu_name,
    );
    $variables['page']['footer']['minipanel'] = $data;
  }

  // Unset powered by block.
  unset($variables['page']['footer']['system_powered-by']);

  // Footer Navigation.
  $menu = ($is_multilingual) ? i18n_menu_navigation_links('menu-wet-footer') : menu_navigation_links('menu-wet-footer');
  $class = ($wxt_active == 'gcwu_fegc' || $wxt_active == 'gc_intranet') ? array('list-inline') : array('links', 'clearfix');
  $footer_bar_markup = theme('links__menu_menu_wet_footer', array(
    'links' => $menu,
    'attributes' => array(
      'id' => 'menu',
      'class' => $class,
    ),
    'heading' => array(),
  ));
  $variables['page']['menu_footer_bar'] = $footer_bar_markup;

  // Footer Navigation (gcweb).
  if ($wxt_active == 'gcweb') {
    $variables['gcweb'] = array(
      'feedback' => array(
        'en' => 'http://www.canada.ca/en/contact/feedback.html',
        'fr' => 'http://www.canada.ca/fr/contact/retroaction.html',
      ),
      'social' => array(
        'en' => 'http://www.canada.ca/en/social/index.html',
        'fr' => 'http://www.canada.ca/fr/sociaux/index.html',
      ),
      'mobile' => array(
        'en' => 'http://www.canada.ca/en/mobile/index.html',
        'fr' => 'http://www.canada.ca/fr/mobile/index.html',
      ),
    );
  }
}

/**
 * Processes variables for the "page" theme hook.
 *
 * See template for list of available variables.
 *
 * @see page.tpl.php
 *
 * @ingroup theme_process
 */
function wetkit_bootstrap_process_page(&$variables) {
  // Store the page variables in cache so it can be used in region
  // preprocessing.
  $variables['navbar_classes'] = implode(' ', $variables['navbar_classes_array']);
}

/**
 * Pre Render handler for cache based menu block handling.
 */
function _wetkit_menu_tree_build_prerender($element) {
  $config = menu_block_get_config($element['#menu_name']);
  $data = menu_tree_build($config);
  $element['content'] = $data['content'];
  return $element;
}

/**
 * Pre Render handler for cache based menu block handling.
 */
function _wetkit_custom_search_prerender($element) {
  global $language;
  $data = drupal_get_form($element['#custom_search_form_name']);
  $data['#id'] = 'search-form';
  $data['custom_search_blocks_form_1']['#id'] = 'wb-srch-q';
  $data['actions']['submit']['#id'] = 'wb-srch-sub';
  $data['actions']['submit']['#attributes']['data-icon'] = 'search';
  $data['actions']['submit']['#attributes']['value'] = t('search');
  $data['#attributes']['class'][] = 'form-inline';
  $data['#attributes']['role'] = 'search';
  $data['actions']['#theme_wrappers'] = NULL;

  // Special handling for GCWeb Theme
  if ($element['#wxt_active'] == 'gcweb') {
    $data['#attributes']['name'] = 'search-form';
    $data['actions']['submit']['#attributes']['name'] = 'wb-srch-sub';
    $data['actions']['submit']['#value'] = '<span class="glyphicon-search glyphicon"></span><span class="wb-inv">' . t('Search') . '</span>';
    $data['custom_search_blocks_form_1']['#attributes']['placeholder'] = t('Search website');
    $cdn_srch = theme_get_setting('canada_search');
    if (!empty($cdn_srch)) {
      $data['custom_search_blocks_form_1']['#attributes']['placeholder'] = t('Search Canada.ca');
      $data['custom_search_blocks_form_1']['#name'] = 'q';
      $data['custom_search_blocks_form_1']['#size'] = '27';
      $data['#action'] = 'http://recherche-search.gc.ca/rGs/s_r?#wb-land';
      $data['#method'] = 'get';
      $data['cdn'] = array(
        '#name' => 'cdn',
        '#value' => 'canada',
        '#type' => 'hidden',
        '#input' => 'TRUE',
      );
      $data['st'] = array(
        '#name' => 'st',
        '#value' => 's',
        '#type' => 'hidden',
        '#input' => 'TRUE',
      );
      $data['num'] = array(
        '#name' => 'num',
        '#value' => '10',
        '#type' => 'hidden',
        '#input' => 'TRUE',
      );
      $data['langs'] = array(
        '#name' => 'langs',
        '#value' => ($language->language == 'fr') ? 'fra' : 'eng',
        '#type' => 'hidden',
        '#input' => 'TRUE',
      );
      $data['st1rt'] = array(
        '#name' => 'st1rt',
        '#value' => '0',
        '#type' => 'hidden',
        '#input' => 'TRUE',
      );
      $data['s5bm3ts21rch'] = array(
        '#name' => 's5bm3ts21rch',
        '#value' => 'x',
        '#type' => 'hidden',
        '#input' => 'TRUE',
      );
    }
  }

  $element['content'] = $data;
  return $element;
}
