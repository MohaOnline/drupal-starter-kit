<?php

/**
 * @file
 * Utility functions for Media: Skyfish.
 */

/**
 * Skyfish media browser form.
 */
function media_skyfish_add($form, &$form_state) {
  $token = $form_state['build_info']['args'][0];

  $form = array(
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'media_skyfish') . '/js/media_skyfish.browser.js',
      ),
      'css' => array(
        drupal_get_path('module', 'media_skyfish') . '/css/media_skyfish.browser.css',
      ),
    ),
    '#attributes' => array(
      'id' => 'media-skyfish-form',
    ),
  );

  if (!$token) {
    $msg = t('No valid token. Please login from <a href="@url" target="_blank">login page</a> to create a new token, then reopen the media browser.' , array(
      '@url' => url(media_skyfish_get_user_login_url(TRUE))
    ));
    $form['message'] = media_skyfish_get_form_error($msg);
    return $form;
  }

  // *TODO maybe we can preload this one somehow?
  $folders = media_skyfish_get_user_folders();
  $search_term = !empty($form_state['values']['skyfish_search_term']) ? $form_state['values']['skyfish_search_term'] : '';

  if (empty($folders)) {
    $msg = isset($folders->message) ? $folders->message : t('Error while getting data');
    $form['message'] = media_skyfish_get_form_error($msg);
    return $form;
  }

  // Add search bar.
  $form['skyfish_search_term'] = array(
    '#type' => 'textfield',
    '#title' => t('Search'),
  );

  $form['skyfish_search_submit'] = array(
    '#type' => 'button',
    '#value' => t('Search'),
    '#default_value' => $search_term,
    '#ajax' => array(
      'callback' => 'media_skyfish_get_results',
      'wrapper' => 'media-skyfish-results',
      'method' => 'replace',
      'effect' => 'fade',
      'event' => 'click',
    ),
  );

  $dialog = '<div id="skyfish-dialog-wrapper">';
  $dialog .= '<div id="skyfish-info-dialog">';
  $dialog .= '<div id="skyfish-dialog-content">';
  $dialog .= '<div id="skyfish-dialog-content-inner">';
  $dialog .= '</div>';
  $dialog .= '<div id="skyfish-dialog-close">';
  $dialog .= t('Close');
  $dialog .= '</div>';
  $dialog .= '</div>';
  $dialog .= '</div>';
  $dialog .= '</div>';
  $form['skyfish_info_dialog'] = array(
    '#type' => 'markup',
    '#markup' => $dialog,
  );

  $form['skyfish'] = array(
    '#type' => 'vertical_tabs',
    '#prefix' => '<div id="media-skyfish-results">',
    '#suffix' => '</div>',
  );

  // Keep track of number of results.
  $result_count = 0;
  foreach ($folders as $folder) {
    // *TODO if there is more than 500 images in folder,
    // we need to do more request using media _offset parameter,
    // to get all images.
    // *TODO do loading just then when tab is active.
    // We want to avoid loading it every time when media popup is loaded.
    // *TODO load images just for first active folder.
    // For others load them when they are active.
    $images = media_skyfish_get_user_folder_images($folder->id, $search_term);
    $result_count += count($images);

    if (empty($images)) {
      continue;
    }

    // *TODO do pagination just for active folder.
    $pagination = media_skyfish_do_pagination($images, 49);

    $form['skyfish'][$folder->id] = array(
      '#type' => 'fieldset',
      '#title' => $folder->name,
      '#description' => isset($pagination['pagination']) ? $pagination['pagination'] : '',
    );

    foreach ($pagination['current'] as $image) {

      if (!isset($image->unique_media_id) || !isset($image->thumbnail_url)) {
        continue;
      }

      $output = theme('image', array(
        'path' => $image->thumbnail_url,
        'title' => 'title',
        'alt' => 'alt',
        'attributes' => array(
          'width' => 200,
          'hspace' => 10,
          'vspace' => 10,
          'data-image' => $image->unique_media_id,
        ),
        'getsize' => FALSE,
      ));

      // Set data for the preview.
      $info = media_skyfish_get_image_markup($image);
      // Information for dialog.
      $info .= media_skyfish_get_image_dialog_markup($image);

      $form['skyfish'][$folder->id][$image->unique_media_id] = array(
        '#type' => 'checkbox',
        '#title' => $info,
        '#prefix' => '<div class="media-skyfish-list">' . $output,
        '#suffix' => '</div>',
        '#attributes' => array(
          'class' => array(
            'media-skyfish-checkbox',
          ),
        ),
      );
    }
  }

  // Show notification when no results were found.
  if ($result_count == 0) {
    $form['skyfish']['skyfish_no_results'] = array(
      '#type' => 'markup',
      '#markup' => t('No images found.'),
    );
  } else {
    $form['skyfish']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
      '#attributes' => array(
        'id' => 'media-skyfish-form-submit',
      ),
      '#submit' => array(
        'media_skyfish_upload_image',
      ),
    );
  }

  return $form;
}

function media_skyfish_get_results($form, $form_state) {
  return $form['skyfish'];
}

function media_skyfish_get_image_markup($image) {
  $info = '';
  if (!empty($image->description)) {
    $desc_length = 40;
    $desc = strlen($image->description) > $desc_length ? substr($image->description, 0, $desc_length) . '...' : $image->description;
    $info .= '<div>';
    $info .= t('Description') . ': ';
    $info .= $desc;
    $info .= '</div>';
  }
  if (!empty($image->copyright)) {
    $cr_length = 40;
    $cr = strlen($image->copyright) > $cr_length ? substr($image->copyright, 0, $cr_length) . '...' : $image->copyright;
    $info .= '<div>';
    $info .= t('Copyright') . ': ';
    $info .= $cr;
    $info .= '</div>';
  }
  if (!empty($image->height) && !empty($image->width)) {
    $info .= '<div>';
    $info .= t('Dimensions') . ': ';
    $info .= $image->width . ' x ' . $image->height;
    $info .= '</div>';
  }

  $info .= '<div class="show-more" data-image="' . $image->unique_media_id . '">';
  $info .= t('Show more');
  $info .= '</div>';

  return $info;
}

function media_skyfish_get_image_dialog_markup($image) {
  $info = '<div class="dialog-info dialog-info-' . $image->unique_media_id .  '">';
  $info .= theme('image', array(
    'path' => $image->thumbnail_url,
    'title' => 'title',
    'alt' => 'alt',
    'attributes' => array(
      'width' => 200,
      'hspace' => 10,
      'vspace' => 10,
      'data-image' => $image->unique_media_id,
    ),
    'getsize' => FALSE,
  ));

  $info .= '<div>';
  $info .= '<strong>' . t('ID') . ': </strong>';
  $info .= $image->unique_media_id;
  $info .= '</div>';

  if (!empty($image->title)) {
    $info .= '<div>';
    $info .= '<strong>' . t('Title') . ': </strong>';
    $info .= $image->title;
    $info .= '</div>';
  }
  if (!empty($image->filename)) {
    $info .= '<div>';
    $info .= '<strong>' . t('Filename') . ': </strong>';
    $info .= $image->filename;
    $info .= '</div>';
  }
  if (!empty($image->height) && !empty($image->width)) {
    $info .= '<div>';
    $info .= '<strong>' . t('Dimensions') . ': </strong>';
    $info .= $image->width . ' x ' . $image->height;
    $info .= '</div>';
  }
  if (!empty($image->description)) {
    $info .= '<div>';
    $info .= '<strong>' . t('Description') . ': </strong>';
    $info .= $image->description;
    $info .= '</div>';
  }
  if (!empty($image->keywords)) {
    $info .= '<div>';
    $info .= '<strong>' . t('Keywords') . ': </strong>';
    $info .= implode(', ', $image->keywords);
    $info .= '</div>';
  }
  if (!empty($image->byline)) {
    $info .= '<div>';
    $info .= '<strong>' . t('Photographer') . ': </strong>';
    $info .= $image->byline;
    $info .= '</div>';
  }
  if (!empty($image->copyright)) {
    $info .= '<div>';
    $info .= '<strong>' . t('Copyright') . ': </strong>';
    $info .= $image->copyright;
    $info .= '</div>';
  }

  $info .= '</div>';

  return $info;
}

function media_skyfish_get_form_error($msg) {
  return array(
    array(
      '#type' => 'item',
      '#prefix' => '<p>',
      '#markup' => $msg,
      '#suffix' => '</p>',
    )
  );
}

/**
 * Form media skyfish image upload handler.
 */
function media_skyfish_upload_image($form, &$form_state) {
  // *TODO at the moment just single file upload works.
  // Maybe we should do multiple.
  foreach ($form_state['values'] as $key => $value) {
    if (!is_numeric($key)) {
      continue;
    }
    if ($value == 1) {
      $upload = $key;
      break;
    }
  }

  if (!isset($upload)) {
    form_set_error('skyfish', t('Error while trying to upload image. Please try again later.'));
    return $form;
  }

  // We need to get image name.
  $image_data = media_skyfish_do_request('/media/' . $upload);
  // Get download link for image.
  $image = media_skyfish_do_request('/media/' . $upload . '/download_location');

  if (empty($image_data) || empty($image)) {
    form_set_error('skyfish', t('Error while trying to get image. Please try again later.'));
    return $form;
  }

  global $user;
  // We store it to skyfish user folder. Allow modules to alter this path.
  $folder = file_default_scheme() . '://media-skyfish/' . $user->uid . '/';
  drupal_alter('media_skyfish_alter_path', $folder);
  $uri = $folder . $image_data->filename;
  file_prepare_directory($folder, FILE_CREATE_DIRECTORY);
  $file = system_retrieve_file($image->url, $uri, TRUE, FILE_EXISTS_RENAME);

  // Fire hook to allow modules to perform actions on created file.
  module_invoke_all('media_skyfish_upload_image', $file, $image, $image_data);

  // *TODO maybe we can add image title from image_data object.
  $url = 'media/browser';
  $parameters = array(
    'query' => array(
      'render' => 'media-popup',
      'fid' => $file->fid,
    ),
  );

  drupal_goto($url, $parameters);
}

/**
 * Get user folders from skyfish. Set it to cache.
 *
 * @return bool|mixed
 *   Data received from service.
 */
function media_skyfish_get_user_folders() {
  global $user;

  $cache_name = MEDIA_SKYFISH_NAMESPACE . 'folders_' . $user->uid;
  $folders = cache_get($cache_name);
  if (isset($folders->data) && time() < $folders->expire) {
    return $folders->data;
  }

  $folders = media_skyfish_do_request('/folder?exclude_per_user_system_folders=1');
  if (empty($folders)) {
    return FALSE;
  }

  // Set cache for 60 minutes.
  cache_set($cache_name, $folders, 'cache', time() + 60 * 60);
  return $folders;
}

/**
 * Get all folder images from skyfish.
 *
 * @param string $folder_id
 *   Folder id.
 *
 * @return array
 *   Array of images.
 */
function media_skyfish_get_user_folder_images($folder_id, $search_term) {
  global $user;
  $cache_name = MEDIA_SKYFISH_NAMESPACE . 'images_' . $user->uid . '_' . $folder_id. '_' . $search_term;
  $images = cache_get($cache_name);

  if (isset($images->data) && time() < $images->expire) {
    return $images->data;
  }

  $fields = media_skyfish_get_fields();
  $url = '/search?&return_values=' . implode('+', $fields) . '&folder_ids=' . $folder_id;
  $url = !empty($search_term) ? $url . '&q=' . $search_term : $url;
  $images = media_skyfish_do_request($url);

  // Set cache for 60 minutes.
  cache_set($cache_name, $images->response->media, 'cache', time() + 60 * 60);
  return $images->response->media;
}

/**
 * Do pagination for provided array.
 *
 * @param array $data
 *   Array containing data.
 * @param int $per_page
 *   How many items per page should be.
 *
 * @return array
 *   Array containing pagination html and current page array elements.
 */
function media_skyfish_do_pagination(array $data, $per_page = 25) {
  $total_array = count($data);
  if ($total_array <= $per_page) {
    return array('current' => $data);
  }

  $current_page = pager_default_initialize($total_array, $per_page);
  $chunks = array_chunk($data, $per_page, TRUE);
  $current = $chunks[$current_page];
  $pagination = theme('pager', array('quantity', count($current)));

  return array(
    'current' => $current,
    'pagination' => $pagination,
  );
}

/**
 * Do curl request to skyfish for provided url.
 *
 * @param string $url
 *   Request url.
 *
 * @return bool|array
 *   Object containing received data.
 */
function media_skyfish_do_request($url) {

  $token = media_skyfish_get_api_token();

  $params = [
    'headers' => [
      'Content-length' => '0',
      'Content-type' => 'application/json',
      'Authorization' => sprintf('CBX-SIMPLE-TOKEN Token=%s', $token),
    ],
  ];

  $response = drupal_http_request(MEDIA_SKYFISH_API_BASE_URL . $url, $params);

  if (isset($response->code) && $response->code == 200) {
    return json_decode($response->data);
  }

  return FALSE;
}

/**
 * Authenticate Skyfish API user.
 *
 * @param string $username
 *   The username for the skyfish account.
 * @param string $password
 *   The password for the skyfish account.
 */
function media_skyfish_authenticate($username, $password) {

  $url = MEDIA_SKYFISH_API_BASE_URL . MEDIA_SKYFISH_API_AUTHENTICATION_SLUG;
  $key = media_skyfish_variable_get('api_key');
  $secret = media_skyfish_variable_get('api_secret');
  $ts = time();
  $hmac = hash_hmac('sha1', sprintf("%s:%s", $key, $ts), $secret);

  $params = [
    'method' => 'POST',
    'headers' => [
      'Content-type' => 'application/json',
      'Accept' => 'application/json',
    ],
    'data' => json_encode([
      'username' => $username,
      'password' => $password,
      'key'      => $key,
      'ts'       => $ts,
      'hmac'     => $hmac,
    ]),
  ];

  $response = drupal_http_request($url, $params);

  if (isset($response->code) && $response->code == 200) {
    return json_decode($response->data);
  }
  else {
    throw new Exception('Authentication error');
  }

}

/**
 * Get Skyfish API token for current user.
 *
 * @return string|bool
 *   The token as a string or false
 */
function media_skyfish_get_api_token() {
  global $user;

  return db_select('skyfish_api_tokens', 'sat')
    ->fields('sat', array('token'))
    ->condition('uid', $user->uid)
    ->condition('valid_until', time(), '>')
    ->execute()
    ->fetchField();
}

/**
 * The fields to be returned from the Skyfish API.
 *
 * @return array
 */
function media_skyfish_get_fields() {
  return array(
    'byline',
    'copyright',
    'description',
    'height',
    'keywords',
    'thumbnail_url',
    'title',
    'unique_media_id',
    'width',
  );
}
