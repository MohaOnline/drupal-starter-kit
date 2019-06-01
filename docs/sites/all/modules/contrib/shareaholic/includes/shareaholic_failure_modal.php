<?php

  /**
   * @file
   *
   * This file is responsible for the failure
   * modal popup (rendering/and handling submit)
   */

  /**
  * Returns the form for the failure modal popup
  */
  function shareaholic_failure_modal_form() {
    $form['#theme'] = 'shareaholic_failure_modal';
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Retry'),
    );
    $form['submit']['#attributes']['class'][] = 'btn_main';
    return $form;
  }


  /**
  * Prepare variables to be used in the shareaholic_failure_modal
  * template
  */
  function template_preprocess_shareaholic_failure_modal(&$variables) {
    _prepare_template_form_variables($variables, 'shareaholic_failure_modal');
  }


  /**
  * Submit handler for the shareaholic_failure_modal
  * When submitted, try to create an api key for the user
  *
  */
  function shareaholic_failure_modal_form_submit($form, &$form_state) {
    ShareaholicUtilities::get_or_create_api_key();
  }