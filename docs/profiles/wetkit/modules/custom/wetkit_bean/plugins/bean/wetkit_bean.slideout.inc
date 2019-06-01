<?php
/**
 * @file
 * A bean default plugin.
 */

class WetKitSlideOutBean extends BeanPlugin {

  /**
   * Implements BeanPlugin::values().
   */
  public function values() {
    $values = array(
      'slideout_settings' => array(
        'configure_slideout_widget' => 'textarea',
        'configure_slideout_form' => '',
      ),
    );

    return array_merge(parent::values(), $values);
  }
  /**
   * Implements BeanPlugin::form().
   */
  public function form($bean, $form, &$form_state) {
    $form = array();

    $form['slideout_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Configure Slide Out Widget'),
      '#tree' => TRUE,
      '#weight' => 1,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['slideout_settings']['configure_slideout_widget'] = array(
      '#type' => 'select',
      '#title' => t('Configure Widget'),
      '#options' => array(
        'form' => t('Form'),
        'textarea' => t('HTML'),
      ),
      '#default_value' => isset($bean->slideout_settings['configure_slideout_widget']) ? $bean->slideout_settings['configure_slideout_widget'] : 'textarea',
    );
    $form['slideout_settings']['configure_slideout_form'] = array(
      '#type' => 'textfield',
      '#title' => t('Drupal Form:'),
      '#size' => 25,
      '#maxlength' => 32,
      '#default_value' => isset($bean->slideout_settings['configure_slideout_form']) ? $bean->slideout_settings['configure_slideout_form'] : '',
      '#states' => array(
        'visible' => array(
          ':input[name="slideout_settings[configure_slideout_widget]"]' => array('value' => 'form'),
        ),
      ),
    );

    return $form;
  }

  /**
   * Implements BeanPlugin::view().
   */
  public function view($bean, $content, $view_mode = 'full', $langcode = NULL) {

    // Settings.
    $render = isset($bean->slideout_settings['configure_slideout_widget']) ? $bean->slideout_settings['configure_slideout_widget'] : 'textarea';
    $form_name = isset($bean->slideout_settings['configure_slideout_form']) ? $bean->slideout_settings['configure_slideout_form'] : '';

    // Bean Field(s).
    $showtext = field_get_items('bean', $bean, 'title_field');
    $hidetext = field_get_items('bean', $bean, 'field_slideout_hide');
    $icon = field_get_items('bean', $bean, 'field_bean_icon');

    // Storage.
    $data = array();

    if (is_array($showtext)) {
      $showtext = reset($showtext);
      $showtext = $showtext['safe_value'];
    } else {
      $showtext = '';
    }

    if (is_array($hidetext)) {
      $hidetext = reset($hidetext);
      $hidetext = $hidetext['safe_value'];
    } else {
      $hidetext = '';
    }

    if (is_array($icon)) {
      $icon = reset($icon);
    } else {
      $icon = '';
    }

    // Render Cache or fallback to Bean.
    if ($render == 'form') {
      $data = array(
        '#pre_render' => array('_wetkit_bean_slideout_prerender'),
        '#cache' => array(
          'keys' => array('wetkit_bean', 'slideout', $form_name),
          'expire' => CACHE_TEMPORARY,
          'granularity' => DRUPAL_CACHE_PER_USER
        ),
        '#slideout_form' => $form_name,
      );
    }
    else {
      $data = $content['bean'];
    }

    // Render form.
    $form = array(
      '#theme' => 'wetkit_slideout',
      '#show_title' => $showtext,
      '#hide_title' => $hidetext,
      '#icon' => $icon,
      '#content' => $data,
      '#slideout_render' => $render,
    );

    return $form;
  }

  /**
   * Implements BeanPlugin::submit().
   */
  public function submit(Bean $bean) {
    return parent::submit($bean);
  }
}

/**
 * Pre Render handler for cache based bean handling.
 */
function _wetkit_bean_slideout_prerender($element) {
  $data = drupal_get_form($element['#slideout_form']);
  unset($data['wetkit_wetboew']['wetkit_wetboew_theme']['#description']);
  unset($data['wetkit_wetboew']['wetkit_wetboew_minified']['#description']);
  $element['content'] = $data;
  return $element;
}
