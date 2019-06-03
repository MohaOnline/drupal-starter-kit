<?php

/**
 * @file
 * This file contains the API of the DvG DigiD module.
 */

/**
 * Implements hook_dvg_digid_login_button_alter().
 */
function hook_dvg_digid_login_button_alter(&$html, $settings) {
  // Override the default digid button.
  $html = l('<h2 class="digid-login">' . $settings['title'] . '</h2><div class="digid-login-content">' . $settings['logo'] . $settings['message'] . '</div>', $settings['path']);
}
