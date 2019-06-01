<?php
/**
 * @file
 * A bean default plugin.
 */

class WetKitBean extends BeanPlugin {

  /**
   * Implements BeanPlugin::values().
   */
  public function values() {
    $values = array(
      'wetkit_bean_title_settings' => array(
        'classes' => '',
      ),
    );

    return array_merge(parent::values(), $values);
  }

  /**
   * Implements BeanPlugin::form().
   */
  public function form($bean, $form, &$form_state) {
    $form = array();

    $form['wetkit_bean_title_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Configure Title Field'),
      '#tree' => TRUE,
      '#weight' => 1,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['wetkit_bean_title_settings']['classes'] = array(
      '#description' => t('CSS classes to be added to the title field.'),
      '#type' => 'textfield',
      '#title' => t('CSS'),
      '#default_value' => isset($bean->wetkit_bean_title_settings['classes']) ? $bean->wetkit_bean_title_settings['classes'] : '',
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => FALSE,
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
