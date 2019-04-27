<?php

/**
 * @file
 * Administrative pages for the Audiofield module.
 */

/**
 * Settings form for administering module.
 */
function audiofield_admin_settings_form() {
  // Get a list of all installed players.
  $form['players_info'] = array(
    '#type' => 'item',
    '#title' => t('Installed Players'),
    '#description' => t("For more information on how to install additional players, see the README file for this module. The player can be selected under the individual field's display settings when using AudioField Player as the field formatter."),
    'players' => array(
      '#theme' => 'item_list',
      '#type' => 'ul',
      '#items' => array(),
      '#attributes' => array(),
    ),
  );
  foreach (audiofield_players() as $player) {
    if ((isset($player['path']) && file_exists($player['path'])) || (isset($player['local']) && $player['local']) || (isset($player['module']) && module_exists($player['module']))) {
      $form['players_info']['players']['#items'][] = $player['name'];
    }
  }

  // Where players are installed.
  $form['audiofield_players_dir'] = array(
    '#type' => 'textfield',
    '#title' => t('Audio Players Directory'),
    '#description' => t('Download and extract audio players in this directory'),
    '#default_value' => variable_get('audiofield_players_dir', 'sites/all/libraries/player'),
  );

  // File details settings.
  $detect_ffprobe = audiofield_accessible_ffprobe();
  $detect_getid3 = audiofield_accessible_getid3();

  $detail_value = variable_get('audiofield_detail');
  $form['audiofield_detail'] = array(
    '#type' => 'fieldset',
    '#title' => t('File Details tools'),
    'description' => array(
      '#theme' => 'item_list',
      '#type' => 'ul',
      '#prefix' => t('In order to display the file details for Audiofield entities, one of the following tools must be installed:'),
      '#items' => array(
        t('Command line tools !ffmpeg and !ffprobe. Status: !ffprobe_status', array(
          '!ffmpeg' => l(t('ffmpeg'), 'https://www.ffmpeg.org/documentation.html'),
          '!ffprobe' => l(t('ffprobe'), 'https://www.ffmpeg.org/ffprobe.html'),
          '!ffprobe_status' => $detect_ffprobe ? t('Enabled') : t('Disabled or not fully installed'),
        )),
        t('Drupal module !getid3 and getid3 command line. Status: !getid3_status', array(
          '!getid3' => l(t('getid3'), 'https://www.drupal.org/project/getid3'),
          '!getid3_status' => $detect_getid3 ? t('Enabled') : t('Disabled or not fully installed'),
        )),
      ),
      '#attributes' => array(),
    ),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );

  $form['audiofield_detail']['ffprobe_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path for ffmpeg/ffprobe'),
    '#default_value' => isset($detail_value['ffprobe_path']) ? $detail_value['ffprobe_path'] : '',
    '#description' => t('In Terminal or Command Prompt execute: which ffprobe'),
  );

  return system_settings_form($form);
}

/**
 * Implements hook_validate().
 */
function audiofield_admin_settings_form_validate($form, &$form_state) {
  if (!empty($form_state['values']['audiofield_detail']['ffprobe_path'])) {
    if (!in_array(substr($form_state['values']['audiofield_detail']['ffprobe_path'], -1), array('/', '\\'))) {
      if (preg_match("@^(.*)(/|\\\\)(ffprobe|ffprobe.exe|ffmpeg|ffmpeg.exe)$@", $form_state['values']['audiofield_detail']['ffprobe_path'], $preg)) {
        $form_state['values']['audiofield_detail']['ffprobe_path'] = $preg[1];
      }
      $form_state['values']['audiofield_detail']['ffprobe_path'] .= '/';
    }
    if (!audiofield_accessible_ffprobe($form_state['values']['audiofield_detail']['ffprobe_path'])) {
      form_set_error('ffprobe_path', t('Path for ffmpeg/ffprobe is not accessible.'));
      $form_state['values']['audiofield_detail']['ffprobe_path'] = '';
    }
  }
}
