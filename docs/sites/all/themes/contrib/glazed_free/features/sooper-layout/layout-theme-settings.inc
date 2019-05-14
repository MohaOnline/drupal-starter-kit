<?php

$form['glazed_free_settings']['layout'] = array(
  '#title' => t('Layout'),
  '#type' => 'fieldset',
  '#weight' => 5,
);

$form['glazed_free_settings']['layout']['boxed_layout'] = array(
  '#type' => 'checkbox',
  '#title' => t('Boxed Layout'),
  '#default_value' => ((theme_get_setting('boxed_layout') !== null)) ? theme_get_setting('boxed_layout') : 0,
  '#description' => t('By default containers and their backgrounds are full-width. Boxed containers will squeeze all container backgrounds into a centered column.'),
);

$form['glazed_free_settings']['layout']['sticky_footer'] = array(
  '#type' => 'checkbox',
  '#title' => t('Sticky Footer'),
  '#default_value' => ((theme_get_setting('sticky_footer') !== null)) ? theme_get_setting('sticky_footer') : 1,
  '#description' => t('Makes footer stick to the bottom of the browser on short pages. (Does not work with boxed layout)'),
);

$form['glazed_free_settings']['layout']['boxed_layout_boxbg'] = array(
  '#type' => 'textfield',
  '#title' => t('Box Background'),
  '#default_value' => ((theme_get_setting('boxed_layout_boxbg') !== null)) ? theme_get_setting('boxed_layout_boxbg') : '#ffffff',
  '#description' => t('Background color for main content while in boxed mode'),
  '#states' => array(
    'visible' => array(
      ':input[name="boxed_layout"]' => array('checked' => TRUE),
    ),
  ),
);

$form['glazed_free_settings']['layout']['grid'] = array(
  '#title' => t('Grid Settings'),
  '#type' => 'fieldset',
  '#collapsible' => TRUE,
  '#collapsed' => TRUE,
);

$form['glazed_free_settings']['layout']['grid']['box_max_width'] = array(
  '#type' => 'textfield',
  '#title' => t('Boxed Container Max-width'),
  '#default_value' => ((theme_get_setting('box_max_width') !== null)) ? theme_get_setting('box_max_width') : '1170',
  '#size' => 9,
  '#maxlength' => 9,
  '#description' => t('Width of the boxed layout container. Should be higher than content width.'),
  '#states' => array(
    'visible' => array(
      ':input[name="boxed_layout"]' => array('checked' => TRUE),
    ),
  ),
);

$form['glazed_free_settings']['layout']['grid']['layout_max_width'] = array(
  '#type' => 'textfield',
  '#title' => t('Content Max-width'),
  '#default_value' => ((theme_get_setting('layout_max_width') !== null)) ? theme_get_setting('layout_max_width') : '1170',
  '#size' => 9,
  '#maxlength' => 9,
  '#description' => t('Max width of the website container. 480px - 4100px. Default is 1170px. With Side-Header enabled we typically use 980px.'),
);

$form['glazed_free_settings']['layout']['grid']['gutter_horizontal'] = array(
  '#type' => 'textfield',
  '#title' => t('Space between columns'),
  '#default_value' => ((theme_get_setting('gutter_horizontal') !== null)) ? theme_get_setting('gutter_horizontal') : '30',
  '#size' => 9,
  '#maxlength' => 9,
  '#description' => t('Default is 30px.'),
);

$form['glazed_free_settings']['layout']['grid']['gutter_vertical'] = array(
  '#type' => 'textfield',
  '#title' => t('Space between rows'),
  '#default_value' => ((theme_get_setting('gutter_vertical') !== null)) ? theme_get_setting('gutter_vertical') : '30',
  '#size' => 9,
  '#maxlength' => 9,
  '#description' => t('Default is 30px.'),
);

$form['glazed_free_settings']['layout']['grid']['gutter_container'] = array(
  '#type' => 'textfield',
  '#title' => t('Container Space'),
  '#default_value' => ((theme_get_setting('gutter_container') !== null)) ? theme_get_setting('gutter_container') : '60',
  '#size' => 9,
  '#maxlength' => 9,
  '#description' => t('Default is 60px.'),
);

$form['glazed_free_settings']['layout']['mobile_layout'] = array(
  '#title' => t('Mobile view'),
  '#type' => 'fieldset',
  '#collapsible' => TRUE,
  '#collapsed' => TRUE,
);


$form['glazed_free_settings']['layout']['background'] = array(
  '#title' => t('Background Image'),
  '#type' => 'fieldset',
  '#collapsible' => TRUE,
  '#collapsed' => TRUE,
);

// @see glazed_free_helper.module glazed_free_helper_settings_form_submit()
$form['glazed_free_settings']['layout']['background']['background_image'] = array(
  '#type' => 'media',
  '#theme' => 'media_widget', // required as of media 7.x-2.0-beta2
  '#tree' => TRUE, // Required
  '#title' => t('Body Background'),
  '#description' => t('The default page title image can also be changed per node, using the Body image field in the node form.'),
  '#default_value' => theme_get_setting('background_image'),
  '#media_options' => array(
    'global' => array(
      'file_extensions' => 'gif png jpg jpeg', // File extensions
      'max_filesize' => '100 MB',
      'file_directory' => 'glazed-assets', // Will be a subdirectory of the files directory
      'types' => array('image'), // Refers to a file_entity bundle (such as audio, video, image, etc.)
    ),
  ),
);

$form['glazed_free_settings']['layout']['background']['background_image_style'] = array(
  '#type' => 'radios',
  '#title' => t('Background Style'),
  '#default_value' => ((theme_get_setting('background_image_style') !== null)) ? theme_get_setting('background_image_style') : 'cover',
  '#options' => array(
    'cover' => t('Cover'),
    'contain' => t('Contain'),
    'no_repeat' => t('No Repeat'),
    'repeat' => t('Repeat'),
  ),
);

$form['glazed_free_settings']['layout']['background']['background_image_position'] = array(
  '#type' => 'radios',
  '#title' => t('Background Position'),
  '#default_value' => ((theme_get_setting('background_image_position') !== null)) ? theme_get_setting('background_image_position') : 'center_center',
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


$form['glazed_free_settings']['layout']['mobile_layout']['gutter_horizontal_mobile'] = array(
  '#type' => 'textfield',
  '#title' => t('Space between columns'),
  '#default_value' => ((theme_get_setting('gutter_horizontal_mobile') !== null)) ? theme_get_setting('gutter_horizontal_mobile') : '30',
  '#size' => 9,
  '#maxlength' => 9,
  '#description' => t('Width of the horizontal gutter in Glazed mobile view (<1200px). 0 - 100px. Default is 30.'),
);

$form['glazed_free_settings']['layout']['mobile_layout']['gutter_vertical_mobile'] = array(
  '#type' => 'textfield',
  '#title' => t('Space between rows'),
  '#default_value' => ((theme_get_setting('gutter_vertical_mobile') !== null)) ? theme_get_setting('gutter_vertical_mobile') : '30',
  '#size' => 9,
  '#maxlength' => 9,
  '#description' => t('Width of the vertical gutter in Glazed mobile view (<1200px). 0 - 100px. Default is 30px.'),
);

$form['glazed_free_settings']['layout']['mobile_layout']['gutter_container_mobile'] = array(
  '#type' => 'textfield',
  '#title' => t('Mobile Container space'),
  '#default_value' => ((theme_get_setting('gutter_container_mobile') !== null)) ? theme_get_setting('gutter_container_mobile') : '30',
  '#size' => 9,
  '#maxlength' => 9,
  '#description' => t('Default is 30px.'),
);

$form['glazed_free_settings']['layout']['granular_boxed'] = array(
  '#title' => t('Full Width Regions and Content Types'),
  '#type' => 'fieldset',
  '#collapsible' => TRUE,
  '#collapsed' => TRUE,
);

$form['glazed_free_settings']['layout']['granular_boxed']['full_width_containers'] = array(
    '#type' => 'checkboxes',
    '#multiple' => TRUE,
    '#title' => t('Full width regions'),
    '#description' => t('Full width content regions have 2 use cases: creating blocks that span across the whole browser and creating layouts with full screen backgrounds within your content. If you want all content to be full width, don\'t use these settings but instead set the "Layout Max-width" setting to the maximum value.'),
    '#default_value' => ((theme_get_setting('full_width_containers') !== null)) ? theme_get_setting('full_width_containers') : 0,
    '#options' => array(
      'cnt_secondary_header' => t('Secondary Header'),
      'cnt_header_nav' => t('Header / Navigation'),
      'cnt_page_title' => t('Page Title'),
      'cnt_content_top' => t('Content Top'),
      'cnt_content_bottom' => t('Content Bottom'),
      'cnt_footer' => t('Footer'),
    ),
);

$default_options = _glazed_free_node_types_options();
foreach ($default_options as $key => $value) {
  if (strpos($key, 'drag_drop') !== FALSE) {
    $default_options[$key] = $key;
  }
  else {
    $default_options[$key] = 0;
  }
}
$form['glazed_free_settings']['layout']['granular_boxed']['full_width_content_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Full width content types'),
    '#description' => t('If you want to create Drag and Drop pages with full-width sections you need this setting. This only works if the pages have no sidebar blocks. When you enable sidebar blocks alongside your full-width content pages they will simply squish into fixed containers to make room for the sidebar.'),
    '#default_value' => ((theme_get_setting('full_width_content_types') !== null)) ? theme_get_setting('full_width_content_types') : $default_options,
    '#options' => _glazed_free_node_types_options(),
);

$default_options = _glazed_free_node_types_options();
foreach ($default_options as $key => $value) {
  if (in_array($key, array('blog', 'event', 'news', 'portfolio'))) {
    $default_options[$key] = $key;
  }
  else {
    $default_options[$key] = 0;
  }
}

$form['glazed_free_settings']['layout']['pagers'] = array(
  '#title' => t('Pagers'),
  '#type' => 'fieldset',
  '#collapsible' => TRUE,
  '#collapsed' => TRUE,
);

$form['glazed_free_settings']['layout']['pagers']['prevnext_content_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Previous / Next pager'),
    '#description' => t('Nodes in this content type will get previous/next links at the bottom.'),
    '#default_value' => ((theme_get_setting('prevnext_content_types') !== null)) ? theme_get_setting('prevnext_content_types') : $default_options,
    '#options' => _glazed_free_node_types_options(),
);
