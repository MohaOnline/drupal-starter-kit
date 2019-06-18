<?php
/**
 * @file
 * survey_builder_ui.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function survey_builder_ui_views_default_views() {
  $export = array();

  $view = new view;
  $view->name = 'survey_responses';
  $view->description = 'List of survey responses';
  $view->tag = 'default';
  $view->base_table = 'survey_response';
  $view->human_name = 'Survey Responses';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Survey Responses';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'name' => 'name',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Field: Survey: Label */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'survey';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Survey';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'User';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['name']['link_to_user'] = 0;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  $handler->display->display_options['fields']['name']['format_username'] = 1;
  /* Field: Survey Response: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'survey_response';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['created']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['created']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['created']['alter']['external'] = 0;
  $handler->display->display_options['fields']['created']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['created']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['created']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['created']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['created']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['created']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['created']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['created']['alter']['html'] = 0;
  $handler->display->display_options['fields']['created']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['created']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['created']['hide_empty'] = 0;
  $handler->display->display_options['fields']['created']['empty_zero'] = 0;
  $handler->display->display_options['fields']['created']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['created']['date_format'] = 'medium';
  /* Field: Survey Response: Link */
  $handler->display->display_options['fields']['link']['id'] = 'link';
  $handler->display->display_options['fields']['link']['table'] = 'survey_response';
  $handler->display->display_options['fields']['link']['field'] = 'link';
  $handler->display->display_options['fields']['link']['label'] = 'View Response';
  $handler->display->display_options['fields']['link']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['link']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['link']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['link']['alter']['external'] = 0;
  $handler->display->display_options['fields']['link']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['link']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['link']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['link']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['link']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['link']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['link']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['link']['alter']['html'] = 0;
  $handler->display->display_options['fields']['link']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['link']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['link']['hide_empty'] = 0;
  $handler->display->display_options['fields']['link']['empty_zero'] = 0;
  $handler->display->display_options['fields']['link']['hide_alter_empty'] = 1;
  /* Contextual filter: Survey Response: Survey_id */
  $handler->display->display_options['arguments']['survey_id']['id'] = 'survey_id';
  $handler->display->display_options['arguments']['survey_id']['table'] = 'survey_response';
  $handler->display->display_options['arguments']['survey_id']['field'] = 'survey_id';
  $handler->display->display_options['arguments']['survey_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['survey_id']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['survey_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['survey_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['survey_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['survey_id']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['survey_id']['not'] = 0;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'survey/%/responses';
  $export['survey_responses'] = $view;

  $view = new view;
  $view->name = 'surveys';
  $view->description = 'List of surveys';
  $view->tag = 'default';
  $view->base_table = 'survey';
  $view->human_name = 'Surveys';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Surveys';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'survey create';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = 1;
  $handler->display->display_options['exposed_form']['options']['autosubmit_hide'] = 1;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = 'title';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['summary'] = 'List of surveys';
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Field: Survey: Link */
  $handler->display->display_options['fields']['link']['id'] = 'link';
  $handler->display->display_options['fields']['link']['table'] = 'survey';
  $handler->display->display_options['fields']['link']['field'] = 'link';
  $handler->display->display_options['fields']['link']['label'] = 'Survey';
  $handler->display->display_options['fields']['link']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['link']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['link']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['link']['alter']['external'] = 0;
  $handler->display->display_options['fields']['link']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['link']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['link']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['link']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['link']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['link']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['link']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['link']['alter']['html'] = 0;
  $handler->display->display_options['fields']['link']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['link']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['link']['hide_empty'] = 0;
  $handler->display->display_options['fields']['link']['empty_zero'] = 0;
  $handler->display->display_options['fields']['link']['hide_alter_empty'] = 1;
  /* Field: Survey: Edit Link */
  $handler->display->display_options['fields']['edit_link']['id'] = 'edit_link';
  $handler->display->display_options['fields']['edit_link']['table'] = 'survey';
  $handler->display->display_options['fields']['edit_link']['field'] = 'edit_link';
  $handler->display->display_options['fields']['edit_link']['label'] = 'Edit Survey';
  $handler->display->display_options['fields']['edit_link']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['edit_link']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['edit_link']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['edit_link']['alter']['external'] = 0;
  $handler->display->display_options['fields']['edit_link']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['edit_link']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['edit_link']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['edit_link']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['edit_link']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['edit_link']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['edit_link']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['edit_link']['alter']['html'] = 0;
  $handler->display->display_options['fields']['edit_link']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['edit_link']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['edit_link']['hide_empty'] = 0;
  $handler->display->display_options['fields']['edit_link']['empty_zero'] = 0;
  $handler->display->display_options['fields']['edit_link']['hide_alter_empty'] = 1;
  /* Field: Survey: Responses Link */
  $handler->display->display_options['fields']['responses_Link']['id'] = 'responses_Link';
  $handler->display->display_options['fields']['responses_Link']['table'] = 'survey';
  $handler->display->display_options['fields']['responses_Link']['field'] = 'responses_Link';
  $handler->display->display_options['fields']['responses_Link']['label'] = 'View Responses';
  $handler->display->display_options['fields']['responses_Link']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['responses_Link']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['responses_Link']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['responses_Link']['alter']['external'] = 0;
  $handler->display->display_options['fields']['responses_Link']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['responses_Link']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['responses_Link']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['responses_Link']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['responses_Link']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['responses_Link']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['responses_Link']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['responses_Link']['alter']['html'] = 0;
  $handler->display->display_options['fields']['responses_Link']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['responses_Link']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['responses_Link']['hide_empty'] = 0;
  $handler->display->display_options['fields']['responses_Link']['empty_zero'] = 0;
  $handler->display->display_options['fields']['responses_Link']['hide_alter_empty'] = 1;
  /* Field: Survey: Delete Link */
  $handler->display->display_options['fields']['delete_link']['id'] = 'delete_link';
  $handler->display->display_options['fields']['delete_link']['table'] = 'survey';
  $handler->display->display_options['fields']['delete_link']['field'] = 'delete_link';
  $handler->display->display_options['fields']['delete_link']['label'] = 'Delete Survey';
  $handler->display->display_options['fields']['delete_link']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['delete_link']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['delete_link']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['delete_link']['alter']['external'] = 0;
  $handler->display->display_options['fields']['delete_link']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['delete_link']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['delete_link']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['delete_link']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['delete_link']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['delete_link']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['delete_link']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['delete_link']['alter']['html'] = 0;
  $handler->display->display_options['fields']['delete_link']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['delete_link']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['delete_link']['hide_empty'] = 0;
  $handler->display->display_options['fields']['delete_link']['empty_zero'] = 0;
  $handler->display->display_options['fields']['delete_link']['hide_alter_empty'] = 1;
  /* Filter criterion: Survey: Label */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'survey';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Name';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['required'] = 0;
  $handler->display->display_options['filters']['title']['expose']['multiple'] = FALSE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['exposed_block'] = TRUE;
  $handler->display->display_options['path'] = 'surveys';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Surveys';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $export['surveys'] = $view;

  return $export;
}
