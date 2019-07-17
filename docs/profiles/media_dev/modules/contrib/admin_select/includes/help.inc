<?php
/**
 * @file
 * Help module integration.
 */

/**
 * Implements hook_help().
 */
function admin_select_help($path, $arg) {
  switch ($path) {
    case 'admin/config/user-interface/admin_select':
      return '<p>' . t('Select the appropriate admin menu for each role. These will be the defaults for users who haven\'t selected anything on their account edit page.') . '</p>'
           . '<p>' . t('If a user has multiple roles, the role used to determine which menu they see will be the one that appears highest in the list, so drag the roles to order them as required (remember that all logged in users also have the authenticated user role).') . '</p>';
  }
}
