<?php

$form['glazed_free_settings']['page_title'] = array(
  '#title' => t('Page Title'),
  '#type' => 'fieldset',
  '#weight' => 10,
);

$form['glazed_free_settings']['page_title']['page_title_breadcrumbs'] = array(
  '#type' => 'checkbox',
  '#title' => t('Show Breadcrumbs'),
  '#default_value' => ((theme_get_setting('page_title_breadcrumbs') !== null)) ? theme_get_setting('page_title_breadcrumbs') : 1,
);

$form['glazed_free_settings']['page_title']['page_title_home_hide'] = array(
  '#type' => 'checkbox',
  '#title' => t('Hide on Homepage'),
  '#default_value' => ((theme_get_setting('page_title_home_hide') !== null)) ? theme_get_setting('page_title_home_hide') : 1,
);

$form['glazed_free_settings']['page_title']['page_title_align'] = array(
    '#type' => 'radios',
    '#title' => t('Align Title'),
    '#default_value' => ((theme_get_setting('page_title_align') !== null)) ? theme_get_setting('page_title_align') : 'left',
    '#options' => array(
      'left' => t('Left'),
      'center' => t('Center'),
      'right' => t('Right')
    ),
    '#description' => t('Choose position of Page Title within page title header.'),
);

$form['glazed_free_settings']['page_title']['page_title_height'] = array(
  '#type' => 'textfield',
  '#title' => t('Height'),
  '#default_value' => ((theme_get_setting('page_title_height') !== null)) ? theme_get_setting('page_title_height') : '180',
  '#size' => 9,
  '#maxlength' => 9,
  '#description' => t('height of the header. 50px - 500px. Default is 180px.'),
);

$form['glazed_free_settings']['page_title']['page_title_animate'] = array(
  '#type' => 'radios',
  '#title' => t('Animation'),
  '#default_value' => ((theme_get_setting('page_title_animate') !== null)) ? theme_get_setting('page_title_animate') : '0',
  '#options' => array(
    '0' => t('No Animation'),
    'bounce' => t('Bounce'),
    'flash' => t('Flash'),
    'pulse' => t('Pulse'),
    'fadeIn' => t('Fade in'),
    'bounceInDown' => t('Bounce in down'),
    'bounceInUp' => t('Bounce in up'),
    'fadeInDown' => t('Fade in down'),
    'fadeInUp' => t('Fade in up'),
  ),
'#prefix' => '<br style="clear: both;"><br style="clear: both;">',
  '#description' => t('Choose <a target="_blank" href="@animatecss">animate.css</a> appear animation', array('@drupal-handbook' => 'https://daneden.github.io/animate.css/')),
);

$form['glazed_free_settings']['page_title']['background'] = array(
  '#title' => t('Background Image'),
  '#type' => 'fieldset',
  '#collapsible' => TRUE,
  '#collapsed' => TRUE,
);

// @see glazed_free_helper.module glazed_free_helper_settings_form_submit()
$form['glazed_free_settings']['page_title']['background']['page_title_image'] = array(
  '#type' => 'media',
  '#theme' => 'media_widget', // required as of media 7.x-2.0-beta2
  '#tree' => TRUE, // Required
  '#title' => t('Default Background'),
  '#description' => t('The default page title image can also be changed per node, using the Header image field in the node form.'),
  '#default_value' => theme_get_setting('page_title_image'),
  '#media_options' => array(
    'global' => array(
      'file_extensions' => 'gif png jpg jpeg', // File extensions
      'max_filesize' => '100 MB',
      'file_directory' => 'glazed-assets', // Will be a subdirectory of the files directory
      'types' => array('image'), // Refers to a file_entity bundle (such as audio, video, image, etc.)
    ),
  ),
);

$form['glazed_free_settings']['page_title']['background']['page_title_image_opacity'] = array(
  '#type' => 'textfield',
  '#title' => t('Backound Image Opacity'),
  '#default_value' => ((theme_get_setting('page_title_image_opacity') !== null)) ? theme_get_setting('page_title_image_opacity') : 1,
  '#size' => 9,
  '#maxlength' => 9,
  '#description' => t('Use this to blend the background image with a color. 0 is fully transparent and 1 is fully opaque.'),
);

$form['glazed_free_settings']['page_title']['background']['page_title_image_style'] = array(
  '#type' => 'radios',
  '#title' => t('Background Style'),
  '#default_value' => ((theme_get_setting('page_title_image_style') !== null)) ? theme_get_setting('page_title_image_style') : 'cover',
  '#options' => array(
    'cover' => t('Cover'),
    'contain' => t('Contain'),
    'no_repeat' => t('No Repeat'),
    'repeat' => t('Repeat'),
  ),
);

$form['glazed_free_settings']['page_title']['background']['page_title_image_mode'] = array(
  '#type' => 'radios',
  '#title' => t('Background Mode'),
  '#default_value' => ((theme_get_setting('page_title_image_mode') !== null)) ? theme_get_setting('page_title_image_mode') : 'normal',
  '#options' => array(
    'normal' => t('Normal'),
    'fixed' => t('Fixed (Parallax)'),
  ),
);

$form['glazed_free_settings']['page_title']['background']['page_title_image_position'] = array(
  '#type' => 'radios',
  '#title' => t('Background Position'),
  '#default_value' => ((theme_get_setting('page_title_image_position') !== null)) ? theme_get_setting('page_title_image_position') : 'center_center',
  '#options' => array(
    'center_center' => t('Center Center'),
    'left_top' => t('Left Top'),
    'left_center' => t('Left Center'),
    'left_bottom' => t('Left Bottom'),
    'right_top' => t('Right Top'),
    'right_center' => t('Right Center'),
    'right_bottom' => t('Right Bottom'),
    'center_bottom' => t('Center Bottom'),
  ),
);
