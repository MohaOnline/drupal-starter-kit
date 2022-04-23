<?php

namespace Drupal\pgbar\Source;

/**
 * Provides widget and functionality to add an external source.
 *
 * The external source overrides the configured source.
 */
class ExternalSource {

  /**
   * Build the configuration form widget.
   *
   * @param array $item
   *   A field item.
   * @param array $form
   *   A Form-API array to extend.
   *
   * @return array
   *   Form-API array.
   */
  public function widgetForm(array $item, array $form = []) {
    $item['options']['source'] += [
      'enable_external_url' => 0,
      'external_url' => '',
      'find_at' => '',
    ];
    $source_options = $item['options']['source'];
    $form['enable_external_url'] = [
      '#title' => t('Enable an external URL'),
      '#description' => t('This overrides the configured source.'),
      '#type' => 'checkbox',
      '#default_value' => $source_options['enable_external_url'],
    ];
    $form['external_url'] = [
      '#title' => t('External URL'),
      '#description' => t('Enter an external URL to poll.'),
      '#type' => 'textfield',
      '#default_value' => $source_options['external_url'],
    ];
    $form['find_at'] = [
      '#title' => t('Path to the value'),
      '#description' => t('Enter a path to the value in the JSON to use. Separate parts by ".", index numbers are allowed. The file needs to be valid JSON data.'),
      '#type' => 'textfield',
      '#default_value' => $source_options['find_at'],
    ];
    $form['#element_validate'][] = [$this, 'widgetValidate'];
    return $form;
  }


  /**
   * Element validate callback for widgetForm().
   *
   * @see Drupal\pgbar\Source\ExternalSource::widgetForm()
   */
  public function widgetValidate($element, &$form_state, $form) {
    $item = &$form_state['values'];
    foreach ($element['#parents'] as $key) {
      $item = &$item[$key];
    }
    if (!empty($item['external_url']) && !valid_url($item['external_url'])) {
      form_error($element['external_url'], t('%name: Please enter a valid URL for the external source.', ['%name' => $element['#title']]));
    }
    // A very basic regexp to do some trivial checking: Allow ASCII, ".", "-" and "_".
    if (!empty($item['find_at']) && !preg_match('/^[a-zA-Z0-9.-_]+$/', $item['find_at'])) {
      form_error($element['find_at'], t('%name: Please enter a valid JSON path for the external source.', ['%name' => $element['#title']]));
    }
  }

}
