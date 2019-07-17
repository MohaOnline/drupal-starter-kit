<?php

/**
 * Sharpens an image using imagick unsharpMaskImage().
 *
 * See http://php.net/manual/en/imagick.unsharpmaskimage.php
 * and http://www.imagemagick.org/script/command-line-options.php#unsharp
 *
 * @param $image
 *   An image object. The $image->resource value will be modified by this call.
 * @param $radius
 *   The radius of the sharpen. 0=auto.
 * @param $sigma
 *   The standard deviation of the gaussian, in pixels.
 * @param $amount
 *   The fraction of the difference between the original and the blur image that is added back into the original.
 * @param $threshold
 *   The threshold, as a fraction of QuantumRange, needed to apply the difference amount.
 * @return
 *   TRUE or FALSE, based on success.
 */
function image_imagick_sharpen(stdClass $image, $radius, $sigma, $amount, $threshold) {
  return $image->resource->unsharpMaskImage($radius, $sigma, $amount, $threshold);
}

/**
 * Implements the sharpen effect.
 *
 * @param $image
 *   An image object
 * @param array $data
 *   The data passed from the form
 */
function imagick_sharpen($image, $data = array()) {
  image_toolkit_invoke('sharpen', $image, $data);
}

/**
 * Settings form for the imagick sharpen effect.
 *
 * @param $action
 *   The saved action form parameters.
 */
function imagick_sharpen_form($data) {
  $data = array_merge(imagick_sharpen_defaults(), (array) $data);

  $form['radius'] = array(
    '#type' => 'textfield',
    '#title' => t('Radius'),
    '#description' => t('The radius of the Gaussian, in pixels, not counting the center pixel. Default is 0 which is auto-select.'),
    '#default_value' => $data['radius'],
    '#size' => 3,
  );
  $form['sigma'] = array(
    '#type' => 'textfield',
    '#title' => t('Sigma'),
    '#description' => t('The standard deviation of the Gaussian, in pixels. Default is 1.0.'),
    '#default_value' => $data['sigma'],
    '#size' => 3,
  );
  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount'),
    '#description' => t('The fraction of the difference between the original and the blur image that is added back into the original. Default is 1.0.'),
    '#default_value' => $data['amount'],
    '#size' => 3,
  );
  $form['threshold'] = array(
    '#type' => 'textfield',
    '#title' => t('Threshold'),
    '#description' => t('The threshold, as a fraction of QuantumRange, needed to apply the difference amount. Default is 0.05.'),
    '#default_value' => $data['threshold'],
    '#size' => 3,
  );
  return $form;
}

/**
 * Returns the default settings of this effect.
 */
function imagick_sharpen_defaults() {
  return array(
    'radius' => '0',
    'sigma' => '1.0',
    'amount' => '1.0',
    'threshold' => '0.05',
  );
}
