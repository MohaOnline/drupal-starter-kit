<?php
/**
 * @file
 * settings file
 */

/**
 * Implements hook_form().
 * delete acknowledgements for one user. 
 */
function sign_for_acknowledgement_delete_form() {

  $form['delete'] = array(
    '#type' => 'fieldset',
    '#title' => t('Delete all acknowledgements for selected user'),
  );

  if (variable_get ('sign_for_acknowledgement_delete_dropdown', FALSE) == FALSE) {
    $form['delete']['user'] = array(
      '#title' => t('Select user'),
      '#type' => 'textfield',
      '#autocomplete_path' => 'acknowledgements/autocomplete',
     );
  } else {
    $form['delete']['user'] = array(
      '#title' => t('Select user'),
      '#type' => 'select',
      '#options' => _sign_for_acknowledgement_users_ack_delete(),
     );
  }

  $form['delete']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array('sign_for_acknowledgement_delete_submit'),
  );

  return $form;  
}

/**
 *
 * return users list
 */
function _sign_for_acknowledgement_users_ack_delete () {
  $users = db_query('SELECT name FROM {users} WHERE uid > 0 AND status = 1 ORDER BY name ASC')->fetchAllKeyed(0, 0);    
  return $users;
}

/**
 * Implements hook_submit
 * for sign_for_acknowledgement_delete_form
 */
function sign_for_acknowledgement_delete_submit($form, $form_state) {
  $username = $form_state['values']['user'];
  $usr = user_load_by_name($username);
  if (!$usr) {
    drupal_set_message(t('User %name does not exist; please retry.', array('%name' => $username)), 'warning');
    return;
  }
  $count = db_delete('sign_for_acknowledgement')
    ->condition('user_id', $usr->uid)
    ->execute();
  drupal_set_message(format_plural($count, '%count acknowledgement has been deleted.', '%count acknowledgements have been deleted.', array('%count' => $count)));
}

/**
 * autocomplete function
 * for sign_for_acknowledgement_delete_form
 */
function _sign_for_acknowledgement_autocomplete($string = '') {
  $matches = array();
  if ($string) {
    $result = db_select('users')->fields('users', array('name'))->condition('name', db_like($string) . '%', 'LIKE')->range(0, 10)->execute();
    foreach ($result as $user) {
      $matches[$user->name] = check_plain($user->name);
    }
  }

  drupal_json_output($matches);
}

/**
 * Implements hook_form().
 * admins acknowledgements for the current node. 
 */
function sign_for_acknowledgement_list_form()
{
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  else {
    return array('#markup' => t('WARNING: absent or incorrect node id...'));
  }
  $node = node_load($nodeid);
  $build['filter'] = _sign_for_acknowledgement_create_filter($node);
  $build['table'] = _sign_for_acknowledgement_create_display_table($node);
  return $build;
}
/**
 * @param timestamp $expire (the expiration date)
 * @param timestamp $signed (the signature date)
 * @return string describing the status of the signature.  
 */    
function _sign_for_acknowledgement_status($expire, $signed)
{
  $now = time();
  if ($signed) {
    if (empty($expire) || $signed < $expire) {
      return SIGNED_OK;
    }
    else {
      return OUT_OF_TERMS;
    }
  }
  else {
    if (empty($expire) || $now < $expire) {
      return TO_BE_SIGNED;
    }
    else {
      return TERMS_EXPIRED;
    }
  }
}
/**
 * Configures fiels array to use in table
 * 
 * @param array $array result of variable_get ('sign_for_acknowledgement_fields')
 * @return new configured array
 */  
function _sign_for_acknowledgement_fields($array) {
  $infos = field_info_instances('user', 'user');
  usort($infos,"_sign_for_acknowledgement_sort_fields");
  $ra = array();
  foreach ($infos as $info) {
    $key = $info['field_name'];
    if (!isset($array[$key])) {
      continue;
    }
    if ($array[$key] === $key) {
      if (!empty($info)) {
        $ra[$key] = $info['label'];
      }
    }
  }
  return $ra;
}
/**
 * Returns right user roles for this node
 * @param object $node
 * @param array roles
 * @return array right roles
 */    
function _sign_for_acknowledgement_right_roles($node, $users_roles) {
  if (isset($node->enable_roles) == FALSE || count($node->enable_roles) == 0) {
    // @todo: if user can modify a node send a warning "no user can sign up, please modify your node and set up user roles"
    return FALSE;
  }
  $my_roles = $node->enable_roles[LANGUAGE_NONE];
  foreach($my_roles as $array) {
    foreach($array as $value) {
      if (!(isset($value))) {
        continue;
      }
      if ($value == t('authenticated user')) {
        return TRUE;
      }
      if (in_array($value, $users_roles)) {
        return TRUE;
      }
    }
  }
  return FALSE;
}
function _sign_for_acknowledgement_right_user($node, $uid) {
  if (isset($node->enable_users[LANGUAGE_NONE][0]['value']) == FALSE) {
    return FALSE;
  }
  foreach($node->enable_users[LANGUAGE_NONE] as $item) {
  	if ($item['value'] == $uid) {
		return TRUE;
	}
  }
  return FALSE;
}
/**
 * Creates session name.
 *
 * @param object $node
 *   the current node
 *
 * @return string session name
 */
function _sign_for_acknowledgement_session_name($node)
{
  return 'acknowledgements_' . $node->nid . '_filter';
}
/**
 * Users filter & form creation.
 *
 * @param int $nodeid
 *   id of the current node
 *
 * @return array data (form)
 */
function _sign_for_acknowledgement_create_filter($node)
{
  $session_name = _sign_for_acknowledgement_session_name($node);
  $session = isset($_SESSION[$session_name]) ? $_SESSION[$session_name] : array();
  $filters = _sign_for_acknowledgement_filters($node);

  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only users where'),
    '#attributes' => array('class' => array('exposed-filters')),
  );

  $form['filters']['node'] = array(
    '#type' => 'value',
    '#name' => 'node',
    '#value' => $node->nid,
  );
  $form['filters']['status'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('clearfix')),
  );
  $form['filters']['status']['filters'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('filters')),
  );
  foreach ($filters as $key => $filter) {
    $value = count($session) && isset($session[$key]) && $session[$key] ? $session[$key] : 'any';
    $form['filters']['status']['filters'][$key] = array(
      '#type' => 'select',
      '#options' => $filter['options'],
      '#title' => $filter['title'],
      '#attributes' => array(
        'title' => $value == 'any' ? t('any') : $value,
      ),
      '#default_value' => $value,
    );
  }
  $form['filters']['status']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('sign_for_acknowledgement_filter_submit'),
  );
  $form['filters']['status']['to_default'] = array(
    '#type' => 'submit',
    '#value' => t('Reset filters'),
    '#submit' => array('sign_for_acknowledgement_filter_todefault'),
  );
  $form['filters']['status']['export'] = array(
    '#type' => 'submit',
    '#value' => t('Export to CSV'),
    '#submit' => array('sign_for_acknowledgement_csv_submit'),
  );

  return $form;
}
/**
 * Implements hook_submit
 */
function sign_for_acknowledgement_filter_submit($form, $form_state)
{
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  else {
    return;
  }
  $node = node_load($nodeid);
  $filters = _sign_for_acknowledgement_filters($node);
  $session_name = _sign_for_acknowledgement_session_name($node);
  foreach ($filters as $filter => $options) {
    if (isset($form_state['values'][$filter]) && $form_state['values'][$filter] != '[any]') {
      // Merge an array of arrays into one if necessary.
      $options = $filters[$filter]['options'];
      // Only accept valid selections offered on the dropdown, block bad input.
      if (isset($options[$form_state['values'][$filter]])) {
        $_SESSION[$session_name][$filter] = $form_state['values'][$filter];
      }
    }
  }
  $form_state['redirect'] = $_SERVER["REQUEST_URI"];
}
function sign_for_acknowledgement_filter_todefault($form, $form_state)
{
  if (arg(0) == 'node' && is_numeric(arg(1))) {
    $nodeid = arg(1);
  }
  else {
    return;
  }
  $node = node_load($nodeid);
  $filters = _sign_for_acknowledgement_filters($node);
  $session_name = _sign_for_acknowledgement_session_name($node);
  foreach ($filters as $filter => $options) {
    $_SESSION[$session_name][$filter] = 'any';
  }
  $form_state['redirect'] = $_SERVER["REQUEST_URI"];
}
/**
 * Prepares data forr output
 * @param object $node the current node
 * @param array $header_cells the header cells
 * @param array $rows the rows to display
 */    
function _sign_for_acknowledgement_outdata($node, $timestamp, &$header_cells, &$rows, $csv = FALSE)
{
  $nodeid = $node->nid;
  $rows_limit = variable_get('sign_for_acknowledgement_limit', 10);
  $session_name = _sign_for_acknowledgement_session_name($node);
  $session = isset($_SESSION[$session_name]) ? $_SESSION[$session_name] : array();
  $custom_fields = _sign_for_acknowledgement_fields(variable_get('sign_for_acknowledgement_fields', array()));
  $do_annotation = (count($node->annotation_field) && $node->annotation_field[LANGUAGE_NONE][0]['value']);
  
  // Username cell...
  $header_cells['username'] = $csv? t('Username') : array('data' => t('Username'), 'field' => 'name');
  // Role cell...
  if (variable_get('sign_for_acknowledgement_show_roles', TRUE)) {
    $header_cells['roles'] = t('Roles');
  }
  // $fields cells
  foreach ($custom_fields as $key => $value) {
    $header_cells[$key] = $csv? $value : array('data' => $value, 'field' => $key);
  }
  // $header_cells['read'] = array('data' => t('Signed'), 'field' => 'f.hid');
  if (
    (count($node->alternate_form) && $node->alternate_form[LANGUAGE_NONE][0]['value']) ||
    (count($node->alternate_form_multiselect) && $node->alternate_form_multiselect[LANGUAGE_NONE][0]['value'])
    ) {
    $header_cells['agreement'] = $csv? t('Agreement') : array('data' => t('Agreement'), 'field' => 'alternate');
    if ($csv && $do_annotation) {
      $header_cells['annotation'] = t('Annotation');
    }
  }
  else if ($do_annotation) {
    $header_cells['annotation'] = $csv? t('Annotation') : array('data' => t('Annotation'), 'field' => 'annotation');
  }
  // Datetime cell...
  $header_cells['date'] = $csv? t('Date') : array('data' => t('Date'), 'field' => 'f.mydate');
  // Expiration
  if ($csv && !(empty($timestamp))) {
    $header_cells['expiration'] = t('Expiration');
  }
  // Status cell...
  $header_cells['status'] = t('Status');
    
  // Building query with abstraction layer.
  $query = db_select('users', 'u')
    ->extend('TableSort')
    ->extend('PagerDefault')
    ->orderByHeader($header_cells)
    ->condition('u.uid', 0, '<>')
    ->condition('u.status', 1)
    ->fields('u', array('uid', 'name'))
    ->fields('f', array('hid', 'alternate'))
    ->fields('f', array('hid', 'annotation'))
    ->fields('f', array('hid', 'mydate'));
  if (count($session) && isset($session['agreement']) && $session['agreement'] != 'any') {
    $query->condition('f.alternate', $session['agreement']);
  }
  if ($rows_limit != -1) {
    $query->limit($rows_limit);
  }
  $count = 0;

  foreach ($custom_fields as $key => $value) {
    $field_table = _sign_for_acknowledgement_table_name($key);
    $t = 't' . $count;
    $x = 'x' . ($count++);
    $referenced = false;
    $query->leftjoin($field_table, $x, 'u.uid = ' . $x . '.entity_id');
    if (db_field_exists($field_table, $key . '_tid')) {
      $referenced = true;
      $query->addField($t, 'name', $key);
      $query->leftjoin('taxonomy_term_data', $t, $t . '.tid = ' . $x . '.' . $key . '_tid');
    } else {
      $query->addField($x, $key . '_value', $key);
    }
    if (count($session) && isset($session[$key]) && $session[$key] != 'any') {
      if ($referenced) {
        $query->condition($t . '.name', $session[$key]);
      } else {
        $query->condition($x . '.' . $key . '_value', $session[$key]);
      }
    }
  }

  $query->leftjoin('sign_for_acknowledgement',
    'f',
    'f.user_id = u.uid AND f.node_id = :nid',
    array(':nid' => $nodeid)
  );

  $query->addField('r1', 'rid');
  $query->leftjoin('users_roles',
    'r1',
    'r1.uid = u.uid'
  );

  $query->addField('r2', 'name');
  $query->leftjoin('role',
    'r2',
    'r1.rid = r2.rid'
  );

  $query->addField('r3', 'entity_id');
  $query->addField('r3', 'enable_roles_value');
  $query->leftjoin('field_data_enable_roles',
    'r3',
    '(r3.enable_roles_value = \'' . t('authenticated user') . '\' OR r2.name = r3.enable_roles_value) AND r3.entity_id = :nid',
    array(':nid' => $nodeid)
  );

  $query->addField('u1', 'entity_id');
  $query->addField('u1', 'enable_users_value');
  $query->leftjoin('field_data_enable_users',
    'u1',
    'u1.enable_users_value = u.uid AND u1.entity_id = :nid',
    array(':nid' => $nodeid)
  );

  $db_or = db_or();
  $db_or->condition('r3.enable_roles_value', NULL, 'IS NOT');
  $db_or->condition('u1.enable_users_value', NULL, 'IS NOT');
  $query->condition($db_or);

  if (count($session) && $session['status'] && $session['status'] != 'any') {
    switch($session['status']) {
      case SIGNED_OK:
        $query->condition('mydate', NULL, 'IS NOT');
        if ($timestamp) {
          $query->condition('mydate', $timestamp, '<');
        }
        break;
      case OUT_OF_TERMS:
        $query->condition('mydate', NULL, 'IS NOT');
        $query->condition('mydate', $timestamp, '>=');
        break;
      case TO_BE_SIGNED:
        $query->condition('mydate', NULL, 'IS');
        if ($timestamp) {
          $query->condition($timestamp, time(), '>');
        }
        break;
      case TERMS_EXPIRED:        
        $query->condition('mydate', NULL, 'IS');
        $query->condition($timestamp, time(), '<=');
        break;
    }
  }

  //echo $query->__toString();
  
  $records = $query->execute()->fetchAll();

  // init array
  $roles = array_map('check_plain', user_roles(TRUE));

  foreach ($records as $record) {
    // Set up user roles.
    $users_roles = array();
    $roles_result = db_query('SELECT rid FROM {users_roles} WHERE uid = :uid', array(':uid' => $record->uid));
    foreach ($roles_result as $user_role) {
      $users_roles[] = $roles[$user_role->rid];
    }
    /*
    if (!(_sign_for_acknowledgement_right_roles($node, $users_roles))
      && !(_sign_for_acknowledgement_right_user($node, $record->uid) ) ) {
      continue;
    }
    */
    $rows[$record->uid] = array();
    $rows[$record->uid]['username'] = $record->name;    
    if (variable_get('sign_for_acknowledgement_show_roles', TRUE)) {
      $rows[$record->uid]['roles'] = implode(', ', $users_roles);
    }
    $count = 0;
    foreach ($custom_fields as $key => $value) {
      $users_fields = array();
      $field_table = _sign_for_acknowledgement_table_name($key);
      if (db_field_exists($field_table, $key . '_tid')) {
        $member = 'name';
        $fields_result = db_query('SELECT v.name FROM '. $field_table .' t JOIN taxonomy_term_data v ON ('.$key . '_tid'.' = v.tid) WHERE entity_id = :uid', array(':uid' => $record->uid));
      }
      else { 
        $member = filter_xss($key . '_value');
        $fields_result = db_query('SELECT '. $member .' FROM '. $field_table .' WHERE entity_id = :uid', array(':uid' => $record->uid));
      }
      foreach ($fields_result as $user_field) {
        $users_fields[] = $user_field->$member;
      }
      $rows[$record->uid][$key] = implode(', ', $users_fields);
    }
    if (
      (isset($node->alternate_form[LANGUAGE_NONE]) && $node->alternate_form[LANGUAGE_NONE][0]['value']) ||
      (isset($node->alternate_form_multiselect[LANGUAGE_NONE]) && $node->alternate_form_multiselect[LANGUAGE_NONE][0]['value'])
      ) {
      $rows[$record->uid]['agreement'] = $record->hid ? filter_xss($record->alternate) : t('---');
      if ($do_annotation && $record->hid && $record->annotation && !$csv) {
        $rows[$record->uid]['agreement'] .= '<br /><em>' . filter_xss($record->annotation) . '</em>'; 
      }
      if ($csv && $do_annotation) {
        $rows[$record->uid]['annotation'] = $record->hid ? filter_xss($record->annotation) : t('---');
      }
    }
    else if ($do_annotation) {
      $rows[$record->uid]['annotation'] = $record->hid ? filter_xss($record->annotation) : t('---');
    }
    $rows[$record->uid]['date'] = $record->hid && $record->mydate ? format_date($record->mydate, 'short') : t('---');
    if ($csv && !(empty($timestamp))) {
      $rows[$record->uid]['expiration'] = format_date($timestamp, 'short');
    }
    $rows[$record->uid]['status'] = _sign_for_acknowledgement_status($timestamp, $record->mydate);
  }
}
/**
 * @param string original string
 * @return string the input string without accents
 */   
function _sign_for_acknowledgement_remove_accents($str)
{
  //$a = array('À', 'Á', 'Â', 'Ã', 'Ä', 'Å', 'Æ', 'Ç', 'È', 'É', 'Ê', 'Ë', 'Ì', 'Í', 'Î', 'Ï', 'Ð', 'Ñ', 'Ò', 'Ó', 'Ô', 'Õ', 'Ö', 'Ø', 'Ù', 'Ú', 'Û', 'Ü', 'Ý', 'ß', 'à', 'á', 'â', 'ã', 'ä', 'å', 'æ', 'ç', chr(0xE8), 'é', 'ê', 'ë', 'ì', 'í', 'î', 'ï', 'ñ', 'ò', 'ó', 'ô', 'õ', 'ö', 'ø', 'ù', 'ú', 'û', 'ü', 'ý', 'ÿ', 'A', 'a', 'A', 'a', 'A', 'a', 'C', 'c', 'C', 'c', 'C', 'c', 'C', 'c', 'D', 'd', 'Ð', 'd', 'E', 'e', 'E', 'e', 'E', 'e', 'E', 'e', 'E', 'e', 'G', 'g', 'G', 'g', 'G', 'g', 'G', 'g', 'H', 'h', 'H', 'h', 'I', 'i', 'I', 'i', 'I', 'i', 'I', 'i', 'I', 'i', '?', '?', 'J', 'j', 'K', 'k', 'L', 'l', 'L', 'l', 'L', 'l', '?', '?', 'L', 'l', 'N', 'n', 'N', 'n', 'N', 'n', '?', 'O', 'o', 'O', 'o', 'O', 'o', 'Œ', 'œ', 'R', 'r', 'R', 'r', 'R', 'r', 'S', 's', 'S', 's', 'S', 's', 'Š', 'š', 'T', 't', 'T', 't', 'T', 't', 'U', 'u', 'U', 'u', 'U', 'u', 'U', 'u', 'U', 'u', 'U', 'u', 'W', 'w', 'Y', 'y', 'Ÿ', 'Z', 'z', 'Z', 'z', 'Ž', 'ž', '?', 'ƒ', 'O', 'o', 'U', 'u', 'A', 'a', 'I', 'i', 'O', 'o', 'U', 'u', 'U', 'u', 'U', 'u', 'U', 'u', 'U', 'u', '?', '?', '?', '?', '?', '?');
  $a = array(chr(0xc0), chr(0xc1), chr(0xc2), chr(0xc3), chr(0xc4), chr(0xc5), chr(0xc6), chr(0xc7), chr(0xc8), chr(0xc9), chr(0xca), chr(0xcb), chr(0xcc), chr(0xcd), chr(0xce), chr(0xcf), chr(0xd0), chr(0xd1), chr(0xd2), chr(0xd3), chr(0xd4), chr(0xd5), chr(0xd6), chr(0xd8), chr(0xd9), chr(0xda), chr(0xdb), chr(0xdc), chr(0xdd), chr(0xdf), chr(0xe0), chr(0xe1), chr(0xe2), chr(0xe3), chr(0xe4), chr(0xe5), chr(0xe6), chr(0xe7), chr(0xe8), chr(0xe9), chr(0xea), chr(0xeb), chr(0xec), chr(0xed), chr(0xee), chr(0xef), chr(0xf1), chr(0xf2), chr(0xf3), chr(0xf4), chr(0xf5), chr(0xf6), chr(0xf8), chr(0xf9), chr(0xfa), chr(0xfb), chr(0xfc), chr(0xfd), chr(0xff), chr(0x41), chr(0x61), chr(0x41), chr(0x61), chr(0x41), chr(0x61), chr(0x43), chr(0x63), chr(0x43), chr(0x63), chr(0x43), chr(0x63), chr(0x43), chr(0x63), chr(0x44), chr(0x64), chr(0xd0), chr(0x64), chr(0x45), chr(0x65), chr(0x45), chr(0x65), chr(0x45), chr(0x65), chr(0x45), chr(0x65), chr(0x45), chr(0x65), chr(0x47), chr(0x67), chr(0x47), chr(0x67), chr(0x47), chr(0x67), chr(0x47), chr(0x67), chr(0x48), chr(0x68), chr(0x48), chr(0x68), chr(0x49), chr(0x69), chr(0x49), chr(0x69), chr(0x49), chr(0x69), chr(0x49), chr(0x69), chr(0x49), chr(0x69), chr(0x3f), chr(0x3f), chr(0x4a), chr(0x6a), chr(0x4b), chr(0x6b), chr(0x4c), chr(0x6c), chr(0x4c), chr(0x6c), chr(0x4c), chr(0x6c), chr(0x3f), chr(0x3f), chr(0x4c), chr(0x6c), chr(0x4e), chr(0x6e), chr(0x4e), chr(0x6e), chr(0x4e), chr(0x6e), chr(0x3f), chr(0x4f), chr(0x6f), chr(0x4f), chr(0x6f), chr(0x4f), chr(0x6f), chr(0xa6), chr(0xb6), chr(0x52), chr(0x72), chr(0x52), chr(0x72), chr(0x52), chr(0x72), chr(0x53), chr(0x73), chr(0x53), chr(0x73), chr(0x53), chr(0x73), chr(0xa9), chr(0xb9), chr(0x54), chr(0x74), chr(0x54), chr(0x74), chr(0x54), chr(0x74), chr(0x55), chr(0x75), chr(0x55), chr(0x75), chr(0x55), chr(0x75), chr(0x55), chr(0x75), chr(0x55), chr(0x75), chr(0x55), chr(0x75), chr(0x57), chr(0x77), chr(0x59), chr(0x79), chr(0xbc), chr(0x5a), chr(0x7a), chr(0x5a), chr(0x7a), chr(0xae), chr(0xbe), chr(0x3f), chr(0x83), chr(0x4f), chr(0x6f), chr(0x55), chr(0x75), chr(0x41), chr(0x61), chr(0x49), chr(0x69), chr(0x4f), chr(0x6f), chr(0x55), chr(0x75), chr(0x55), chr(0x75), chr(0x55), chr(0x75), chr(0x55), chr(0x75), chr(0x55), chr(0x75), chr(0x3f), chr(0x3f), chr(0x3f), chr(0x3f), chr(0x3f), chr(0x3f));
  $b = array('A', 'A', 'A', 'A', 'A', 'A', 'AE', 'C', 'E', 'E', 'E', 'E', 'I', 'I', 'I', 'I', 'D', 'N', 'O', 'O', 'O', 'O', 'O', 'O', 'U', 'U', 'U', 'U', 'Y', 's', 'a', 'a', 'a', 'a', 'a', 'a', 'ae', 'c', 'e', 'e', 'e', 'e', 'i', 'i', 'i', 'i', 'n', 'o', 'o', 'o', 'o', 'o', 'o', 'u', 'u', 'u', 'u', 'y', 'y', 'A', 'a', 'A', 'a', 'A', 'a', 'C', 'c', 'C', 'c', 'C', 'c', 'C', 'c', 'D', 'd', 'D', 'd', 'E', 'e', 'E', 'e', 'E', 'e', 'E', 'e', 'E', 'e', 'G', 'g', 'G', 'g', 'G', 'g', 'G', 'g', 'H', 'h', 'H', 'h', 'I', 'i', 'I', 'i', 'I', 'i', 'I', 'i', 'I', 'i', 'IJ', 'ij', 'J', 'j', 'K', 'k', 'L', 'l', 'L', 'l', 'L', 'l', 'L', 'l', 'l', 'l', 'N', 'n', 'N', 'n', 'N', 'n', 'n', 'O', 'o', 'O', 'o', 'O', 'o', 'OE', 'oe', 'R', 'r', 'R', 'r', 'R', 'r', 'S', 's', 'S', 's', 'S', 's', 'S', 's', 'T', 't', 'T', 't', 'T', 't', 'U', 'u', 'U', 'u', 'U', 'u', 'U', 'u', 'U', 'u', 'U', 'u', 'W', 'w', 'Y', 'y', 'Y', 'Z', 'z', 'Z', 'z', 'Z', 'z', 's', 'f', 'O', 'o', 'U', 'u', 'A', 'a', 'I', 'i', 'O', 'o', 'U', 'u', 'U', 'u', 'U', 'u', 'U', 'u', 'U', 'u', 'A', 'a', 'AE', 'ae', 'O', 'o');
  return str_replace($a, $b, $str);
}
/**
 * @param string original string
 * @return string the input string converted to filename
 */   
function _sign_for_acknowledgement_convert_to_filename($string) {
  $string = utf8_decode(strtolower($string));
  $string = _sign_for_acknowledgement_remove_accents($string);
  $string = str_replace  (" ", "_", $string);
  $string = str_replace  ("..", ".", $string);
  preg_replace  ("/[^0-9^a-z^_^.]/", "", $string);
  return $string;
}
/**
 * Implements hook_submit
 */
function sign_for_acknowledgement_csv_submit($form, $form_state)
{
  $nodeid = $form_state['values']['node'];
  $node = node_load($nodeid);
  $timestamp = _sign_for_acknowledgement_get_expiration_date(TRUE, $nodeid, $node);
  $header_cells = array();
  $rows = array();

  _sign_for_acknowledgement_outdata($node, $timestamp, $header_cells, $rows, TRUE);

  $filename = $node->title . '.csv';
  $filename = _sign_for_acknowledgement_convert_to_filename($filename);

  header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
  header('Content-Description: File Transfer');
  header("Content-type: text/csv");
  header("Content-Disposition: attachment; filename={$filename}");
  header("Expires: 0");
  header("Pragma: public");

  $separator = filter_xss(variable_get('sign_for_acknowledgement_separator', ';'));
  $fh = @fopen('php://output', 'w');
  $firstrow = trim(theme('sign_for_acknowledgement_pre_csv', array('node' => $node)));
  if (!(empty($firstrow))) {
    fwrite($fh, $firstrow);
    for ($i = 0; $i < count($header_cells) - 1; $i++) {
      fwrite($fh, $separator);
    }
    fwrite($fh, "\r\n");
  }
  fputcsv($fh, $header_cells, $separator);
  foreach ($rows as $row) {
    // Put the data into the stream
    fputcsv($fh, $row, $separator);
  }
  // Close the file
  fclose($fh);
  // Make sure nothing else is sent, our file is done
  exit;
}
/**
 * Users table & form creation.
 *
 * @param int $nodeid
 *   id of the current node
 *
 * @return array data (form)
 */
function _sign_for_acknowledgement_create_display_table($node)
{
  // Get expiration datetime.
  $date = NULL;
  $timestamp = _sign_for_acknowledgement_get_expiration_date(TRUE, $node->nid, $node);
  if ($timestamp) {
    $dateobject = new DateObject($timestamp);
    $date = date_format_date($dateobject, 'long');
  }
  $header_cells = array();
  $rows = array();

  _sign_for_acknowledgement_outdata($node, $timestamp, $header_cells, $rows);

  $form = array();
  $form['date'] = array(
    '#type' => 'markup',
    '#markup' => theme('sign_for_acknowledgement_pre_table', array('node' => $node, 'expire' => $date)),
  );
  $form['myselector'] = array (
    '#type' => 'tableselect',
    '#header' => $header_cells,
    '#options' => $rows,
    '#attributes' => array(),
    '#empty' => t('No content available.'),
  );
  $form['node'] = array(
    '#type' => 'value',
    '#name' => 'nodeid',
    '#value' => $node->nid,
  );
  $timestamp = _sign_for_acknowledgement_get_expiration_date(TRUE, $node->nid, $node);
  $is_expired = ($timestamp && variable_get('sign_for_acknowledgement_block_expired', FALSE) && time() > $timestamp);
  if (!$is_expired && user_access('delete acknowledgements')) {
    $form['unsign_submit'] = array(
      '#type' => 'submit',
      '#value' => t('remove acknowledgement'),
      '#submit' => array('sign_for_acknowledgement_table_form_submit'),
      '#prefix' => '<div style="float:left">' . t('...if selected ') . '&nbsp;</div>',
    );
    if (
      !(count($node->alternate_form) && $node->alternate_form[LANGUAGE_NONE][0]['value']) &&
      !(count($node->alternate_form_multiselect) && $node->alternate_form_multiselect[LANGUAGE_NONE][0]['value'])
      ) 
    {
      $form['sign_submit'] = array(
        '#type' => 'submit',
        '#value' => t('add acknowledgement'),
        '#submit' => array('sign_for_acknowledgement_table_sign_form_submit'),
      );
    }
  }
  $form['pager'] = array('#markup' => theme('pager'));
  return ($form);
}
/**
 * Sort user custom fields.
 */ 
function _sign_for_acknowledgement_sort_fields($a,$b){
    return $a['widget']['weight']-$b['widget']['weight'];
}
/**
 * Settings form: presents the sign_for_acknowledgement settings page.
 */
function sign_for_acknowledgement_admin_settings_form() {
  $my_nodes = array();
  node_types_rebuild();
  $node_types = node_type_get_types();
  foreach ($node_types as $node_type) {
    $my_nodes[$node_type->type] = $node_type->type;
  }

  $form['sign_for_acknowledgement_content1'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node types'),
  );
  $form['sign_for_acknowledgement_content1']['sign_for_acknowledgement_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node types to be handled by the module'),
    '#default_value' => variable_get('sign_for_acknowledgement_node_types', array('article' => 'article')),
    '#options' => $my_nodes,
    '#multiple' => TRUE,
  );
  $form['sign_for_acknowledgement_content2'] = array(
    '#type' => 'fieldset',
    '#title' => t('Messages'),
  );
  $form['sign_for_acknowledgement_content2']['sign_for_acknowledgement_warning'] = array(
    '#type' => 'textfield',
    '#title' => t('"Remember to sign" warning'),
    '#default_value' => variable_get('sign_for_acknowledgement_warning', t('<strong>Please, remember to sign for acknowledgement.</strong>')),
  );
  $form['sign_for_acknowledgement_content2']['sign_for_acknowledgement_data_sent'] = array(
    '#type' => 'textfield',
    '#title' => t('"Data sent" message'),
    '#default_value' => variable_get('sign_for_acknowledgement_data_sent', t('Document has been signed.')),
  );
  $form['sign_for_acknowledgement_content2']['sign_for_acknowledgement_do_sign'] = array(
    '#type' => 'textfield',
    '#title' => t('"Sign for acknowledgement" message'),
    '#default_value' => variable_get('sign_for_acknowledgement_do_sign', t('Click here to confirm you have read the document')),
  );
  $form['sign_for_acknowledgement_content2']['sign_for_acknowledgement_is_signed'] = array(
    '#type' => 'textfield',
    '#title' => t('"Document already signed" message'),
    '#default_value' => variable_get('sign_for_acknowledgement_is_signed', t('You have already signed this document')),
  );
  $form['sign_for_acknowledgement_content2']['sign_for_acknowledgement_is_expired'] = array(
    '#type' => 'textfield',
    '#title' => t('"Time is expired" message'),
    '#default_value' => variable_get('sign_for_acknowledgement_is_expired', t('Can\'t sign for acknowledgement, deadline closed.')),
  );
  $form['sign_for_acknowledgement_content2']['sign_for_acknowledgement_marked'] = array(
    '#type' => 'textfield',
    '#title' => t('"Documents to be signed" message'),
    '#default_value' => variable_get('sign_for_acknowledgement_marked', t('The documents marked with *** are to be signed by you.')),
  );
  $form['sign_for_acknowledgement_content2']['sign_for_acknowledgement_show_nobody'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show message when a node has no acknowledgement'),
    '#defaul_value' => variable_get('sign_for_acknowledgement_show_nobody', FALSE),
    '#attributes' => variable_get('sign_for_acknowledgement_show_nobody', FALSE) ? array('checked' => 'checked') : NULL,
  );
  $form['sign_for_acknowledgement_content3'] = array(
    '#type' => 'fieldset',
    '#title' => t('User & Roles'),
  );
  $infos = field_info_instances('user', 'user');
  usort($infos,"_sign_for_acknowledgement_sort_fields");
  $my_fields = array();
  $my_fields[''] = t('No field');
  foreach ($infos as $info) {
    $my_fields[$info['field_name']] = $info['label'];
  }
  $form['sign_for_acknowledgement_content3']['sign_for_acknowledgement_groups_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('User roles to support by default'),
    '#description' => t('These values will be used as default values while creating a new node...'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['sign_for_acknowledgement_content3']['sign_for_acknowledgement_groups_fieldset']['sign_for_acknowledgement_roles'] = array(
    '#type' => 'checkboxes',
    '#default_value' => variable_get('sign_for_acknowledgement_roles', _sign_for_acknowledgement_get_roles(FALSE)),
    '#options' => _sign_for_acknowledgement_get_roles(TRUE),
    '#multiple' => TRUE,
  );
  $form['sign_for_acknowledgement_content3']['sign_for_acknowledgement_use_default_roles'] =  array(
    '#type' => 'checkbox',
    '#title' => t('Use default roles only?'),
    '#defaul_value' => variable_get('sign_for_acknowledgement_use_default_roles', FALSE),
    '#attributes' => variable_get('sign_for_acknowledgement_use_default_roles', FALSE) ? array('checked' => 'checked') : NULL,
  );
  $form['sign_for_acknowledgement_content3']['sign_for_acknowledgement_use_single_user'] =  array(
    '#type' => 'checkbox',
    '#title' => t('Show single user insertion?'),
    '#defaul_value' => variable_get('sign_for_acknowledgement_use_single_user', TRUE),
    '#attributes' => variable_get('sign_for_acknowledgement_use_single_user', TRUE) ? array('checked' => 'checked') : NULL,
  );
  $form['sign_for_acknowledgement_content3']['sign_for_acknowledgement_show_roles'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show roles in the table?'),
    '#defaul_value' => variable_get('sign_for_acknowledgement_show_roles', TRUE),
    '#attributes' => variable_get('sign_for_acknowledgement_show_roles', TRUE) ? array('checked' => 'checked') : NULL,
  );
  $form['sign_for_acknowledgement_content4'] = array(
    '#type' => 'fieldset',
    '#title' => t('Layout'),
  );
  $form['sign_for_acknowledgement_content4']['sign_for_acknowledgement_limit'] = array(
    '#type' => 'select',
    '#title' => t('Number of rows in the table'),
    '#default_value' => variable_get('sign_for_acknowledgement_limit', 10),
    '#options' => array(
      5 => 5,
      10 => 10,
      50 => 50,
      100 => 100,
      500 => 500,
      1000 => 1000,
      -1 => t('All'),
    ),
  );
  $form['sign_for_acknowledgement_content4']['sign_for_acknowledgement_embedded'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fields to be shown in the table'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  array_shift($my_fields);
  $form['sign_for_acknowledgement_content4']['sign_for_acknowledgement_embedded']['sign_for_acknowledgement_fields'] = array(
    '#type' => 'checkboxes',
    '#default_value' => variable_get('sign_for_acknowledgement_fields', array()),
    '#options' => $my_fields,
    '#multiple' => TRUE,
  );
  $form['sign_for_acknowledgement_content4']['sign_for_acknowledgement_weight'] = array(
    '#type' => 'textfield',
    '#title' => t('Checkbox and text weight'),
    '#size' => 4,
    '#element_validate' => array('element_validate_number'),
    '#default_value' => variable_get('sign_for_acknowledgement_weight', 1),
  );
  $form['sign_for_acknowledgement_content4']['sign_for_acknowledgement_separator'] = array(
    '#type' => 'textfield',
    '#title' => t('Separator for csv cells'),
    '#size' => 4,
    '#maxlength' => 1,
    '#default_value' => variable_get('sign_for_acknowledgement_separator', ';'),
  );
  $form['sign_for_acknowledgement_content4']['sign_for_acknowledgement_show_submit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the submit button?'),
    '#defaul_value' => variable_get('sign_for_acknowledgement_show_submit', FALSE),
    '#attributes' => variable_get('sign_for_acknowledgement_show_submit', FALSE) ? array('checked' => 'checked') : NULL,
  );
  $form['sign_for_acknowledgement_content4']['sign_for_acknowledgement_block_expired'] = array(
    '#type' => 'checkbox',
    '#title' => t('Prevent the signature if terms have expired.'),
    '#defaul_value' => variable_get('sign_for_acknowledgement_block_expired', FALSE),
    '#attributes' => variable_get('sign_for_acknowledgement_block_expired', FALSE) ? array('checked' => 'checked') : NULL,
  );
  $form['sign_for_acknowledgement_content4']['sign_for_acknowledgement_checkbox_in_views'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show signature checkbox in views list of nodes.'),
    '#defaul_value' => variable_get('sign_for_acknowledgement_checkbox_in_views', FALSE),
    '#attributes' => variable_get('sign_for_acknowledgement_checkbox_in_views', FALSE) ? array('checked' => 'checked') : NULL,
  );
  $form['sign_for_acknowledgement_content4']['sign_for_acknowledgement_delete_dropdown'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use dropdown for user selection in "Acknowledgements deletion".'),
    '#defaul_value' => variable_get('sign_for_acknowledgement_delete_dropdown', FALSE),
    '#attributes' => variable_get('sign_for_acknowledgement_delete_dropdown', FALSE) ? array('checked' => 'checked') : NULL,
  );
  $form['sign_for_acknowledgement_content5'] = array(
    '#type' => 'fieldset',
    '#title' => t('alternate form'),
  );
  $form['sign_for_acknowledgement_content5']['sign_for_acknowledgement_alternate_request'] = array(
    '#type' => 'textarea',
    '#title' => t('Please insert your radio or checkbox buttons labels, one per line.'),
    '#default_value' => variable_get('sign_for_acknowledgement_alternate_request', "I don't agree\nI agree"),
  );
  $form['sign_for_acknowledgement_content6'] = array(
    '#type' => 'fieldset',
    '#title' => t('notification email'),
  );
  $form['sign_for_acknowledgement_content6']['sign_for_acknowledgement_email_to_roles'] = array(
    '#type' => 'checkbox',
    '#title' => t('Permit notification email to selected roles.'),
    '#defaul_value' => variable_get('sign_for_acknowledgement_email_to_roles', TRUE),
    '#attributes' => variable_get('sign_for_acknowledgement_email_to_roles', TRUE) ? array('checked' => 'checked') : NULL,
  );
  $form['sign_for_acknowledgement_content6']['sign_for_acknowledgement_email_to_users'] = array(
    '#type' => 'checkbox',
    '#title' => t('Permit notification email to selected users.'),
    '#defaul_value' => variable_get('sign_for_acknowledgement_email_to_users', TRUE),
    '#attributes' => variable_get('sign_for_acknowledgement_email_to_users', TRUE) ? array('checked' => 'checked') : NULL,
  );
  $form['sign_for_acknowledgement_content6']['sign_for_acknowledgement_email_to'] = array(
    '#type' => 'select',
    '#title' => t('Visible email receiver'),
    '#default_value' => variable_get('sign_for_acknowledgement_email_to', 0),
    '#options' => array(
      0 => 'Undisclosed recipients',
      1 => variable_get('site_mail', 'generic@nowhere.com'),
    ),
  );
  $form['sign_for_acknowledgement_content6']['sign_for_acknowledgement_email_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Please insert your email subject using tokens for node and site data.'),
    '#default_value' => variable_get('sign_for_acknowledgement_email_subject', 'Signature notification from [site:name]'),
  );
  $email_body = 
'On [node:created:short], a new [node:type] titled "[node:title]" was published.
Please, remember to sign for acknowledgement before [node:expire-date:long].
Page is available here [site:url]node/[node:nid]

Best regards,
[site:name]
';
  $form['sign_for_acknowledgement_content6']['sign_for_acknowledgement_email_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Please insert your email body using tokens for node and site data.'),
    '#default_value' => variable_get('sign_for_acknowledgement_email_body', $email_body),
  );
  
  $form['sign_for_acknowledgement_content7'] = array(
    '#type' => 'fieldset',
    '#title' => t('Notification email no signature'),
  );
  $form['sign_for_acknowledgement_content7']['sign_for_acknowledgement_email_subject_nosign'] = array(
    '#type' => 'textfield',
    '#title' => t('Please insert your email subject using tokens for node and site data.'),
    '#default_value' => variable_get('sign_for_acknowledgement_email_subject_nosign', 'Notification from [site:name]'),
  );
  $email_body = 
'On [node:created:short], a new [node:type] titled "[node:title]" was published.
Please, remember to read it.
Page is available here [site:url]node/[node:nid]

Best regards,
[site:name]
';
  $form['sign_for_acknowledgement_content7']['sign_for_acknowledgement_email_body_nosign'] = array(
    '#type' => 'textarea',
    '#title' => t('Please insert your email body using tokens for node and site data.'),
    '#default_value' => variable_get('sign_for_acknowledgement_email_body_nosign', $email_body),
  );

  $form['#submit'][] = '_sign_for_acknowledgement_reset_node_fields';
  //$form['#submit'][] = '_sign_for_acknowledgement_reset_update';
  return system_settings_form($form);
}
