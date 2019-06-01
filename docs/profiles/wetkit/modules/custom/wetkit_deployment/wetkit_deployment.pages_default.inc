<?php
/**
 * @file
 * wetkit_deployment.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function wetkit_deployment_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_edit_panel_context_3';
  $handler->task = 'node_edit';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = -30;
  $handler->conf = array(
    'title' => 'Deployment',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'css_id' => 'node-edit',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'entity_bundle:node',
          'settings' => array(
            'type' => array(
              'deploy_content_moderated' => 'deploy_content_moderated',
              'deploy_content_simple' => 'deploy_content_simple',
            ),
          ),
          'context' => 'argument_node_edit_1',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
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
  $display->uuid = 'dc16c0bb-6e27-4b3f-af28-3d8a3944f3ea';
  $display->storage_type = 'page_manager';
  $display->storage_id = 'node_edit_panel_context_3';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-36cf0562-8576-4ac3-9e1f-45b944d0d482';
  $pane->panel = 'contentmain';
  $pane->type = 'page_title';
  $pane->subtype = 'page_title';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'markup' => 'h1',
    'class' => 'node-edit-title',
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
  $pane->uuid = '36cf0562-8576-4ac3-9e1f-45b944d0d482';
  $display->content['new-36cf0562-8576-4ac3-9e1f-45b944d0d482'] = $pane;
  $display->panels['contentmain'][0] = 'new-36cf0562-8576-4ac3-9e1f-45b944d0d482';
  $pane = new stdClass();
  $pane->pid = 'new-085fec49-af04-4ea5-8f58-dca3ff174bbf';
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
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $pane->locks = array();
  $pane->uuid = '085fec49-af04-4ea5-8f58-dca3ff174bbf';
  $display->content['new-085fec49-af04-4ea5-8f58-dca3ff174bbf'] = $pane;
  $display->panels['contentmain'][1] = 'new-085fec49-af04-4ea5-8f58-dca3ff174bbf';
  $pane = new stdClass();
  $pane->pid = 'new-04ae4180-538f-4f0a-95bc-deab962b4247';
  $pane->panel = 'contentmain';
  $pane->type = 'entity_form_field';
  $pane->subtype = 'node:title_field';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => '',
    'formatter' => '',
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
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = '04ae4180-538f-4f0a-95bc-deab962b4247';
  $display->content['new-04ae4180-538f-4f0a-95bc-deab962b4247'] = $pane;
  $display->panels['contentmain'][2] = 'new-04ae4180-538f-4f0a-95bc-deab962b4247';
  $pane = new stdClass();
  $pane->pid = 'new-376d030e-a37c-4e8d-8ea1-b0d451ce632b';
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
  $pane->position = 3;
  $pane->locks = array();
  $pane->uuid = '376d030e-a37c-4e8d-8ea1-b0d451ce632b';
  $display->content['new-376d030e-a37c-4e8d-8ea1-b0d451ce632b'] = $pane;
  $display->panels['contentmain'][3] = 'new-376d030e-a37c-4e8d-8ea1-b0d451ce632b';
  $pane = new stdClass();
  $pane->pid = 'new-7d5d8bb1-ae73-4db2-8dcd-b6848cd77c01';
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
  $pane->position = 4;
  $pane->locks = array();
  $pane->uuid = '7d5d8bb1-ae73-4db2-8dcd-b6848cd77c01';
  $display->content['new-7d5d8bb1-ae73-4db2-8dcd-b6848cd77c01'] = $pane;
  $display->panels['contentmain'][4] = 'new-7d5d8bb1-ae73-4db2-8dcd-b6848cd77c01';
  $pane = new stdClass();
  $pane->pid = 'new-f08aaa06-90da-4c9f-976b-85e8d9f74495';
  $pane->panel = 'contentmain';
  $pane->type = 'node_form_language';
  $pane->subtype = 'node_form_language';
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
  $pane->position = 5;
  $pane->locks = array();
  $pane->uuid = 'f08aaa06-90da-4c9f-976b-85e8d9f74495';
  $display->content['new-f08aaa06-90da-4c9f-976b-85e8d9f74495'] = $pane;
  $display->panels['contentmain'][5] = 'new-f08aaa06-90da-4c9f-976b-85e8d9f74495';
  $pane = new stdClass();
  $pane->pid = 'new-f286517d-e97b-413c-8162-64279b0ebe22';
  $pane->panel = 'contentmain';
  $pane->type = 'entity_form_field';
  $pane->subtype = 'node:body';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => '',
    'formatter' => '',
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
  $pane->position = 6;
  $pane->locks = array();
  $pane->uuid = 'f286517d-e97b-413c-8162-64279b0ebe22';
  $display->content['new-f286517d-e97b-413c-8162-64279b0ebe22'] = $pane;
  $display->panels['contentmain'][6] = 'new-f286517d-e97b-413c-8162-64279b0ebe22';
  $pane = new stdClass();
  $pane->pid = 'new-d669a05c-4b57-4ba8-ac12-b7d2e5fcd21f';
  $pane->panel = 'contentmain';
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
    'settings' => array(
      'pane_collapsed' => 1,
    ),
    'style' => 'collapsible',
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 7;
  $pane->locks = array();
  $pane->uuid = 'd669a05c-4b57-4ba8-ac12-b7d2e5fcd21f';
  $display->content['new-d669a05c-4b57-4ba8-ac12-b7d2e5fcd21f'] = $pane;
  $display->panels['contentmain'][7] = 'new-d669a05c-4b57-4ba8-ac12-b7d2e5fcd21f';
  $pane = new stdClass();
  $pane->pid = 'new-f2c133ed-cc2c-45f5-a185-c0fe53039a10';
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
  $pane->position = 8;
  $pane->locks = array();
  $pane->uuid = 'f2c133ed-cc2c-45f5-a185-c0fe53039a10';
  $display->content['new-f2c133ed-cc2c-45f5-a185-c0fe53039a10'] = $pane;
  $display->panels['contentmain'][8] = 'new-f2c133ed-cc2c-45f5-a185-c0fe53039a10';
  $pane = new stdClass();
  $pane->pid = 'new-0bd9b1ce-3978-4f11-88ba-c26980ce461a';
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
  $pane->uuid = '0bd9b1ce-3978-4f11-88ba-c26980ce461a';
  $display->content['new-0bd9b1ce-3978-4f11-88ba-c26980ce461a'] = $pane;
  $display->panels['sidebar'][0] = 'new-0bd9b1ce-3978-4f11-88ba-c26980ce461a';
  $pane = new stdClass();
  $pane->pid = 'new-2953e41d-de66-4cea-9218-d2f81f426e92';
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
  $pane->uuid = '2953e41d-de66-4cea-9218-d2f81f426e92';
  $display->content['new-2953e41d-de66-4cea-9218-d2f81f426e92'] = $pane;
  $display->panels['sidebar'][1] = 'new-2953e41d-de66-4cea-9218-d2f81f426e92';
  $pane = new stdClass();
  $pane->pid = 'new-4871c27e-c575-4b70-8d76-a3f594d58126';
  $pane->panel = 'sidebar';
  $pane->type = 'entity_form_field';
  $pane->subtype = 'node:field_deploy_to_destination_site';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => '',
    'formatter' => '',
    'context' => 'argument_node_edit_1',
    'override_title' => 1,
    'override_title_text' => 'Deployment',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = '4871c27e-c575-4b70-8d76-a3f594d58126';
  $display->content['new-4871c27e-c575-4b70-8d76-a3f594d58126'] = $pane;
  $display->panels['sidebar'][2] = 'new-4871c27e-c575-4b70-8d76-a3f594d58126';
  $pane = new stdClass();
  $pane->pid = 'new-dae0b2f7-2ef5-46a9-a94d-74af775420e7';
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
  $pane->position = 3;
  $pane->locks = array();
  $pane->uuid = 'dae0b2f7-2ef5-46a9-a94d-74af775420e7';
  $display->content['new-dae0b2f7-2ef5-46a9-a94d-74af775420e7'] = $pane;
  $display->panels['sidebar'][3] = 'new-dae0b2f7-2ef5-46a9-a94d-74af775420e7';
  $pane = new stdClass();
  $pane->pid = 'new-dc306ab3-f9b9-4ab5-a281-4fa51c2eccbc';
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
  $pane->position = 4;
  $pane->locks = array();
  $pane->uuid = 'dc306ab3-f9b9-4ab5-a281-4fa51c2eccbc';
  $display->content['new-dc306ab3-f9b9-4ab5-a281-4fa51c2eccbc'] = $pane;
  $display->panels['sidebar'][4] = 'new-dc306ab3-f9b9-4ab5-a281-4fa51c2eccbc';
  $pane = new stdClass();
  $pane->pid = 'new-df0699d8-3c1a-452a-b8e7-5d1e8d1fe522';
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
  $pane->position = 5;
  $pane->locks = array();
  $pane->uuid = 'df0699d8-3c1a-452a-b8e7-5d1e8d1fe522';
  $display->content['new-df0699d8-3c1a-452a-b8e7-5d1e8d1fe522'] = $pane;
  $display->panels['sidebar'][5] = 'new-df0699d8-3c1a-452a-b8e7-5d1e8d1fe522';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-f2c133ed-cc2c-45f5-a185-c0fe53039a10';
  $handler->conf['display'] = $display;
  $export['node_edit_panel_context_3'] = $handler;

  return $export;
}
