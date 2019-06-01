<?php
/**
 * @file
 * wetkit_users.panelizer.inc
 */

/**
 * Implements hook_panelizer_defaults().
 */
function wetkit_users_panelizer_defaults() {
  $export = array();

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->title = 'Default';
  $panelizer->panelizer_type = 'user';
  $panelizer->panelizer_key = 'user';
  $panelizer->access = array();
  $panelizer->view_mode = 'page_manager';
  $panelizer->name = 'user:user:default';
  $panelizer->css_id = 'user-panel';
  $panelizer->css_class = '';
  $panelizer->css = '';
  $panelizer->no_blocks = FALSE;
  $panelizer->title_element = 'H2';
  $panelizer->link_to_entity = TRUE;
  $panelizer->extra = '';
  $panelizer->pipeline = 'ipe';
  $panelizer->contexts = array();
  $panelizer->relationships = array();
  $display = new panels_display();
  $display->layout = 'phelan';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => NULL,
      'contentmain' => NULL,
      'sidebar' => NULL,
      'column1' => NULL,
      'column2' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'ecd9cca4-6ad9-4816-8c50-a7441c4b408b';
  $display->storage_type = 'panelizer_default';
  $display->storage_id = 'user:user:default';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-a06fca91-6257-463e-bcaa-65aaf7c40879';
  $pane->panel = 'column1';
  $pane->type = 'user_profile';
  $pane->subtype = 'user_profile';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'view_mode' => 'full',
    'context' => 'panelizer',
    'override_title' => 1,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = 'a06fca91-6257-463e-bcaa-65aaf7c40879';
  $display->content['new-a06fca91-6257-463e-bcaa-65aaf7c40879'] = $pane;
  $display->panels['column1'][0] = 'new-a06fca91-6257-463e-bcaa-65aaf7c40879';
  $pane = new stdClass();
  $pane->pid = 'new-a9263191-a2a8-1f64-916d-64b99aa88f97';
  $pane->panel = 'column2';
  $pane->type = 'entity_field';
  $pane->subtype = 'user:field_user_picture';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => 'hidden',
    'formatter' => 'image',
    'delta_limit' => 0,
    'delta_offset' => '0',
    'delta_reversed' => FALSE,
    'formatter_settings' => array(
      'image_style' => 'wetkit_image_full',
      'image_link' => '',
    ),
    'context' => 'panelizer',
    'override_title' => 1,
    'override_title_text' => '',
    'view_mode' => NULL,
    'widget_title' => NULL,
    'items_per_page' => NULL,
    'exposed' => array(
      'sort_by' => NULL,
      'sort_order' => NULL,
    ),
    'use_pager' => NULL,
    'pager_id' => NULL,
    'offset' => NULL,
    'link_to_view' => NULL,
    'more_link' => NULL,
    'path' => NULL,
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = 'a9263191-a2a8-1f64-916d-64b99aa88f97';
  $display->content['new-a9263191-a2a8-1f64-916d-64b99aa88f97'] = $pane;
  $display->panels['column2'][0] = 'new-a9263191-a2a8-1f64-916d-64b99aa88f97';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $panelizer->display = $display;
  $export['user:user:default'] = $panelizer;

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->title = 'Default';
  $panelizer->panelizer_type = 'user';
  $panelizer->panelizer_key = 'user';
  $panelizer->access = array();
  $panelizer->view_mode = 'default';
  $panelizer->name = 'user:user:default:default';
  $panelizer->css_id = '';
  $panelizer->css_class = 'user-default';
  $panelizer->css = '';
  $panelizer->no_blocks = FALSE;
  $panelizer->title_element = 'H2';
  $panelizer->link_to_entity = TRUE;
  $panelizer->extra = '';
  $panelizer->pipeline = 'standard';
  $panelizer->contexts = array();
  $panelizer->relationships = array();
  $display = new panels_display();
  $display->layout = 'boxton';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'contentmain' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '635bec8d-5671-428f-a5da-7658278f764a';
  $display->storage_type = 'panelizer_default';
  $display->storage_id = 'user:user:default:default';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-3a599c50-fabe-43e5-9015-32804aa18711';
  $pane->panel = 'contentmain';
  $pane->type = 'entity_field';
  $pane->subtype = 'user:field_user_picture';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => 'hidden',
    'formatter' => 'image',
    'delta_limit' => 0,
    'delta_offset' => '0',
    'delta_reversed' => FALSE,
    'formatter_settings' => array(
      'image_style' => 'wetkit_image_half',
      'image_link' => '',
    ),
    'context' => 'panelizer',
    'override_title' => 1,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '3a599c50-fabe-43e5-9015-32804aa18711';
  $display->content['new-3a599c50-fabe-43e5-9015-32804aa18711'] = $pane;
  $display->panels['contentmain'][0] = 'new-3a599c50-fabe-43e5-9015-32804aa18711';
  $pane = new stdClass();
  $pane->pid = 'new-84e0d2b2-ea99-44af-91e2-2aa0c16fbd1a';
  $pane->panel = 'contentmain';
  $pane->type = 'entity_field';
  $pane->subtype = 'user:field_user_about';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => 'hidden',
    'formatter' => 'text_default',
    'delta_limit' => 0,
    'delta_offset' => '0',
    'delta_reversed' => FALSE,
    'formatter_settings' => array(),
    'context' => 'panelizer',
    'override_title' => 1,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $pane->locks = array();
  $pane->uuid = '84e0d2b2-ea99-44af-91e2-2aa0c16fbd1a';
  $display->content['new-84e0d2b2-ea99-44af-91e2-2aa0c16fbd1a'] = $pane;
  $display->panels['contentmain'][1] = 'new-84e0d2b2-ea99-44af-91e2-2aa0c16fbd1a';
  $pane = new stdClass();
  $pane->pid = 'new-9db6ce84-8333-55e4-a939-2aecec255bfa';
  $pane->panel = 'contentmain';
  $pane->type = 'entity_field_extra';
  $pane->subtype = 'user:summary';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'view_mode' => 'full',
    'context' => 'panelizer',
    'override_title' => 1,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = '9db6ce84-8333-55e4-a939-2aecec255bfa';
  $display->content['new-9db6ce84-8333-55e4-a939-2aecec255bfa'] = $pane;
  $display->panels['contentmain'][2] = 'new-9db6ce84-8333-55e4-a939-2aecec255bfa';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $panelizer->display = $display;
  $export['user:user:default:default'] = $panelizer;

  $panelizer = new stdClass();
  $panelizer->disabled = FALSE; /* Edit this to true to make a default panelizer disabled initially */
  $panelizer->api_version = 1;
  $panelizer->title = 'Default';
  $panelizer->panelizer_type = 'user';
  $panelizer->panelizer_key = 'user';
  $panelizer->access = array();
  $panelizer->view_mode = 'featured';
  $panelizer->name = 'user:user:default:featured';
  $panelizer->css_id = '';
  $panelizer->css_class = 'user-featured';
  $panelizer->css = '';
  $panelizer->no_blocks = FALSE;
  $panelizer->title_element = 'H2';
  $panelizer->link_to_entity = TRUE;
  $panelizer->extra = '';
  $panelizer->pipeline = 'standard';
  $panelizer->contexts = array();
  $panelizer->relationships = array();
  $display = new panels_display();
  $display->layout = 'phelan';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'column1' => NULL,
      'column2' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'dce93ac2-895b-4622-97c7-88e1755d5f69';
  $display->storage_type = 'panelizer_default';
  $display->storage_id = 'user:user:default:featured';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-22106504-1d50-4c19-8990-dedfc91f462e';
  $pane->panel = 'column1';
  $pane->type = 'entity_field';
  $pane->subtype = 'user:field_user_picture';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => 'hidden',
    'formatter' => 'image',
    'delta_limit' => 0,
    'delta_offset' => '0',
    'delta_reversed' => FALSE,
    'formatter_settings' => array(
      'image_style' => 'wetkit_image_half',
      'image_link' => 'content',
    ),
    'context' => 'panelizer',
    'override_title' => 1,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '22106504-1d50-4c19-8990-dedfc91f462e';
  $display->content['new-22106504-1d50-4c19-8990-dedfc91f462e'] = $pane;
  $display->panels['column1'][0] = 'new-22106504-1d50-4c19-8990-dedfc91f462e';
  $pane = new stdClass();
  $pane->pid = 'new-74907020-a7d3-4143-897a-290bd164ef78';
  $pane->panel = 'column2';
  $pane->type = 'token';
  $pane->subtype = 'user:name';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'sanitize' => 1,
    'context' => 'panelizer',
    'override_title' => 1,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '74907020-a7d3-4143-897a-290bd164ef78';
  $display->content['new-74907020-a7d3-4143-897a-290bd164ef78'] = $pane;
  $display->panels['column2'][0] = 'new-74907020-a7d3-4143-897a-290bd164ef78';
  $pane = new stdClass();
  $pane->pid = 'new-411cc677-c5ba-40ca-99b3-fba14818e8e5';
  $pane->panel = 'column2';
  $pane->type = 'entity_field';
  $pane->subtype = 'user:field_user_about';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => 'hidden',
    'formatter' => 'text_summary_or_trimmed',
    'delta_limit' => 0,
    'delta_offset' => '0',
    'delta_reversed' => FALSE,
    'formatter_settings' => array(
      'trim_length' => '300',
    ),
    'context' => 'panelizer',
    'override_title' => 1,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $pane->locks = array();
  $pane->uuid = '411cc677-c5ba-40ca-99b3-fba14818e8e5';
  $display->content['new-411cc677-c5ba-40ca-99b3-fba14818e8e5'] = $pane;
  $display->panels['column2'][1] = 'new-411cc677-c5ba-40ca-99b3-fba14818e8e5';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $panelizer->display = $display;
  $export['user:user:default:featured'] = $panelizer;

  return $export;
}
