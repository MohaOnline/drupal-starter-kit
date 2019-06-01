<?php
/**
 * @file
 * wetkit_webform.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function wetkit_webform_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_view__panel_context_45ae25e5-04e8-494d-9e7f-762da0e60239';
  $handler->task = 'node_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = -27;
  $handler->conf = array(
    'title' => 'Webform',
    'no_blocks' => 0,
    'pipeline' => 'ipe',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => 'page-page',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'logic' => 'and',
      'plugins' => array(
        0 => array(
          'name' => 'node_type',
          'settings' => array(
            'type' => array(
              'webform' => 'webform',
            ),
          ),
          'context' => 'argument_entity_id:node_1',
          'not' => FALSE,
        ),
      ),
    ),
  );
  $display = new panels_display();
  $display->layout = 'laurier';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'pearson_top' => NULL,
      'pearson_first' => NULL,
      'pearson_second' => NULL,
      'pearson_third' => NULL,
      'pearson_bottom' => NULL,
      'laurier_top' => NULL,
      'laurier_content' => NULL,
      'laurier_bottom' => NULL,
    ),
    'pearson_first' => array(
      'style' => '0',
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '4286442d-61e8-8974-c145-399811c5794b';
  $display->storage_type = 'page_manager';
  $display->storage_id = 'node_view__panel_context_45ae25e5-04e8-494d-9e7f-762da0e60239';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-e4ef5c53-ce91-4ea4-cd0b-fecf62de32cd';
    $pane->panel = 'laurier_content';
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
    $pane->uuid = 'e4ef5c53-ce91-4ea4-cd0b-fecf62de32cd';
    $display->content['new-e4ef5c53-ce91-4ea4-cd0b-fecf62de32cd'] = $pane;
    $display->panels['laurier_content'][0] = 'new-e4ef5c53-ce91-4ea4-cd0b-fecf62de32cd';
    $pane = new stdClass();
    $pane->pid = 'new-6c1a548a-cc6a-5004-51f6-a92b7785bde7';
    $pane->panel = 'laurier_content';
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
    $pane->uuid = '6c1a548a-cc6a-5004-51f6-a92b7785bde7';
    $display->content['new-6c1a548a-cc6a-5004-51f6-a92b7785bde7'] = $pane;
    $display->panels['laurier_content'][1] = 'new-6c1a548a-cc6a-5004-51f6-a92b7785bde7';
    $pane = new stdClass();
    $pane->pid = 'new-b7b245cf-188d-48f0-8265-eba267adb76b';
    $pane->panel = 'laurier_content';
    $pane->type = 'node_content';
    $pane->subtype = 'node_content';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'links' => 1,
      'no_extras' => 1,
      'override_title' => 1,
      'override_title_text' => '',
      'identifier' => '',
      'link' => 0,
      'leave_node_title' => 0,
      'build_mode' => 'full',
      'context' => array(),
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = 'b7b245cf-188d-48f0-8265-eba267adb76b';
    $display->content['new-b7b245cf-188d-48f0-8265-eba267adb76b'] = $pane;
    $display->panels['laurier_content'][2] = 'new-b7b245cf-188d-48f0-8265-eba267adb76b';
    $pane = new stdClass();
    $pane->pid = 'new-695a12aa-7541-4c87-a116-de520b092f64';
    $pane->panel = 'laurier_content';
    $pane->type = 'entity_field_extra';
    $pane->subtype = 'node:webform';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_mode' => 'full',
      'context' => 'argument_entity_id:node_1',
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
    $pane->uuid = '695a12aa-7541-4c87-a116-de520b092f64';
    $display->content['new-695a12aa-7541-4c87-a116-de520b092f64'] = $pane;
    $display->panels['laurier_content'][3] = 'new-695a12aa-7541-4c87-a116-de520b092f64';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['node_view__panel_context_45ae25e5-04e8-494d-9e7f-762da0e60239'] = $handler;

  return $export;
}
