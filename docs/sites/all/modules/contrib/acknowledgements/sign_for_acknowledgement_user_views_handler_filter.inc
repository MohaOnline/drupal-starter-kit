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
class sign_for_acknowledgement_user_views_handler_filter extends views_handler_filter_boolean_operator/*views_handler_filter_in_operator*/ {
  function construct() {
    parent::construct();
    $this->value_value = t('Select user');
  }
  function get_value_options() {
    $users = array();
    if (user_access('view acknowledgements table')) {
      $users_list = entity_load('user');
      foreach ($users_list as $user) {
        if ($user->uid == 0 || $user->status == 0) {
          continue;
        }
        $users[$user->uid] = $user->name;
      }
    }
    // Sort by username.
    natsort($users);

    $this->value_options = array('0' => t('Current user')) + $users;

    return $users;
  }
  function expose_form(&$form, &$form_state) {
    parent::expose_form($form, $form_state);
    $form['expose']['remember']['#default_value'] = FALSE;
    $form['expose']['remember']['#disabled'] = TRUE;
    $form['expose']['required']['#default_value'] = FALSE;
    $form['expose']['required']['#disabled'] = TRUE;
  }
  function value_form(&$form, &$form_state) {
    if (empty($this->value_options)) {
      // Initialize the array of possible values for this filter.
      $this->get_value_options();
    }
    if (!empty($form_state['exposed'])) {
      $filter_disabled = FALSE;
    }
    else {
      $filter_disabled = TRUE;
    }
    $form['value'] = array(
      '#type' => 'select',
      '#title' => $this->value_value,
      '#options' => $this->value_options,
      '#default_value' => 0, //$this->value,
      '#disabled' => $filter_disabled,
    );
    if (!empty($this->options['exposed'])) {
      $identifier = $this->options['expose']['identifier'];
      if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier])) {
        $form_state['input'][$identifier] = $this->value;
      }
      // If we're configuring an exposed filter, add an <Any> option.
      if (empty($form_state['exposed']) || empty($this->options['expose']['required'])) {
        $any_label = variable_get('views_exposed_filter_any_label', 'new_any') == 'old_any' ? '<Any>' : t('- Any -');
        $form['value']['#options'] = array('All' => $any_label) + $form['value']['#options'];
      }
    }
  }
  function query() {
    $this->ensure_my_table();
    $or = db_or();
    $uid = $this->table_alias . '.' . $this->real_field . ' ';
    $superuser = user_access('view acknowledgements table');
    if ($superuser) {
      if ($this->value[0] == 'All') {
        $this->query->table_queue['sfa']['join']->extra = NULL;
        return;
      }
      $cur_uid = $this->value[0] > 0 ? $this->value : $GLOBALS['user']->uid;
      $this->query->table_queue['sfa']['join']->extra[0]['value'] = $cur_uid;
    }
  }
}
