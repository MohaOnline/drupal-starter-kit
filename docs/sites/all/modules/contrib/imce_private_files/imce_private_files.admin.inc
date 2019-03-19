<?php
/**
 * @file
 * Included with IMCE Private Files module.
 */

/**
 * Settings form callback.
 *
 * @return mixed
 *    Returns an array of form render items.
 */
function imce_private_files_settings_form() {

  $form = array();
  $form['imce_private_files_debug'] = array(
    '#title' => 'Debug mode',
    '#description' => t('Debug mode causes additional information to be written to the watchdog table.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('imce_private_files_debug'),
  );
  $form['imce_private_files_skip_check'] = array(
    '#title' => "Skip 'IMCE-owned files' table check",
    '#description' => t('By default, IMCE Private Files only applies access control to files that are "owned" by IMCE in the file_usage table. Tick this box to apply to all files.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('imce_private_files_skip_check'),
  );
  $form['mode'] = array(
    '#title' => 'Privacy Mode',
    '#description' => t('Select which privacy mode to use.'),
    '#type' => 'fieldset',
  );
  $form['mode']['imce_private_files_markup'] = array(
    '#markup' => '
<div>
<ol>
<li><strong>' . t('Simple Mode:') . '</strong>' . t('Direct links to any files
within Drupal\'s private file system will be restricted to users with the
"Authenticated" Role.') . '
</li>
<li><strong>' . t('Pass-Through Mode:') . '</strong>' . t('Direct links to any
files within Drupal\'s private file system will be restricted according to
IMCE\'s Role-Profile configuration, using the "Browse" permission as a proxy for
"download". Any user who would be allowed to browse a directory will be allowed
to directly download any files within that directory.') . '
</li>
</ol>
</div>
');
  $form['mode']['imce_private_files_mode'] = array(
    '#type' => 'radios',
    '#default_value' => variable_get('imce_private_files_mode'),
    '#options' => array(t('Simple'), t('Pass-Through')),
  );

  $form['mode']['imce_private_files_role'] = array(
    '#title' => 'Allowed Role',
    '#description' => t('In "Simple" Mode, any user with this role will be able to download all files.'),
    '#type' => 'select',
    '#options' => user_roles(),
    '#default_value' => variable_get('imce_private_files_role', 2),
    '#states' => array(
      'visible' => array(
        ':input[name="imce_private_files_mode"]' => array('value' => '0'),
      ),
    ),
  );

  return system_settings_form($form);
}
