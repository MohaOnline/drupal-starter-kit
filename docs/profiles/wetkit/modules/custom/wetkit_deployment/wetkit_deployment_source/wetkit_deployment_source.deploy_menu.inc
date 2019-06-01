<?php
/**
 * @file
 * Alters the menu item form to allow the administrator to specify additional
 * deploy attribute(s) for the menu link
 */

/**
 * Implements hook_permission().
 */
function wetkit_deployment_source_permission() {
  return array(
    'administer deploy menu' => array(
      'title' => t('administer deploy menu'),
      'description' => t('administer deploy menu.'),
    ),
    'administer deploy dashboard' => array(
      'title' => t('administer deploy dashboard'),
      'description' => t('administer deploy dashboard.'),
    ),
  );
}

/**
 * Implements hook_menu_link_alter().
 */
function wetkit_deployment_source_menu_link_alter(&$item, $menu) {
  if (isset($item['options']['deploy_attributes']) && is_array($item['options']['deploy_attributes'])) {
    // Filter out blank attributes.
    foreach ($item['options']['deploy_attributes'] as $key => $value) {
      if ((is_array($value) && empty($value)) || is_int($value['yes'])) {
        unset($item['options']['deploy_attributes'][$key]);
      }
    }
  }
}

/**
 * Implements hook_menu_attriute_info().
 */
function wetkit_deployment_source_deploy_attribute_info() {
  $info['deploy_menu_item'] = array(
    'label' => t('Deploy Menu Item'),
    'form' => array(
      '#type' => 'checkboxes',
      '#options' => array(
        'yes' => t('Deploy this menu item to the Destination Site'),
      ),
      '#weight' => 10,
    ),
  );
  return $info;
}

/**
 * Fetch an array of deploy attributes.
 */
function wetkit_deployment_source_get_deploy_attribute_info() {
  $attributes = module_invoke_all('deploy_attribute_info');
  drupal_alter('deploy_attribute_info', $attributes);

  // Merge in default values.
  foreach ($attributes as $attribute => &$info) {
    $info += array(
      'form' => array(),
      'enabled' => variable_get("wetkit_deployment_source_{$attribute}_enable", 1),
      'default' => '',
    );
    $info['form'] += array(
      '#type' => 'textfield',
      '#title' => $info['label'],
      '#default_value' => variable_get("wetkit_deployment_source_{$attribute}_default", $info['form']['#options']),
    );
  }

  return $attributes;
}

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Adds menu attribute options to the edit menu item form.
 *
 * @see menu_edit_item()
 * @see _wetkit_deployment_source_form_alter()
 * @see wetkit_deployment_source_form_menu_edit_item_submit()
 */
function wetkit_deployment_source_form_menu_edit_item_alter(&$form, $form_state) {
  $item = $form['original_item']['#value'];
  _wetkit_deployment_source_form_alter($form, $item, $form);
}

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Adds menu attribute options to the node's edit menu item form.
 *
 * @see _wetkit_deployment_source_form_alter()
 */
function wetkit_deployment_source_form_node_form_alter(&$form, $form_state) {
  if (isset($form['menu']['link']) && isset($form['#node']->menu)) {
    $item = $form['#node']->menu;
    _wetkit_deployment_source_form_alter($form['menu']['link'], $item, $form);
    $form['menu']['link']['options']['deploy_attributes']['#type'] = 'container';
  }
}

/**
 * Add the deploy attributes to a menu item edit form.
 *
 * @param array $form
 *   The menu item edit form passed by reference.
 * @param array $item
 *   The optional existing menu item for context.
 */
function _wetkit_deployment_source_form_alter(array &$form, array $item = array(), array &$complete_form) {
  $form['options']['#tree'] = TRUE;
  $form['options']['#weight'] = 50;

  // Unset the previous value so that the new values get saved.
  unset($form['options']['#value']['deploy_attributes']);

  $form['options']['deploy_attributes'] = array(
    '#type' => 'fieldset',
    '#title' => t('Deploy'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );

  $attributes = wetkit_deployment_source_get_deploy_attribute_info();
  foreach ($attributes as $attribute => $info) {
    // Merge in the proper default value.
    if (isset($item['options']['deploy_attributes'][$attribute])) {
      // If the menu link already has this attribute, use it.
      $info['form']['#default_value'] = $item['options']['deploy_attributes'][$attribute];
    }
    elseif ($item['mlid']) {
      // If this is an existing link, use the raw default (usually empty).
      $info['form']['#default_value'] = array($info['default']);
    }
    $form['options']['deploy_attributes'][$attribute] = $info['form'] + array(
      '#access' => $info['enabled'],
    );
  }

  // Add form values for the reset of $item['options'] and
  // $item['options']['deploy_attributes'] so the values will carry over
  // during save.
  foreach ($item['options'] as $key => $value) {
    if ($key !== 'attributes' && !isset($form['options'][$key])) {
      $form['options'][$key] = array(
        '#type' => 'value',
        '#value' => $value,
      );
    }
  }
  if (isset($item['options']['deploy_attributes'])) {
    foreach ($item['options']['deploy_attributes'] as $key => $value) {
      if (!isset($form['options']['deploy_attributes'][$key])) {
        $form['options']['deploy_attributes'][$key] = array(
          '#type' => 'value',
          '#value' => $value,
        );
      }
    }
  }

  // Restrict access to the new form elements.
  $has_visible_children = (bool) element_get_visible_children($form['options']['deploy_attributes']);
  $user_has_access = user_access('administer deploy menu');
  $form['options']['deploy_attributes']['#access'] = ($has_visible_children && $user_has_access);
}

/**
 * Form submit handler for menu item links.
 *
 * Move the title attributes value into the 'description' value so that it
 * will get properly saved.
 */
function _wetkit_deployment_source_form_submit($form, &$form_state) {
  if (isset($form_state['values']['menu']['options']['deploy_attributes']['title'])) {
    $form_state['values']['menu']['description'] = $form_state['values']['menu']['options']['deploy_attributes']['title'];
  }
}
