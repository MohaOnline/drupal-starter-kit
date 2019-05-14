<?php
$form['glazed_free_settings']['typography'] = array(
  '#title' => t('Typography'),
  '#type' => 'fieldset',
  '#weight' => 4,
  '#weight' => 15,
);

$form['glazed_free_settings']['typography']['body_line_height'] = array(
  '#type' => 'textfield',
  '#title' => t('Body Line Height'),
  '#default_value' => ((theme_get_setting('body_line_height') !== null)) ? theme_get_setting('body_line_height') : '1.42857',
  '#attributes' => array('class' => array('line-height-slider')),
  '#size' => 9,
  '#maxlength' => 9,
);

$form['glazed_free_settings']['typography']['body_font_size'] = array(
  '#type' => 'textfield',
  '#title' => t('Body Font Size'),
  '#default_value' => ((theme_get_setting('body_font_size') !== null)) ? theme_get_setting('body_font_size') : '14',
  '#size' => 9,
  '#maxlength' => 9,
);

$form['glazed_free_settings']['typography']['nav_font_size'] = array(
  '#type' => 'textfield',
  '#title' => t('Main menu Font Size'),
  '#default_value' => ((theme_get_setting('nav_font_size') !== null)) ? theme_get_setting('nav_font_size') : '14',
  '#size' => 9,
  '#maxlength' => 9,
);

$form['glazed_free_settings']['typography']['headings_line_height'] = array(
  '#type' => 'textfield',
  '#title' => t('Headings Line Height'),
  '#default_value' => ((theme_get_setting('headings_line_height') !== null)) ? theme_get_setting('headings_line_height') : '1.05',
  '#attributes' => array('class' => array('line-height-slider')),
  '#size' => 9,
  '#maxlength' => 9,
);

$form['glazed_free_settings']['typography']['scale_factor'] = array(
  '#type' => 'textfield',
  '#title' => t('Typography Scale Factor') . ' <small>' . t('(This overrides advanced type controls)') . '</small>',
  '#default_value' => ((theme_get_setting('scale_factor') !== null)) ? theme_get_setting('scale_factor') : '1.25',
  '#size' => 9,
  '#maxlength' => 9,
);

$form['glazed_free_settings']['typography']['advanced_type'] = array(
  '#title' => t('Advanced Type Controls'),
  '#type' => 'fieldset',
  '#collapsible' => TRUE,
  '#collapsed' => TRUE,
);

$form['glazed_free_settings']['typography']['advanced_type']['h1_font_size'] = array(
  '#type' => 'textfield',
  '#title' => t('H1 Font Size (Page Title)'),
  '#default_value' => ((theme_get_setting('h1_font_size') !== null)) ? theme_get_setting('h1_font_size') : '36',
  '#attributes' => array('class' => array('font-size-slider')),
  '#size' => 9,
  '#maxlength' => 9,
);

$form['glazed_free_settings']['typography']['advanced_type']['h2_font_size'] = array(
  '#type' => 'textfield',
  '#title' => t('H2 Font Size'),
  '#default_value' => ((theme_get_setting('h2_font_size') !== null)) ? theme_get_setting('h2_font_size') : '30',
  '#attributes' => array('class' => array('font-size-slider')),
  '#size' => 9,
  '#maxlength' => 9,
);

$form['glazed_free_settings']['typography']['advanced_type']['h3_font_size'] = array(
  '#type' => 'textfield',
  '#title' => t('H3 Font Size'),
  '#default_value' => ((theme_get_setting('h3_font_size') !== null)) ? theme_get_setting('h3_font_size') : '24',
  '#attributes' => array('class' => array('font-size-slider')),
  '#size' => 9,
  '#maxlength' => 9,
);

$form['glazed_free_settings']['typography']['advanced_type']['h4_font_size'] = array(
  '#type' => 'textfield',
  '#title' => t('H4 Font Size'),
  '#default_value' => ((theme_get_setting('h4_font_size') !== null)) ? theme_get_setting('h4_font_size') : '18',
  '#attributes' => array('class' => array('font-size-slider')),
  '#size' => 9,
  '#maxlength' => 9,
);

$form['glazed_free_settings']['typography']['advanced_type']['blockquote_font_size'] = array(
  '#type' => 'textfield',
  '#title' => t('Blockquote Font Size'),
  '#default_value' => ((theme_get_setting('blockquote_font_size') !== null)) ? theme_get_setting('blockquote_font_size') : '18',
  '#attributes' => array('class' => array('font-size-slider')),
  '#size' => 9,
  '#maxlength' => 9,
);

$form['glazed_free_settings']['typography']['advanced_type']['blockquote_line_height'] = array(
  '#type' => 'textfield',
  '#title' => t('Blockquote Line Height'),
  '#default_value' => ((theme_get_setting('blockquote_line_height') !== null)) ? theme_get_setting('blockquote_line_height') : '1.5',
  '#attributes' => array('class' => array('line-height-slider')),
  '#size' => 9,
  '#maxlength' => 9,
);

$form['glazed_free_settings']['typography']['advanced_type']['headings_letter_spacing'] = array(
  '#type' => 'textfield',
  '#title' => t('Headings Letter Spacing'),
  '#default_value' => ((theme_get_setting('headings_letter_spacing') !== null)) ? theme_get_setting('headings_letter_spacing') : '0',
  '#size' => 9,
  '#maxlength' => 9,
);

$form['glazed_free_settings']['typography']['advanced_type']['headings_uppercase'] = array(
  '#type' => 'checkbox',
  '#title' => t('Headings Uppercase'),
  '#default_value' => ((theme_get_setting('headings_uppercase') !== null)) ? theme_get_setting('headings_uppercase') : FALSE,
  '#size' => 9,
  '#maxlength' => 9,
);


$form['glazed_free_settings']['typography']['advanced_type']['mobile_type'] = array(
  '#title' => t('Mobile Type Controls'),
  '#type' => 'fieldset',
  '#collapsible' => TRUE,
  '#collapsed' => TRUE,
);

$form['glazed_free_settings']['typography']['advanced_type']['mobile_type']['body_mobile_font_size'] = array(
  '#type' => 'textfield',
  '#title' => t('Body Font Size'),
  '#default_value' => ((theme_get_setting('body_mobile_font_size') !== null)) ? theme_get_setting('body_mobile_font_size') : '14',
  '#size' => 9,
  '#maxlength' => 9,
);

$form['glazed_free_settings']['typography']['advanced_type']['mobile_type']['nav_mobile_font_size'] = array(
  '#type' => 'textfield',
  '#title' => t('Nav Font Size'),
  '#default_value' => ((theme_get_setting('nav_mobile_font_size') !== null)) ? theme_get_setting('nav_mobile_font_size') : '14',
  '#size' => 9,
  '#maxlength' => 9,
);

$form['glazed_free_settings']['typography']['advanced_type']['mobile_type']['h1_mobile_font_size'] = array(
  '#type' => 'textfield',
  '#title' => t('H1 Font Size (Page Title)'),
  '#default_value' => ((theme_get_setting('h1_mobile_font_size') !== null)) ? theme_get_setting('h1_mobile_font_size') : '36',
  '#attributes' => array('class' => array('font-size-slider')),
  '#size' => 9,
  '#maxlength' => 9,
);

$form['glazed_free_settings']['typography']['advanced_type']['mobile_type']['h2_mobile_font_size'] = array(
  '#type' => 'textfield',
  '#title' => t('H2 Font Size'),
  '#default_value' => ((theme_get_setting('h2_mobile_font_size') !== null)) ? theme_get_setting('h2_mobile_font_size') : '30',
  '#attributes' => array('class' => array('font-size-slider')),
  '#size' => 9,
  '#maxlength' => 9,
);

$form['glazed_free_settings']['typography']['advanced_type']['mobile_type']['h3_mobile_font_size'] = array(
  '#type' => 'textfield',
  '#title' => t('H3 Font Size'),
  '#default_value' => ((theme_get_setting('h3_mobile_font_size') !== null)) ? theme_get_setting('h3_mobile_font_size') : '24',
  '#attributes' => array('class' => array('font-size-slider')),
  '#size' => 9,
  '#maxlength' => 9,
);

$form['glazed_free_settings']['typography']['advanced_type']['mobile_type']['blockquote_mobile_font_size'] = array(
  '#type' => 'textfield',
  '#title' => t('Blockquote Font Size'),
  '#default_value' => ((theme_get_setting('blockquote_mobile_font_size') !== null)) ? theme_get_setting('blockquote_mobile_font_size') : '18',
  '#attributes' => array('class' => array('font-size-slider')),
  '#size' => 9,
  '#maxlength' => 9,
);

$form['glazed_free_settings']['typography']['divider'] = array(
  '#title' => t('Divider'),
  '#type' => 'fieldset',
  '#collapsible' => TRUE,
  '#collapsed' => TRUE,
  '#suffix' => _glazed_free_type_preview(),
);

$form['glazed_free_settings']['typography']['divider']['divider_thickness'] = array(
  '#type' => 'textfield',
  '#title' => t('Divider Thickness'),
  '#default_value' => ((theme_get_setting('divider_thickness') !== null)) ? theme_get_setting('divider_thickness') : '4',
  '#size' => 9,
  '#maxlength' => 9,
);

$form['glazed_free_settings']['typography']['divider']['divider_length'] = array(
  '#type' => 'textfield',
  '#title' => t('Divider Length') . ' <small>' . t('(0 = full width)') . '</small>',
  '#default_value' => ((theme_get_setting('divider_length') !== null)) ? theme_get_setting('divider_length') : '0',
  '#size' => 9,
  '#maxlength' => 9,
);

$form['glazed_free_settings']['typography']['divider']['divider_position'] = array(
  '#type' => 'textfield',
  '#title' => t('Divider Position'),
  '#default_value' => ((theme_get_setting('divider_position') !== null)) ? theme_get_setting('divider_position') : '0',
  '#attributes' => array('class' => array('element-invisible')),
  '#size' => 9,
  '#maxlength' => 9,
  '#states' => array(
    'invisible' => array(
      ':input[name="divider_length"]' => array('value' => '0'),
    ),
  ),
);

$form['glazed_free_settings']['typography']['divider']['divider_color'] = array(
  '#type' => 'select',
  '#title' => t('Divider Color'),
  '#default_value' =>  theme_get_setting('divider_color'),
  '#options' => _glazed_free_color_options($theme),
);

$form['glazed_free_settings']['typography']['divider']['divider_color_custom'] = array(
  '#type' => 'textfield',
  '#title' => t('Custom Value'),
  '#default_value' => ((theme_get_setting('divider_color_custom') !== null)) ? theme_get_setting('divider_color_custom') : '#eeeeee',
  '#states' => array(
    'visible' => array(
      'select[name="divider_color"]' => array('value' => 'custom'),
    ),
  ),
);
