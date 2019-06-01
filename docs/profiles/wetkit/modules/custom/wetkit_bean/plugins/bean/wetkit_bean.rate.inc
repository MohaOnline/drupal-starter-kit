<?php
/**
 * @file
 * Listing bean plugin.
 */

class WetKitRateBean extends BeanPlugin {
  /**
   * Declares default block settings.
   */
  public function values() {
    $values = array(
      'rate_settings' => array(
        'configure_rate_widget_span' => 'col-md-4',
        'rate_machine_name' => '',
      ),
    );

    return array_merge(parent::values(), $values);
  }
  /**
   * Builds extra settings for the block edit form.
   */
  public function form($bean, $form, &$form_state) {
    $form = array();

    $form['rate_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Configure Rate Widget'),
      '#tree' => TRUE,
      '#weight' => -8,
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['rate_settings']['configure_rate_widget_span'] = array(
      '#type' => 'select',
      '#title' => t('Configure the Rate Widget Span'),
      '#options' => array(
        'col-md-1' => t('Medium 1'),
        'col-md-2' => t('Medium 2'),
        'col-md-3' => t('Medium 3'),
        'col-md-4' => t('Medium 4'),
        'col-md-5' => t('Medium 5'),
        'col-md-6' => t('Medium 6'),
        'col-md-7' => t('Medium 7'),
        'col-md-8' => t('Medium 8'),
        'col-md-9' => t('Medium 9'),
        'col-md-10' => t('Medium 10'),
        'col-md-11' => t('Medium 11'),
        'col-md-12' => t('Medium 12'),
      ),
      '#default_value' => isset($bean->rate_settings['configure_rate_widget_span']) ? $bean->rate_settings['configure_rate_widget_span'] : '',
    );
    $form['rate_settings']['rate_machine_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Enter the machine name for the Rate widget'),
      '#default_value' => isset($bean->rate_settings['rate_machine_name']) ? $bean->rate_settings['rate_machine_name'] : '',
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => FALSE,
    );

    return $form;
  }

  /**
   * Displays the bean.
   */
  public function view($bean, $content, $view_mode = 'default', $langcode = NULL) {

    // Settings.
    $span = isset($bean->rate_settings['configure_rate_widget_span']) ? $bean->rate_settings['configure_rate_widget_span'] : '';
    $machine_name = isset($bean->rate_settings['rate_machine_name']) ? $bean->rate_settings['rate_machine_name'] : '';

    // Reset content.
    $content = array();

    // Rendered markup.
    if (arg(0) == 'node' && is_numeric(arg(1)) && ($node = node_load(arg(1))) && ($machine_name != '')) {
      $markup = rate_embed($node, $machine_name);
    }
    else {
      $markup = '<p>' . t('There is no node context for the rate widget to leverage.') . '</p>';
    }

    $content['rate_bean']['#markup'] = $markup;

    return $content;
  }
}
