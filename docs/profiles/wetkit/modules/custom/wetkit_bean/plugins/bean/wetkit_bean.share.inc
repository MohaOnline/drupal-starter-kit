<?php
/**
 * @file
 * Listing bean plugin.
 */

class WetKitShareBean extends BeanPlugin {
  /**
   * Declares default block settings.
   */
  public function values() {
    $values = array(
      'share_settings' => array(
        'configure_share_widget' => 'col-md-6',
        'custom_css' => '',
      ),
    );

    return array_merge(parent::values(), $values);
  }

  /**
   * Builds extra settings for the block edit form.
   */
  public function form($bean, $form, &$form_state) {
    $form = array();

    $form['share_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Configure Share Widget'),
      '#tree' => TRUE,
      '#weight' => -8,
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['share_settings']['configure_share_widget'] = array(
      '#type' => 'select',
      '#title' => t('Configure Share Span'),
      '#options' => array(
        '' => t('None'),
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
      '#default_value' => isset($bean->share_settings['configure_share_widget']) ? $bean->share_settings['configure_share_widget'] : '',
    );
    $form['share_settings']['custom_css'] = array(
      '#type' => 'textfield',
      '#title' => t('Configure Shared Widget Custom CSS'),
      '#default_value' => isset($bean->share_settings['custom_css']) ? $bean->share_settings['custom_css'] : '',
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

    $span = isset($bean->share_settings['configure_share_widget']) ? $bean->share_settings['configure_share_widget'] : '';
    $custom_css = isset($bean->share_settings['custom_css']) ? $bean->share_settings['custom_css'] : '';

    // Sites.
    $sites = array();
    if (!empty($bean->field_share_widget)) {
      foreach ($bean->field_share_widget[LANGUAGE_NONE] as $service) {
        $sites[] = $service['value'];
      }
    }

    // Reset content.
    $content = array();

    // JSON params.
    $arr = array(
      'filter' => $sites,
      'pnlId' => 'pnl1',
      'lnkClass' => 'btn btn-default btn-block'
    );
    $arr = json_encode($arr);

    // Rendered markup.
    $markup = '<div class="clearfix">';
    $markup .= '<div class="wb-share ' . $span . ' ' . $custom_css . ' mrgn-bttm-sm" data-wb-share=\'' . $arr . '\'></div>';
    $markup .= '</div>';

    $content['share_bean']['#markup'] = $markup;

    return $content;
  }
}
