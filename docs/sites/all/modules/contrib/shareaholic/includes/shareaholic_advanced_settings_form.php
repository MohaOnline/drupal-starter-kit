<?php

  /**
   * @file
   *
   * This file is responsible for the advanced
   * settings form (rendering/and handling submit)
   */

  /**
   * The form object for the advanced settings
   *
   */
  function shareaholic_advanced_settings_form() {
    $disable_og_tags_checked = ShareaholicUtilities::get_option('disable_og_tags');
    $disable_internal_share_counts_api_checked = ShareaholicUtilities::get_option('disable_internal_share_counts_api');
    $form['advanced_settings'] = array(
      '#prefix' => '<fieldset class="app"><legend><h2>' . t('Advanced') . '</h2></legend>',
      '#suffix' => '</fieldset>',
    );
    $form['advanced_settings']['disable_og_tags'] = array(
      '#type' => 'checkbox',
      '#title' => t('Disable ') . '<code>' . t('Open Graph') . '</code>' . t(' tags (it is recommended NOT to disable open graph tags)'),
    );
    $form['advanced_settings']['disable_internal_share_counts_api'] = array(
      '#type' => 'checkbox',
      '#title' => t('Disable server-side Share Counts API (This feature uses server resources. When "enabled" share counts will appear for additional social networks.)'),
    );
    $form['advanced_settings']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save Changes')
    );
    $form['advanced_settings']['submit']['#attributes']['class'][] = 'settings';
    $form['advanced_settings']['submit']['#attributes']['onclick'][] = 'this.value="Saving Settings..."';
    if($disable_og_tags_checked === 'on') {
      $form['advanced_settings']['disable_og_tags']['#attributes'] = array('checked' => 'checked');
    }
    if($disable_internal_share_counts_api_checked === 'on') {
      $form['advanced_settings']['disable_internal_share_counts_api']['#attributes'] = array('checked' => 'checked');
    }
    return $form;
  }

  function shareaholic_advanced_settings_form_submit($form, &$form_state) {
    if(ShareaholicUtilities::has_tos_and_apikey()) {
      ShareaholicUtilities::update_options(array(
        'disable_og_tags' => ($form_state['values']['disable_og_tags'] === 1) ? 'on' : 'off',
        'disable_internal_share_counts_api' => ($form_state['values']['disable_internal_share_counts_api'] === 1) ? 'on' : 'off',
      ));
    drupal_set_message(t('Settings Saved: please clear your cache.'), 'status');
    }
  }
