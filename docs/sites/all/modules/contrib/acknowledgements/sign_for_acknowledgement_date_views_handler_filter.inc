<?php

/**
 * @file
 * Definition of sign_for_acknowledgement_user_views_handler_filter.
 */

/**
 * Filter handler for the current user.
 *
 * @ingroup views_filter_handlers
 */
class sign_for_acknowledgement_date_views_handler_filter extends views_handler_filter_boolean_operator {
  function construct() {
    parent::construct();
    $this->value_value = t('Show nodes if...');
    $this->accept_null = TRUE;
  }
  function get_value_options() {
    $this->value_options = array(1 => t('Signed'), 0 => t('Unsigned'));
  }
}
