<?php
/**
 * @file
 * wetkit_migrate.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function wetkit_migrate_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_view_panel_context';
  $handler->task = 'node_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 99;
  $handler->conf = array(
    'title' => 'Migrate (Sidebar)',
    'no_blocks' => 0,
    'pipeline' => 'ipe',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'node_type',
          'settings' => array(
            'type' => array(
              'wetkit_documentation' => 'wetkit_documentation',
            ),
          ),
          'context' => 'argument_entity_id:node_1',
          'not' => FALSE,
        ),
        1 => array(
          'name' => 'entity_field_value:node:wetkit_documentation:field_column',
          'settings' => array(
            'field_column' => array(
              'und' => array(
                0 => array(
                  'value' => 1,
                ),
              ),
            ),
            'field_column_value' => '1',
          ),
          'context' => 'argument_entity_id:node_1',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $display = new panels_display();
  $display->layout = 'pearson';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'pearson_top' => NULL,
      'pearson_first' => NULL,
      'pearson_second' => NULL,
      'pearson_bottom' => NULL,
      'pearson_third' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '%node:title';
  $display->uuid = 'ae003499-e39f-41e5-9ee4-b6ab4380015b';
  $display->storage_type = 'page_manager';
  $display->storage_id = 'node_view_panel_context';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-4217ce05-63d2-1424-8d4d-9a39e356d000';
  $pane->panel = 'pearson_first';
  $pane->type = 'page_title';
  $pane->subtype = 'page_title';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'markup' => 'h1',
    'class' => '',
    'id' => '',
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
  $pane->uuid = '4217ce05-63d2-1424-8d4d-9a39e356d000';
  $display->content['new-4217ce05-63d2-1424-8d4d-9a39e356d000'] = $pane;
  $display->panels['pearson_first'][0] = 'new-4217ce05-63d2-1424-8d4d-9a39e356d000';
  $pane = new stdClass();
  $pane->pid = 'new-ca205368-4d28-4e66-a687-c5860bd06eeb';
  $pane->panel = 'pearson_first';
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
  $pane->uuid = 'ca205368-4d28-4e66-a687-c5860bd06eeb';
  $display->content['new-ca205368-4d28-4e66-a687-c5860bd06eeb'] = $pane;
  $display->panels['pearson_first'][1] = 'new-ca205368-4d28-4e66-a687-c5860bd06eeb';
  $pane = new stdClass();
  $pane->pid = 'new-a47e7a75-0cb7-4e3a-93dd-35de03f3cce2';
  $pane->panel = 'pearson_first';
  $pane->type = 'entity_view';
  $pane->subtype = 'node';
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
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = 'a47e7a75-0cb7-4e3a-93dd-35de03f3cce2';
  $display->content['new-a47e7a75-0cb7-4e3a-93dd-35de03f3cce2'] = $pane;
  $display->panels['pearson_first'][2] = 'new-a47e7a75-0cb7-4e3a-93dd-35de03f3cce2';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['node_view_panel_context'] = $handler;

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_view_panel_context_2';
  $handler->task = 'node_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 98;
  $handler->conf = array(
    'title' => 'Migrate',
    'no_blocks' => 0,
    'pipeline' => 'ipe',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'node_type',
          'settings' => array(
            'type' => array(
              'wetkit_documentation' => 'wetkit_documentation',
            ),
          ),
          'context' => 'argument_entity_id:node_1',
          'not' => FALSE,
        ),
        1 => array(
          'name' => 'entity_field_value:node:wetkit_documentation:field_column',
          'settings' => array(
            'field_column' => array(
              'und' => array(
                0 => array(
                  'value' => 1,
                ),
              ),
            ),
            'field_column_value' => '1',
          ),
          'context' => 'argument_entity_id:node_1',
          'not' => TRUE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $display = new panels_display();
  $display->layout = 'pearson';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'pearson_top' => NULL,
      'pearson_first' => NULL,
      'pearson_second' => NULL,
      'pearson_bottom' => NULL,
      'pearson_third' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '%node:title';
  $display->uuid = '82a39aae-45d3-5c74-9563-9354123cf4a3';
  $display->storage_type = 'page_manager';
  $display->storage_id = 'node_view_panel_context_2';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-a266530a-d6af-4be2-91a8-3478365d5ba1';
  $pane->panel = 'pearson_first';
  $pane->type = 'page_title';
  $pane->subtype = 'page_title';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'markup' => 'h1',
    'class' => '',
    'id' => '',
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
  $pane->uuid = 'a266530a-d6af-4be2-91a8-3478365d5ba1';
  $display->content['new-a266530a-d6af-4be2-91a8-3478365d5ba1'] = $pane;
  $display->panels['pearson_first'][0] = 'new-a266530a-d6af-4be2-91a8-3478365d5ba1';
  $pane = new stdClass();
  $pane->pid = 'new-6fe40395-00db-4d5c-98af-18ac73794bab';
  $pane->panel = 'pearson_first';
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
  $pane->uuid = '6fe40395-00db-4d5c-98af-18ac73794bab';
  $display->content['new-6fe40395-00db-4d5c-98af-18ac73794bab'] = $pane;
  $display->panels['pearson_first'][1] = 'new-6fe40395-00db-4d5c-98af-18ac73794bab';
  $pane = new stdClass();
  $pane->pid = 'new-3902be93-949c-4c2b-8430-e4398443cb78';
  $pane->panel = 'pearson_first';
  $pane->type = 'entity_view';
  $pane->subtype = 'node';
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
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = '3902be93-949c-4c2b-8430-e4398443cb78';
  $display->content['new-3902be93-949c-4c2b-8430-e4398443cb78'] = $pane;
  $display->panels['pearson_first'][2] = 'new-3902be93-949c-4c2b-8430-e4398443cb78';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['node_view_panel_context_2'] = $handler;

  return $export;
}
