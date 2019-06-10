<?php

/**
 * @file
 * Configuration form for Lazy-load.
 */

/**
 * Lazy-load configuration form.
 */
function lazy_configuration_form($form, $form_state) {
  $defaults = variable_get('lazy_filter_defaults');
  $filter_enabled = lazy_is_filter_enabled();

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global settings'),
    '#collapsible' => FALSE,
  );

  $form['settings']['lazy_filter_alter_tag'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select the inline elements to be lazy-loaded via filter.'),
    '#options' => array(
      'img' => t('Images (%img tags)', array('%img' => '<img>')),
      'iframe' => t('Iframes (%iframe tags)', array('%iframe' => '<iframe>')),
    ),
    '#default_value' => variable_get('lazy_filter_alter_tag', $defaults['lazy_filter_alter_tag']),
    '#description' => t('Only selected tags will be lazy-loaded in activated text-formats.'),
    '#disabled' => !$filter_enabled,
  );

  $form['settings']['lazy_filter_placeholderSrc'] = array(
    '#type' => 'textfield',
    '#title' => t('Placeholder image URL'),
    '#description' => t('Default is %val', array('%val' => $defaults['lazy_filter_placeholderSrc'])),
    '#default_value' => variable_get('lazy_filter_placeholderSrc', $defaults['lazy_filter_placeholderSrc']),
    '#size' => 100,
  );

  $form['paths'] = array(
    '#type' => 'fieldset',
    '#title' => t('Disabled pages'),
    '#description' => t('<p>There could be pages Lazy-loading should be disabled; set them here.</p>'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['paths']['lazy_disabled_paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Lazy-loading is disabled for both <em>image fields</em> and <em>inline images/iframes</em> in content on following pages'),
    '#default_value' => variable_get('lazy_disabled_paths', $defaults['lazy_disabled_paths']),
    '#description' => t('Specify pages by using their paths. Enter one path per line.'),
  );

  $form['blazy'] = array(
    '#type' => 'fieldset',
    '#title' => t('bLazy configuration'),
    '#description' => t('<p><a href="!url">bLazy</a> is a lightweight lazy loading and multi-serving image script created by Bjoern Klinggaard. See its website for usage details and demos.</p>', array(
      '!url' => 'http://dinbror.dk/blog/blazy/',
    )),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['blazy']['lazy_filter_loadInvisible'] = array(
    '#type' => 'checkbox',
    '#title' => t('loadInvisible'),
    '#description' => t('If checked loads invisible (hidden) elements. Default is %val', array('%val' => ($defaults['lazy_filter_loadInvisible']) ? 'checked' : 'unchecked')),
    '#default_value' => variable_get('lazy_filter_loadInvisible', $defaults['lazy_filter_loadInvisible']),
    '#return_value' => TRUE,
  );

  $form['blazy']['lazy_filter_offset'] = array(
    '#type' => module_exists('elements') ? 'numberfield' : 'textfield',
    '#title' => t('offset'),
    '#description' => t('The offset controls how early you want the elements to be loaded before they’re visible. Default is %val, so %val pixel before an element is visible it’ll start loading.', array('%val' => $defaults['lazy_filter_offset'])),
    '#default_value' => variable_get('lazy_filter_offset', $defaults['lazy_filter_offset']),
  );

  $form['blazy']['lazy_filter_saveViewportOffsetDelay'] = array(
    '#type' => module_exists('elements') ? 'numberfield' : 'textfield',
    '#title' => t('saveViewportOffsetDelay'),
    '#description' => t('Delay for how often it should call the saveViewportOffset function on resize. Default is %val', array('%val' => $defaults['lazy_filter_saveViewportOffsetDelay'])),
    '#default_value' => variable_get('lazy_filter_saveViewportOffsetDelay', $defaults['lazy_filter_saveViewportOffsetDelay']),
  );

  $form['blazy']['lazy_filter_validateDelay'] = array(
    '#type' => module_exists('elements') ? 'numberfield' : 'textfield',
    '#title' => t('validateDelay'),
    '#description' => t('Delay for how often it should call the validate function on scroll/resize. Default is %val', array('%val' => $defaults['lazy_filter_validateDelay'])),
    '#default_value' => variable_get('lazy_filter_validateDelay', $defaults['lazy_filter_validateDelay']),
  );

  $form['blazy']['lazy_filter_selector'] = array(
    '#type' => 'textfield',
    '#title' => t('Selector class'),
    '#description' => t('Element selector for elements that should lazy load. Do not include a leading period. Default is %val', array('%val' => $defaults['lazy_filter_selector'])),
    '#default_value' => variable_get('lazy_filter_selector', $defaults['lazy_filter_selector']),
  );

  $form['blazy']['lazy_filter_skipClass'] = array(
    '#type' => 'textfield',
    '#title' => t('skipClass'),
    '#description' => t('Elements having this class name will be ignored. Default is %val', array('%val' => $defaults['lazy_filter_skipClass'])),
    '#default_value' => variable_get('lazy_filter_skipClass', $defaults['lazy_filter_skipClass']),
  );

  $form['blazy']['lazy_filter_errorClass'] = array(
    '#type' => 'textfield',
    '#title' => t('errorClass'),
    '#description' => t('The classname an element will get if something goes wrong. Default is %val', array('%val' => $defaults['lazy_filter_errorClass'])),
    '#default_value' => variable_get('lazy_filter_errorClass', $defaults['lazy_filter_errorClass']),
  );

  $form['blazy']['lazy_filter_successClass'] = array(
    '#type' => 'textfield',
    '#title' => t('successClass'),
    '#description' => t('The classname an element will get when loaded. Default is %val', array('%val' => $defaults['lazy_filter_successClass'])),
    '#default_value' => variable_get('lazy_filter_successClass', $defaults['lazy_filter_successClass']),
  );

  $form['blazy']['lazy_filter_src'] = array(
    '#type' => 'textfield',
    '#title' => t('src'),
    '#description' => t('Attribute where the original element source will be assigned. Do not change this unless attribute is used for other purposes. Default is %val', array('%val' => $defaults['lazy_filter_src'])),
    '#default_value' => variable_get('lazy_filter_src', $defaults['lazy_filter_src']),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  // Notify the user if the library is not available, and disable the form.
  $library_installed = variable_get('lazy_library_installed');
  if (!$library_installed) {
    drupal_set_message(t('The bLazy library could not be found.'), 'warning');
    drupal_set_message(t('Follow the instructions in the README.md file or in the <a href="!url">help</a> page for installing the bLazy library.', array('!url' => url('admin/help/lazy'))), 'status');

    $form['submit']['#attributes']['disabled'] = TRUE;
  }

  return $form;
}

/**
 * Submit handler for lazy_configuration_form().
 */
function lazy_configuration_form_submit($form, &$form_state) {
  $int_value = array(
    'lazy_filter_offset',
    'lazy_filter_saveViewportOffsetDelay',
    'lazy_filter_validateDelay',
  );
  $bool_value = array(
    'lazy_filter_loadInvisible',
  );

  // Exclude unnecessary elements.
  form_state_values_clean($form_state);
  foreach ($form_state['values'] as $key => $value) {
    if (in_array($key, $int_value)) {
      $value = intval($value);
    }
    if (in_array($key, $bool_value)) {
      $value = !!$value;
    }
    if ('string' === gettype($value)) {
      $value = trim($value);
    }
    variable_set($key, $value);
  }
  drupal_set_message(t('The configuration options have been saved.'));
}
