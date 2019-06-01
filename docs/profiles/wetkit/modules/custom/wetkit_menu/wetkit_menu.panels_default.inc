<?php
/**
 * @file
 * wetkit_menu.panels_default.inc
 */

/**
 * Implements hook_default_panels_mini().
 */
function wetkit_menu_default_panels_mini() {
  $export = array();

  $mini = new stdClass();
  $mini->disabled = FALSE; /* Edit this to true to make a default mini disabled initially */
  $mini->api_version = 1;
  $mini->name = 'wetkit_mini_panels_drupal_github';
  $mini->category = 'WetKit';
  $mini->admin_title = 'WetKit Mini Panels: GitHub';
  $mini->admin_description = 'WetKit Mini Panels: Drupal GitHub';
  $mini->requiredcontexts = array();
  $mini->contexts = array();
  $mini->relationships = array();
  $display = new panels_display();
  $display->layout = 'cohen';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'cohen_left' => NULL,
      'cohen_middle_left' => NULL,
      'cohen_middle_right' => NULL,
      'cohen_right' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '42b9d303-1d98-44e7-bcde-3be49400f890';
  $display->storage_type = 'panels_mini';
  $display->storage_id = 'wetkit_mini_panels_drupal_github';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-98dba45e-011a-4584-bcce-7b262994c19f';
  $pane->panel = 'cohen_left';
  $pane->type = 'menu_tree';
  $pane->subtype = 'megamenu';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'menu_name' => 'megamenu',
    'parent_mlid' => _wetkit_menu_get_mlid('megamenu_eng_3_a'),
    'parent' => 'megamenu:' . _wetkit_menu_get_mlid('megamenu_eng_3_a'),
    'title_link' => 1,
    'admin_title' => '',
    'level' => '1',
    'follow' => 0,
    'depth' => '0',
    'expanded' => 1,
    'sort' => 0,
    'override_title' => 0,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => array(
      'wetkit_menu_settings_wrap_menu_items' => '1',
      'wetkit_menu_settings_custom_css' => '',
    ),
    'style' => 'wetkit_menu_settings',
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '98dba45e-011a-4584-bcce-7b262994c19f';
  $display->content['new-98dba45e-011a-4584-bcce-7b262994c19f'] = $pane;
  $display->panels['cohen_left'][0] = 'new-98dba45e-011a-4584-bcce-7b262994c19f';
  $pane = new stdClass();
  $pane->pid = 'new-419ca9af-1dc2-4060-8f8c-715a4790c820';
  $pane->panel = 'cohen_middle_left';
  $pane->type = 'menu_tree';
  $pane->subtype = 'megamenu';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'menu_name' => 'megamenu',
    'parent_mlid' => _wetkit_menu_get_mlid('megamenu_eng_3_e'),
    'parent' => 'megamenu:' . _wetkit_menu_get_mlid('megamenu_eng_3_e'),
    'title_link' => 1,
    'admin_title' => '',
    'level' => '1',
    'follow' => 0,
    'depth' => '0',
    'expanded' => 1,
    'sort' => 0,
    'override_title' => 0,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => array(
      'wetkit_menu_settings_wrap_menu_items' => '1',
      'wetkit_menu_settings_custom_css' => '',
    ),
    'style' => 'wetkit_menu_settings',
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '419ca9af-1dc2-4060-8f8c-715a4790c820';
  $display->content['new-419ca9af-1dc2-4060-8f8c-715a4790c820'] = $pane;
  $display->panels['cohen_middle_left'][0] = 'new-419ca9af-1dc2-4060-8f8c-715a4790c820';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-98dba45e-011a-4584-bcce-7b262994c19f';
  $mini->display = $display;
  $export['wetkit_mini_panels_drupal_github'] = $mini;

  $mini = new stdClass();
  $mini->disabled = FALSE; /* Edit this to true to make a default mini disabled initially */
  $mini->api_version = 1;
  $mini->name = 'wetkit_mini_panels_drupal_wcms';
  $mini->category = 'WetKit';
  $mini->admin_title = 'WetKit Mini Panels: Drupal WCMS';
  $mini->admin_description = 'WetKit Mini Panels: Drupal WCMS';
  $mini->requiredcontexts = array();
  $mini->contexts = array();
  $mini->relationships = array();
  $display = new panels_display();
  $display->layout = 'cohen';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'cohen_left' => NULL,
      'cohen_middle_left' => NULL,
      'cohen_middle_right' => NULL,
      'cohen_right' => NULL,
      'cohen_top' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'fecc816d-9197-4f0f-85b1-d7ddfe600aa1';
  $display->storage_type = 'panels_mini';
  $display->storage_id = 'wetkit_mini_panels_drupal_wcms';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-ccb49f22-ab6e-42e0-940d-6519d835be1c';
  $pane->panel = 'cohen_left';
  $pane->type = 'menu_tree';
  $pane->subtype = 'megamenu';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'menu_name' => 'megamenu',
    'parent_mlid' => _wetkit_menu_get_mlid('megamenu_eng_2'),
    'parent' => 'megamenu:' . _wetkit_menu_get_mlid('megamenu_eng_2'),
    'title_link' => 0,
    'admin_title' => '',
    'level' => '1',
    'follow' => 0,
    'depth' => '0',
    'expanded' => 1,
    'sort' => 0,
    'override_title' => 1,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => array(
      'wetkit_menu_settings_strip_sub_items' => '1',
      'wetkit_menu_settings_wrap_pane' => '0',
      'wetkit_menu_settings_wrap_menu_items' => '1',
      'wetkit_menu_settings_custom_css' => 'top-level',
    ),
    'style' => 'wetkit_menu_settings',
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = 'ccb49f22-ab6e-42e0-940d-6519d835be1c';
  $display->content['new-ccb49f22-ab6e-42e0-940d-6519d835be1c'] = $pane;
  $display->panels['cohen_left'][0] = 'new-ccb49f22-ab6e-42e0-940d-6519d835be1c';
  $pane = new stdClass();
  $pane->pid = 'new-a81f5835-b2fe-4aae-b93d-087479ee1110';
  $pane->panel = 'cohen_left';
  $pane->type = 'menu_tree';
  $pane->subtype = 'megamenu';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'menu_name' => 'megamenu',
    'parent_mlid' => _wetkit_menu_get_mlid('megamenu_eng_2_d'),
    'parent' => 'megamenu:' . _wetkit_menu_get_mlid('megamenu_eng_2_d'),
    'title_link' => 1,
    'admin_title' => '',
    'level' => '1',
    'follow' => 0,
    'depth' => '0',
    'expanded' => 1,
    'sort' => 0,
    'override_title' => 0,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => array(
      'wetkit_menu_settings_wrap_menu_items' => '1',
      'wetkit_menu_settings_custom_css' => '',
    ),
    'style' => 'wetkit_menu_settings',
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $pane->locks = array();
  $pane->uuid = 'a81f5835-b2fe-4aae-b93d-087479ee1110';
  $display->content['new-a81f5835-b2fe-4aae-b93d-087479ee1110'] = $pane;
  $display->panels['cohen_left'][1] = 'new-a81f5835-b2fe-4aae-b93d-087479ee1110';
  $pane = new stdClass();
  $pane->pid = 'new-484512dd-e7fb-4c8b-a5d1-1f2979afb5f9';
  $pane->panel = 'cohen_middle_left';
  $pane->type = 'menu_tree';
  $pane->subtype = 'megamenu';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'menu_name' => 'megamenu',
    'parent_mlid' => _wetkit_menu_get_mlid('megamenu_eng_2_g'),
    'parent' => 'megamenu:' . _wetkit_menu_get_mlid('megamenu_eng_2_g'),
    'title_link' => 1,
    'admin_title' => '',
    'level' => '1',
    'follow' => 0,
    'depth' => '0',
    'expanded' => 1,
    'sort' => 0,
    'override_title' => 0,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => array(
      'wetkit_menu_settings_wrap_menu_items' => '1',
      'wetkit_menu_settings_custom_css' => '',
    ),
    'style' => 'wetkit_menu_settings',
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '484512dd-e7fb-4c8b-a5d1-1f2979afb5f9';
  $display->content['new-484512dd-e7fb-4c8b-a5d1-1f2979afb5f9'] = $pane;
  $display->panels['cohen_middle_left'][0] = 'new-484512dd-e7fb-4c8b-a5d1-1f2979afb5f9';
  $pane = new stdClass();
  $pane->pid = 'new-dfbd6769-52d3-4617-af5d-53d4db2d6c56';
  $pane->panel = 'cohen_top';
  $pane->type = 'menu_tree';
  $pane->subtype = 'megamenu-focus';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'menu_name' => 'megamenu-focus',
    'parent_mlid' => _wetkit_menu_get_mlid('megamenu_focus_eng_1'),
    'parent' => 'megamenu-focus:' . _wetkit_menu_get_mlid('megamenu_focus_eng_1'),
    'title_link' => 0,
    'admin_title' => '',
    'level' => '1',
    'follow' => 0,
    'depth' => '0',
    'expanded' => 1,
    'sort' => 0,
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
  $pane->uuid = 'dfbd6769-52d3-4617-af5d-53d4db2d6c56';
  $display->content['new-dfbd6769-52d3-4617-af5d-53d4db2d6c56'] = $pane;
  $display->panels['cohen_top'][0] = 'new-dfbd6769-52d3-4617-af5d-53d4db2d6c56';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-a81f5835-b2fe-4aae-b93d-087479ee1110';
  $mini->display = $display;
  $export['wetkit_mini_panels_drupal_wcms'] = $mini;

  $mini = new stdClass();
  $mini->disabled = FALSE; /* Edit this to true to make a default mini disabled initially */
  $mini->api_version = 1;
  $mini->name = 'wetkit_mini_panels_example_pages';
  $mini->category = 'WetKit';
  $mini->admin_title = 'WetKit Mini Panels: Example Pages';
  $mini->admin_description = 'WetKit Mini Panels: Example Pages';
  $mini->requiredcontexts = array();
  $mini->contexts = array();
  $mini->relationships = array();
  $display = new panels_display();
  $display->layout = 'cohen';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'cohen_left' => NULL,
      'cohen_middle_left' => NULL,
      'cohen_middle_right' => NULL,
      'cohen_right' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '956718df-de86-45c2-82ac-a9aff717a8dd';
  $display->storage_type = 'panels_mini';
  $display->storage_id = 'wetkit_mini_panels_example_pages';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-72dbf014-720f-4c31-9ae8-6d9af747caf8';
  $pane->panel = 'cohen_left';
  $pane->type = 'menu_tree';
  $pane->subtype = 'megamenu';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'menu_name' => 'megamenu',
    'parent_mlid' => _wetkit_menu_get_mlid('megamenu_eng_4'),
    'parent' => 'megamenu:' . _wetkit_menu_get_mlid('megamenu_eng_4'),
    'title_link' => 0,
    'admin_title' => '',
    'level' => '1',
    'follow' => 0,
    'depth' => '0',
    'expanded' => 1,
    'sort' => 0,
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
  $pane->uuid = '72dbf014-720f-4c31-9ae8-6d9af747caf8';
  $display->content['new-72dbf014-720f-4c31-9ae8-6d9af747caf8'] = $pane;
  $display->panels['cohen_left'][0] = 'new-72dbf014-720f-4c31-9ae8-6d9af747caf8';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-72dbf014-720f-4c31-9ae8-6d9af747caf8';
  $mini->display = $display;
  $export['wetkit_mini_panels_example_pages'] = $mini;

  $mini = new stdClass();
  $mini->disabled = FALSE; /* Edit this to true to make a default mini disabled initially */
  $mini->api_version = 1;
  $mini->name = 'wetkit_mini_panels_home';
  $mini->category = 'WetKit';
  $mini->admin_title = 'WetKit Mini Panels: Home';
  $mini->admin_description = 'WetKit Mini Panels: Home';
  $mini->requiredcontexts = array();
  $mini->contexts = array();
  $mini->relationships = array();
  $display = new panels_display();
  $display->layout = 'cohen';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'cohen_left' => NULL,
      'cohen_middle_left' => NULL,
      'cohen_middle_right' => NULL,
      'cohen_right' => NULL,
      'cohen_top' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '8dd53cec-4d69-40f5-aaf2-55a797bb7201';
  $display->storage_type = 'panels_mini';
  $display->storage_id = 'wetkit_mini_panels_home';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-47d8fd44-231d-402d-8fa2-ab2305f02b88';
  $pane->panel = 'cohen_left';
  $pane->type = 'menu_tree';
  $pane->subtype = 'megamenu';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'menu_name' => 'megamenu',
    'parent_mlid' => _wetkit_menu_get_mlid('megamenu_eng_1'),
    'parent' => 'megamenu:' . _wetkit_menu_get_mlid('megamenu_eng_1'),
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
    'theme' => '',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => array(
      'wetkit_menu_settings_strip_sub_items' => '0',
      'wetkit_menu_settings_wrap_pane' => '0',
      'wetkit_menu_settings_wrap_menu_items' => '0',
      'wetkit_menu_settings_custom_css' => '',
    ),
    'style' => 'wetkit_menu_settings',
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '47d8fd44-231d-402d-8fa2-ab2305f02b88';
  $display->content['new-47d8fd44-231d-402d-8fa2-ab2305f02b88'] = $pane;
  $display->panels['cohen_left'][0] = 'new-47d8fd44-231d-402d-8fa2-ab2305f02b88';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-47d8fd44-231d-402d-8fa2-ab2305f02b88';
  $mini->display = $display;
  $export['wetkit_mini_panels_home'] = $mini;

  return $export;
}

/**
 * Custom function for retrieving mlids by link path.
 */
function _wetkit_menu_get_mlid($source_id) {

  // Avoid checking if table exists multiple times.
  $table_exists = &drupal_static(__FUNCTION__ . '_table_exists');
  if (!isset($table_exists)) {
    $table_exists = db_table_exists('migrate_map_wetkitmigratemegamenulinks');
  }
  if (!$table_exists) {
    return FALSE;
  }
  // Statically cache results of db queries.
  $menu_links = &drupal_static(__FUNCTION__ . '_menu_links', array());
  if (!isset($menu_links[$source_id])) {
    $menu_links[$source_id] = db_select('migrate_map_wetkitmigratemegamenulinks')
        ->fields('migrate_map_wetkitmigratemegamenulinks')
        ->condition('sourceid1', $source_id)
        ->execute()->fetch();
  }
  if (isset($menu_links[$source_id]->destid1)) {
    return $menu_links[$source_id]->destid1;
  }
}
