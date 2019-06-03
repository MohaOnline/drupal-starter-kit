<?php
/**
 * @file
 * Custom Help Text Ctools Export Plugin.
 */

/**
 * Class for Custom Help Text export UI.
 */
class custom_help_text_export_ui extends ctools_export_ui {

  /**
   * Initialize the plugin with some modifications.
   */
  function init($plugin) {
    $plugin['menu']['items']['list callback']['access callback'] = 'user_access';
    $plugin['menu']['items']['list callback']['access arguments'] = array('administer custom help text');

    $plugin['menu']['items']['list']['title'] = 'List';

    $plugin['menu']['items']['edit callback']['type'] = MENU_VISIBLE_IN_BREADCRUMB;

    return parent::init($plugin);
  }

  /**
   * Override menu items.
   */
  function hook_menu(&$items) {
    $stored_items = $this->plugin['menu']['items'];

    parent::hook_menu($items);

    $this->plugin['menu']['items'] = $stored_items;
  }

  /**
   * Provide the actual editing form.
   */
  function edit_form(&$form, &$form_state) {
    parent::edit_form($form, $form_state);

    $item = $form_state['item'];

    $form['info']['admin_title']['#required'] = TRUE;
    $form['info']['admin_title']['#maxlength'] = 80;

    $form['path'] = array(
      '#type' => 'textarea',
      '#title' => t('Paths'),
      '#default_value' => $item->path,
      '#maxlength' => 128,
      '#size' => 45,
      '#description' => t('Specify pages by using their paths. Enter one path per line. The \'*\' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.', array(
        '%blog' => 'blog',
        '%blog-wildcard' => 'blog/*',
        '%front' => '<front>'
      )),
      '#required' => TRUE,
    );

    $form['help'] = array(
      '#type' => 'textarea',
      '#title' => t('Help message'),
      '#default_value' => $item->help,
      '#description' => t('Specify a help message.'),
      '#required' => TRUE,
    );

    $form['options'] = array(
      '#type' => 'vertical_tabs',
    );

    $form['options_roles'] = array(
      '#type' => 'fieldset',
      '#title' => t('Roles'),
      '#collapsible' => TRUE,
      '#group' => 'options',
      '#weight' => -50,
    );

    $item->options = unserialize($item->options);

    $roles = user_roles(FALSE, 'view custom help text');

    // Get all Authenticated roles
    if (isset($roles[DRUPAL_AUTHENTICATED_RID])) {
      $roles += array_diff(user_roles(TRUE), $roles);
    }

    if (user_access('administer permissions')) {
      $permission_link = l(t('View custom help text'), 'admin/people/permissions', array(
        'fragment' => 'module-custom_help_text'
      ));
    }
    else {
      $permission_link = t('View custom help text');
    }

    $form['options_roles']['roles'] = array(
      '#type' => 'checkboxes',
      '#options' => $roles,
      '#title' => t('User roles that can view the custom help text'),
      '#required' => TRUE,
      '#description' => t('Check the roles that needs to view the help message and have currently the permission \'!permission_url\'.', array(
          '!permission_url' => $permission_link,
        )
      ),
    );

    if (!empty($item->options['roles'])) {
      foreach ($item->options['roles'] as $role_name) {
        if ($role = user_role_load_by_name($role_name)) {
          $form['options_roles']['roles']['#default_value'][] = $role->rid;
        }
      }
    }
    else {
      $form['options_roles']['roles']['#default_value'] = array();
    }
  }

  /**
   * Handle the submission of the edit form.
   */
  function edit_form_submit(&$form, &$form_state) {
    // Update old help with new help string.
    if (function_exists('i18n_string_update')) {
      $name = $this->_create_string_key($form_state['item']->name, 'help');
      $options = array(
        'format' => I18N_STRING_FILTER_XSS_ADMIN,
        'messages' => FALSE,
      );
      i18n_string_update($name, $form_state['item']->help, $options);
    }

    if (empty($form_state['item']->weight)) {
      $form_state['values']['weight'] = '-50';
    }

    $this->_reformat_roles($form, $form_state);

    $form_state['values']['options'] = serialize($form_state['values']['options']);

    parent::edit_form_submit($form, $form_state);
  }

  /**
   * Remove translations.
   */
  function delete_form_submit(&$form_state) {
    // Cleanup translated strings.
    if (function_exists('i18n_string_remove')) {
      $name = $this->_create_string_key($form_state['item']->name, 'help');
      i18n_string_remove($name);
    }

    parent::delete_form_submit($form_state);
  }

  /**
   * Helper method to serialize options.
   */
  function _reformat_roles($form, &$form_state) {
    $roles = array();

    foreach ($form_state['values']['roles'] as $rid) {
      if ($rid !== 0 && $role = user_role_load($rid)) {
        $roles[] = $role->name;
      }
    }
    unset($form_state['values']['roles']);

    $form_state['values']['options']['roles'] = $roles;
  }

  /**
   * Create translation array of the field of an object.
   */
  function _create_string_key($name, $field) {
    return array(
      'custom_help_text',
      'text',
      $name,
      $field,
    );
  }

  /**
   * Highly specialized list.
   */
  function list_page($js, $input) {
    $this->items = ctools_export_crud_load_all($this->plugin['schema'], $js);

    // Respond to a reset command by clearing session and doing a drupal goto
    // back to the base URL.
    if (isset($input['op']) && $input['op'] == t('Reset')) {
      unset($_SESSION['ctools_export_ui'][$this->plugin['name']]);
      if (!$js) {
        drupal_goto($_GET['q']);
      }
      // clear everything but form id, form build id and form token:
      $keys = array_keys($input);
      foreach ($keys as $id) {
        if (!in_array($id, array('form_id', 'form_build_id', 'form_token'))) {
          unset($input[$id]);
        }
      }
    }

    // If there is no input, check to see if we have stored input in the
    // session.
    if (!isset($input['form_id'])) {
      if (isset($_SESSION['ctools_export_ui'][$this->plugin['name']]) && is_array($_SESSION['ctools_export_ui'][$this->plugin['name']])) {
        $input = $_SESSION['ctools_export_ui'][$this->plugin['name']];
      }
    }
    else {
      $_SESSION['ctools_export_ui'][$this->plugin['name']] = $input;
      unset($_SESSION['ctools_export_ui'][$this->plugin['name']]['q']);
    }

    // This is where the form will put the output.
    $this->rows = array();
    $this->sorts = array();

    $form_state = array(
      'plugin' => $this->plugin,
      'input' => $input,
      'rerender' => TRUE,
      'no_redirect' => TRUE,
      'object' => &$this,
    );
    if (!isset($form_state['input']['form_id'])) {
      $form_state['input']['form_id'] = 'ctools_export_ui_list_form';
    }

    // If we do any form rendering, it's to completely replace a form on the
    // page, so don't let it force our ids to change.
    if ($js && isset($_POST['ajax_html_ids'])) {
      unset($_POST['ajax_html_ids']);
    }

    $form = drupal_build_form('custom_help_text_list_form', $form_state);

    if (!$js) {
      $this->list_css();
      return drupal_render($form);
    }

    $table = theme('custom_help_text_list_form', array(
      'form' => $form,
      'js' => $js
    ));

    $commands = array();
    $commands[] = ajax_command_replace('#custom_help_text_table', $table);

    print ajax_render($commands);
    ajax_footer();
  }
}
