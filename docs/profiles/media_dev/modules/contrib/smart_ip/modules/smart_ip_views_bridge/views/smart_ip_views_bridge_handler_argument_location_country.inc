<?php
// $Id$

/**
 * @file
 * Contains the 'smart_ip_views_bridge_handler_argument_location_country' argument handler.
 */

class smart_ip_views_bridge_handler_argument_location_country extends location_handler_argument_location_country {
  /**
   * {@inheritdoc}
   */
  function init(&$view, &$options) {
    if (empty($view->args) && isset($options['default_action']) && $options['default_action'] == 'smart_ip') {
      $smart_ip_session = smart_ip_session_get('smart_ip');
      if (isset($smart_ip_session['location']['country_code'])) {
        $view->args = array($smart_ip_session['location']['country_code']);
      }
    }
    parent::init($view, $options);
  }

  /**
   * {@inheritdoc}
   */
  function default_actions($which = NULL) {
    $defaults = parent::default_actions();
    $defaults['smart_ip'] = array(
      'title' => t("Use Smart IP detected visitor's country value"),
      'method' => 'default_ignore',
      'breadcrumb' => TRUE,
    );
 
    if ($which) {
      if (!empty($defaults[$which])) {
        return $defaults[$which];
      }
    }
    else {
      return $defaults;
    }
  }
}