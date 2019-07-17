<?php

/**
 * Convert an image resource to grayscale.
 *
 * Note that transparent GIFs loose transparency when desaturated.
 *
 * @param $image
 *   An image object. The $image->resource value will be modified by this call.
 * @return
 *   TRUE or FALSE, based on success.
 *
 * @see image_desaturate()
 */
function image_imagick_desaturate(stdClass $image) {
  return _imagick_process_frames($image, 'image_imagick_desaturate_process', func_get_args());
}

/**
 * @param $res
 * @return bool
 */
function image_imagick_desaturate_process($res) {
  return (bool) $res->setImageType(imagick::IMGTYPE_GRAYSCALEMATTE);
}
