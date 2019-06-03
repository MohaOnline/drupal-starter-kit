<?php

/**
 * @file
 * Default views.
 */

/**
 * Implements hook_views_default_views().
 */
function opigno_views_default_views() {
  $export = array();

  /**
   * My trainings
   */
  $view = new view();
  $view->name = 'my_trainings';
  $view->description = 'Show the user\'s trainings';
  $view->tag = 'og';
  $view->base_table = 'og_membership';
  $view->human_name = 'My trainings';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'My trainings';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    2 => '2',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'You have no trainings.';
  $handler->display->display_options['empty']['area']['format'] = 'html';
  /* Relationship: OG membership: Group Node from OG membership */
  $handler->display->display_options['relationships']['og_membership_related_node_group']['id'] = 'og_membership_related_node_group';
  $handler->display->display_options['relationships']['og_membership_related_node_group']['table'] = 'og_membership';
  $handler->display->display_options['relationships']['og_membership_related_node_group']['field'] = 'og_membership_related_node_group';
  /* Field: Content: Question */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['relationship'] = 'og_membership_related_node_group';
  $handler->display->display_options['fields']['body']['exclude'] = TRUE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['relationship'] = 'og_membership_related_node_group';
  $handler->display->display_options['fields']['created']['exclude'] = TRUE;
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'privatemsg_current_day';
  /* Field: OG membership: State */
  $handler->display->display_options['fields']['state']['id'] = 'state';
  $handler->display->display_options['fields']['state']['table'] = 'og_membership';
  $handler->display->display_options['fields']['state']['field'] = 'state';
  $handler->display->display_options['fields']['state']['exclude'] = TRUE;
  /* Field: Content: Image */
  $handler->display->display_options['fields']['opigno_course_image']['id'] = 'opigno_course_image';
  $handler->display->display_options['fields']['opigno_course_image']['table'] = 'field_data_opigno_course_image';
  $handler->display->display_options['fields']['opigno_course_image']['field'] = 'opigno_course_image';
  $handler->display->display_options['fields']['opigno_course_image']['relationship'] = 'og_membership_related_node_group';
  $handler->display->display_options['fields']['opigno_course_image']['label'] = '';
  $handler->display->display_options['fields']['opigno_course_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['opigno_course_image']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['opigno_course_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['opigno_course_image']['settings'] = array(
    'image_style' => 'course_thumbnail_image',
    'image_link' => '',
  );
  /* Field: Content: Image */
  $handler->display->display_options['fields']['opigno_class_image']['id'] = 'opigno_class_image';
  $handler->display->display_options['fields']['opigno_class_image']['table'] = 'field_data_opigno_class_image';
  $handler->display->display_options['fields']['opigno_class_image']['field'] = 'opigno_class_image';
  $handler->display->display_options['fields']['opigno_class_image']['relationship'] = 'og_membership_related_node_group';
  $handler->display->display_options['fields']['opigno_class_image']['label'] = '';
  $handler->display->display_options['fields']['opigno_class_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['opigno_class_image']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['opigno_class_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['opigno_class_image']['settings'] = array(
    'image_style' => 'course_thumbnail_image',
    'image_link' => '',
  );
  /* Field: Opigno Quiz App: Progression */
  $handler->display->display_options['fields']['course_class_progress']['id'] = 'course_class_progress';
  $handler->display->display_options['fields']['course_class_progress']['table'] = 'opigno_quiz_app_quiz_sort';
  $handler->display->display_options['fields']['course_class_progress']['field'] = 'course_class_progress';
  $handler->display->display_options['fields']['course_class_progress']['relationship'] = 'og_membership_related_node_group';
  $handler->display->display_options['fields']['course_class_progress']['exclude'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'og_membership_related_node_group';
  $handler->display->display_options['fields']['title_1']['label'] = '';
  $handler->display->display_options['fields']['title_1']['element_label_colon'] = FALSE;
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  $handler->display->display_options['sorts']['title']['relationship'] = 'og_membership_related_node_group';
  /* Contextual filter: We pass the user ID as the Entity ID. */
  $handler->display->display_options['arguments']['etid']['id'] = 'etid';
  $handler->display->display_options['arguments']['etid']['table'] = 'og_membership';
  $handler->display->display_options['arguments']['etid']['field'] = 'etid';
  $handler->display->display_options['arguments']['etid']['ui_name'] = 'We pass the user ID as the Entity ID.';
  $handler->display->display_options['arguments']['etid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['etid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['etid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['etid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['etid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['etid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['etid']['validate']['type'] = 'user';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'og_membership_related_node_group';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['relationship'] = 'og_membership_related_node_group';
  $handler->display->display_options['filters']['type']['value'] = array(
    'class' => 'class',
    'course' => 'course',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Course categories (opigno_course_categories) */
  $handler->display->display_options['filters']['opigno_course_categories_tid']['id'] = 'opigno_course_categories_tid';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['table'] = 'field_data_opigno_course_categories';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['field'] = 'opigno_course_categories_tid';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['relationship'] = 'og_membership_related_node_group';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['group'] = 1;
  $handler->display->display_options['filters']['opigno_course_categories_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['opigno_course_categories_tid']['expose']['operator_id'] = 'opigno_course_categories_tid_op';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['expose']['label'] = 'Course category';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['expose']['operator'] = 'opigno_course_categories_tid_op';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['expose']['identifier'] = 'opigno_course_categories_tid';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    5 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['opigno_course_categories_tid']['group_info']['label'] = 'Course categories (opigno_course_categories)';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['group_info']['identifier'] = 'opigno_course_categories_tid';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['group_info']['remember'] = FALSE;
  $handler->display->display_options['filters']['opigno_course_categories_tid']['group_info']['group_items'] = array(
    1 => array(),
    2 => array(),
    3 => array(),
  );
  $handler->display->display_options['filters']['opigno_course_categories_tid']['type'] = 'select';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['vocabulary'] = 'course_categories';

  /* Display: My trainings */
  $handler = $view->new_display('page', 'My trainings', 'my_trainings');
  $handler->display->display_options['display_description'] = 'User\'s trainings';
  $handler->display->display_options['path'] = 'my-trainings';
  $translatables['my_trainings'] = array(
    t('Master'),
    t('My trainings'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('You have no trainings.'),
    t('Group node from OG membership'),
    t('Question'),
    t('Post date'),
    t('State'),
    t('Progression'),
    t('All'),
    t('Course category'),
    t('Course categories (opigno_course_categories)'),
    t('User\'s trainings'),
  );



  $export['my_trainings'] = $view;

  /**
   * Course catalogue
   */

  $view = new view();
  $view->name = 'opigno_course_catalgue';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Training catalogue';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Training catalogue';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['disable_sql_rewrite'] = TRUE;
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['query']['options']['query_comment'] = 'exclude_own_groups';
  $handler->display->display_options['exposed_form']['type'] = 'better_exposed_filters';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['bef'] = array(
    'general' => array(
      'allow_secondary' => 0,
      'secondary_label' => 'Advanced options',
      'collapsible_label' => NULL,
      'combine_rewrite' => NULL,
      'reset_label' => NULL,
      'bef_filter_description' => NULL,
      'any_label' => NULL,
      'filter_rewrite_values' => NULL,
    ),
    'opigno_course_categories_tid' => array(
      'bef_format' => 'default',
      'more_options' => array(
        'bef_select_all_none' => FALSE,
        'bef_collapsible' => 0,
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
            'secondary_label' => NULL,
            'collapsible_label' => NULL,
            'combine_rewrite' => NULL,
            'reset_label' => NULL,
            'bef_filter_description' => NULL,
            'any_label' => NULL,
            'filter_rewrite_values' => NULL,
          ),
          'secondary_label' => NULL,
          'collapsible_label' => NULL,
          'combine_rewrite' => NULL,
          'reset_label' => NULL,
          'bef_filter_description' => NULL,
          'any_label' => NULL,
          'filter_rewrite_values' => NULL,
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
          'secondary_label' => NULL,
          'collapsible_label' => NULL,
          'combine_rewrite' => NULL,
          'reset_label' => NULL,
          'bef_filter_description' => NULL,
          'any_label' => NULL,
        ),
        'secondary_label' => NULL,
        'collapsible_label' => NULL,
        'combine_rewrite' => NULL,
        'reset_label' => NULL,
        'filter_rewrite_values' => NULL,
      ),
      'secondary_label' => NULL,
      'collapsible_label' => NULL,
      'combine_rewrite' => NULL,
      'reset_label' => NULL,
      'bef_filter_description' => NULL,
      'any_label' => NULL,
      'filter_rewrite_values' => NULL,
    ),
    'secondary_label' => NULL,
    'collapsible_label' => NULL,
    'combine_rewrite' => NULL,
    'reset_label' => NULL,
    'bef_filter_description' => NULL,
    'any_label' => NULL,
    'filter_rewrite_values' => NULL,
  );
  $handler->display->display_options['exposed_form']['options']['input_required'] = 0;
  $handler->display->display_options['exposed_form']['options']['text_input_required_format'] = 'html';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '30';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'group_group_1',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['row_plugin'] = 'fields';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'There is no training available.';
  $handler->display->display_options['empty']['area']['format'] = 'html';
  /* Field: Content: Group */
  $handler->display->display_options['fields']['group_group']['id'] = 'group_group';
  $handler->display->display_options['fields']['group_group']['table'] = 'field_data_group_group';
  $handler->display->display_options['fields']['group_group']['field'] = 'group_group';
  $handler->display->display_options['fields']['group_group']['label'] = '';
  $handler->display->display_options['fields']['group_group']['exclude'] = TRUE;
  $handler->display->display_options['fields']['group_group']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['group_group']['type'] = 'og_group_subscribe';
  $handler->display->display_options['fields']['group_group']['settings'] = array(
    'field_name' => '0',
  );
  /* Field: Content: Image */
  $handler->display->display_options['fields']['opigno_course_image']['id'] = 'opigno_course_image';
  $handler->display->display_options['fields']['opigno_course_image']['table'] = 'field_data_opigno_course_image';
  $handler->display->display_options['fields']['opigno_course_image']['field'] = 'opigno_course_image';
  $handler->display->display_options['fields']['opigno_course_image']['label'] = '';
  $handler->display->display_options['fields']['opigno_course_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['opigno_course_image']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['opigno_course_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['opigno_course_image']['settings'] = array(
    'image_style' => 'course_thumbnail_image',
    'image_link' => '',
  );
  /* Field: Content: Image */
  $handler->display->display_options['fields']['opigno_class_image']['id'] = 'opigno_class_image';
  $handler->display->display_options['fields']['opigno_class_image']['table'] = 'field_data_opigno_class_image';
  $handler->display->display_options['fields']['opigno_class_image']['field'] = 'opigno_class_image';
  $handler->display->display_options['fields']['opigno_class_image']['label'] = '';
  $handler->display->display_options['fields']['opigno_class_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['opigno_class_image']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['opigno_class_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['opigno_class_image']['settings'] = array(
    'image_style' => 'course_thumbnail_image',
    'image_link' => '',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Question */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = 'Description';
  $handler->display->display_options['fields']['body']['exclude'] = TRUE;
  $handler->display->display_options['fields']['body']['type'] = 'text_summary_or_trimmed';
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_length' => '600',
  );
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['teachers']['id'] = 'teachers';
  $handler->display->display_options['fields']['teachers']['table'] = 'views';
  $handler->display->display_options['fields']['teachers']['field'] = 'nothing';
  $handler->display->display_options['fields']['teachers']['label'] = 'Teachers';
  $handler->display->display_options['fields']['teachers']['exclude'] = TRUE;
  $handler->display->display_options['fields']['teachers']['hide_empty'] = TRUE;
  /* Field: Content: Hide for anonymous users */
  $handler->display->display_options['fields']['anomymous_visibility']['id'] = 'anomymous_visibility';
  $handler->display->display_options['fields']['anomymous_visibility']['table'] = 'field_data_anomymous_visibility';
  $handler->display->display_options['fields']['anomymous_visibility']['field'] = 'anomymous_visibility';
  $handler->display->display_options['fields']['anomymous_visibility']['label'] = '';
  $handler->display->display_options['fields']['anomymous_visibility']['exclude'] = TRUE;
  $handler->display->display_options['fields']['anomymous_visibility']['element_label_colon'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'class' => 'class',
    'course' => 'course',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Course categories (opigno_course_categories) */
  $handler->display->display_options['filters']['opigno_course_categories_tid']['id'] = 'opigno_course_categories_tid';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['table'] = 'field_data_opigno_course_categories';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['field'] = 'opigno_course_categories_tid';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['group'] = 1;
  $handler->display->display_options['filters']['opigno_course_categories_tid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['opigno_course_categories_tid']['expose']['operator_id'] = 'opigno_course_categories_tid_op';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['expose']['label'] = 'Course category';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['expose']['operator'] = 'opigno_course_categories_tid_op';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['expose']['identifier'] = 'opigno_course_categories_tid';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  $handler->display->display_options['filters']['opigno_course_categories_tid']['type'] = 'select';
  $handler->display->display_options['filters']['opigno_course_categories_tid']['vocabulary'] = 'course_categories';
  /* Filter criterion: Content: Hide for anonymous users (anomymous_visibility) */
  $handler->display->display_options['filters']['anomymous_visibility_value']['id'] = 'anomymous_visibility_value';
  $handler->display->display_options['filters']['anomymous_visibility_value']['table'] = 'field_data_anomymous_visibility';
  $handler->display->display_options['filters']['anomymous_visibility_value']['field'] = 'anomymous_visibility_value';
  $handler->display->display_options['filters']['anomymous_visibility_value']['value'] = array(
    0 => '0',
    1 => '1',
  );
  $handler->display->display_options['filters']['anomymous_visibility_value']['group'] = 1;
  /* Filter criterion: Content: Group visibility (group_access) */
  $handler->display->display_options['filters']['group_access_value']['id'] = 'group_access_value';
  $handler->display->display_options['filters']['group_access_value']['table'] = 'field_data_group_access';
  $handler->display->display_options['filters']['group_access_value']['field'] = 'group_access_value';
  $handler->display->display_options['filters']['group_access_value']['value'] = array(
    0 => '0',
    1 => '1',
  );
  $handler->display->display_options['filters']['group_access_value']['group'] = 1;
  /* Filter criterion: Opigno Quiz App: Hide groups that the user is part of */
  $handler->display->display_options['filters']['filter_own_groups']['id'] = 'filter_own_groups';
  $handler->display->display_options['filters']['filter_own_groups']['table'] = 'node';
  $handler->display->display_options['filters']['filter_own_groups']['field'] = 'filter_own_groups';
  $handler->display->display_options['filters']['filter_own_groups']['group'] = 1;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'training-catalogue';
  $translatables['opigno_course_catalgue'] = array(
    t('Master'),
    t('Training catalogue'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Advanced options'),
    t('Select any filter and click on Apply to see results'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Class referencing the course'),
    t('Description'),
    t('Teachers'),
    t('[title_1] <div class="catalogue-class-group"> [group_group_1] </div>'),
    t('State'),
    t('Course category'),
    t('Page'),
  );



  $export['opigno_course_catalgue'] = $view;

  /**
   * Last active trainings
   */

  $view = new view();
  $view->name = 'active_trainings';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'og_membership';
  $view->human_name = 'Last active trainings';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Latest active trainings';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    2 => '2',
  );
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['pure_distinct'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'better_exposed_filters';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['bef'] = array(
    'general' => array(
      'allow_secondary' => 0,
      'secondary_label' => 'Advanced options',
      'collapsible_label' => NULL,
      'combine_rewrite' => NULL,
      'reset_label' => NULL,
    ),
    'opigno_course_categories_tid' => array(
      'bef_format' => 'default',
      'more_options' => array(
        'bef_select_all_none' => FALSE,
        'bef_collapsible' => 0,
        'is_secondary' => 0,
        'any_label' => '',
        'bef_filter_description' => '',
        'tokens' => array(
          'available' => array(
            0 => 'global_types',
            'secondary_label' => NULL,
            'collapsible_label' => NULL,
            'combine_rewrite' => NULL,
            'reset_label' => NULL,
          ),
          'secondary_label' => NULL,
          'collapsible_label' => NULL,
          'combine_rewrite' => NULL,
          'reset_label' => NULL,
        ),
        'rewrite' => array(
          'filter_rewrite_values' => '',
          'secondary_label' => NULL,
          'collapsible_label' => NULL,
          'combine_rewrite' => NULL,
          'reset_label' => NULL,
        ),
        'secondary_label' => NULL,
        'collapsible_label' => NULL,
        'combine_rewrite' => NULL,
        'reset_label' => NULL,
      ),
      'secondary_label' => NULL,
      'collapsible_label' => NULL,
      'combine_rewrite' => NULL,
      'reset_label' => NULL,
    ),
    'secondary_label' => NULL,
    'collapsible_label' => NULL,
    'combine_rewrite' => NULL,
    'reset_label' => NULL,
  );
  $handler->display->display_options['exposed_form']['options']['input_required'] = 0;
  $handler->display->display_options['exposed_form']['options']['text_input_required_format'] = 'html';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '3';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'title_1',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: OG membership: Group Node from OG membership */
  $handler->display->display_options['relationships']['og_membership_related_node_group']['id'] = 'og_membership_related_node_group';
  $handler->display->display_options['relationships']['og_membership_related_node_group']['table'] = 'og_membership';
  $handler->display->display_options['relationships']['og_membership_related_node_group']['field'] = 'og_membership_related_node_group';
  /* Field: Content: Image */
  $handler->display->display_options['fields']['opigno_course_image']['id'] = 'opigno_course_image';
  $handler->display->display_options['fields']['opigno_course_image']['table'] = 'field_data_opigno_course_image';
  $handler->display->display_options['fields']['opigno_course_image']['field'] = 'opigno_course_image';
  $handler->display->display_options['fields']['opigno_course_image']['relationship'] = 'og_membership_related_node_group';
  $handler->display->display_options['fields']['opigno_course_image']['label'] = '';
  $handler->display->display_options['fields']['opigno_course_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['opigno_course_image']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['opigno_course_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['opigno_course_image']['settings'] = array(
    'image_style' => 'course_thumbnail_image',
    'image_link' => '',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'og_membership_related_node_group';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Content: Question */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['relationship'] = 'og_membership_related_node_group';
  $handler->display->display_options['fields']['body']['label'] = 'Description';
  $handler->display->display_options['fields']['body']['exclude'] = TRUE;
  $handler->display->display_options['fields']['body']['type'] = 'text_summary_or_trimmed';
  $handler->display->display_options['fields']['body']['settings'] = array(
    'trim_length' => '600',
  );
  /* Sort criterion: Latest activity: Timestamp */
  $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['table'] = 'opigno_latest_group_activity';
  $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['relationship'] = 'og_membership_related_node_group';
  $handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
  /* Contextual filter: OG membership: Entity id */
  $handler->display->display_options['arguments']['etid']['id'] = 'etid';
  $handler->display->display_options['arguments']['etid']['table'] = 'og_membership';
  $handler->display->display_options['arguments']['etid']['field'] = 'etid';
  $handler->display->display_options['arguments']['etid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['etid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['etid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['etid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['etid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['etid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['etid']['validate']['type'] = 'user';
  /* Contextual filter: Latest activity: Who has activity */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'opigno_latest_group_activity';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['relationship'] = 'og_membership_related_node_group';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['uid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['uid']['validate']['type'] = 'user';
  /* Filter criterion: OG membership: State */
  $handler->display->display_options['filters']['state']['id'] = 'state';
  $handler->display->display_options['filters']['state']['table'] = 'og_membership';
  $handler->display->display_options['filters']['state']['field'] = 'state';
  $handler->display->display_options['filters']['state']['value'] = array(
    1 => '1',
  );
  /* Filter criterion: OG membership: Entity_type */
  $handler->display->display_options['filters']['entity_type']['id'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['table'] = 'og_membership';
  $handler->display->display_options['filters']['entity_type']['field'] = 'entity_type';
  $handler->display->display_options['filters']['entity_type']['value'] = 'user';
  /* Filter criterion: Latest activity: Timestamp */
  $handler->display->display_options['filters']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['table'] = 'opigno_latest_group_activity';
  $handler->display->display_options['filters']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['filters']['timestamp']['relationship'] = 'og_membership_related_node_group';
  $handler->display->display_options['filters']['timestamp']['operator'] = 'not empty';

  /* Display: Latest active trainings */
  $handler = $view->new_display('block', 'Latest active trainings', 'block_1');
  $translatables['active_trainings'] = array(
    t('Master'),
    t('Latest active trainings'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Advanced options'),
    t('Select any filter and click on Apply to see results'),
    t('Group node from OG membership'),
    t('Description'),
    t('All'),
  );

  $export['active_trainings'] = $view;

  return $export;
}
