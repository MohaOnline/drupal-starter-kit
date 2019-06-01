<?php
/**
 * @file
 * wetkit_deployment_source.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function wetkit_deployment_source_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'pm_existing_pages_deploy__panel_context_0d3c4d30-03fc-4aa6-b8c6-7bb38713df93';
  $handler->task = 'pm_existing_pages';
  $handler->subtask = 'deploy';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Deploy',
    'no_blocks' => 0,
    'pipeline' => 'ipe',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'name' => '',
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'perm',
          'settings' => array(
            'perm' => 'administer deployments',
          ),
          'context' => 'logged-in-user',
          'not' => FALSE,
        ),
        1 => array(
          'name' => 'perm',
          'settings' => array(
            'perm' => 'administer deploy dashboard',
          ),
          'context' => 'logged-in-user',
          'not' => FALSE,
        ),
      ),
      'logic' => 'or',
    ),
  );
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
  $display->uuid = '04c18f33-2b59-4911-ad78-2051e0d430b7';
  $display->storage_type = 'page_manager';
  $display->storage_id = 'pm_existing_pages_deploy__panel_context_0d3c4d30-03fc-4aa6-b8c6-7bb38713df93';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-5c809756-8bfe-4cda-a338-98fc06e44e79';
  $pane->panel = 'contentmain';
  $pane->type = 'page_title';
  $pane->subtype = 'page_title';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'markup' => 'h1',
    'class' => '',
    'id' => '#wb-cont',
    'override_title' => 0,
    'override_title_text' => '',
    'override_title_heading' => 'h2',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array(
    'css_id' => '',
    'css_class' => 'mrgn-bttm-lg',
  );
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '5c809756-8bfe-4cda-a338-98fc06e44e79';
  $display->content['new-5c809756-8bfe-4cda-a338-98fc06e44e79'] = $pane;
  $display->panels['contentmain'][0] = 'new-5c809756-8bfe-4cda-a338-98fc06e44e79';
  $pane = new stdClass();
  $pane->pid = 'new-798c67f6-ba13-4a1a-83c7-595d0907d483';
  $pane->panel = 'contentmain';
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
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $pane->locks = array();
  $pane->uuid = '798c67f6-ba13-4a1a-83c7-595d0907d483';
  $display->content['new-798c67f6-ba13-4a1a-83c7-595d0907d483'] = $pane;
  $display->panels['contentmain'][1] = 'new-798c67f6-ba13-4a1a-83c7-595d0907d483';
  $pane = new stdClass();
  $pane->pid = 'new-5f619426-74ad-4dcb-afa8-442f1e922a1c';
  $pane->panel = 'contentmain';
  $pane->type = 'deploy';
  $pane->subtype = 'deploy';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => 1,
    'override_title_text' => '',
    'override_title_heading' => 'h2',
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = '5f619426-74ad-4dcb-afa8-442f1e922a1c';
  $display->content['new-5f619426-74ad-4dcb-afa8-442f1e922a1c'] = $pane;
  $display->panels['contentmain'][2] = 'new-5f619426-74ad-4dcb-afa8-442f1e922a1c';
  $pane = new stdClass();
  $pane->pid = 'new-34508137-2cfa-4638-9cbb-ad67a4605a43';
  $pane->panel = 'contentmain';
  $pane->type = 'block';
  $pane->subtype = 'quicktabs-deployment_quicktabs';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => 1,
    'override_title_text' => 'Entities Deployment',
    'override_title_heading' => 'h2',
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 3;
  $pane->locks = array();
  $pane->uuid = '34508137-2cfa-4638-9cbb-ad67a4605a43';
  $display->content['new-34508137-2cfa-4638-9cbb-ad67a4605a43'] = $pane;
  $display->panels['contentmain'][3] = 'new-34508137-2cfa-4638-9cbb-ad67a4605a43';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['pm_existing_pages_deploy__panel_context_0d3c4d30-03fc-4aa6-b8c6-7bb38713df93'] = $handler;

  return $export;
}

/**
 * Implements hook_default_page_manager_pages().
 */
function wetkit_deployment_source_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'wetkit_deployment_source';
  $page->task = 'page';
  $page->admin_title = 'WetKit Deployment';
  $page->admin_description = 'Deployment Administrative Screen';
  $page->path = 'admin/dashboard/deployment';
  $page->access = array(
    'type' => 'none',
    'settings' => NULL,
    'plugins' => array(
      0 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'administer deploy dashboard',
        ),
        'context' => 'logged-in-user',
        'not' => FALSE,
      ),
      1 => array(
        'name' => 'perm',
        'settings' => array(
          'perm' => 'administer deployments',
        ),
        'context' => 'logged-in-user',
        'not' => FALSE,
      ),
    ),
    'logic' => 'or',
  );
  $page->menu = array(
    'type' => 'tab',
    'title' => 'Deployment',
    'name' => 'navigation',
    'weight' => '-1',
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
  $handler->name = 'page_wetkit_deployment_source_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'wetkit_deployment_source';
  $handler->handler = 'panel_context';
  $handler->weight = -29;
  $handler->conf = array(
    'title' => 'Deployment Staging Area',
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
  $display->layout = 'boxton';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'contentmain' => NULL,
      'header' => NULL,
      'sidebar' => NULL,
      'footer' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Deployment';
  $display->uuid = '79cd11af-918d-4897-a51d-35db85f3ae0c';
  $display->storage_type = 'page_manager';
  $display->storage_id = 'page_wetkit_deployment_source_panel_context';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-b9044f55-b545-448d-bfea-68b21eb33b98';
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
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = 'b9044f55-b545-448d-bfea-68b21eb33b98';
  $display->content['new-b9044f55-b545-448d-bfea-68b21eb33b98'] = $pane;
  $display->panels['contentmain'][0] = 'new-b9044f55-b545-448d-bfea-68b21eb33b98';
  $pane = new stdClass();
  $pane->pid = 'new-bef8bdf2-1228-4554-af65-a76d5c7bfa25';
  $pane->panel = 'contentmain';
  $pane->type = 'deploy';
  $pane->subtype = 'deploy';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => 1,
    'override_title_text' => '',
    'override_title_heading' => 'h2',
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $pane->locks = array();
  $pane->uuid = 'bef8bdf2-1228-4554-af65-a76d5c7bfa25';
  $display->content['new-bef8bdf2-1228-4554-af65-a76d5c7bfa25'] = $pane;
  $display->panels['contentmain'][1] = 'new-bef8bdf2-1228-4554-af65-a76d5c7bfa25';
  $pane = new stdClass();
  $pane->pid = 'new-6ae4a19b-ab1a-4aab-b7fc-e1f7a9922536';
  $pane->panel = 'contentmain';
  $pane->type = 'block';
  $pane->subtype = 'quicktabs-deployment_quicktabs';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
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
  $pane->uuid = '6ae4a19b-ab1a-4aab-b7fc-e1f7a9922536';
  $display->content['new-6ae4a19b-ab1a-4aab-b7fc-e1f7a9922536'] = $pane;
  $display->panels['contentmain'][2] = 'new-6ae4a19b-ab1a-4aab-b7fc-e1f7a9922536';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_wetkit_deployment_source__http_response_a45d2d28-b3a0-44d3-bab4-0a6f57082ca6';
  $handler->task = 'page';
  $handler->subtask = 'wetkit_deployment_source';
  $handler->handler = 'http_response';
  $handler->weight = -30;
  $handler->conf = array(
    'title' => 'Deploy Redirect',
    'contexts' => array(),
    'relationships' => array(),
    'code' => '301',
    'destination' => 'admin/structure/deploy',
    'name' => '',
  );
  $page->default_handlers[$handler->name] = $handler;
  $pages['wetkit_deployment_source'] = $page;

  return $pages;

}
