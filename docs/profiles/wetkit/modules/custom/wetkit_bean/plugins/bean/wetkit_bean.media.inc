<?php
/**
 * @file
 * A bean media plugin.
 */

class WetKitMediaBean extends BeanPlugin {

  /**
   * Implements BeanPlugin::values().
   */
  public function values() {
    return array(
      'image_style' => '<none>',
      'media_span' => 'col-md-3',
    );
  }

  /**
   * Implements BeanPlugin::form().
   */
  public function form($bean, $form, &$form_state) {

    $form['image_style'] = array(
      '#type' => 'select',
      '#title' => t('Image style'),
      '#options' => drupal_map_assoc(array_keys(image_styles())),
      '#default_value' => $bean->image_style,
      '#empty_value' => '<none>',
      '#weight' => 2,
    );

    $form['media_span'] = array(
      '#type' => 'select',
      '#title' => t('Media span'),
      '#options' => array(
        'col-md-2' => t('Span 2'),
        'col-md-3' => t('Span 3'),
        'col-md-4' => t('Span 4'),
        'col-md-5' => t('Span 5'),
        'col-md-6' => t('Span 6'),
        'col-md-7' => t('Span 7'),
        'col-md-8' => t('Span 8'),
        'col-md-9' => t('Span 9'),
        'col-md-10' => t('Span 10'),
        'col-md-11' => t('Span 11'),
        'col-md-12' => t('Span 12'),
      ),
      '#default_value' => isset($bean->media_span) ? $bean->media_span : '',
      '#weight' => 2,
    );

    return $form;
  }

  /**
   * Implements BeanPlugin::view().
   */
  public function view($bean, $content, $view_mode = 'full', $langcode = NULL) {
    return $content;
  }

  /**
   * Implements BeanPlugin::submit().
   */
  public function submit(Bean $bean) {
    return parent::submit($bean);
  }
}
