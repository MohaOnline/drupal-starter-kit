<?php
/**
 * Implements hook_views_data().
 */
function campaignion_activity_views_data() {

  // ----------------------------------------------------------------
  // -------------------- Campaignion Activity ----------------------
  // ----------------------------------------------------------------
  $data['campaignion_activity']['table']['group'] = t('Activity');

  // Advertise this table as a possible base table
  $data['campaignion_activity']['table']['base'] = array(
    'field'    => 'activity_id',
    'title'    => t('Activity'),
    'defaults' => array(
      'field' => 'contact_id',
    ),
  );

  $data['campaignion_activity']['table']['default_relationship'] = array(
    'redhen_contact' => array(
      'table' => 'redhen_contact',
      'field' => 'contact_id',
    ),
  );

  // ----------------------------------------------------------------
  // fields

  // activity_id
  $data['campaignion_activity']['activity_id'] = array(
    'title' => t('Activity ID'),
    'help'  => t('The ID of this activity.'),
    'field' => array(
      'handler'        => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // contact_id
  $data['campaignion_activity']['contact_id'] = array(
    'title' => t('Contact ID'),
    'help'  => t('The ID of the RedHen contact.'),
    'field' => array(
      'handler'        => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'relationship' => array(
      'title'      => t('RedHen Contact'),
      'help'       => t('The RedHen contact which is connected to this activity.'),
      'base'       => 'redhen_contact',
      'base field' => 'contact_id',
      'handler'    => 'views_handler_relationship',
      'label'      => t('RedHen Contact'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // activity type
  $data['campaignion_activity']['type'] = array(
    'title' => t('Activity Type'),
    'help'  => t('The type of activity.'),
    'field' => array(
      'handler'        => 'views_handler_field',
      'click sortable' => TRUE,
      ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_activity_type',
    ),
  );

  // created field
  $data['campaignion_activity']['created'] = array(
    'title' => t('Activity Date'),
    'help'  => t('The date/time this activity happened.'),
    'field' => array(
      'handler'        => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $data['example_table']['table']['join'] = array(
    
    'campaignion_activity_webform' => array(
      'left_field' => 'activity_id',
      'field'      => 'activity_id',
    ),
  );
  
  // ----------------------------------------------------------------
  // --------------- Campaignion Activity Webform -------------------
  // ----------------------------------------------------------------
  $data['campaignion_activity_webform']['table']['group'] = t('Activity Webform');

  // Advertise this table as a possible base table
  $data['campaignion_activity_webform']['table']['base'] = array(
    'field'    => 'activity_id',
    'title'    => t('Activity Webform'),
    'defaults' => array(
      'field' => 'activity_id',
    ),
  );

  $data['campaignion_activity_webform']['table']['default_relationship'] = array(
    'campaignion_activity' => array(
      'table' => 'campaignion_activity',
      'field' => 'activity_id',
    ),
    'action_node' => array(
      'table' => 'node',
      'field' => 'nid',
    ),
  );

  // ----------------------------------------------------------------
  // fields

  // activity_id
  $data['campaignion_activity_webform']['activity_id'] = array(
    'title' => t('Activity ID'),
    'help'  => t('The ID of the Activity.'),
    'field' => array(
      'handler'        => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'relationship' => array(
      'title'      => t('Activity Base Table'),
      'help'       => t('The activity base table that this activity is connected to.'),
      'base'       => 'campaignion_activity',
      'base field' => 'activity_id',
      'handler'    => 'views_handler_relationship',
      'label'      => t('Activity Base'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // nid
  $data['campaignion_activity_webform']['nid'] = array(
    'title' => t('Node ID'),
    'help'  => t('The node ID of the action for this activity.'),
    'field' => array(
      'handler'        => 'views_handler_field',
      'click sortable' => TRUE,
      ),
    'relationship' => array(
      'title'      => t('Action Node Base Table'),
      'help'       => t('The action node base table that this activity is connected to.'),
      'base'       => 'node',
      'base field' => 'nid',
      'handler'    => 'views_handler_relationship',
      'label'      => t('Action Node'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  // sid
  $data['campaignion_activity_webform']['sid'] = array(
    'title' => t('Submission ID'),
    'help'  => t('The submission ID of this activity'),
    'field' => array(
      'handler'        => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  return $data;
}