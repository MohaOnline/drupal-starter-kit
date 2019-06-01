<?php
/**
 * @file
 * wetkit_demo.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function wetkit_demo_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'wetkit_demo_panel_front';
  $page->task = 'page';
  $page->admin_title = 'WetKit Demo Front';
  $page->admin_description = 'This is a demo panel for WetKit so folks can see the magic';
  $page->path = 'demo';
  $page->access = array(
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'none',
    'title' => 'Homepage Demo',
    'name' => 'main-menu',
    'weight' => '-10',
    'parent' => array(
      'type' => 'none',
      'title' => '',
      'name' => 'navigation',
      'weight' => '0',
    ),
  );
  $page->arguments = array();
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_wetkit_demo_panel_front_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'wetkit_demo_panel_front';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Demo Panel',
    'no_blocks' => 0,
    'pipeline' => 'ipe',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'sutherland';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'header' => NULL,
      'contentmain' => NULL,
      'footer' => NULL,
      'sutherland_top' => NULL,
      'sutherland_first' => NULL,
      'sutherland_second' => NULL,
      'sutherland_third' => NULL,
      'sutherland_bottom' => NULL,
      'sutherland_top_inner' => NULL,
      'sutherland_bottom_inner' => NULL,
    ),
    'sidebar' => array(
      'style' => '-1',
    ),
    'sutherland_top' => array(
      'style' => '-1',
    ),
  );
  $display->cache = array();
  $display->title = 'Homepage Demo';
  $display->uuid = '9e5e6c64-35f7-488b-b768-13edc1514e37';
  $display->storage_type = 'page_manager';
  $display->storage_id = 'page_wetkit_demo_panel_front_panel_context';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-979647c2-bc9a-4b1c-90b2-8a399d6fb8b5';
  $pane->panel = 'sutherland_bottom';
  $pane->type = 'block';
  $pane->subtype = 'bean-main-features';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => 0,
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
  $pane->uuid = '979647c2-bc9a-4b1c-90b2-8a399d6fb8b5';
  $display->content['new-979647c2-bc9a-4b1c-90b2-8a399d6fb8b5'] = $pane;
  $display->panels['sutherland_bottom'][0] = 'new-979647c2-bc9a-4b1c-90b2-8a399d6fb8b5';
  $pane = new stdClass();
  $pane->pid = 'new-735ed33b-32eb-4784-9165-803153aeea24';
  $pane->panel = 'sutherland_bottom_inner';
  $pane->type = 'block';
  $pane->subtype = 'bean-feedback-promo';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => 1,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '735ed33b-32eb-4784-9165-803153aeea24';
  $display->content['new-735ed33b-32eb-4784-9165-803153aeea24'] = $pane;
  $display->panels['sutherland_bottom_inner'][0] = 'new-735ed33b-32eb-4784-9165-803153aeea24';
  $pane = new stdClass();
  $pane->pid = 'new-b1adf71c-2539-4eaa-b11f-388a5b34fb39';
  $pane->panel = 'sutherland_bottom_inner';
  $pane->type = 'views_panes';
  $pane->subtype = 'wxt_theme-home_department';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array();
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $pane->locks = array();
  $pane->uuid = 'b1adf71c-2539-4eaa-b11f-388a5b34fb39';
  $display->content['new-b1adf71c-2539-4eaa-b11f-388a5b34fb39'] = $pane;
  $display->panels['sutherland_bottom_inner'][1] = 'new-b1adf71c-2539-4eaa-b11f-388a5b34fb39';
  $pane = new stdClass();
  $pane->pid = 'new-24800c93-44a8-4a92-8cfb-f37c9e635424';
  $pane->panel = 'sutherland_bottom_inner';
  $pane->type = 'date_modified';
  $pane->subtype = 'date_modified';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array();
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = '24800c93-44a8-4a92-8cfb-f37c9e635424';
  $display->content['new-24800c93-44a8-4a92-8cfb-f37c9e635424'] = $pane;
  $display->panels['sutherland_bottom_inner'][2] = 'new-24800c93-44a8-4a92-8cfb-f37c9e635424';
  $pane = new stdClass();
  $pane->pid = 'new-1680191d-ac70-4823-a623-7122856db05c';
  $pane->panel = 'sutherland_top';
  $pane->type = 'block';
  $pane->subtype = 'bean-main-slider';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => 1,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '1680191d-ac70-4823-a623-7122856db05c';
  $display->content['new-1680191d-ac70-4823-a623-7122856db05c'] = $pane;
  $display->panels['sutherland_top'][0] = 'new-1680191d-ac70-4823-a623-7122856db05c';
  $pane = new stdClass();
  $pane->pid = 'new-3b10c54f-3e5f-4f44-981c-9c919e1254f0';
  $pane->panel = 'sutherland_top_inner';
  $pane->type = 'page_messages';
  $pane->subtype = 'page_messages';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array();
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '3b10c54f-3e5f-4f44-981c-9c919e1254f0';
  $display->content['new-3b10c54f-3e5f-4f44-981c-9c919e1254f0'] = $pane;
  $display->panels['sutherland_top_inner'][0] = 'new-3b10c54f-3e5f-4f44-981c-9c919e1254f0';
  $pane = new stdClass();
  $pane->pid = 'new-6ab648d6-514f-4dbd-bf1d-c32e2f7d9874';
  $pane->panel = 'sutherland_top_inner';
  $pane->type = 'views_panes';
  $pane->subtype = 'wxt_theme-home_highlight';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => 0,
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
  $pane->uuid = '6ab648d6-514f-4dbd-bf1d-c32e2f7d9874';
  $display->content['new-6ab648d6-514f-4dbd-bf1d-c32e2f7d9874'] = $pane;
  $display->panels['sutherland_top_inner'][1] = 'new-6ab648d6-514f-4dbd-bf1d-c32e2f7d9874';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['wetkit_demo_panel_front'] = $page;

  return $pages;

}
