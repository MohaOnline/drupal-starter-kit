<?php
/**
 * @file
 * Definition of the 'Configure Panes' panel styles.
 */

$plugin = array(
  'title' => t('Configure Panes'),
  'description' => t('Wraps Panel Panes into different WetKit pre-configured styles.'),
  'render pane' => 'wetkit_configure_panes_render_pane',
  'pane settings form' => 'wetkit_configure_panes_style_settings_form',
  'weight' => -10,
);

/**
 * Theme function for the pane style.
 */
function theme_wetkit_configure_panes_render_pane($vars) {
  $content = $vars['content'];
  $pane = $vars['pane'];
  $display = $vars['display'];
  $settings = $vars['settings'];

  $class_stack = array();
  if (!empty($settings['configure_pane_styles'])) {
    array_push($class_stack, $settings['configure_pane_styles']);
  }
  if (!empty($settings['configure_pane_backgrounds'])) {
    array_push($class_stack, $settings['configure_pane_backgrounds']);
  }
  if (!empty($settings['configure_pane_modules'])) {
    array_push($class_stack, $settings['configure_pane_modules']);
  }
  if (!empty($settings['configure_pane_opacity'])) {
    array_push($class_stack, $settings['configure_pane_opacity']);
  }
  if (!empty($settings['configure_pane_custom_css'])) {
    array_push($class_stack, $settings['configure_pane_custom_css']);
  }
  $class_stack = implode(" ", $class_stack);
  if (!isset($content->css_class)) {
    $content->css_class = '';
  }
  $content->css_class .= ' ' . $class_stack;
  return theme('panels_pane', array(
    'content' => $content,
    'pane' => $pane,
    'display' => $display,
  ));
}

/**
 * Settings Form.
 */
function wetkit_configure_panes_style_settings_form($style_settings) {

  $form['configure_pane_styles'] = array(
    '#type' => 'select',
    '#title' => t('Configure Pane Styles'),
    '#options' => array(
      '' => t('None'),
      'visible-xs' => t('Visible Extra Small'),
      'visible-sm' => t('Visible Small'),
      'visible-md' => t('Visible Medium'),
      'visible-lg' => t('Visible Large'),
      'hidden-xs' => t('Hidden Extra Small'),
      'hidden-md' => t('Hidden Medium'),
      'hidden-lg' => t('Hidden Large'),
    ),
    '#default_value' => (isset($style_settings['configure_pane_styles'])) ? $style_settings['configure_pane_styles'] : '',
  );

  $form['configure_pane_backgrounds'] = array(
    '#type' => 'select',
    '#title' => t('Configure Pane Backgrounds'),
    '#options' => array(
      '' => t('None'),
      'bg-primary' => t('Background Primary'),
      'bg-success' => t('Background Success'),
      'bg-info' => t('Background Info'),
      'bg-warning' => t('Background Warning'),
      'bg-danger' => t('Background Danger'),
    ),
    '#default_value' => (isset($style_settings['configure_pane_backgrounds'])) ? $style_settings['configure_pane_backgrounds'] : '',
  );

  $form['configure_pane_modules'] = array(
    '#type' => 'select',
    '#title' => t('Configure Alert'),
    '#options' => array(
      '' => t('None'),
      'alert-success' => t('Alert Success'),
      'alert-info' => t('Alert Info'),
      'alert-warning' => t('Alert Warning'),
      'alert-danger' => t('Alert Danger'),
    ),
    '#default_value' => (isset($style_settings['configure_pane_modules'])) ? $style_settings['configure_pane_modules'] : '',
  );

  $form['configure_pane_opacity'] = array(
    '#type' => 'select',
    '#title' => t('Configure Pane Opacity'),
    '#options' => array(
      '' => t('None'),
      'opct-100' => t('100% opacity'),
      'opct-90' => t('90% opacity'),
      'opct-80' => t('80% opacity'),
      'opct-70' => t('70% opacity'),
      'opct-60' => t('60% opacity'),
      'opct-50' => t('50% opacity'),
      'opct-40' => t('40% opacity'),
      'opct-30' => t('30% opacity'),
      'opct-20' => t('20% opacity'),
      'opct-10' => t('10% opacity'),
    ),
    '#default_value' => (isset($style_settings['configure_pane_opacity'])) ? $style_settings['configure_pane_opacity'] : '',
  );

  $form['configure_pane_custom_css'] = array(
    '#type' => 'textfield',
    '#title' => t('Configure Pane Custom CSS'),
    '#default_value' => (isset($style_settings['configure_pane_custom_css'])) ? $style_settings['configure_pane_custom_css'] : '',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
  );

  return $form;
}
