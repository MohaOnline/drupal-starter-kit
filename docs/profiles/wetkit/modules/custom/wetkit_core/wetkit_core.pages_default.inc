<?php
/**
 * @file
 * wetkit_core.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function wetkit_core_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'dashboard';
  $page->task = 'page';
  $page->admin_title = 'Total Control administrative dashboard';
  $page->admin_description = '';
  $page->path = 'admin/dashboard';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'have total control',
        ),
        'context' => 'logged-in-user',
        'not' => FALSE,
      ),
    ),
    'logic' => 'and',
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'normal',
    'title' => 'Dashboard',
    'name' => 'management',
    'weight' => '-100',
    'parent' => array(
      'type' => 'normal',
      'title' => 'Dashboard',
      'name' => 'management',
      'weight' => '0',
    ),
  );
  $page->arguments = array();
  $page->conf = array(
    'admin_paths' => TRUE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_dashboard_panel_context_2';
  $handler->task = 'page';
  $handler->subtask = 'dashboard';
  $handler->handler = 'panel_context';
  $handler->weight = -30;
  $handler->conf = array(
    'title' => 'WetKit Admin Dashboard',
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
  $display->layout = 'sutro';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'one_main' => NULL,
      'two_50_top' => NULL,
      'two_50_first' => NULL,
      'two_50_second' => NULL,
      'two_50_bottom' => NULL,
      'column1' => NULL,
      'column2' => NULL,
      'header' => NULL,
      'footer' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'c0bc0c9e-5ca9-4a37-aa5e-ced552f0637e';
  $display->storage_type = 'page_manager';
  $display->storage_id = 'page_dashboard_panel_context_2';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-51ec3b2f-b098-417c-88a8-ebaa993809ab';
  $pane->panel = 'column1';
  $pane->type = 'create';
  $pane->subtype = 'create';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'types' => array(
      'wetkit_documentation' => 'wetkit_documentation',
      'wetkit_page' => 'wetkit_page',
      'wetkit_page_other' => 0,
    ),
    'override_title' => 0,
    'override_title_text' => '<div class="background-accent">Create New Content</div>',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '51ec3b2f-b098-417c-88a8-ebaa993809ab';
  $display->content['new-51ec3b2f-b098-417c-88a8-ebaa993809ab'] = $pane;
  $display->panels['column1'][0] = 'new-51ec3b2f-b098-417c-88a8-ebaa993809ab';
  $pane = new stdClass();
  $pane->pid = 'new-5f5617c8-9f81-4172-b06c-50d4316ff948';
  $pane->panel = 'column1';
  $pane->type = 'overview_content';
  $pane->subtype = 'overview_content';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'types' => array(
      'wetkit_documentation' => 'wetkit_documentation',
      'wetkit_page' => 'wetkit_page',
      'wetkit_page_other' => 0,
    ),
    'comments' => NULL,
    'spam' => NULL,
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
  $pane->uuid = '5f5617c8-9f81-4172-b06c-50d4316ff948';
  $display->content['new-5f5617c8-9f81-4172-b06c-50d4316ff948'] = $pane;
  $display->panels['column1'][1] = 'new-5f5617c8-9f81-4172-b06c-50d4316ff948';
  $pane = new stdClass();
  $pane->pid = 'new-7616683f-5c80-4899-9096-c5c80e170b50';
  $pane->panel = 'column1';
  $pane->type = 'views_panes';
  $pane->subtype = 'control_users_panes-pane_tc_new_users';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'items_per_page' => '10',
    'view_mode' => 'teaser',
    'widget_title' => NULL,
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
    'view_settings' => 'fields',
    'header_type' => 'none',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = '7616683f-5c80-4899-9096-c5c80e170b50';
  $display->content['new-7616683f-5c80-4899-9096-c5c80e170b50'] = $pane;
  $display->panels['column1'][2] = 'new-7616683f-5c80-4899-9096-c5c80e170b50';
  $pane = new stdClass();
  $pane->pid = 'new-e835e473-a22c-5054-9903-1dee16581e9b';
  $pane->panel = 'column1';
  $pane->type = 'views_panes';
  $pane->subtype = 'control_content_panes-pane_tc_new';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'items_per_page' => '5',
    'override_title' => '',
    'override_title_text' => '',
    'view_mode' => 'teaser',
    'widget_title' => 'New Content',
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
    'view_settings' => 'fields',
    'header_type' => 'none',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 3;
  $pane->locks = array();
  $pane->uuid = 'e835e473-a22c-5054-9903-1dee16581e9b';
  $display->content['new-e835e473-a22c-5054-9903-1dee16581e9b'] = $pane;
  $display->panels['column1'][3] = 'new-e835e473-a22c-5054-9903-1dee16581e9b';
  $pane = new stdClass();
  $pane->pid = 'new-0d606209-3ea6-40c2-8519-36d3cf9ff921';
  $pane->panel = 'column1';
  $pane->type = 'taxonomy';
  $pane->subtype = 'taxonomy';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'vids' => array(
      2 => '2',
      1 => 0,
    ),
    'override_title' => 0,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 4;
  $pane->locks = array();
  $pane->uuid = '0d606209-3ea6-40c2-8519-36d3cf9ff921';
  $display->content['new-0d606209-3ea6-40c2-8519-36d3cf9ff921'] = $pane;
  $display->panels['column1'][4] = 'new-0d606209-3ea6-40c2-8519-36d3cf9ff921';
  $pane = new stdClass();
  $pane->pid = 'new-995df43a-3573-4e8e-ae6a-5439c8ef2a88';
  $pane->panel = 'column2';
  $pane->type = 'node_types';
  $pane->subtype = 'node_types';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'types' => array(
      'wetkit_documentation' => 'wetkit_documentation',
      'wetkit_page' => 'wetkit_page',
      'wetkit_page_other' => 0,
    ),
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
  $pane->uuid = '995df43a-3573-4e8e-ae6a-5439c8ef2a88';
  $display->content['new-995df43a-3573-4e8e-ae6a-5439c8ef2a88'] = $pane;
  $display->panels['column2'][0] = 'new-995df43a-3573-4e8e-ae6a-5439c8ef2a88';
  $pane = new stdClass();
  $pane->pid = 'new-4474d2ad-1d65-4db9-833e-076154b6134a';
  $pane->panel = 'column2';
  $pane->type = 'menus';
  $pane->subtype = 'menus';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'menus' => array(
      'menu-wet-footer' => 'menu-wet-footer',
      'menu-wet-header' => 'menu-wet-header',
      'main-menu' => 'main-menu',
      'menu-wet-mid-footer' => 'menu-wet-mid-footer',
      'navigation' => 'navigation',
      'menu-wet-terms' => 'menu-wet-terms',
      'management' => 0,
      'user-menu' => 0,
    ),
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
  $pane->uuid = '4474d2ad-1d65-4db9-833e-076154b6134a';
  $display->content['new-4474d2ad-1d65-4db9-833e-076154b6134a'] = $pane;
  $display->panels['column2'][1] = 'new-4474d2ad-1d65-4db9-833e-076154b6134a';
  $pane = new stdClass();
  $pane->pid = 'new-e3bf1253-690c-4c34-9ae7-79c9b826d050';
  $pane->panel = 'column2';
  $pane->type = 'panel_pages';
  $pane->subtype = 'panel_pages';
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
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = 'e3bf1253-690c-4c34-9ae7-79c9b826d050';
  $display->content['new-e3bf1253-690c-4c34-9ae7-79c9b826d050'] = $pane;
  $display->panels['column2'][2] = 'new-e3bf1253-690c-4c34-9ae7-79c9b826d050';
  $pane = new stdClass();
  $pane->pid = 'new-74139f6e-e336-4be1-a319-471b74edacf5';
  $pane->panel = 'header';
  $pane->type = 'page_title';
  $pane->subtype = 'page_title';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'markup' => 'h1',
    'class' => '',
    'id' => 'wb-cont',
    'override_title' => 0,
    'override_title_text' => '',
    'override_title_heading' => 'h2',
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '74139f6e-e336-4be1-a319-471b74edacf5';
  $display->content['new-74139f6e-e336-4be1-a319-471b74edacf5'] = $pane;
  $display->panels['header'][0] = 'new-74139f6e-e336-4be1-a319-471b74edacf5';
  $pane = new stdClass();
  $pane->pid = 'new-726d48c7-ce98-4bed-82e0-53090c7564d0';
  $pane->panel = 'header';
  $pane->type = 'page_tabs';
  $pane->subtype = 'page_tabs';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'type' => 'both',
    'id' => 'tabs',
    'override_title' => 0,
    'override_title_text' => '',
    'override_title_heading' => 'h2',
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $pane->locks = array();
  $pane->uuid = '726d48c7-ce98-4bed-82e0-53090c7564d0';
  $display->content['new-726d48c7-ce98-4bed-82e0-53090c7564d0'] = $pane;
  $display->panels['header'][1] = 'new-726d48c7-ce98-4bed-82e0-53090c7564d0';
  $pane = new stdClass();
  $pane->pid = 'new-ad1604aa-a843-4eb3-b534-e753233ee255';
  $pane->panel = 'header';
  $pane->type = 'welcome';
  $pane->subtype = 'welcome';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array();
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = 'ad1604aa-a843-4eb3-b534-e753233ee255';
  $display->content['new-ad1604aa-a843-4eb3-b534-e753233ee255'] = $pane;
  $display->panels['header'][2] = 'new-ad1604aa-a843-4eb3-b534-e753233ee255';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_dashboard_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'dashboard';
  $handler->handler = 'panel_context';
  $handler->weight = -29;
  $handler->conf = array(
    'title' => 'Admin Dashboard',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
  );
  $display = new panels_display();
  $display->layout = 'twocol_stacked';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'middle' => NULL,
      'top' => NULL,
      'left' => NULL,
      'right' => NULL,
      'bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Dashboard';
  $display->uuid = 'fe94cfe2-c2a0-1e64-992e-5eba9aa665bd';
  $display->storage_type = 'page_manager';
  $display->storage_id = 'page_dashboard_panel_context';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-74b90dfa-ccb3-4459-96ce-3f90444e7869';
  $pane->panel = 'left';
  $pane->type = 'create';
  $pane->subtype = 'create';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'types' => array(
      'article' => 'article',
      'project' => 'project',
      'page' => 0,
    ),
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
  $pane->locks = '';
  $pane->uuid = '74b90dfa-ccb3-4459-96ce-3f90444e7869';
  $display->content['new-74b90dfa-ccb3-4459-96ce-3f90444e7869'] = $pane;
  $display->panels['left'][0] = 'new-74b90dfa-ccb3-4459-96ce-3f90444e7869';
  $pane = new stdClass();
  $pane->pid = 'new-1f5aec55-0b62-4102-8958-a49e0ae80fdf';
  $pane->panel = 'left';
  $pane->type = 'overview_content';
  $pane->subtype = 'overview_content';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'types' => array(
      'article' => 'article',
      'page' => 'page',
      'project' => 'project',
      'video' => 'video',
    ),
    'comments' => array(
      'article' => 'article',
      'page' => 0,
      'project' => 0,
      'video' => 0,
    ),
    'spam' => 0,
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $pane->locks = '';
  $pane->uuid = '1f5aec55-0b62-4102-8958-a49e0ae80fdf';
  $display->content['new-1f5aec55-0b62-4102-8958-a49e0ae80fdf'] = $pane;
  $display->panels['left'][1] = 'new-1f5aec55-0b62-4102-8958-a49e0ae80fdf';
  $pane = new stdClass();
  $pane->pid = 'new-e0dd3c93-1e0a-413f-b23f-27121b1f9abd';
  $pane->panel = 'left';
  $pane->type = 'views_panes';
  $pane->subtype = 'control_users_panes-pane_tc_new_users';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'items_per_page' => '10',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = 'e0dd3c93-1e0a-413f-b23f-27121b1f9abd';
  $display->content['new-e0dd3c93-1e0a-413f-b23f-27121b1f9abd'] = $pane;
  $display->panels['left'][2] = 'new-e0dd3c93-1e0a-413f-b23f-27121b1f9abd';
  $pane = new stdClass();
  $pane->pid = 'new-e1d25eae-00cc-48dd-b6ad-c994e30ad433';
  $pane->panel = 'left';
  $pane->type = 'views_panes';
  $pane->subtype = 'control_content_panes-pane_tc_new';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'items_per_page' => '10',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 3;
  $pane->locks = '';
  $pane->uuid = 'e1d25eae-00cc-48dd-b6ad-c994e30ad433';
  $display->content['new-e1d25eae-00cc-48dd-b6ad-c994e30ad433'] = $pane;
  $display->panels['left'][3] = 'new-e1d25eae-00cc-48dd-b6ad-c994e30ad433';
  $pane = new stdClass();
  $pane->pid = 'new-2155bb9e-c930-434f-8d79-65fa93cc33b2';
  $pane->panel = 'right';
  $pane->type = 'menus';
  $pane->subtype = 'menus';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'item1' => NULL,
    'item2' => NULL,
    'override_title' => 0,
    'override_title_text' => '',
    'menus' => array(
      'main-menu' => 'main-menu',
      'navigation' => 'navigation',
      'devel' => 0,
      'management' => 0,
      'user-menu' => 0,
    ),
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = '';
  $pane->uuid = '2155bb9e-c930-434f-8d79-65fa93cc33b2';
  $display->content['new-2155bb9e-c930-434f-8d79-65fa93cc33b2'] = $pane;
  $display->panels['right'][0] = 'new-2155bb9e-c930-434f-8d79-65fa93cc33b2';
  $pane = new stdClass();
  $pane->pid = 'new-7a4489e1-bd75-4666-93c6-479e5257d453';
  $pane->panel = 'right';
  $pane->type = 'node_types';
  $pane->subtype = 'node_types';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'types' => array(
      'article' => 'article',
      'page' => 'page',
      'project' => 'project',
      'video' => 'video',
    ),
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
  $pane->locks = '';
  $pane->uuid = '7a4489e1-bd75-4666-93c6-479e5257d453';
  $display->content['new-7a4489e1-bd75-4666-93c6-479e5257d453'] = $pane;
  $display->panels['right'][1] = 'new-7a4489e1-bd75-4666-93c6-479e5257d453';
  $pane = new stdClass();
  $pane->pid = 'new-25419c26-60e9-4e7e-8321-4e9c0006a4ac';
  $pane->panel = 'right';
  $pane->type = 'taxonomy';
  $pane->subtype = 'taxonomy';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'vids' => array(
      0 => '2',
      1 => '1',
    ),
    'override_title' => 0,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 2;
  $pane->locks = '';
  $pane->uuid = '25419c26-60e9-4e7e-8321-4e9c0006a4ac';
  $display->content['new-25419c26-60e9-4e7e-8321-4e9c0006a4ac'] = $pane;
  $display->panels['right'][2] = 'new-25419c26-60e9-4e7e-8321-4e9c0006a4ac';
  $pane = new stdClass();
  $pane->pid = 'new-fa969c00-f144-4fd3-9c90-5a7ae0bc950a';
  $pane->panel = 'right';
  $pane->type = 'panel_pages';
  $pane->subtype = 'panel_pages';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'item1' => NULL,
    'item2' => NULL,
    'override_title' => 0,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 3;
  $pane->locks = '';
  $pane->uuid = 'fa969c00-f144-4fd3-9c90-5a7ae0bc950a';
  $display->content['new-fa969c00-f144-4fd3-9c90-5a7ae0bc950a'] = $pane;
  $display->panels['right'][3] = 'new-fa969c00-f144-4fd3-9c90-5a7ae0bc950a';
  $pane = new stdClass();
  $pane->pid = 'new-f3e2fd48-09c9-4227-a087-5fdd0e1063b2';
  $pane->panel = 'top';
  $pane->type = 'welcome';
  $pane->subtype = 'welcome';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array();
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = '';
  $pane->uuid = 'f3e2fd48-09c9-4227-a087-5fdd0e1063b2';
  $display->content['new-f3e2fd48-09c9-4227-a087-5fdd0e1063b2'] = $pane;
  $display->panels['top'][0] = 'new-f3e2fd48-09c9-4227-a087-5fdd0e1063b2';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['dashboard'] = $page;

  return $pages;

}
