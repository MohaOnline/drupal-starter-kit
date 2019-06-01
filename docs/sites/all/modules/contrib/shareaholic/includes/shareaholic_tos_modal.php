<?php

  /**
   * @file
   *
   * This file is responsible for the terms of service
   * modal popup (rendering/and handling submit)
   */

  /**
   * Returns the form for the terms of service modal popup
   */
  function shareaholic_tos_modal_form() {
    $form['#theme'] = 'shareaholic_tos_modal';
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Get Started »'),
    );
    $form['submit']['#attributes']['class'][] = 'btn_main';
    return $form;
  }

  /**
   * Prepare variables to be used in the shareaholic_tos_modal
   * template
   */
  function template_preprocess_shareaholic_tos_modal(&$variables) {
    _prepare_template_form_variables($variables, 'shareaholic_tos_modal');
    $variables['image_url'] = SHAREAHOLIC_ASSET_DIR . '/img';
  }

  /**
   * Submit handler for the ToS modal: update values in the database
   * By storing 'true' for shareaholic_has_accepted_tos
   * and get or create an api key if it does not already exists
   *
   */
  function shareaholic_tos_modal_form_submit($form, &$form_state) {
    ShareaholicUtilities::accept_terms_of_service();
    ShareaholicUtilities::get_or_create_api_key();
  }
