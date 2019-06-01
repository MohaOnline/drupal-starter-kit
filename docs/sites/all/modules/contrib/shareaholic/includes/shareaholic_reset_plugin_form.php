<?php

  /**
   * @file
   *
   * This file is responsible for the reset
   * plugin form (rendering/handling)
   */

   /**
    * The form object for the reset plugin
    * The form will have button to reset the plugin
    *
    */
   function shareaholic_reset_plugin_form() {
     $form['reset'] = array(
       '#prefix' => '<fieldset class="app"><legend><h2>' . t('Reset') . '</h2></legend>',
       '#suffix' => '</fieldset>',
     );
     $form['reset']['submit'] = array(
       '#type' => 'submit',
       '#prefix' => '<p>' . t('This will reset all of your settings and start you from scratch. This can not be undone.') . '</p>',
       '#value' => t('Reset Module')
     );
     $form['reset']['submit']['#attributes']['class'][] = 'settings';
     $form['reset']['submit']['#attributes']['onclick'][] = 'this.value="Resetting..."';
     return $form;
   }

   /**
    * The submit handler for the reset plugin form
    * When the user resets the plugin, destroy settings
    * and get a new api key
    *
    */
   function shareaholic_reset_plugin_form_submit($form, &$form_state) {
     ShareaholicUtilities::reset_settings();
     drupal_set_message(t('Module has been reset. Please clear your cache.'), 'status');
   }

