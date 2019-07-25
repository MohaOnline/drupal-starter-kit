<?php

/**
 * @file
 * Shurly Views data include file
 */

function shurly_views_data() {

  // Basic table information.

  $data['shurly']['table']['group']  = t('Shurly');

  // Advertise this table as a possible base table
  $data['shurly']['table']['base'] = array(
    'field' => 'rid',
    'title' => t('Short URLs'),
    'help' => t('Shurly shortened URLs.'),
    'weight' => 10,
  );

  $data['shurly']['table']['entity type'] = 'shurly_item';

  //$data['shurly']['table']['join'] = array(
  //  'users' => array(
  //    'left_field' => 'uid',
  //    'field' => 'uid',
  //  ),
  //);

  // note that this next line adds to the $data['users'] array
  // and avoids us from having to do relationships in order to get
  // user data into our Views of Shurly links
  $data['users']['table']['join']['shurly'] = array(
    'left_field' => 'uid',
    'field' => 'uid',
  );

  // User ID
  $data['shurly']['uid'] = array(
    'title' => t('User ID'),
    'help' => t('ID of user who created the link.'),
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

  // uid
  $data['shurly']['uid_current'] = array(
    'real field' => 'uid',
    'title' => t('Current user'),
    'help' => t('Filter the view to the currently logged in user.'),
    'filter' => array(
      'handler' => 'views_handler_filter_user_current',
      'type' => 'yes-no',
    ),
  );

  // Shurly Source - Short URL
  $data['shurly']['source'] = array(
    'title' => t('Short URL'),
    'help' => t('The source URL which generates a redirect'),
    'field' => array(
      'handler' => 'shurly_handler_field_short_url',
      'parent' => 'views_handler_field',
      'click sortable' => TRUE,
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

  // Shurly Desination - Long URL
  $data['shurly']['destination'] = array(
    'title' => t('Long URL'),
    'help' => t('The destination URL'),
    'field' => array(
      //'handler' => 'shurly_handler_field_destination',
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
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

  // Link created timestamp
  $data['shurly']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date/time the link was created.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
  );

  // Count
  $data['shurly']['count'] = array(
    'title' => t('Clicks'),
    'help' => t('Number of times this link has been followed.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),

  );

  // Link last used timestamp
  $data['shurly']['last_used'] = array(
    'title' => t('Last used date'),
    'help' => t('The last date/time the link was used.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
  );

  // Boolean for custom entry
  $data['shurly']['custom'] = array(
    'title' => t('Custom URL'),
    'help' => t('Boolean flag set for custom short URLs entered by user.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Custom'),
      'type' => 'yes-no',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Boolean for active link
  $data['shurly']['active'] = array(
    'title' => t('URL active'),
    'help' => t('Is this link active or has it been deactivated?'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Custom'),
      'type' => 'yes-no',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['shurly']['link_test'] = array(
    'field' => array(
      'title' => t('Test URL'),
      'help' => t('Redirecting link.'),
      'handler' => 'shurly_handler_field_shurly_link_test',
      'parent' => 'views_handler_field',
    ),
  );

  $data['shurly']['link_edit'] = array(
    'field' => array(
      'title' => t('Edit URL'),
      'help' => t('Link to edit the URL'),
      'handler' => 'shurly_handler_field_shurly_link_edit',
      'parent' => 'views_handler_field',
    ),
  );

  $data['shurly']['link_delete'] = array(
    'field' => array(
      'title' => t('Delete URL'),
      'help' => t('Link to delete the URL'),
      'handler' => 'shurly_handler_field_shurly_link_delete',
      'parent' => 'views_handler_field',
    ),
  );

  return $data;
}
