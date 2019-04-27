<?php

/**
 * @file
 * Declares the available audio players for the Audio Field module.
 */

/**
 * Implements hook_audiofield_players().
 */
function audiofield_players() {
  $players = module_invoke_all('audiofield_players');

  $players['html5'] = array(
    'name' => 'HTML5 <audio>',
    'filetypes' => array('mp3', 'wav', 'ogg', 'opus', 'webm', 'flac'),
    'local' => TRUE,
    'callback' => 'audiofield_html5_audio',
  );

  $players['wpaudioplayer'] = array(
    'path' => variable_get('audiofield_players_dir', 'sites/all/libraries/player') . "/audio-player/player.swf",
    'name' => 'WordPress Audio Player',
    'download_link' => 'http://wpaudioplayer.com/download',
    'filetypes' => array('mp3'),
    'callback' => 'audiofield_wpaudioplayer',
  );

  $players['xspf_slim'] = array(
    'path' => variable_get('audiofield_players_dir', 'sites/all/libraries/player') . "/xspf_player_slim.swf",
    'name' => 'XSPF Slim Player',
    'download_link' => 'http://prdownloads.sourceforge.net/musicplayer/xspf_player_slim-correct-0.2.3.zip?download',
    'filetypes' => array('mp3'),
    'callback' => 'audiofield_xspf_slim',
  );

  $players['xspf_button'] = array(
    'path' => variable_get('audiofield_players_dir', 'sites/all/libraries/player') . "/button/musicplayer.swf",
    'name' => 'XSPF Button Player',
    'download_link' => 'http://prdownloads.sourceforge.net/musicplayer/button_player-0.1.zip?download',
    'filetypes' => array('mp3'),
    'callback' => 'audiofield_xspf_button',
  );

  // #1414398: Premium Beat players no longer available.
  $players['premium_beat_single_track'] = array(
    'path' => variable_get('audiofield_players_dir', 'sites/all/libraries/player') . "/playerSinglePackage/playerSingle.swf",
    'name' => 'Premium Beat Single Player',
    'filetypes' => array('mp3'),
    'callback' => 'audiofield_premium_beat_single_track',
  );

  // #1414398: Premium Beat players no longer available.
  $players['premium_beat_thin'] = array(
    'path' => variable_get('audiofield_players_dir', 'sites/all/libraries/player') . "/OriginalThinMusicPlayer.swf",
    'name' => 'Premium Beat Thin Player',
    'filetypes' => array('mp3'),
    'callback' => 'audiofield_premium_beat_thin',
  );

  // #1414398: Premium Beat players no longer available.
  $players['premium_beat_mini'] = array(
    'path' => variable_get('audiofield_players_dir', 'sites/all/libraries/player') . "/LWMusicPlayer.swf",
    'name' => 'Premium Beat Mini Player',
    'filetypes' => array('mp3'),
    'callback' => 'audiofield_premium_beat_mini',
  );

  $players['flowplayer'] = array(
    'name' => 'Flowplayer',
    'download_link' => 'http://drupal.org/project/flowplayer',
    'filetypes' => array('mp3'),
    'callback' => 'audiofield_flowplayer',
    'module' => 'flowplayer',
  );

  $players['soundmanager2'] = array(
    'path' => variable_get('audiofield_players_dir', 'sites/all/libraries/player') . "/soundmanager2",
    'name' => 'SoundManager2 360',
    'download_link' => 'http://www.schillmania.com/projects/soundmanager2/',
    'filetypes' => array('mp3', 'wav', 'ogg', 'opus', 'wav', 'flac'),
    'callback' => 'audiofield_soundmanager2_360player',
  );

  $players['soundmanager2_barui'] = array(
    'path' => variable_get('audiofield_players_dir', 'sites/all/libraries/player') . "/soundmanager2",
    'name' => 'SoundManager2 Bar UI',
    'download_link' => 'http://www.schillmania.com/projects/soundmanager2/',
    'filetypes' => array('mp3', 'wav', 'ogg', 'opus', 'wav', 'flac'),
    'callback' => 'audiofield_soundmanager2_baruiplayer',
  );

  $players['soundmanager2_button'] = array(
    'path' => variable_get('audiofield_players_dir', 'sites/all/libraries/player') . "/soundmanager2",
    'name' => 'SoundManager2 Inline',
    'download_link' => 'http://www.schillmania.com/projects/soundmanager2/',
    'filetypes' => array('mp3', 'wav', 'ogg', 'opus', 'wav', 'flac'),
    'callback' => 'audiofield_soundmanager2_buttonplayer',
  );

  $players['jplayer'] = array(
    'path' => variable_get('audiofield_players_dir', 'sites/all/libraries/player') . "/jplayer/jquery.jplayer.min.js",
    'css' => variable_get('audiofield_players_dir', 'sites/all/libraries/player') . "/jplayer/jplayer.css",
    'name' => 'jPlayer',
    'download_link' => 'https://www.drupal.org/project/jplayer',
    'module' => 'jplayer',
    'filetypes' => array('mp3', 'mp4', 'wav', 'ogg', 'oga', 'webm'),
    'callback' => 'audiofield_jplayer',
  );

  $players['wavesurfer'] = array(
    'path' => variable_get('audiofield_players_dir', 'sites/all/libraries/player') . "/wavesurfer/dist/wavesurfer.min.js",
    'name' => 'wavesurfer.js',
    'download_link' => 'https://github.com/katspaugh/wavesurfer.js/',
    'module' => 'jplayer',
    'filetypes' => array('mp3', 'wav', 'ogg', 'oga'),
    'callback' => 'audiofield_wavesurfer_player',
  );

  return $players;
}

/**
 * Defining AudioField theme callbacks.
 */
function _audiofield_theme() {
  return array(
    // Themes for the players.
    'audiofield_html5_audio' => array(
      'path' => drupal_get_path('module', 'audiofield') . '/templates',
      'template' => 'audiofield--html5',
      'variables' => array(
        'audio_file' => NULL,
        'download_access' => FALSE,
      ),
    ),
    'audiofield_players_wpaudioplayer' => array(
      'path' => drupal_get_path('module', 'audiofield') . '/templates',
      'template' => 'audiofield--wpaudio',
      'variables' => array(
        'id' => NULL,
        'audio_file' => NULL,
      ),
    ),
    'audiofield_players_xspf_slim' => array(
      'path' => drupal_get_path('module', 'audiofield') . '/templates',
      'template' => 'audiofield--xspf-slim',
      'variables' => array(
        'player_path' => NULL,
        'audio_file' => NULL,
        'audio_title' => NULL,
      ),
    ),
    'audiofield_players_xspf_button' => array(
      'path' => drupal_get_path('module', 'audiofield') . '/templates',
      'template' => 'audiofield--xspf-button',
      'variables' => array(
        'player_path' => NULL,
        'audio_file' => NULL,
        'audio_title' => NULL,
      ),
    ),
    'audiofield_players_premium_beat_single_track' => array(
      'path' => drupal_get_path('module', 'audiofield') . '/templates',
      'template' => 'audiofield--premiumbeat-singletrack',
      'variables' => array(
        'player_path' => NULL,
        'audio_file' => NULL,
      ),
    ),
    'audiofield_players_premium_beat_thin' => array(
      'path' => drupal_get_path('module', 'audiofield') . '/templates',
      'template' => 'audiofield--premiumbeat-thin',
      'variables' => array(
        'player_path' => NULL,
        'audio_file' => NULL,
      ),
    ),
    'audiofield_players_premium_beat_mini' => array(
      'path' => drupal_get_path('module', 'audiofield') . '/templates',
      'template' => 'audiofield--premiumbeat-mini',
      'variables' => array(
        'player_path' => NULL,
        'audio_file' => NULL,
      ),
    ),
    'audiofield_players_soundmanager2_360player' => array(
      'path' => drupal_get_path('module', 'audiofield') . '/templates',
      'template' => 'audiofield--soundmanager2-360player',
      'variables' => array(
        'id' => NULL,
        'audio_file' => NULL,
      ),
    ),
    'audiofield_players_soundmanager2_baruiplayer' => array(
      'path' => drupal_get_path('module', 'audiofield') . '/templates',
      'template' => 'audiofield--soundmanager2-barui',
      'variables' => array(
        'audio_file' => NULL,
        'audio_title' => NULL,
      ),
    ),
    'audiofield_players_soundmanager2_buttonplayer' => array(
      'path' => drupal_get_path('module', 'audiofield') . '/templates',
      'template' => 'audiofield--soundmanager2-button',
      'variables' => array(
        'audio_file' => NULL,
        'audio_title' => NULL,
      ),
    ),
    'audiofield_players_wavesurfer_player' => array(
      'path' => drupal_get_path('module', 'audiofield') . '/templates',
      'template' => 'audiofield--wavesurfer',
      'variables' => array(
        'container_id' => NULL,
        'audio_title' => NULL,
      ),
    ),
  );
}

/**
 * Callback function for AudioField HTML5 player.
 */
function audiofield_html5_audio($player_path, $audio_path, $options) {
  return theme('audiofield_html5_audio', array(
    'audio_file' => file_create_url($audio_path),
    'download_access' => $options['display']['settings']['download_link'] && user_access('download all audio files'),
  ));
}

/**
 * Callback function for AudioField WordPress standalone player.
 */
function audiofield_wpaudioplayer($player_path, $audio_file) {
  // Add the javascript which renders the player.
  drupal_add_js(variable_get('audiofield_players_dir', 'sites/all/libraries/player') . '/audio-player/audio-player.js', array(
    'type' => 'file',
    'scope' => 'footer',
    'group' => JS_LIBRARY,
    'weight' => 0,
  ));
  drupal_add_js(drupal_get_path('module', 'audiofield') . '/js/wordpress.builder.js', array(
    'type' => 'file',
    'scope' => 'footer',
    'group' => JS_LIBRARY,
    'weight' => 10,
  ));
  return theme('audiofield_players_wpaudioplayer', array(
    'id' => drupal_html_id('audiofield-wordpress-player'),
    'audio_file' => file_create_url($audio_file),
  ));
}

/**
 * Callback function for AudioField XSPF Slim player.
 */
function audiofield_xspf_slim($player_path, $audio_file, $options) {
  $audio_title = t('XSPF Slim Music Player');
  if (!empty($options)) {
    if (!empty($options['item']['description'])) {
      $audio_title = $options['item']['description'];
    }
    elseif (!empty($options['item']['filename'])) {
      $audio_title = $options['item']['filename'];
    }
  }
  return theme('audiofield_players_xspf_slim', array(
    'player_path' => $player_path,
    'audio_file' => file_create_url($audio_file),
    'audio_title' => urlencode($audio_title),
  ));
}

/**
 * Callback function for AudioField XSPF Button player.
 */
function audiofield_xspf_button($player_path, $audio_file, $options) {
  $audio_title = t('XSPF Slim Music Player');
  if (!empty($options)) {
    if (!empty($options['item']['description'])) {
      $audio_title = $options['item']['description'];
    }
    elseif (!empty($options['item']['filename'])) {
      $audio_title = $options['item']['filename'];
    }
  }
  return theme('audiofield_players_xspf_button', array(
    'player_path' => $player_path,
    'audio_file' => file_create_url($audio_file),
    'audio_title' => urlencode($audio_title),
  ));
}

/**
 * Callback function for AudioField Premium Beat single-track player.
 */
function audiofield_premium_beat_single_track($player_path, $audio_file) {
  return theme('audiofield_players_premium_beat_single_track', array(
    'player_path' => $player_path,
    'audio_file' => $audio_file,
  ));
}

/**
 * Callback function for AudioField Premium Beat thin player.
 */
function audiofield_premium_beat_thin($player_path, $audio_file) {
  return theme('audiofield_players_premium_beat_thin', array(
    'player_path' => $player_path,
    'audio_file' => $audio_file,
  ));
}

/**
 * Callback function for AudioField Premium Beat mini player.
 */
function audiofield_premium_beat_mini($player_path, $audio_file) {
  return theme('audiofield_players_premium_beat_mini', array(
    'player_path' => $player_path,
    'audio_file' => $audio_file,
  ));
}

/**
 * Callback function for AudioField SoundManager2 360 player.
 */
function audiofield_soundmanager2_360player($player_path, $audio_file) {

  $player_base_path = variable_get('audiofield_players_dir', 'sites/all/libraries/player') . "/soundmanager2";
  $ie_js = array(
    '#type' => 'markup',
    '#markup' => '<!--[if IE]><script type="text/javascript" src="' . $player_path . '/demo/360-player/script/excanvas.js"></script><![endif]-->',
  );
  drupal_add_html_head($ie_js, 'audiofield');

  drupal_add_js($player_base_path . '/script/soundmanager2.js', array('weight' => 0));

  // Add 360 viewer javascript.
  drupal_add_js($player_base_path . '/demo/360-player/script/berniecode-animator.js', array('weight' => 1));
  drupal_add_js($player_base_path . '/demo/360-player/script/360player.js', array('weight' => 1));
  drupal_add_css($player_base_path . '/demo/360-player/360player.css');
  // Pass settings to builder.
  drupal_add_js(array(
    'audiofield' => array(
      'swf_path' => $player_path . '/swf',
    ),
  ), array('type' => 'setting'));
  // Add the builder javascript.
  drupal_add_js(drupal_get_path('module', 'audiofield') . '/js/soundmanager2-360player.builder.js', array(
    'type' => 'file',
    'scope' => 'footer',
    'weight' => 10,
  ));

  return theme('audiofield_players_soundmanager2_360player', array(
    'id' => drupal_html_id('sm2-container-real'),
    'audio_file' => file_create_url($audio_file),
  ));
}

/**
 * Callback function for AudioField SoundManager2 Bar UI player.
 */
function audiofield_soundmanager2_baruiplayer($player_path, $audio_file, $options) {
  $audio_title = $audio_file;
  if (!empty($options)) {
    if (!empty($options['item']['description'])) {
      $audio_title = $options['item']['description'];
    }
    elseif (!empty($options['item']['filename'])) {
      $audio_title = $options['item']['filename'];
    }
  }

  $player_base_path = variable_get('audiofield_players_dir', 'sites/all/libraries/player') . "/soundmanager2";
  drupal_add_js($player_base_path . '/script/soundmanager2.js', array('weight' => 0));

  // Add Bar UI javascript.
  drupal_add_js($player_base_path . '/demo/bar-ui/script/bar-ui.js', array('weight' => 1));
  // Add Bar UI style.
  drupal_add_css($player_base_path . '/demo/bar-ui/css/bar-ui.css');

  return theme('audiofield_players_soundmanager2_baruiplayer', array(
    'audio_file' => file_create_url($audio_file),
    'audio_title' => $audio_title,
  ));
}

/**
 * Callback function for AudioField SoundManager2 Button player.
 */
function audiofield_soundmanager2_buttonplayer($player_path, $audio_file, $options) {
  $audio_title = $audio_file;
  if (!empty($options)) {
    if (!empty($options['item']['description'])) {
      $audio_title = $options['item']['description'];
    }
    elseif (!empty($options['item']['filename'])) {
      $audio_title = $options['item']['filename'];
    }
  }

  $player_base_path = variable_get('audiofield_players_dir', 'sites/all/libraries/player') . "/soundmanager2";
  drupal_add_js($player_base_path . '/script/soundmanager2.js', array('weight' => 0));

  // Add Bar UI javascript.
  drupal_add_js($player_base_path . '/demo/mp3-player-button/script/mp3-player-button.js', array('weight' => 1));
  // Add Bar UI style.
  drupal_add_css($player_base_path . '/demo/mp3-player-button/css/mp3-player-button.css');

  return theme('audiofield_players_soundmanager2_buttonplayer', array(
    'audio_file' => file_create_url($audio_file),
    'audio_title' => $audio_title,
  ));
}

/**
 * Callback function for AudioField Flowplayer player.
 */
function audiofield_flowplayer($player_path, $audio_file) {
  static $seq = 1;
  return theme('flowplayer', array(
    'config' => array(
      'clip' => array(
        'url' => $audio_file,
        'autoPlay' => FALSE,
      ),
    ),
    'id' => 'audiofield-' . $seq++,
    'attributes' => array('style' => 'height: 24px'),
  ));
}

/**
 * Callback function for AudioField jPlayer player.
 */
function audiofield_jplayer($player_path, $audio_file, $options) {
  return theme('jplayer', array(
    'entity_type' => $options['entity_type'],
    'entity' => $options['entity'],
    'field_name' => $options['instance']['field_name'],
    'items' => array($options['item']),
    'settings' => array(
      'autoplay' => 0,
      'solution' => 'html, flash',
      'preload' => 'metadata',
      'volume' => 80,
      'muted' => FALSE,
      'repeat' => 'none',
      'backgroundColor' => '000000',
      'mode' => 'playlist',
      'continuous' => FALSE,
    ),
  ));
}

/**
 * Callback function for AudioField wavesurfer.js player.
 */
function audiofield_wavesurfer_player($player_path, $audio_file, $options) {
  $audio_title = $audio_file;
  if (!empty($options)) {
    if (!empty($options['item']['description'])) {
      $audio_title = $options['item']['description'];
    }
    elseif (!empty($options['item']['filename'])) {
      $audio_title = $options['item']['filename'];
    }
  }

  $player_base_path = variable_get('audiofield_players_dir', 'sites/all/libraries/player') . '/wavesurfer';
  drupal_add_js($player_base_path . '/dist/wavesurfer.min.js', array('weight' => 0));

  // Generate a unique ID.
  $unique_id = drupal_html_id('wavesurfer-player-id');

  // Pass settings to builder.
  drupal_add_js(array(
    'audiofield' => array(
      $unique_id => file_create_url($audio_file),
    ),
  ), array('type' => 'setting'));
  // Add the builder javascript.
  drupal_add_js(drupal_get_path('module', 'audiofield') . '/js/wavesurfer.builder.js', array(
    'type' => 'file',
    'scope' => 'footer',
    'weight' => 10,
  ));

  // Add a default wavesurfer CSS.
  drupal_add_css(drupal_get_path('module', 'audiofield') . '/css/audiofield.wavesurfer.css');

  return theme('audiofield_players_wavesurfer_player', array(
    'container_id' => $unique_id,
    'audio_title' => $audio_title,
  ));
}
