<?php
/**
 * hook for the Views module (if it is used by your site)
 */
/**
 * Implements hook_views_query_alter().
 */ 
function sign_for_acknowledgement_views_query_alter(&$view, &$query) {
  if (user_access('view acknowledgements table')
    &&
    property_exists($view, 'exposed_input') 
    &&
    isset($view->exposed_input['user_id'])
    &&
    $view->exposed_input['user_id'] == 'All'
    &&
    property_exists($query, 'table_queue')
    &&
    isset($query->table_queue['sfa'])
    &&
    isset($query->table_queue['sfa']['join'])
    &&
    property_exists($query->table_queue['sfa']['join'], 'extra')) { 
      $query->table_queue['sfa']['join']->extra = NULL;
      return;
  }

  if (
      property_exists($view->query, 'table_queue') &&
      isset($view->query->table_queue['sfa']) &&
      isset($view->query->table_queue['sfa']['join']) &&
      property_exists($view->query->table_queue['sfa']['join'], 'extra') &&
      isset($view->query->table_queue['sfa']['join']->extra[0]) &&
      isset($view->query->table_queue['sfa']['join']->extra[0]['value'])
      ) {
    $uid = $view->query->table_queue['sfa']['join']->extra[0]['value'];
  } else {
    return;
  }

  $varname = 'nosfa_' . $view->name . '_' . $view->current_display;
  if (variable_get($varname, FALSE)) {
    return;
  }

  if ($uid < 1) {
    $uid = $GLOBALS['user']->uid;
  }
  $usr = user_load($uid);
  if (!$usr) {
    return;
  }
  
  $query->add_table('field_data_enable_roles');
  $query->add_table('field_data_enable_users');
  $condition = db_or();
  foreach ($usr->roles as $role) {
    $condition = $condition->condition('field_data_enable_roles.enable_roles_value', $role, '=');
  }
  $condition = $condition->condition('field_data_enable_users.enable_users_value', $uid, '=');
  $query->add_where(2, $condition);
  $query->distinct = TRUE;
}
/**
 * Get MY contextual filter
 */
function _sign_for_acknowledgement_contextual_filter_uid($view) {
  $result = -1;
  $contextual_filters = $view->get_items('argument');
  $i = 0;
  foreach($contextual_filters as $key => $value) {
    if ($key == 'user_id') {
      $result = $i;
      break;
    }
    $i++;
  }
  return $result;
}
/**
 * Implements hook_views_pre_render().
 */ 
function sign_for_acknowledgement_views_pre_render(&$view) {
  $uid = 0;
  //dsm($view);
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
  $argno = _sign_for_acknowledgement_contextual_filter_uid($view);
  if ($argno > -1 &&  isset($view->args[$argno])) {
    $uid = $view->args[$argno];
  }
  if (!$uid || ($uid != 'All' && !is_numeric($uid))) {
    $uid = $GLOBALS['user']->uid;
  }
  if (!user_access('view acknowledgements table') && $uid != $GLOBALS['user']->uid) {
    $uid = $GLOBALS['user']->uid;
  }
  $is_all = $uid == 'All';
  if (drupal_is_front_page()) {
    return;
  }
  $msg = FALSE;
  foreach ($view->result as $i => &$node) {
    if ($is_all) {
      $uid = 'All';
    }
    if (!isset($node->nid)) {
      continue;
    }
    $nodeobject = node_load($node->nid);
    if ($is_all && property_exists($node, 'sfa_user_id')) {
      $uid = $node->sfa_user_id;
    }
    if ($uid == 'All' || $uid == NULL) {
      continue;
    }
    if (!_sign_for_acknowledgement_applies_to($nodeobject, user_load($uid))) {
      //unset ($view->result[$i]);
      if (property_exists($node, 'sfa_user_id')) {
        $node->sfa_user_id = 'none';
      }      
      continue;
    }
    $timestamp = 0;
    $alternate = '';
    $note = '';
    $is_signed = FALSE;
    if (_sign_for_acknowledgement_already_signed($uid, $node->nid, $timestamp, $alternate, $note)) {
      $is_signed = TRUE;
    }
    $is_alt = FALSE;
    if (isset($nodeobject->alternate_form['und'][0]['value']) && $nodeobject->alternate_form['und'][0]['value'] == 1) {
      $is_alt = TRUE;
    }
    $timestamp = _sign_for_acknowledgement_get_expiration_date(TRUE, $node->nid, $nodeobject);
    $is_expired = ($timestamp && variable_get('sign_for_acknowledgement_block_expired', FALSE) && time() > $timestamp);
    $continue = FALSE;
    if (!empty($node->node_title) && !$is_expired && !$is_signed) {
      $node->node_title .= '***';
    } else {
      $continue = TRUE;
    }
    $html = $final_html = '';
    if ($is_signed) {
      $html = SIGNED_OK;
    } else if ($is_expired) {
      $html = TERMS_EXPIRED;
    } else if ($is_alt) {
      $html = t('alternate form to be compiled');
    }
    if (isset($node->field_data_field_sfa_status_node_entity_type)) {
      if (empty($html)) {
        if (variable_get('sign_for_acknowledgement_checkbox_in_views', FALSE)) {
          $ass_array = array();
          $ass_array['my_checkbox'] = drupal_get_form('sign_for_acknowledgement_form_'.$i, $nodeobject, false, false);
          $final_html = theme('sign_for_acknowledgement_views', array ('content' => '', 'assoc' => $ass_array));
        } else {
          $html = TO_BE_SIGNED;
        }
      }
      if (empty($final_html)) {
        $final_html = theme('sign_for_acknowledgement_views', array ('content' => $html));
      }
      $view->result[$i]->field_field_sfa_status[0]['rendered'][] = array ('#markup' => $final_html, '#access' => '1');
    }
    if ($msg == TRUE || $continue == TRUE) {
      continue;
    }
    $msg = TRUE;
    $varname = 'sfa_' . $view->name . '_' . $view->current_display;
    if (!(variable_get($varname, FALSE))) {
      $view->attachment_before = '<span class="unsigned">' . filter_xss(variable_get('sign_for_acknowledgement_marked', t('The documents marked with *** are to be signed.'))) . '</span>';
    }
  }
}
/**
 * Implements hook_views_data().
 */ 
function sign_for_acknowledgement_views_data() {
  $data = array();
  $data['sfa']['table']['group'] = t('Sign for acknowledgement');
  $data['sfa']['table']['base'] = array(
    'field' => 'hid', // This is the identifier field for the view.
    'table' => 'sign_for_acknowledgement',
    'title' => t('Acknowledgements table'),
    'help' => t('Acknowledgements data.'),
    'weight' => -10,
  );
  $data['sfa']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'node_id',
      'table' => 'sign_for_acknowledgement',
      'extra' => array(
        array(
          'value' => '***CURRENT_USER***',
          'numeric' => TRUE,
          'field' => 'user_id',
        ),
      ),
    ),
  );
  $data['sfa']['hid'] = array(
    'title' => t('Primary key'),
    'help' => t('primary key for acknowledgements.'),
    'table' => 'sign_for_acknowledgement',
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
  );
  $data['sfa']['node_id'] = array(
    'title' => t('Node'),
    'help' => t('Relationship for node.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'label' => t('Node ID'),
      'base' => 'node',
      'base field' => 'nid',
    ),
  );
  $data['sfa']['user_id'] = array(
    'title' => t('User'),
    'help' => t('Acknowledgement user.'),
    'table' => 'sign_for_acknowledgement',
    'field' => array(
      'handler' => 'sign_for_acknowledgement_user_views_handler_field',
    ),
    'argument' => array(
      'handler' => 'sign_for_acknowledgement_user_views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'sign_for_acknowledgement_user_views_handler_filter',
    ),
  );
  $data['sfa']['mydate'] = array(
    'title' => t('Acknowledgement date'),
    'help' => t('Node signed on...'),
    'table' => 'sign_for_acknowledgement',
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'sign_for_acknowledgement_date_views_handler_filter',
    ),
  );

  return $data;
}

function sign_for_acknowledgement_views_ui_display_top_links_alter(&$links, $view, $display_id) {
  if ($view->base_table == 'node') {
    $links['acknowledgements'] = array(
      'title' => strtolower(t('Acknowledgements')),
      'href'  => "admin/structure/views/view/$view->name/$display_id/acknowledgements",
    );
  }
}