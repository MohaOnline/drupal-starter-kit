<?php

/**
 * @file
 * Administrative pages.
 *
 * Created by: Topsitemakers
 * http://www.topsitemakers.com/
 */

/**
 * Main configuration form for administrators.
 */
function custom_menu_toolbar_menu_settings_form() {
  $form = array();
  $menus = menu_get_menus();
  $roles = user_roles();
  $results = db_query("SELECT * FROM {custom_menu_toolbar_role_menus} ORDER BY weight ASC");

  // Add the "no menu" option to the list.
  $menus = array_merge(array('_none' => t('- No toolbar -')), $menus);

  $form['menus']['#tree'] = TRUE;
  foreach ($results as $result) {
    $role = user_role_load($result->rid);
    $form['menus'][$result->rid] = array(
      'name' => array(
        '#markup' => filter_xss_admin($role->name),
      ),
      'menu' => array(
        '#type' => 'select',
        '#options' => $menus,
        '#default_value' => $result->menu_name,
      ),
      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => $result->weight,
        '#delta' => 10,
        '#title_display' => 'invisible',
      ),
    );
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Changes'),
  );

  return $form;
}

/**
 * Submit handler for the custom_menu_toolbar_menu_settings_form().
 */
function custom_menu_toolbar_menu_settings_form_submit($form, &$form_state) {
  foreach ($form_state['values']['menus'] as $rid => $data) {
    db_update('custom_menu_toolbar_role_menus')
      ->condition('rid', $rid)
      ->fields(array(
        'menu_name' => $data['menu'],
        'weight' => $data['weight'],
      ))
      ->execute();
  }
  drupal_set_message(t('Changes have been saved successfully.'));
}

/**
 * Main configuration form for administrators.
 */
function custom_menu_toolbar_admin_settings_form() {
  $form = array();

  $form['colors'] = array(
    '#type' => 'fieldset',
    '#title' => t('Colors'),
  );
  $form['colors']['custom_menu_toolbar_var_color_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Text'),
    '#default_value' => variable_get('custom_menu_toolbar_var_color_text', CUSTOM_MENU_TOOLBAR_DEFAULT_COLOR_TEXT),
  );
  $form['colors']['custom_menu_toolbar_var_color_bg'] = array(
    '#type' => 'textfield',
    '#title' => t('Background'),
    '#default_value' => variable_get('custom_menu_toolbar_var_color_bg', CUSTOM_MENU_TOOLBAR_DEFAULT_COLOR_BG),
  );
  $form['colors']['custom_menu_toolbar_var_color_bg_active'] = array(
    '#type' => 'textfield',
    '#title' => t('Active / hover background'),
    '#default_value' => variable_get('custom_menu_toolbar_var_color_bg_active', CUSTOM_MENU_TOOLBAR_DEFAULT_COLOR_BG_ACTIVE),
  );

  $form['other'] = array(
    '#type' => 'fieldset',
    '#title' => t('Other settings'),
  );
  $form['other']['custom_menu_toolbar_var_other_toggle_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Mobile toggle text'),
    '#description' => t('Text displayed when the menu is collapsed for mobile devices.'),
    '#default_value' => variable_get('custom_menu_toolbar_var_other_toggle_text', CUSTOM_MENU_TOOLBAR_DEFAULT_TOGGLE_TEXT),
  );

  return system_settings_form($form);
}
