<?php
/**
 * @file
 * survey_builder.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function survey_builder_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'questions';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'survey_question';
  $view->human_name = 'Questions';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Add existing question';
  $handler->display->display_options['css_class'] = 'block-views-questions-block';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['style_options']['row_class'] = 'form-builder-palette-element';
  $handler->display->display_options['style_options']['class'] = 'form-builder-fields';
  $handler->display->display_options['style_options']['wrapper_class'] = 'survey-questions';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Question: Question ID */
  $handler->display->display_options['fields']['pid']['id'] = 'pid';
  $handler->display->display_options['fields']['pid']['table'] = 'survey_question';
  $handler->display->display_options['fields']['pid']['field'] = 'pid';
  /* Sort criterion: Question: Label */
  $handler->display->display_options['sorts']['label']['id'] = 'label';
  $handler->display->display_options['sorts']['label']['table'] = 'survey_question';
  $handler->display->display_options['sorts']['label']['field'] = 'label';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $export['questions'] = $view;

  return $export;
}
