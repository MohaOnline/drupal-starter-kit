<?php

/**
 * Adds trim to an image
 *
 * @param $image
 *   An image object. The $image->resource value will be modified by this call.
 * @param $fuzz
 *   The fuzz tolerance.
 * @return
 *   TRUE or FALSE, based on success.
 */
function image_imagick_trim(stdClass $image, $fuzz) {
  $res = $image->resource;

  if (!$res->trimImage($fuzz)) {
    return FALSE;
  }

  // Reset image dimensions
  $dimensions = $res->getImageGeometry();
  $res->setImagePage($dimensions['width'], $dimensions['height'], 0, 0);

  $image->info['width'] = $dimensions['width'];
  $image->info['height'] = $dimensions['height'];

  return $res;
}

/**
 * Implements the imagick trim effect.
 *
 * @param $image
 *   An image object
 * @param array $data
 *   The data passed from the form
 */
function imagick_trim($image, $data = array()) {
  image_toolkit_invoke('trim', $image, $data);
}

/**
 * Settings form for the imagick fuzz effect.
 *
 * @param $action
 *   The saved action form parameters.
 */
function imagick_trim_form($data) {
  $data = array_merge(imagick_trim_defaults(), (array) $data);

  $form['fuzz'] = array(
    '#type' => 'textfield',
    '#title' => t('Fuzz'),
    '#description' => t('The fuzz tolerance.'),
    '#default_value' => $data['fuzz'],
    '#size' => 3,
  );

  return $form;
}

/**
 * Returns the default settings of this effect.
 */
function imagick_trim_defaults() {
  return array(
    'fuzz' => 0,
  );
}
