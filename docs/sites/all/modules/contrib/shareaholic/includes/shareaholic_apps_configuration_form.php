<?php

  /**
   * @file
   *
   * This file is responsible for the apps
   * manager page (rendering/handling form)
   */

  /**
   * Returns the form to configure the shareaholic apps
   */
  function shareaholic_apps_configuration_form() {
    $form['#theme'] = 'shareaholic_apps_configuration';
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save Changes'),
    );
    $form['submit']['#attributes']['class'][] = 'settings';
    $form['submit']['#attributes']['onclick'] = 'this.value="' . t('Saving Changes...') . '"';
    return $form;
  }


  /**
   * Prepare variables to be used in the shareaholic_apps_configuration
   * template
   */
  function template_preprocess_shareaholic_apps_configuration(&$variables) {
    _prepare_template_form_variables($variables, 'shareaholic_apps_configuration');
  }


  /**
   * Submit handler for the shareaholic_apps_configuration form
   * When submitted, update the location settings
   *
   */
  function shareaholic_apps_configuration_form_submit($form, &$form_state) {
    $settings = ShareaholicUtilities::get_settings();
    if(empty($settings['recommendations']) || empty($settings['share_buttons'])) {
      return;
    }
    $form_input = $form_state['input'];
    $page_types = ShareaholicUtilities::page_types();
    foreach($page_types as $key => $page_type) {
      foreach(array('share_buttons', 'recommendations') as $app) {
        foreach(array('above', 'below') as $location) {
          $location_name = "{$page_type->type}_{$location}_content";

          if($location === 'above' && $app === 'recommendations') {
            continue;
          }

          if(!isset($form_input[$app][$location_name]) ||
              !isset($form_input[$app]["{$location_name}_location_id"]) ||
              $form_input[$app][$location_name] !== 'on') {
            $settings[$app][$location_name] = 'off';
          } else {
            $settings[$app][$location_name] = 'on';
            $settings['location_name_ids'][$app][$location_name] = $form_input[$app]["{$location_name}_location_id"];
          }

        }
      }
    }
    ShareaholicUtilities::set_settings($settings);
    ShareaholicUtilities::log_event('UpdatedSettings');
    drupal_set_message(t('Shareaholic Settings Saved'), 'status');
  }

