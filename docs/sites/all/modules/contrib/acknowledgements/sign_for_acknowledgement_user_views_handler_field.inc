<?php
class sign_for_acknowledgement_user_views_handler_field extends views_handler_field {
  function render($values) {
    $uid = $this->get_value($values);
    if ($uid == NULL) {
      $view = $this->view;
      if (property_exists($view, 'exposed_raw_input') && isset($view->exposed_raw_input['user_id'])) {
        $uid = $view->exposed_raw_input['user_id'];
      } elseif (
          property_exists($view->query, 'table_queue') &&
          isset($view->query->table_queue['sfa']) &&
          isset($view->query->table_queue['sfa']['join']) &&
          property_exists($view->query->table_queue['sfa']['join'], 'extra') &&
          isset($view->query->table_queue['sfa']['join']->extra[0]) &&
          isset($view->query->table_queue['sfa']['join']->extra[0]['value'])
          ) {
        $uid = $view->query->table_queue['sfa']['join']->extra[0]['value'];
      }
    }
    if (!user_access('view acknowledgements table') && is_numeric($uid)) {
      $uid = $GLOBALS['user']->uid;
    } elseif (!(is_numeric($uid))) {
        return '';
    }
    if ($uid == 0) {
      $uid = $GLOBALS['user']->uid;
    }
    $user = user_load($uid);
    return $user->name;
  }
}
