<?php
/**
 * @file
 * wetkit_pages.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function wetkit_pages_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_edit_panel_context_2';
  $handler->task = 'node_edit';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = -1;
  $handler->conf = array(
    'title' => 'Basic Page',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'css_id' => 'node-edit',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
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
  $display->uuid = 'b1980bcb-88c1-472e-b3c0-149a7a33fa4a';
  $display->storage_type = 'page_manager';
  $display->storage_id = 'node_edit_panel_context_2';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-93198d91-afcb-4c76-8075-0752734f670f';
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
  $pane->uuid = '93198d91-afcb-4c76-8075-0752734f670f';
  $display->content['new-93198d91-afcb-4c76-8075-0752734f670f'] = $pane;
  $display->panels['contentmain'][0] = 'new-93198d91-afcb-4c76-8075-0752734f670f';
  $pane = new stdClass();
  $pane->pid = 'new-ece4a332-ee13-4c15-8f75-d9bdda90e0c2';
  $pane->panel = 'contentmain';
  $pane->type = 'page_tabs';
  $pane->subtype = 'page_tabs';
  $pane->shown = TRUE;
  $pane->access = array(
    'plugins' => array(),
  );
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
  $pane->uuid = 'ece4a332-ee13-4c15-8f75-d9bdda90e0c2';
  $display->content['new-ece4a332-ee13-4c15-8f75-d9bdda90e0c2'] = $pane;
  $display->panels['contentmain'][1] = 'new-ece4a332-ee13-4c15-8f75-d9bdda90e0c2';
  $pane = new stdClass();
  $pane->pid = 'new-3d5f62f6-e983-4d54-94c8-a8b9676a6225';
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
  $pane->uuid = '3d5f62f6-e983-4d54-94c8-a8b9676a6225';
  $display->content['new-3d5f62f6-e983-4d54-94c8-a8b9676a6225'] = $pane;
  $display->panels['contentmain'][2] = 'new-3d5f62f6-e983-4d54-94c8-a8b9676a6225';
  $pane = new stdClass();
  $pane->pid = 'new-21bc4842-63ef-4125-bda5-5436f393cc3d';
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
  $pane->uuid = '21bc4842-63ef-4125-bda5-5436f393cc3d';
  $display->content['new-21bc4842-63ef-4125-bda5-5436f393cc3d'] = $pane;
  $display->panels['contentmain'][3] = 'new-21bc4842-63ef-4125-bda5-5436f393cc3d';
  $pane = new stdClass();
  $pane->pid = 'new-dc60f60d-f1ff-4fcb-b54b-fcbc5c9cb90e';
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
  $pane->uuid = 'dc60f60d-f1ff-4fcb-b54b-fcbc5c9cb90e';
  $display->content['new-dc60f60d-f1ff-4fcb-b54b-fcbc5c9cb90e'] = $pane;
  $display->panels['contentmain'][4] = 'new-dc60f60d-f1ff-4fcb-b54b-fcbc5c9cb90e';
  $pane = new stdClass();
  $pane->pid = 'new-cf2cd409-1f3c-4d09-a945-8ac3c132830c';
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
  $pane->uuid = 'cf2cd409-1f3c-4d09-a945-8ac3c132830c';
  $display->content['new-cf2cd409-1f3c-4d09-a945-8ac3c132830c'] = $pane;
  $display->panels['contentmain'][5] = 'new-cf2cd409-1f3c-4d09-a945-8ac3c132830c';
  $pane = new stdClass();
  $pane->pid = 'new-1e68352d-c6b4-4f3b-9688-352b823705ae';
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
  $pane->uuid = '1e68352d-c6b4-4f3b-9688-352b823705ae';
  $display->content['new-1e68352d-c6b4-4f3b-9688-352b823705ae'] = $pane;
  $display->panels['contentmain'][6] = 'new-1e68352d-c6b4-4f3b-9688-352b823705ae';
  $pane = new stdClass();
  $pane->pid = 'new-f22f6afe-2ea4-460f-8914-75e81dce294b';
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
  $pane->uuid = 'f22f6afe-2ea4-460f-8914-75e81dce294b';
  $display->content['new-f22f6afe-2ea4-460f-8914-75e81dce294b'] = $pane;
  $display->panels['contentmain'][7] = 'new-f22f6afe-2ea4-460f-8914-75e81dce294b';
  $pane = new stdClass();
  $pane->pid = 'new-a347c471-8d72-49de-9531-1e16e7ebfcde';
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
  $pane->uuid = 'a347c471-8d72-49de-9531-1e16e7ebfcde';
  $display->content['new-a347c471-8d72-49de-9531-1e16e7ebfcde'] = $pane;
  $display->panels['contentmain'][8] = 'new-a347c471-8d72-49de-9531-1e16e7ebfcde';
  $pane = new stdClass();
  $pane->pid = 'new-c183ed44-e93a-4b56-9fa6-ca7dfb8b989e';
  $pane->panel = 'sidebar';
  $pane->type = 'entity_form_field';
  $pane->subtype = 'node:field_featured_status';
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
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = 'c183ed44-e93a-4b56-9fa6-ca7dfb8b989e';
  $display->content['new-c183ed44-e93a-4b56-9fa6-ca7dfb8b989e'] = $pane;
  $display->panels['sidebar'][0] = 'new-c183ed44-e93a-4b56-9fa6-ca7dfb8b989e';
  $pane = new stdClass();
  $pane->pid = 'new-1d7eb320-e647-4482-9c77-07720860839a';
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
  $pane->position = 1;
  $pane->locks = array();
  $pane->uuid = '1d7eb320-e647-4482-9c77-07720860839a';
  $display->content['new-1d7eb320-e647-4482-9c77-07720860839a'] = $pane;
  $display->panels['sidebar'][1] = 'new-1d7eb320-e647-4482-9c77-07720860839a';
  $pane = new stdClass();
  $pane->pid = 'new-1a7e2fef-acc6-4532-959b-6d15984feee0';
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
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = '1a7e2fef-acc6-4532-959b-6d15984feee0';
  $display->content['new-1a7e2fef-acc6-4532-959b-6d15984feee0'] = $pane;
  $display->panels['sidebar'][2] = 'new-1a7e2fef-acc6-4532-959b-6d15984feee0';
  $pane = new stdClass();
  $pane->pid = 'new-a5da7071-5b97-4bd0-9820-e7472af1d170';
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
  $pane->uuid = 'a5da7071-5b97-4bd0-9820-e7472af1d170';
  $display->content['new-a5da7071-5b97-4bd0-9820-e7472af1d170'] = $pane;
  $display->panels['sidebar'][3] = 'new-a5da7071-5b97-4bd0-9820-e7472af1d170';
  $pane = new stdClass();
  $pane->pid = 'new-d0d88d80-0486-40d8-9c57-d0a8964dbf84';
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
  $pane->uuid = 'd0d88d80-0486-40d8-9c57-d0a8964dbf84';
  $display->content['new-d0d88d80-0486-40d8-9c57-d0a8964dbf84'] = $pane;
  $display->panels['sidebar'][4] = 'new-d0d88d80-0486-40d8-9c57-d0a8964dbf84';
  $pane = new stdClass();
  $pane->pid = 'new-a9f0ee0e-ef54-459f-bd86-0bc2ecf1d648';
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
  $pane->uuid = 'a9f0ee0e-ef54-459f-bd86-0bc2ecf1d648';
  $display->content['new-a9f0ee0e-ef54-459f-bd86-0bc2ecf1d648'] = $pane;
  $display->panels['sidebar'][5] = 'new-a9f0ee0e-ef54-459f-bd86-0bc2ecf1d648';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-a347c471-8d72-49de-9531-1e16e7ebfcde';
  $handler->conf['display'] = $display;
  $export['node_edit_panel_context_2'] = $handler;

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_view_panel_context_3';
  $handler->task = 'node_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 100;
  $handler->conf = array(
    'title' => 'Node',
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
      'plugins' => array(),
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
      'pearson_third' => NULL,
      'pearson_bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '1bf15d51-1e58-4020-a094-07e73bcc4d53';
  $display->storage_type = 'page_manager';
  $display->storage_id = 'node_view_panel_context_3';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-8437dbf9-f722-45bd-a04f-4479156d8bf6';
  $pane->panel = 'pearson_first';
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
  $pane->uuid = '8437dbf9-f722-45bd-a04f-4479156d8bf6';
  $display->content['new-8437dbf9-f722-45bd-a04f-4479156d8bf6'] = $pane;
  $display->panels['pearson_first'][0] = 'new-8437dbf9-f722-45bd-a04f-4479156d8bf6';
  $pane = new stdClass();
  $pane->pid = 'new-548f6dd8-b51f-42dc-a2d4-381485f1398f';
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
  $pane->uuid = '548f6dd8-b51f-42dc-a2d4-381485f1398f';
  $display->content['new-548f6dd8-b51f-42dc-a2d4-381485f1398f'] = $pane;
  $display->panels['pearson_first'][1] = 'new-548f6dd8-b51f-42dc-a2d4-381485f1398f';
  $pane = new stdClass();
  $pane->pid = 'new-fef5f38e-474a-4752-9c0b-b8f465cf820e';
  $pane->panel = 'pearson_first';
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
  $pane->uuid = 'fef5f38e-474a-4752-9c0b-b8f465cf820e';
  $display->content['new-fef5f38e-474a-4752-9c0b-b8f465cf820e'] = $pane;
  $display->panels['pearson_first'][2] = 'new-fef5f38e-474a-4752-9c0b-b8f465cf820e';
  $pane = new stdClass();
  $pane->pid = 'new-9370734e-69d4-46b7-80a1-1b9fc3b17091';
  $pane->panel = 'pearson_first';
  $pane->type = 'node_updated';
  $pane->subtype = 'node_updated';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'format' => 'short',
    'context' => 'argument_entity_id:node_1',
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
  $pane->uuid = '9370734e-69d4-46b7-80a1-1b9fc3b17091';
  $display->content['new-9370734e-69d4-46b7-80a1-1b9fc3b17091'] = $pane;
  $display->panels['pearson_first'][3] = 'new-9370734e-69d4-46b7-80a1-1b9fc3b17091';
  $pane = new stdClass();
  $pane->pid = 'new-99569479-f08f-4583-829d-b141df43000b';
  $pane->panel = 'pearson_second';
  $pane->type = 'menu_tree';
  $pane->subtype = 'sidebar';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'menu_name' => 'sidebar',
    'parent_mlid' => 0,
    'show_menu_items' => array(),
    'parent' => 'sidebar:0',
    'title_link' => 0,
    'admin_title' => '',
    'level' => '1',
    'follow' => 0,
    'depth' => '0',
    'expanded' => 1,
    'sort' => 0,
    'override_title' => 1,
    'override_title_text' => '',
    'title_embed' => 0,
    'theme' => 'menu_link__menu_block__wxt_sidebar',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '99569479-f08f-4583-829d-b141df43000b';
  $display->content['new-99569479-f08f-4583-829d-b141df43000b'] = $pane;
  $display->panels['pearson_second'][0] = 'new-99569479-f08f-4583-829d-b141df43000b';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-99569479-f08f-4583-829d-b141df43000b';
  $handler->conf['display'] = $display;
  $export['node_view_panel_context_3'] = $handler;

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_view_panel_context_4';
  $handler->task = 'node_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 99;
  $handler->conf = array(
    'title' => 'Node (WxT)',
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
              'wetkit_page' => 'wetkit_page',
            ),
          ),
          'context' => 'argument_entity_id:node_1',
          'not' => FALSE,
        ),
      ),
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
      'pearson_third' => NULL,
      'pearson_bottom' => NULL,
    ),
    'pearson_first' => array(
      'style' => '0',
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '3bbeea49-7539-4b22-90c7-a0a1d588106b';
  $display->storage_type = 'page_manager';
  $display->storage_id = 'node_view_panel_context_4';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-aa132602-332c-40a7-8634-b3ecfce3a075';
  $pane->panel = 'pearson_first';
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
  $pane->uuid = 'aa132602-332c-40a7-8634-b3ecfce3a075';
  $display->content['new-aa132602-332c-40a7-8634-b3ecfce3a075'] = $pane;
  $display->panels['pearson_first'][0] = 'new-aa132602-332c-40a7-8634-b3ecfce3a075';
  $pane = new stdClass();
  $pane->pid = 'new-1e302bb0-8226-4d3b-b2ee-a5ca14106935';
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
  $pane->uuid = '1e302bb0-8226-4d3b-b2ee-a5ca14106935';
  $display->content['new-1e302bb0-8226-4d3b-b2ee-a5ca14106935'] = $pane;
  $display->panels['pearson_first'][1] = 'new-1e302bb0-8226-4d3b-b2ee-a5ca14106935';
  $pane = new stdClass();
  $pane->pid = 'new-06f702e1-7dcb-4859-8646-e196e4c81a5d';
  $pane->panel = 'pearson_first';
  $pane->type = 'archived_widget';
  $pane->subtype = 'archived_widget';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => 'argument_entity_id:node_1',
    'span_item' => 'span-6',
    'form_build_id' => 'form-xLkW9cJZ71w6g_X2K4LiskkkbULD0eXhQJkexWvx0Ls',
    'form_token' => 'XFsHblzUTyYAErcjF3uZ-shvhu2EtcziOaPfmRs3W30',
    'form_id' => 'wetkit_wetboew_archived_widget_edit_form',
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
  $pane->uuid = '06f702e1-7dcb-4859-8646-e196e4c81a5d';
  $display->content['new-06f702e1-7dcb-4859-8646-e196e4c81a5d'] = $pane;
  $display->panels['pearson_first'][2] = 'new-06f702e1-7dcb-4859-8646-e196e4c81a5d';
  $pane = new stdClass();
  $pane->pid = 'new-505f19a8-a0c9-4c36-ab92-711267c3cfa9';
  $pane->panel = 'pearson_first';
  $pane->type = 'entity_field';
  $pane->subtype = 'node:field_featured_image';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => 'title',
    'formatter' => 'wetkit_widgets_lightbox',
    'delta_limit' => 0,
    'delta_offset' => '0',
    'delta_reversed' => FALSE,
    'formatter_settings' => array(
      'file_view_mode' => 'default',
      'image_link' => '',
      'picture_group' => 'wxt_responsive_image',
      'fallback_image_style' => '',
      'colorbox' => 'wxt_responsive_image',
      'image_style_destination' => 'wetkit_image_half',
      'image_style_source' => 'wetkit_image_half',
      'custom_class' => '',
    ),
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
  $pane->position = 3;
  $pane->locks = array();
  $pane->uuid = '505f19a8-a0c9-4c36-ab92-711267c3cfa9';
  $display->content['new-505f19a8-a0c9-4c36-ab92-711267c3cfa9'] = $pane;
  $display->panels['pearson_first'][3] = 'new-505f19a8-a0c9-4c36-ab92-711267c3cfa9';
  $pane = new stdClass();
  $pane->pid = 'new-d7ae79c9-99c4-4b11-85be-f01600ba49fe';
  $pane->panel = 'pearson_first';
  $pane->type = 'entity_field';
  $pane->subtype = 'node:body';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => 'title',
    'formatter' => 'text_default',
    'delta_limit' => 0,
    'delta_offset' => '0',
    'delta_reversed' => FALSE,
    'formatter_settings' => array(),
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
  $pane->position = 4;
  $pane->locks = array();
  $pane->uuid = 'd7ae79c9-99c4-4b11-85be-f01600ba49fe';
  $display->content['new-d7ae79c9-99c4-4b11-85be-f01600ba49fe'] = $pane;
  $display->panels['pearson_first'][4] = 'new-d7ae79c9-99c4-4b11-85be-f01600ba49fe';
  $pane = new stdClass();
  $pane->pid = 'new-6cba6356-6fca-4013-b37f-586ecc43e30a';
  $pane->panel = 'pearson_first';
  $pane->type = 'entity_field';
  $pane->subtype = 'node:field_featured_categories';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => 'title',
    'formatter' => 'taxonomy_term_reference_link',
    'delta_limit' => '0',
    'delta_offset' => '0',
    'delta_reversed' => 0,
    'formatter_settings' => array(),
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
  $pane->uuid = '6cba6356-6fca-4013-b37f-586ecc43e30a';
  $display->content['new-6cba6356-6fca-4013-b37f-586ecc43e30a'] = $pane;
  $display->panels['pearson_first'][5] = 'new-6cba6356-6fca-4013-b37f-586ecc43e30a';
  $pane = new stdClass();
  $pane->pid = 'new-ddb8bcf4-4eb0-4c2d-99bb-5a361a034cc1';
  $pane->panel = 'pearson_first';
  $pane->type = 'date_modified';
  $pane->subtype = 'date_modified';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array();
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 6;
  $pane->locks = array();
  $pane->uuid = 'ddb8bcf4-4eb0-4c2d-99bb-5a361a034cc1';
  $display->content['new-ddb8bcf4-4eb0-4c2d-99bb-5a361a034cc1'] = $pane;
  $display->panels['pearson_first'][6] = 'new-ddb8bcf4-4eb0-4c2d-99bb-5a361a034cc1';
  $pane = new stdClass();
  $pane->pid = 'new-b8ebe185-dc0e-43da-8873-decde6dffdab';
  $pane->panel = 'pearson_second';
  $pane->type = 'menu_tree';
  $pane->subtype = 'sidebar';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'menu_name' => 'sidebar',
    'parent_mlid' => 0,
    'show_menu_items' => array(),
    'parent' => 'sidebar:0',
    'title_link' => 0,
    'admin_title' => '',
    'level' => '1',
    'follow' => 0,
    'depth' => '0',
    'expanded' => 1,
    'sort' => 0,
    'override_title' => 1,
    'override_title_text' => '',
    'title_embed' => 0,
    'theme' => 'menu_link__menu_block__wxt_sidebar',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = 'b8ebe185-dc0e-43da-8873-decde6dffdab';
  $display->content['new-b8ebe185-dc0e-43da-8873-decde6dffdab'] = $pane;
  $display->panels['pearson_second'][0] = 'new-b8ebe185-dc0e-43da-8873-decde6dffdab';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-b8ebe185-dc0e-43da-8873-decde6dffdab';
  $handler->conf['display'] = $display;
  $export['node_view_panel_context_4'] = $handler;

  return $export;
}
