<?php

/**
 * @file
 * Definition of views_handler_argument_user_uid.
 */

/**
 * Argument handler to accept a user id.
 *
 * @ingroup views_argument_handlers
 */
class sign_for_acknowledgement_user_views_handler_argument extends views_handler_argument_numeric {
  /**
   * Override the behavior of title(). Get the name of the user.
   *
   * @return array
   *    A list of usernames.
   */
  function title_query() {
    if (!$this->argument) {
      return array(variable_get('anonymous', t('Anonymous')));
    }

    $titles = array();

    $result = db_query("SELECT u.name FROM {users} u WHERE u.uid IN (:uids)", array(':uids' => $this->value));
    foreach ($result as $term) {
      $titles[] = check_plain($term->name);
    }
    return $titles;
  }
  /**
   * Set up the query for this argument.
   *
   * The argument sent may be found at $this->argument.
   */
  function query($group_by = FALSE) {
    $this->ensure_my_table();
    if (user_access('view acknowledgements table')) {
      $this->query->table_queue['sfa']['join']->extra[0]['value'] = $this->argument;
    }
  }
}
