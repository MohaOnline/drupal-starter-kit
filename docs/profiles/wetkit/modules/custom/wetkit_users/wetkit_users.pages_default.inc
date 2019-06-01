<?php
/**
 * @file
 * wetkit_users.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function wetkit_users_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'pm_existing_pages_user_login_panel_context';
  $handler->task = 'pm_existing_pages';
  $handler->subtask = 'user_login';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'User Login Page',
    'no_blocks' => 1,
    'pipeline' => 'standard',
    'css_id' => 'user-login-panel',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'role',
          'settings' => array(
            'rids' => array(
              0 => 1,
            ),
          ),
          'context' => 'argument_entity_id:user_1',
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
      'sidebar' => NULL,
      'contentmain' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Log in';
  $display->uuid = 'a781cfbb-4bb0-4b2e-8295-956d60734fd8';
  $display->storage_type = 'page_manager';
  $display->storage_id = 'pm_existing_pages_user_login_panel_context';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-4dc85e6a-9552-4124-810b-10c71802b1da';
  $pane->panel = 'contentmain';
  $pane->type = 'pm_existing_pages';
  $pane->subtype = 'pm_existing_pages';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    0 => 'task_id',
    'task_id' => 'user_login',
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
  $pane->uuid = '4dc85e6a-9552-4124-810b-10c71802b1da';
  $display->content['new-4dc85e6a-9552-4124-810b-10c71802b1da'] = $pane;
  $display->panels['contentmain'][0] = 'new-4dc85e6a-9552-4124-810b-10c71802b1da';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['pm_existing_pages_user_login_panel_context'] = $handler;

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'pm_existing_pages_user_password_panel_context';
  $handler->task = 'pm_existing_pages';
  $handler->subtask = 'user_password';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'User Password Page',
    'no_blocks' => 1,
    'pipeline' => 'standard',
    'css_id' => 'user-password-panel',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'role',
          'settings' => array(
            'rids' => array(
              0 => 1,
            ),
          ),
          'context' => 'argument_entity_id:user_1',
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
      'sidebar' => NULL,
      'contentmain' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'cd1627f6-6d13-44eb-bbc6-43a784d4eb04';
  $display->storage_type = 'page_manager';
  $display->storage_id = 'pm_existing_pages_user_password_panel_context';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-a1b97b7a-d099-459d-acca-f350ebe53819';
  $pane->panel = 'contentmain';
  $pane->type = 'pm_existing_pages';
  $pane->subtype = 'pm_existing_pages';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    0 => 'task_id',
    'task_id' => 'user_password',
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
  $pane->uuid = 'a1b97b7a-d099-459d-acca-f350ebe53819';
  $display->content['new-a1b97b7a-d099-459d-acca-f350ebe53819'] = $pane;
  $display->panels['contentmain'][0] = 'new-a1b97b7a-d099-459d-acca-f350ebe53819';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['pm_existing_pages_user_password_panel_context'] = $handler;

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'pm_existing_pages_user_register_panel_context';
  $handler->task = 'pm_existing_pages';
  $handler->subtask = 'user_register';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'User Register Page',
    'no_blocks' => 1,
    'pipeline' => 'ipe',
    'css_id' => 'user-register-panel',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'role',
          'settings' => array(
            'rids' => array(
              0 => 1,
            ),
          ),
          'context' => 'argument_entity_id:user_1',
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
      'sidebar' => NULL,
      'contentmain' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Create an Account';
  $display->uuid = '7bd1b3bd-e462-4b17-93f3-0ca7fabe600e';
  $display->storage_type = 'page_manager';
  $display->storage_id = 'pm_existing_pages_user_register_panel_context';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-98e1736d-0b7d-450f-bff3-a0b6a582e001';
  $pane->panel = 'contentmain';
  $pane->type = 'pm_existing_pages';
  $pane->subtype = 'pm_existing_pages';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    0 => 'task_id',
    'task_id' => 'user_register',
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
  $pane->uuid = '98e1736d-0b7d-450f-bff3-a0b6a582e001';
  $display->content['new-98e1736d-0b7d-450f-bff3-a0b6a582e001'] = $pane;
  $display->panels['contentmain'][0] = 'new-98e1736d-0b7d-450f-bff3-a0b6a582e001';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['pm_existing_pages_user_register_panel_context'] = $handler;

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'user_edit_panel_context';
  $handler->task = 'user_edit';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'User Edit Page',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => 'user-edit',
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
      'sidebar' => NULL,
      'contentmain' => NULL,
      'column1' => NULL,
      'column2' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '9c39b3e2-e41e-43a3-acba-7e8ef9b49b46';
  $display->storage_type = 'page_manager';
  $display->storage_id = 'user_edit_panel_context';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-89168d29-da90-5784-9524-3ff245745e25';
  $pane->panel = 'contentmain';
  $pane->type = 'form';
  $pane->subtype = 'form';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => 'argument_user_edit_1',
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
  $pane->uuid = '89168d29-da90-5784-9524-3ff245745e25';
  $display->content['new-89168d29-da90-5784-9524-3ff245745e25'] = $pane;
  $display->panels['contentmain'][0] = 'new-89168d29-da90-5784-9524-3ff245745e25';
  $pane = new stdClass();
  $pane->pid = 'new-40876a6e-5252-43b7-b445-d84ac849083a';
  $pane->panel = 'sidebar';
  $pane->type = 'entity_form_field';
  $pane->subtype = 'user:field_user_picture';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'label' => '',
    'formatter' => '',
    'context' => 'argument_user_edit_1',
    'override_title' => 1,
    'override_title_text' => 'User Picture',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '40876a6e-5252-43b7-b445-d84ac849083a';
  $display->content['new-40876a6e-5252-43b7-b445-d84ac849083a'] = $pane;
  $display->panels['sidebar'][0] = 'new-40876a6e-5252-43b7-b445-d84ac849083a';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-89168d29-da90-5784-9524-3ff245745e25';
  $handler->conf['display'] = $display;
  $export['user_edit_panel_context'] = $handler;

  return $export;
}
