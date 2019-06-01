<?php
/**
 * @file
 * Bean Slide plugin for Tabbed Interface.
 */

class WetKitBeanSlideTabbedInterfacePlugin implements WetKitBeanSlidePluginInterface {

  /**
   * Implements WetKitBeanSlidePluginInterface::attachments().
   */
  public function attachments() {}

  /**
   * Implements WetKitBeanSlidePluginInterface::form().
   */
  public function form($settings) {
    $form = array();

    $form['carousel'] = array(
      '#type' => 'select',
      '#title' => t('Carousel style'),
      '#options' => array(
        'carousel-s1' => 'carousel-s1',
        'carousel-s2' => 'carousel-s2',
      ),
      '#default_value' => $settings['carousel'],
    );

    $form['autoplay'] = array(
      '#type' => 'checkbox',
      '#title' => t('Auto Play'),
      '#default_value' => $settings['autoplay'],
    );

    $form['rotate_speed'] = array(
      '#type' => 'select',
      '#title' => t('Cycle speed'),
      '#options' => array(
        '' => t('none'),
        'fast' => t('Fast'),
        'slow' => t('Slow'),
        'custom' => t('custom'),
      ),
      '#default_value' => $settings['rotate_speed'],
    );

    $form['rotate_speed_custom'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom speed'),
      '#size' => 10,
      '#default_value' => isset($settings['rotate_speed_custom']) ? $settings['rotate_speed_custom'] : '',
      '#states' => array(
        'visible' => array(
          ':input[name="plugin_settings[rotate_speed]"]' => array('value' => 'custom'),
        ),
      ),
      '#description' => t('Enter a number of seconds.'),
    );

    $form['animate_effect'] = array(
      '#type' => 'select',
      '#title' => t('Animation effect'),
      '#options' => array(
        '' => 'none',
        'fade' => 'fade',
        'slide' => 'slide',
        'slidevert' => 'slidevert',
      ),
      '#default_value' => $settings['animate_effect'],
    );

    return $form;
  }

  /**
   * Implements WetKitBeanSlidePluginInterface::values().
   */
  public function values() {
    return array(
      'animate_effect' => 'fade',
      'autoplay' => TRUE,
      'rotate_speed' => 'slow',
      'carousel' => 'carousel-s1',
    );
  }

  /**
   * Implements WetKitBeanSlidePluginInterface::submit().
   */
  public function submit(&$settings) {

  }
}
