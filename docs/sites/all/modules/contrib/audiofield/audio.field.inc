<?php

/**
 * @file
 * Implement an audio field, based on the file module's file field.
 */

/**
 * Implements hook_field_prepare_view().
 */
function audiofield_field_prepare_view($entity_type, $entities, $field, $instances, $langcode, &$items) {
  // TODO: Check this.
  // Remove files specified to not be displayed.
  foreach ($entities as $id => $entity) {
    foreach ($items[$id] as $delta => $item) {
      if (!file_field_displayed($item, $field)) {
        unset($items[$id][$delta]);
      }
      // Ensure consecutive deltas.
      $items[$id] = array_values($items[$id]);
    }
  }
}

/**
 * Implements hook_field_is_empty().
 */
function audiofield_field_is_empty($item, $field) {
  return file_field_is_empty($item, $field);
}

/**
 * Implements hook_field_widget_info().
 */
function audiofield_field_widget_info() {
  return array(
    'audiofield_widget' => array(
      'label' => t('Audio Upload'),
      'field types' => array('file'),
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
        'default value' => FIELD_BEHAVIOR_NONE,
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_settings_form().
 */
function audiofield_field_widget_settings_form($field, $instance) {
  $widget = $instance['widget'];
  $settings = $widget['settings'];

  $form['progress_indicator'] = array(
    '#type' => 'radios',
    '#title' => t('Progress indicator'),
    '#options' => array(
      'throbber' => t('Throbber'),
      'bar' => t('Bar with progress meter'),
    ),
    '#default_value' => $settings['progress_indicator'],
    '#description' => t('The throbber display does not show the status of uploads but takes up less space. The progress bar is helpful for monitoring progress on large uploads.'),
    '#weight' => 16,
    '#access' => file_progress_implementation(),
  );

  return $form;
}

/**
 * Implements hook_field_widget_form().
 */
function audiofield_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  // Add display_field setting; file_field_widget_form() assumes it is set.
  $field['settings']['display_field'] = 0;

  $elements = file_field_widget_form($form, $form_state, $field, $instance, $langcode, $items, $delta, $element);

  if ($field['cardinality'] == 1) {
    // If there's only one field, return it as delta 0.
    if (empty($elements[0]['#default_value']['fid'])) {
      $elements[0]['#description'] = theme('file_upload_help', array(
        'description' => $instance['description'],
        'upload_validators' => $elements[0]['#upload_validators'],
      ));
    }
  }
  else {
    $elements['#file_upload_description'] = theme('file_upload_help', array('upload_validators' => $elements[0]['#upload_validators']));
  }

  return $elements;
}

/**
 * An element #process callback for the audiofield_widget field type.
 *
 * Display audio player in node edit mode.
 */
function audiofield_field_widget_process($element, &$form_state, $form) {
  $fid = isset($element['#value']['fid']) ? $element['#value']['fid'] : 0;

  $item = $element['#value'];
  $item['fid'] = $element['fid']['#value'];
  $field = field_widget_field($element, $form_state);
  $instance = field_widget_instance($element, $form_state);
  // Add the display field if enabled.
  if (!empty($field['settings']['display_field']) && $item['fid']) {
    $element['display'] = array(
      '#type' => empty($item['fid']) ? 'hidden' : 'checkbox',
      '#title' => t('Include file in display'),
      '#value' => isset($item['display']) ? $item['display'] : $field['settings']['display_default'],
      '#attributes' => array('class' => array('file-display')),
    );
  }
  else {
    $element['display'] = array(
      '#type' => 'hidden',
      '#value' => '1',
    );
  }
  if ($fid && $element['#file']) {
    $info = pathinfo($element['#file']->uri);
    $op = $info['extension'];
    $element['filename'] = array(
      'player' => audiofield_get_player($element['#file']->uri, $op),
      '#weight' => -10,
    );
  }

  // Add the description field if enabled.
  if (!empty($instance['settings']['description_field']) && $item['fid']) {
    $element['description'] = array(
      '#title' => t('Description'),
      '#value' => isset($item['description']) ? $item['description'] : '',
      '#type' => variable_get('file_description_type', 'textfield'),
      '#maxlength' => variable_get('file_description_length', 128),
      '#description' => t('The description may be used as the label of the link to the file.'),
    );
  }

  /*
   * Adjust the Ajax settings so that on upload and remove of any individual
   * file, the entire group of file fields is updated together.
   */
  if ($field['cardinality'] != 1) {
    $parents = array_slice($element['#array_parents'], 0, -1);
    $new_path = 'file/ajax/' . implode('/', $parents) . '/' . $form['form_build_id']['#value'];
    $field_element = drupal_array_get_nested_value($form, $parents);
    $new_wrapper = $field_element['#id'] . '-ajax-wrapper';
    foreach (element_children($element) as $key) {
      if (isset($element[$key]['#ajax'])) {
        $element[$key]['#ajax']['path'] = $new_path;
        $element[$key]['#ajax']['wrapper'] = $new_wrapper;
      }
    }
    unset($element['#prefix'], $element['#suffix']);
  }

  /*
   * Add another submit handler to the upload and remove buttons, to implement
   * functionality needed by the field widget. This submit handler, along with
   * the rebuild logic in audiofield_field_widget_form() requires the entire
   * field, not just the individual item, to be valid.
   */
  foreach (array('upload_button', 'remove_button') as $key) {
    $element[$key]['#submit'][] = 'file_field_widget_submit';
    $element[$key]['#limit_validation_errors'] = array(
      array_slice($element['#parents'], 0, -1),
    );
  }

  return $element;
}

/**
 * Implements hook_field_formatter_info().
 */
function audiofield_field_formatter_info() {
  $formatters = array(
    'audiofield_player' => array(
      'label' => t('AudioField player'),
      'field types' => array('file'),
      'description' => t('Displays an Audiofield player.'),
      'settings' => array(
        'audiofield_audioplayer_mp3' => 'html5',
        'audiofield_audioplayer_ogg' => 'html5',
        'audiofield_audioplayer_wav' => 'html5',
        'audiofield_audioplayer_opus' => 'html5',
        'audiofield_audioplayer_webm' => 'html5',
        'audiofield_audioplayer_flac' => 'html5',
        'audiofield_audioplayer_mp4' => 'html5',
        'audiofield_audioplayer_oga' => 'html5',
        'download_link' => 0,
        'display_file_details' => 0,
        'audiofield_detail' => array(
          'filename' => 0,
          'filesize' => 0,
          'length' => 0,
          'codec' => 0,
          'channelmode' => 0,
          'samplerate' => 0,
          'bitrate' => 0,
          'tags_id3' => 0,
          'tags_id3_picture' => 0,
        ),
      ),
    ),
  );

  return $formatters;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function audiofield_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  if ($display['type'] == 'audiofield_player') {
    // Get the players.
    $players = audiofield_players();

    // Build the summary.
    $summary = array();

    // Loop over each available filetype for the field and show the player.
    $accepted_filetypes = $instance['settings']['file_extensions'];
    foreach (preg_split('/\s+/', $accepted_filetypes) as $filetype) {
      $summary[] = array(
        '#type' => 'item',
        '#title' => t('Selected audio player (@filetype)', array(
          '@filetype' => $filetype,
        )),
        '#markup' => $players[$settings['audiofield_audioplayer_' . $filetype]]['name'],
      );
    }
    // Additional settings.
    $summary[] = array(
      '#type' => 'item',
      '#title' => t('Display Download Link'),
      '#markup' => ($settings['download_link'] == 1 ? t('Yes') : t('No')),
    );
    $summary[] = array(
      '#type' => 'item',
      '#title' => t('Display File Details'),
      '#markup' => ($settings['display_file_details'] == 1 ? t('Yes') : t('No')),
    );
  }
  return drupal_render($summary);
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function audiofield_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $form = array();

  // Load settings for audiofield players.
  if ($display['type'] == 'audiofield_player') {

    // Get all the player data.
    $players = array();
    foreach (audiofield_players() as $id => $player) {
      if ((isset($player['path']) && file_exists($player['path'])) || (isset($player['local']) && $player['local']) || (isset($player['module']) && module_exists($player['module']))) {
        foreach ($player['filetypes'] as $filetype) {
          $players[$filetype][$id] = $player['name'];
        }
      }
    }

    // Info on accepted filetypes.
    $accepted_filetypes = $instance['settings']['file_extensions'];
    $form['accepted_filetypes'] = array(
      '#type' => 'container',
      '#attributes' => array(),
      '#children' => t('Field accepts file types: %filetypes', array(
        '%filetypes' => $accepted_filetypes,
      )),
    );

    // Loop over each available filetype for the field.
    foreach (preg_split('/\s+/', $accepted_filetypes) as $filetype) {
      // Add a select box for this filetype if it exists.
      if (isset($players[$filetype])) {
        $form['audiofield_audioplayer_' . $filetype] = array(
          '#type' => 'select',
          '#title' => check_plain($filetype . ' ' . t('Audio Players')),
          '#options' => $players[$filetype],
          '#default_value' => $settings['audiofield_audioplayer_' . $filetype],
        );
      }
    }

    // Download settings.
    $form['download_link'] = array(
      '#title' => t('Display Download Link'),
      '#description' => t('This will allow users with the appropriate permissions to view a link to directly download the audio file.'),
      '#type' => 'select',
      '#options' => array(
        0 => t('No'),
        1 => t('Yes'),
      ),
      '#default_value' => $settings['download_link'],
    );

    // File details settings.
    $form['display_file_details'] = array(
      '#title' => t('Display File Details'),
      '#description' => t('This will display additional details about the audio file.'),
      '#type' => 'select',
      '#options' => array(
        0 => t('No'),
        1 => t('Yes'),
      ),
      '#default_value' => $settings['display_file_details'],
    );

    // Determine if the probes are installed.
    $detect_ffprobe = audiofield_accessible_ffprobe();
    $detect_getid3 = audiofield_accessible_getid3();

    $form['audiofield_detail'] = array(
      '#type' => 'fieldset',
      '#title' => t('Show details'),
      'description' => array(
        '#theme' => 'item_list',
        '#type' => 'ul',
        '#prefix' => t('These details are stored on each mp3 file either as part of the file or as ID3 tags. To display these details, one of the following tools must be installed:'),
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
        '#suffix' => t('You can set the path for these tools on the !admin_link.', array(
          '!admin_link' => l(t('Audio Field admin page'), 'admin/config/media/audiofield'),
        )),
        '#attributes' => array(),
      ),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
    );

    $form['audiofield_detail']['filename'] = array(
      '#type' => 'select',
      '#title' => t('Filename'),
      '#options' => array(
        '0' => t('- None -'),
        '1' => t('Filename'),
        '2' => t('Filename (remove extension)'),
        '3' => t('File extension only'),
      ),
      '#default_value' => $settings['audiofield_detail']['filename'],
    );

    $form['audiofield_detail']['filesize'] = array(
      '#type' => 'select',
      '#title' => t('Filesize'),
      '#options' => array(
        '0' => t('- None -'),
        '1' => t('Drupal format'),
        '2' => t('Row format'),
      ),
      '#default_value' => $settings['audiofield_detail']['filesize'],
    );

    // These options are only available if one of the ID3 readers is installed.
    if ($detect_getid3 || $detect_ffprobe) {
      $form['audiofield_detail']['length'] = array(
        '#type' => 'select',
        '#title' => t('Length'),
        '#options' => array(
          '0' => t('- None -'),
          '1' => t('H:m:s'),
          '2' => t('seconds'),
        ),
        '#default_value' => $settings['audiofield_detail']['length'],
      );
      $form['audiofield_detail']['codec'] = array(
        '#type' => 'select',
        '#title' => t('Codec'),
        '#options' => array(
          '0' => t('- None -'),
          '1' => t('Short name'),
          '2' => t('Long name'),
        ),
        '#default_value' => $settings['audiofield_detail']['codec'],
      );
      $form['audiofield_detail']['channelmode'] = array(
        '#type' => 'select',
        '#title' => t('Channel mode'),
        '#options' => array(
          '0' => t('- None -'),
          '1' => t('Stereo / Mono'),
        ),
        '#default_value' => $settings['audiofield_detail']['channelmode'],
      );
      $form['audiofield_detail']['samplerate'] = array(
        '#type' => 'select',
        '#title' => t('Sample rate'),
        '#options' => array(
          '0' => t('- None -'),
          '1' => t('Hz'),
        ),
        '#default_value' => $settings['audiofield_detail']['samplerate'],
      );
      $form['audiofield_detail']['bitrate'] = array(
        '#type' => 'select',
        '#title' => t('Bitrate'),
        '#options' => array(
          '0' => t('- None -'),
          'b/s' => t('b/s'),
          'bit/s' => t('bit/s'),
          'kb/s' => t('kb/s'),
          'kbit/s' => t('kbit/s'),
        ),
        '#default_value' => $settings['audiofield_detail']['bitrate'],
      );
    }
    if ($detect_getid3) {
      $form['audiofield_detail']['tags_id3'] = array(
        '#type' => 'select',
        '#title' => t('ID3 tags support'),
        '#options' => array(
          '0' => t('- None -'),
          'id3' => t('ID3 row data (id3v1 & id3v2)'),
          'title-artist' => t('Title - Artist'),
          'artist-title' => t('Artist - Title'),
          'title' => t('Title'),
          'title-year' => t('Title - Year'),
          'artist-album-title' => t('Artist - Album - Title'),
          'language' => t('Language'),
          'title-language' => t('Title - Language'),
        ),
        '#default_value' => $settings['audiofield_detail']['tags_id3'],
        '#description' => t('Limited on mp3 format'),
      );
      $form['audiofield_detail']['tags_id3_picture'] = array(
        '#type' => 'select',
        '#title' => t('Picture'),
        '#options' => array(
          '0' => t('- None -'),
          '50x50' => t('50x50 px'),
          '100x100' => t('100x100 px'),
          '150x150' => t('150x150 px'),
          '200x200' => t('200x200 px'),
          '300x300' => t('300x300 px'),
          '600x600' => t('600x600 px'),
          'original' => t('Original size'),
        ),
        '#default_value' => $settings['audiofield_detail']['tags_id3_picture'],
        '#description' => t('Limited on mp3 format'),
      );
    }
  }

  return $form;
}

/**
 * Implements hook_field_formatter_view().
 *
 * TODO: Implement playlists to group audios hold in multiple valued fields.
 */
function audiofield_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $elements = array();

  if ($display['type'] == 'audiofield_player') {
    // Get a list of all the players.
    $audio_players = audiofield_players();

    // Load the user so we can check permissions.
    global $user;
    // Loop over each file and format the player.
    foreach ($items as $delta => $item) {
      // Get file info.
      $fileinfo = pathinfo($item['uri']);
      // Determine the player for this item (default to HTML5).
      $selected_player = isset($display['settings']['audiofield_audioplayer_' . $fileinfo['extension']]) ? $display['settings']['audiofield_audioplayer_' . $fileinfo['extension']] : 'html5';
      $selected_player = $audio_players[$selected_player];
      // Set options for the item.
      $options = array(
        'entity_type' => $entity_type,
        'entity' => $entity,
        'field' => $field,
        'instance' => $instance,
        'langcode' => $langcode,
        'item' => $item,
        'display' => $display,
      );
      // Set the path to the audio player.
      $player_path = '';
      if (isset($selected_player['path'])) {
        $player_path = base_path() . $selected_player['path'];
      }

      // Render the player.
      $elements[$delta] = array(
        'player' => array(
          '#markup' => call_user_func($selected_player['callback'], $player_path, $item['uri'], $options),
        ),
      );

      // Display the file description if one is available.
      if (isset($item['description']) && !empty($item['description'])) {
        $elements[$delta]['description'] = array(
          '#type' => 'container',
          '#attributes' => array(
            'class' => 'description',
          ),
          '#children' => $item['description'],
        );
      }

      // Display download link if access granted.
      if ($display['settings']['download_link'] == 1 && (user_access('download all audio files') || ($user->uid == $item['uid'] && user_access('download own audio files')))) {
        $elements[$delta]['download'] = array(
          '#type' => 'container',
          '#attributes' => array(
            'class' => array(
              'audio-download',
            ),
          ),
          '#children' => t('<strong>Download</strong>: !file_link', array(
            '!file_link' => theme('file_link', array('file' => (object) $item)),
          )),
        );
      }

      // Display file details.
      if ($display['settings']['display_file_details'] == 1) {
        // GetID3/ffprobe details.
        $audio_details = audiofield_details_formatter($item['uri'], $display['settings']['audiofield_detail']);

        $elements[$delta]['file_details'] = array(
          '#type' => 'container',
          '#attributes' => array(
            'class' => array(
              'audio-details',
            ),
          ),
        );
        if (isset($audio_details['img']['attributes']['src'])) {
          $elements[$delta]['file_details'][] = array(
            '#type' => 'container',
            '#attributes' => array(
              'class' => array(
                'audiofield_img',
              ),
            ),
            array(
              '#theme' => 'html_tag',
              '#tag' => 'img',
              '#attributes' => $audio_details['img']['attributes'],
            ),
          );
        }

        $elements[$delta]['file_details'][] = array(
          '#theme' => 'item_list',
          '#items' => $audio_details['list'],
          '#title' => NULL,
          '#type' => 'ul',
          '#attributes' => array(
            'class' => array(
              'tips',
              'audiofield_detail',
            ),
          ),
        );
      }
    }
  }

  return $elements;
}
