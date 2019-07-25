<?php

/**
 * @file Shurly Service Views data include file
 */

function shurly_service_views_data() {
  
  // Basic table information.
  $data['shurly_keys']['table']['group']  = t('Shurly Service');

  // Create a join association with the shurly table
  $data['shurly_keys']['table']['join'] = array(
    'shurly' => array(
      'left_field' => 'uid',
      'field'      => 'uid',
      )
  );

  // UID 
  $data['shurly_keys']['uid'] = array(
    'title' => t('User ID'),
    'help' => t('ID of user associated with the Shurly API Key.'),
    'field' => array(
      'handler' => 'views_handler_field_user',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_user_uid',
      'name field' => 'name', // display this field in the summary
    ),
    'filter' => array(
      'title' => t('Name'),
      'handler' => 'views_handler_filter_user_name',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'label' => t('user'),
    ),
  );

  // API Key
  $data['shurly_keys']['apikey'] = array(
    'title' => t('API Key'),
    'help' => t('The API key associated with the user.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  return $data;

}
