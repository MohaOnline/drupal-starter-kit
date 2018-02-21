<?php

/**
 * Implements hook_views_data().
 */
function opigno_views_data() {
  $data = array();
  $data['opigno_latest_group_activity']['table']['group'] = t('Latest activity');
  $data['opigno_latest_group_activity']['table']['base'] = array(
    'field' => 'nid',
    'title' => t('Latest activity'),
    'help' => t('This table contatins the latest activity.'),
    'weight' => -10,
  );
  $data['opigno_latest_group_activity']['table']['join'] = array(
    'node' => array(
      'handler' => 'views_join',
      'left_table' => 'node',
      'left_field' => 'nid',
      'field' => 'nid',
    ),
    'users' => array(
      'handler' => 'views_join',
      'left_table' => 'users',
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  $data['opigno_latest_group_activity']['nid'] = array(
    'title' => t('Node id'),
    'help' => t('Node id.'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Node Activity'),
    ),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );
  $data['opigno_latest_group_activity']['uid'] = array(
    'title' => t('Who has activity'),
    'help' => t('Who activity of a node'),
    'relationship' => array(
      'base' => 'users',
      'field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('Node Activity User'),
    ),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );
  $data['opigno_latest_group_activity']['timestamp'] = array(
    'title' => t('Timestamp'),
    'help' => t('When user had node activity'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  $data['node']['filter_own_groups'] = array(
    'title' => t('Hide groups that the user is part of'),
    'help' => t('This filter will remove the groups from the view if the user is member'),
    'filter' => array(
      'handler' => 'opigno_filter_own_groups'
    ),
  );

  return $data;
}
