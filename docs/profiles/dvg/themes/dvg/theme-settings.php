<?php

function dvg_form_system_theme_settings_alter(&$form, &$form_state) {

  // Title stuff
  $form['general_settings']['title'] = array(
    '#type' => 'fieldset',
    '#title' => t('Head title'),
  );

  // Simpler frontpage title
  $form['general_settings']['title']['simpler_frontpage_title'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Simpler frontpage title'),
    '#default_value' => theme_get_setting('simpler_frontpage_title'),
    '#description'   => t('Removes the <code>[node:title]</code> from the head title on the frontpage.'),
  );

  // <head> <title> separator
  $form['general_settings']['title']['head_title_separator'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Head title separator: &lt;head&gt; &lt;title&gt;'),
    '#default_value' => theme_get_setting('head_title_separator'),
    '#description'   => t('This is how the multi part title is glued together. Including spaces!'),
  );

}
