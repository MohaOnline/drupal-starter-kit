<?php
/**
 * This file holds the ShareaholicAdmin class.
 *
 * @package shareaholic
 */

/**
 * This class takes care of all of the admin interface.
 *
 * @package shareaholic
 */
class ShareaholicAdmin {


  /**
   * Outputs the actual html for either the terms_of_service modal or the
   * failed_create_api_key modal depending on what is in the database
   *
   * @return String html output for the modals
   */
  public static function draw_modal_popup() {
    if(!ShareaholicUtilities::has_accepted_terms_of_service()) {
      self::draw_tos_popup();
    } else if (!ShareaholicUtilities::get_option('api_key')) {
      self::draw_failure_popup();
    }
  }


  /**
   * Outputs the actual html for either the terms_of_service modal
   * to be rendered on the admin pages
   *
   */
  public static function draw_tos_popup() {
    $form = drupal_get_form('shareaholic_tos_modal_form');
    print(drupal_render($form));
  }


  /**
   * Outputs the actual html for either the terms_of_service modal
   * to be rendered on the admin pages
   *
   */
  public static function draw_failure_popup() {
    $form = drupal_get_form('shareaholic_failure_modal_form');
    print(drupal_render($form));
  }


  /**
   * Show the terms of service notice on admin pages
   * except for shareaholic admin settings page
   */
  public static function show_terms_of_service_notice(&$vars) {
    if(ShareaholicUtilities::is_admin_page() &&
        !ShareaholicUtilities::is_shareaholic_settings_page() &&
        !ShareaholicUtilities::has_accepted_terms_of_service() &&
        user_access('administer modules')) {
          $link = l('Get started now >>', 'admin/config/shareaholic/settings', array('attributes' => array('style' => 'background: #f3f3f3; border-color: #bbb; color: #333; display: inline-block; text-decoration: none; cursor: pointer; border-radius: 3px; padding: 0 10px; 1px; font-size: 12px; height: 20px;')));
          $message = sprintf(t('Action required: You\'ve installed Shareaholic for Drupal.  We\'re ready when you are. %s'), $link);
      $vars['page'] = self::header_message_html($message) . $vars['page'];
    }
  }

  /**
   * Show the pending update notice on admin pages
   *
   */
  public static function show_pending_update_notice(&$vars) {
    if(ShareaholicUtilities::is_admin_page() &&
       ShareaholicUtilities::has_accepted_terms_of_service() &&
       !db_table_exists('shareaholic_content_settings') &&
        user_access('administer modules')) {
          $message = sprintf(t('Action required: You have pending updates required by Shareaholic. Please go to update.php for more information.'));
      $vars['page'] = self::header_message_html($message) . $vars['page'];
    }
  }

  /**
   * The html for the Shareaholic notice as a string
   * @return String The html for the notice as a string
   */
  private static function header_message_html($message) {
    $img_check = SHAREAHOLIC_ASSET_DIR . '/img/check.png';
    $html = <<< DOC
  <div id="shareaholic-wrap-container" style="padding: 0 20px 0px 15px; background-color: #45a147; margin: 25px 0px 20px -18px;">
    <img src="$img_check" style="vertical-align:middle;" />
    <span id="shareaholic-text-container" style="color: #fff; text-shadow: 0px 1px 1px rgba(0,0,0,0.4); font-size: 14px; vertical-align:middle;">
        <strong>$message</strong>
    </span>
  </div>
  <div style="clear:both;"></div>
DOC;
    return $html;
  }

  /**
   * Inserts the necessary css and js assets
   * for the Shareaholic Admin Pages
   *
   */
  public static function include_css_js_assets() {
    $module_path = drupal_get_path('module', 'shareaholic');
    drupal_add_css('//fonts.googleapis.com/css?family=Open+Sans:400,300,700', array('type' => 'external', 'group' => CSS_DEFAULT));
    drupal_add_css($module_path . '/assets/css/bootstrap.css', array('group' => CSS_DEFAULT));
    drupal_add_css($module_path . '/assets/css/bootstrap-responsive.css', array('group' => CSS_DEFAULT));
    drupal_add_css($module_path . '/assets/css/reveal.css', array('group' => CSS_DEFAULT));
    drupal_add_css($module_path . '/assets/css/main.css', array('group' => CSS_DEFAULT));
    drupal_add_js(ShareaholicUtilities::asset_url('assets/pub/utilities.js'), array('type' => 'external', 'group' => JS_DEFAULT));
    drupal_add_js(ShareaholicUtilities::asset_url('assets/pub/shareaholic_sdk.js'), array('type' => 'external', 'group' => JS_DEFAULT));
    drupal_add_js($module_path . '/assets/js/jquery_custom.js', array('group' => JS_DEFAULT));
    drupal_add_js($module_path . '/assets/js/jquery_ui_custom.js', array('group' => JS_DEFAULT));
    drupal_add_js($module_path . '/assets/js/jquery.reveal.modified.js', array('group' => JS_DEFAULT));
    drupal_add_js($module_path . '/assets/js/bootstrap.js', array('group' => JS_DEFAULT));
    drupal_add_js($module_path . '/assets/js/main.js', array('group' => JS_DEFAULT));
  }

  /**
   * This function is in charge of the logic for
   * showing whatever it is we want to show a user
   * about whether they have verified their api
   * key or not.
   */
  public static function draw_verify_api_key() {
    if (!ShareaholicUtilities::api_key_verified()) {
      print theme('shareaholic_verify_api_key');
    }
  }

  /**
   * Sends an event when the user has updated
   * the Drupal module
   */
  public static function update_check() {
    $version = ShareaholicUtilities::get_option('version');
    if (ShareaholicUtilities::get_option('api_key') && $version != ShareaholicUtilities::get_version()) {
      ShareaholicUtilities::set_version(ShareaholicUtilities::get_version());
      ShareaholicUtilities::log_event('Upgrade', array ('previous_plugin_version' => $version));
    }
  }

  /**
   * Renders SnapEngage
   */
  public static function include_snapengage() {
    ShareaholicUtilities::load_template('script_chat');
  }

  /**
   * This function will run post install tasks
   * when a shareaholic flag is set
   */
  public static function post_install() {
    if (variable_get('Installed_Module_Shareaholic', '') == 'shareaholic') {
      // delete this so we do not check again
      variable_del('Installed_Module_Shareaholic');
      // Do share counts check
      ShareaholicUtilities::share_counts_api_connectivity_check();
    }
  }

  /**
   * Renders footer
   */
  public static function show_footer() {
    ShareaholicUtilities::load_template('footer');
  }

  /**
   *
   */
  public static function show_header() {
    $settings = ShareaholicUtilities::get_settings();
    $settings['base_link'] = ShareaholicUtilities::URL . '/publisher_tools/' . $settings['api_key'] . '/';
    $settings['website_settings_link'] = $settings['base_link'] . 'websites/edit?verification_key=' . $settings['verification_key'];
    ShareaholicUtilities::load_template('header', array(
      'settings' => $settings
    ));
  }

}

