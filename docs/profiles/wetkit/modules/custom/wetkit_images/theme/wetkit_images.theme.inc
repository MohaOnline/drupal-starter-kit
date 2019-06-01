<?php
/**
 * @file
 * wetkit_images.theme.inc
 */

/**
 * Implements theme_picture().
 */
function wetkit_images_picture($variables) {
  // Make sure that width and height are proper values
  // If they exists we'll output them
  // @see http://www.w3.org/community/respimg/2012/06/18/florians-compromise/
  if (isset($variables['width']) && empty($variables['width'])) {
    unset($variables['width']);
    unset($variables['height']);
  }
  elseif (isset($variables['height']) && empty($variables['height'])) {
    unset($variables['width']);
    unset($variables['height']);
  }

  $sources = array();
  $output = array();

  // Fallback image, output as source with media query.
  $sources[] = array(
    'src' => _picture_image_style_url($variables['style_name'], $variables['uri']),
    'dimensions' => picture_get_image_dimensions($variables),
  );

  // All breakpoints and multipliers.
  foreach ($variables['breakpoints'] as $breakpoint_name => $multipliers) {
    $breakpoint = breakpoints_breakpoint_load_by_fullkey($breakpoint_name);
    if ($breakpoint) {
      $new_sources = array();
      switch (variable_get('picture_implementation', PICTURE_IMPLEMENTATION_WXT)) {
        case PICTURE_IMPLEMENTATION_WXT:
          foreach ($multipliers as $multiplier => $image_style) {
            $new_source = $variables;
            $new_source['style_name'] = $image_style;
            $new_source['#media_query'] = picture_get_multiplier_media_query($multiplier, $breakpoint->breakpoint);
            $new_sources[] = $new_source;
          }
          foreach ($new_sources as $new_source) {
            $sources[] = array(
              'src' => _picture_image_style_url($new_source['style_name'], $new_source['uri']),
              'dimensions' => picture_get_image_dimensions($new_source),
              'media' => $new_source['#media_query'],
            );
          }
          break;
    	}
    }
  }

  if (!empty($sources)) {
    $attributes = array();
    foreach (array('alt', 'title') as $key) {
      $field = sprintf('field_file_image_%s_text', $key);
      if (isset($variables[$key]) && !empty($variables[$key])) {
        $attributes['data-' . $key] = $variables[$key];
      }
      elseif (isset($variables[$field]) && is_array($variables[$field]) && isset($variables[$field]['und'][0]['safe_value'])) {
        $attributes['data-' . $key] = $variables[$field]['und'][0]['safe_value'];
      }
    }

    // Add attributes that are already prefixed by 'data-'
    foreach (array('data-picture-group', 'data-picture-align') as $key) {
      if (isset($variables[$key]) && !empty($variables[$key])) {
        $attributes[$key] = $variables[$key];
      }
    }

    // Add wet-boew reponsive image class
    $attributes['class'] = 'wet-boew-responsiveimg';

    // Add additional attributes passed in through the render array.
    if (isset($variables['attributes']) && !empty($variables['attributes'])) {
      $attributes = array_merge($attributes, $variables['attributes']);
    }
    $output[] = '<div data-picture="data-picture"' . drupal_attributes($attributes) . ' >';

    // Add source tags to the output.
    foreach ($sources as $source) {
      $output[] = theme('picture_source', $source);
    }

    // Output the fallback image.
    if (empty($variables['path'])) {
      $variables['path'] = $variables['uri'];
    }

    $output[] = '<noscript>' . theme('image_style', $variables) . '</noscript>';
    $output[] = '</div>';
    return implode("\n", $output);
  }
}

/**
 * Implements theme_picture_source().
 */
function wetkit_images_picture_source($variables) {
  $output = array();
  // Convert width, height to data-width, data-height.
  foreach (array('width', 'height') as $key) {
    if (isset($variables['dimensions'][$key])) {
      $variables['dimensions']['data-' . $key] = $variables['dimensions'][$key];
    }
    unset($variables['dimensions'][$key]);
  }

  if (isset($variables['media']) && !empty($variables['media'])) {
    if (isset($variables['srcset']) && !empty($variables['srcset'])) {
      $output[] = '<div data-media="' . $variables['media'] . '" data-srcset="' . $variables['srcset'] . '" ' . drupal_attributes($variables['dimensions']) . '></div>';
    }
    else {
      $output[] = '<div data-media="' . $variables['media'] . '" data-src="' . $variables['src'] . '" ' . drupal_attributes($variables['dimensions']) . '></div>';
    }
  }
  else {
    $output[] = '<div data-src="' . $variables['src'] . '" ' . drupal_attributes($variables['dimensions']) . '></div>';
  }
  return implode("\n", $output);
}
