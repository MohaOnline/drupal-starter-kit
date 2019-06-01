<?php
/**
 * @file
 * wetkit_admin.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function wetkit_admin_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_edit_panel_context';
  $handler->task = 'node_edit';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 100;
  $handler->conf = array(
    'title' => 'Node Edit Page',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'css_id' => 'node-edit',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'burr_flipped';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'contentmain' => NULL,
      'sidebar' => NULL,
    ),
    'sidebar' => array(
      'style' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '7b749187-6bc6-4468-b8fb-b4583c740b4b';
  $display->storage_type = 'page_manager';
  $display->storage_id = 'node_edit_panel_context';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-858912ce-f639-47ed-8e48-bafa7e312751';
  $pane->panel = 'contentmain';
  $pane->type = 'node_form_title';
  $pane->subtype = 'node_form_title';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => 'argument_node_edit_1',
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
  $pane->uuid = '858912ce-f639-47ed-8e48-bafa7e312751';
  $display->content['new-858912ce-f639-47ed-8e48-bafa7e312751'] = $pane;
  $display->panels['contentmain'][0] = 'new-858912ce-f639-47ed-8e48-bafa7e312751';
  $pane = new stdClass();
  $pane->pid = 'new-b5f50cf2-e574-1354-a547-4d25cb9695d2';
  $pane->panel = 'contentmain';
  $pane->type = 'node_form_path';
  $pane->subtype = 'node_form_path';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => 'argument_node_edit_1',
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
  $pane->uuid = 'b5f50cf2-e574-1354-a547-4d25cb9695d2';
  $display->content['new-b5f50cf2-e574-1354-a547-4d25cb9695d2'] = $pane;
  $display->panels['contentmain'][1] = 'new-b5f50cf2-e574-1354-a547-4d25cb9695d2';
  $pane = new stdClass();
  $pane->pid = 'new-0fe4f2bb-8f6b-4c16-9bab-3574ce0d18b9';
  $pane->panel = 'contentmain';
  $pane->type = 'form';
  $pane->subtype = 'form';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => 'argument_node_edit_1',
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
  $pane->uuid = '0fe4f2bb-8f6b-4c16-9bab-3574ce0d18b9';
  $display->content['new-0fe4f2bb-8f6b-4c16-9bab-3574ce0d18b9'] = $pane;
  $display->panels['contentmain'][2] = 'new-0fe4f2bb-8f6b-4c16-9bab-3574ce0d18b9';
  $pane = new stdClass();
  $pane->pid = 'new-dc0f37cd-592a-46f0-b314-6b0d93b9d815';
  $pane->panel = 'sidebar';
  $pane->type = 'entity_form_field';
  $pane->subtype = 'node:field_featured_image';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => '',
    'formatter' => '',
    'context' => 'argument_node_edit_1',
    'override_title' => 1,
    'override_title_text' => 'Featured image',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = 'dc0f37cd-592a-46f0-b314-6b0d93b9d815';
  $display->content['new-dc0f37cd-592a-46f0-b314-6b0d93b9d815'] = $pane;
  $display->panels['sidebar'][0] = 'new-dc0f37cd-592a-46f0-b314-6b0d93b9d815';
  $pane = new stdClass();
  $pane->pid = 'new-059e4f76-1c82-439a-921b-d58a84911431';
  $pane->panel = 'sidebar';
  $pane->type = 'entity_form_field';
  $pane->subtype = 'node:field_featured_categories';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => '',
    'formatter' => '',
    'context' => 'argument_node_edit_1',
    'override_title' => 1,
    'override_title_text' => 'Content category',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $pane->locks = array();
  $pane->uuid = '059e4f76-1c82-439a-921b-d58a84911431';
  $display->content['new-059e4f76-1c82-439a-921b-d58a84911431'] = $pane;
  $display->panels['sidebar'][1] = 'new-059e4f76-1c82-439a-921b-d58a84911431';
  $pane = new stdClass();
  $pane->pid = 'new-af6bd27c-977c-4a1e-829d-e98e318b1a37';
  $pane->panel = 'sidebar';
  $pane->type = 'panelizer_form_default';
  $pane->subtype = 'panelizer_form_default';
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
  $pane->uuid = 'af6bd27c-977c-4a1e-829d-e98e318b1a37';
  $display->content['new-af6bd27c-977c-4a1e-829d-e98e318b1a37'] = $pane;
  $display->panels['sidebar'][2] = 'new-af6bd27c-977c-4a1e-829d-e98e318b1a37';
  $pane = new stdClass();
  $pane->pid = 'new-0bc9ffdb-7dda-4b1b-98c0-334ef5217602';
  $pane->panel = 'sidebar';
  $pane->type = 'node_form_menu';
  $pane->subtype = 'node_form_menu';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => 'argument_node_edit_1',
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
  $pane->locks = array();
  $pane->uuid = '0bc9ffdb-7dda-4b1b-98c0-334ef5217602';
  $display->content['new-0bc9ffdb-7dda-4b1b-98c0-334ef5217602'] = $pane;
  $display->panels['sidebar'][3] = 'new-0bc9ffdb-7dda-4b1b-98c0-334ef5217602';
  $pane = new stdClass();
  $pane->pid = 'new-3c9363e8-38e3-4f17-974b-342703e41245';
  $pane->panel = 'sidebar';
  $pane->type = 'node_form_publishing';
  $pane->subtype = 'node_form_publishing';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => 'argument_node_edit_1',
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
  $pane->uuid = '3c9363e8-38e3-4f17-974b-342703e41245';
  $display->content['new-3c9363e8-38e3-4f17-974b-342703e41245'] = $pane;
  $display->panels['sidebar'][4] = 'new-3c9363e8-38e3-4f17-974b-342703e41245';
  $pane = new stdClass();
  $pane->pid = 'new-fe476827-26d1-4431-9508-005402e54b85';
  $pane->panel = 'sidebar';
  $pane->type = 'node_form_author';
  $pane->subtype = 'node_form_author';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => array(),
    'override_title' => 0,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 5;
  $pane->locks = array();
  $pane->uuid = 'fe476827-26d1-4431-9508-005402e54b85';
  $display->content['new-fe476827-26d1-4431-9508-005402e54b85'] = $pane;
  $display->panels['sidebar'][5] = 'new-fe476827-26d1-4431-9508-005402e54b85';
  $pane = new stdClass();
  $pane->pid = 'new-887eebc9-034c-4cd6-b81d-4daa61199e03';
  $pane->panel = 'sidebar';
  $pane->type = 'node_form_buttons';
  $pane->subtype = 'node_form_buttons';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => array(),
    'override_title' => 1,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 6;
  $pane->locks = array();
  $pane->uuid = '887eebc9-034c-4cd6-b81d-4daa61199e03';
  $display->content['new-887eebc9-034c-4cd6-b81d-4daa61199e03'] = $pane;
  $display->panels['sidebar'][6] = 'new-887eebc9-034c-4cd6-b81d-4daa61199e03';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-0fe4f2bb-8f6b-4c16-9bab-3574ce0d18b9';
  $handler->conf['display'] = $display;
  $export['node_edit_panel_context'] = $handler;

  return $export;
}

/**
 * Implements hook_default_page_manager_pages().
 */
function wetkit_admin_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'wetkit_admin_layout_library';
  $page->task = 'page';
  $page->admin_title = 'WetKit Layouts';
  $page->admin_description = 'This is the page library that manages page layouts.';
  $page->path = 'admin/wetkit/layouts';
  $page->access = array(
    'plugins' => array(
      1 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'access administration pages',
        ),
        'context' => 'logged-in-user',
        'not' => FALSE,
      ),
      2 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'administer panels layouts',
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
    'title' => 'Layouts',
    'name' => 'management',
    'weight' => '0',
    'parent' => array(
      'type' => 'normal',
      'title' => 'Dashboard',
      'name' => 'management',
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
  $handler->name = 'page_wetkit_admin_layout_library_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'wetkit_admin_layout_library';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Layout Library',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'css_id' => 'layout-library',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
  );
  $display = new panels_display();
  $display->layout = 'boxton';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'header' => NULL,
      'column1' => NULL,
      'column2' => NULL,
      'footer' => NULL,
      'contentmain' => NULL,
      'sidebar' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Layout Library';
  $display->uuid = '0e5cfce2-068e-4c29-b3b6-bfc3f95cdeb7';
  $display->storage_type = 'page_manager';
  $display->storage_id = 'page_wetkit_admin_layout_library_panel_context';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-21242b25-fb1d-4e69-8533-4795043f5622';
  $pane->panel = 'contentmain';
  $pane->type = 'panels_layouts';
  $pane->subtype = 'panels_layouts';
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
  $pane->locks = array();
  $pane->uuid = '21242b25-fb1d-4e69-8533-4795043f5622';
  $display->content['new-21242b25-fb1d-4e69-8533-4795043f5622'] = $pane;
  $display->panels['contentmain'][0] = 'new-21242b25-fb1d-4e69-8533-4795043f5622';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['wetkit_admin_layout_library'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'wetkit_admin_page_library';
  $page->task = 'page';
  $page->admin_title = 'WetKit Pages';
  $page->admin_description = 'This is the page library that manages page templates, landing pages, and overridden pages.';
  $page->path = 'admin/wetkit/pages';
  $page->access = array(
    'plugins' => array(
      1 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'access administration pages',
        ),
        'context' => 'logged-in-user',
        'not' => FALSE,
      ),
      2 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'use page manager',
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
    'title' => 'Pages',
    'name' => 'management',
    'weight' => '0',
    'parent' => array(
      'type' => 'normal',
      'title' => 'Dashboard',
      'name' => 'management',
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
  $handler->name = 'page_wetkit_admin_page_library_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'wetkit_admin_page_library';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Page Library',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'css_id' => 'page-library',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
  );
  $display = new panels_display();
  $display->layout = 'boxton';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'header' => NULL,
      'column1' => NULL,
      'column2' => NULL,
      'footer' => NULL,
      'contentmain' => NULL,
      'sidebar' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Page Library';
  $display->uuid = '103cdbc1-e55d-4779-9128-3b2116b71940';
  $display->storage_type = 'page_manager';
  $display->storage_id = 'page_wetkit_admin_page_library_panel_context';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-1a75c8f6-fd1b-4862-99e0-1aaab819e260';
  $pane->panel = 'contentmain';
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
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '1a75c8f6-fd1b-4862-99e0-1aaab819e260';
  $display->content['new-1a75c8f6-fd1b-4862-99e0-1aaab819e260'] = $pane;
  $display->panels['contentmain'][0] = 'new-1a75c8f6-fd1b-4862-99e0-1aaab819e260';
  $pane = new stdClass();
  $pane->pid = 'new-20cb91d9-124c-4a0d-aece-0b65f6af6676';
  $pane->panel = 'contentmain';
  $pane->type = 'landing_pages';
  $pane->subtype = 'landing_pages';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array();
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $pane->locks = array();
  $pane->uuid = '20cb91d9-124c-4a0d-aece-0b65f6af6676';
  $display->content['new-20cb91d9-124c-4a0d-aece-0b65f6af6676'] = $pane;
  $display->panels['contentmain'][1] = 'new-20cb91d9-124c-4a0d-aece-0b65f6af6676';
  $pane = new stdClass();
  $pane->pid = 'new-6cb52185-1858-478e-adb7-69b692469859';
  $pane->panel = 'contentmain';
  $pane->type = 'overridden_page_templates';
  $pane->subtype = 'overridden_page_templates';
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
  $pane->uuid = '6cb52185-1858-478e-adb7-69b692469859';
  $display->content['new-6cb52185-1858-478e-adb7-69b692469859'] = $pane;
  $display->panels['contentmain'][2] = 'new-6cb52185-1858-478e-adb7-69b692469859';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['wetkit_admin_page_library'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'wetkit_admin_pane_library';
  $page->task = 'page';
  $page->admin_title = 'WetKit Panes';
  $page->admin_description = 'This is the pane library for the site. See all of the panes you can add to your pages.';
  $page->path = 'admin/wetkit/panes';
  $page->access = array(
    'plugins' => array(
      1 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'access administration pages',
        ),
        'context' => 'logged-in-user',
        'not' => FALSE,
      ),
      2 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'administer page manager',
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
    'title' => 'Panes',
    'name' => 'management',
    'weight' => '0',
    'parent' => array(
      'type' => 'normal',
      'title' => 'Dashboard',
      'name' => 'management',
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
  $handler->name = 'page_wetkit_admin_pane_library_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'wetkit_admin_pane_library';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Pane Library',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'css_id' => 'pane-library',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
  );
  $display = new panels_display();
  $display->layout = 'boxton';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'header' => NULL,
      'column1' => NULL,
      'column2' => NULL,
      'footer' => NULL,
      'contentmain' => NULL,
      'sidebar' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Pane Library';
  $display->uuid = 'c36ec58f-719d-4064-8534-853b1803d21e';
  $display->storage_type = 'page_manager';
  $display->storage_id = 'page_wetkit_admin_pane_library_panel_context';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-bc8db260-387b-480f-bc6f-3681980c9360';
  $pane->panel = 'contentmain';
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
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = 'bc8db260-387b-480f-bc6f-3681980c9360';
  $display->content['new-bc8db260-387b-480f-bc6f-3681980c9360'] = $pane;
  $display->panels['contentmain'][0] = 'new-bc8db260-387b-480f-bc6f-3681980c9360';
  $pane = new stdClass();
  $pane->pid = 'new-98b628ad-5a77-42a0-9970-e710808fdfd2';
  $pane->panel = 'contentmain';
  $pane->type = 'general_panes';
  $pane->subtype = 'general_panes';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array();
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $pane->locks = array();
  $pane->uuid = '98b628ad-5a77-42a0-9970-e710808fdfd2';
  $display->content['new-98b628ad-5a77-42a0-9970-e710808fdfd2'] = $pane;
  $display->panels['contentmain'][1] = 'new-98b628ad-5a77-42a0-9970-e710808fdfd2';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['wetkit_admin_pane_library'] = $page;

  return $pages;

}
