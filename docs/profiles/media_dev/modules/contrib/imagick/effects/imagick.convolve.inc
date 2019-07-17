<?php

/**
 * Image effect form callback for the convolve image format effect.
 *
 * @param array $data
 *   The current configuration for this image effect.
 *
 * @return array
 *   The form definition for this effect.
 */
function image_imagick_convolve(stdClass $image, $kernel, $label) {
  $matrix = [];
  foreach(new RecursiveIteratorIterator(new RecursiveArrayIterator($kernel)) as $value) {
    $matrix[] = $value;
  }

  return $image->resource->convolveImage($matrix);
}

/**
 * Implements the imagick convolve effect.
 *
 * @param $image
 *   An image object
 * @param array $data
 *   The data passed from the form
 */
function imagick_convolve($image, $data = array()) {
  image_toolkit_invoke('convolve', $image, $data);
}

/**
 * Settings form for the imagick convolve effect.
 *
 * @param $action
 *   The saved action form parameters.
 */
function imagick_convolve_form($data) {
  $data = array_merge(imagick_convert_defaults(), (array) $data);

  if (isset($data['matrix'])) {
    $matrix_values = $data['matrix']['entries'];
  }
  else {
    $matrix_values = array_fill(0, 3, array_fill(0, 3, 1));
  }

  // kernel matrix inputs
  $form['matrix'] = array(
    '#type' => 'item',
    '#title' => t('Kernel matrix'),
    '#collapset' => FALSE,
    '#required' => TRUE,
    '#prefix' => '<div class="matrix-wrapper">',
    '#suffix' => '</div>',
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'imagick') . '/css/convolve-matrix.css'),
    )
  );
  $form['matrix']['entries'] = array();
  for ($i = 0; $i < 3; $i++) {
    $form['matrix']['entries'][$i] = array(
      '#type' => 'fieldset',
    );
    for ($j = 0; $j < 3; $j++) {
      $form['matrix']['entries'][$i][$j] = array(
        '#type' => 'textfield',
        '#title' => t('Matrix entry') . " ($i,$j)",
        '#title_display' => 'invisible',
        '#default_value' => $matrix_values[$i][$j],
        '#size' => 3,
        '#required' => TRUE,
        '#element_validate' => array('element_validate_number'),
      );
    }
  }

  // filter label input
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#description' => t('A label to identify this filter effect.'),
    '#default_value' => isset($data['label']) ? $data['label'] : '',
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Returns the default settings of this effect.
 */
function imagick_convolve_defaults() {
  return array(
    'matrix' => [],
    'label' => '',
  );
}

/**
 * @param $variables
 * @return string
 */
function theme_imagick_convolve_summary($variables) {
  return ' - ' . check_plain($variables['data']['label']);
}