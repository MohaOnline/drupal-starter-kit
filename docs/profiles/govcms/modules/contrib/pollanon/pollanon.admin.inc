<?php
/**
*/

/**
 * @file
 * Administration functions and forms.
 */

/**
 * Settings form pollanon module.
 */
function pollanon_settings_form() {
  $form = array();

  $form['pollanon_use_cookie_method'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use cookies to determine poll displays'),
    '#default_value' => variable_get('pollanon_use_cookie_method', TRUE),
  );
  $form['pollanon_threshold_count'] = array(
    '#type' => 'select',
    '#title' => t('How many votes is allowed to from a single IP address in ten minutes?'),
    '#default_value' => variable_get('pollanon_threshold_count', 10),
    '#options' => array(
      '0' => t('Unlimited'),
      '1' => '1',
      '5' => '5',
      '10' => '10',
      '20' => '20',
      '50' => '50',
      '500' => '500',
    ),
  );
  $form['pollanon_use_ajah'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use AJAH'),
    '#description' => t('Provides more user friendly method for anonymous user to vote.'),
    '#default_value' => variable_get('pollanon_use_ajah', FALSE),
  );
  return system_settings_form($form);
}
