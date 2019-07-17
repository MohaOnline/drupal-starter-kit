<?php

/**
 * Autorotates an image
 *
 * @param $image
 *   An image object. The $image->resource value will be modified by this call.
 * @return
 *   TRUE or FALSE, based on success.
 */
function image_imagick_autorotate(stdClass $image, $data = array()) {
  $orientation = $image->resource->getImageOrientation();

  // See https://stackoverflow.com/a/40055711/8048794
  // First rotate to correct position
  switch($orientation) {
    case Imagick::ORIENTATION_BOTTOMRIGHT:
    case Imagick::ORIENTATION_BOTTOMLEFT:
      $image->resource->rotateimage(new ImagickPixel(), 180); // rotate 180 degrees
      break;
    case Imagick::ORIENTATION_RIGHTTOP:
    case Imagick::ORIENTATION_LEFTTOP:
      $image->resource->rotateimage(new ImagickPixel(), 90); // rotate 90 degrees CW
      break;
    case Imagick::ORIENTATION_LEFTBOTTOM:
    case Imagick::ORIENTATION_RIGHTBOTTOM:
      $image->resource->rotateimage(new ImagickPixel(), -90); // rotate 90 degrees CCW
      break;
  }

  // Flop image if required
  if (in_array($orientation, array(
    Imagick::ORIENTATION_TOPRIGHT,
    Imagick::ORIENTATION_BOTTOMLEFT,
    Imagick::ORIENTATION_LEFTTOP,
    Imagick::ORIENTATION_RIGHTBOTTOM))) {
    $image->resource->flopImage();
  }

  // Now that it's auto-rotated, make sure the EXIF data is correct in case the EXIF gets saved with the image!
  return $image->resource->setImageOrientation(Imagick::ORIENTATION_TOPLEFT);
}

/**
 * Imagick autorotate effect.
 *
 * @param $image
 *   An image object
 * @param array $data
 *   The data passed from the form
 */
function imagick_autorotate($image, $data = array()) {
  image_toolkit_invoke('autorotate', $image, $data);
}
