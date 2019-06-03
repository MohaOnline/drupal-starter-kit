<?php
/**
 * @file
 * theming file.
 * NOTE: USERS TABLE TEMPLATE has been removed since version 2.0 
 */

 /**
  * Implements template_preprocess() hook
  * (prepares data for template to sign the document)
  */     
function template_preprocess_sign_for_acknowledgement_views(&$variables) {
  if (empty($variables['content'])) {
    $assoc = $variables['assoc'];
    $variables['content'] = drupal_render($assoc);
  }
}
 /**
  * Implements template_preprocess() hook
  * (prepares data for template to sign the document)
  */     
function template_preprocess_sign_for_acknowledgement_checkbox(&$variables) {
  $assoc = $variables['assoc'];
  $variables['content'] = drupal_render($assoc);
  $variables['user'] = user_load($GLOBALS['user']->uid);
  $variables['expire'] = _sign_for_acknowledgement_get_expiration_date();
  $variables['date'] = format_date(time(), 'long');
}
 /**
  * Implements template_preprocess() hook
  * (prepares data for template if document is signed)
  */     
function template_preprocess_sign_for_acknowledgement_checkbox_signed(&$variables) {
  $assoc = $variables['assoc'];
  // get message with sanitization
  $assoc['my_checkbox']['signature']['#title'] = filter_xss(variable_get('sign_for_acknowledgement_is_signed', t('You have already signed this document')));
  // Check and disable checkox.
  $assoc['my_checkbox']['signature']['#attributes'] = array(
    'checked' => 'checked',
    'disabled' => 'disabled',
  );
  $variables['content'] = drupal_render($assoc);
  $variables['user'] = user_load($GLOBALS['user']->uid);
  $variables['expire'] = _sign_for_acknowledgement_get_expiration_date();
}
 /**
  * Implements template_preprocess() hook
  * (prepares data for template if signature time is expired)
  */     
function template_preprocess_sign_for_acknowledgement_checkbox_expired(&$variables) {
  $variables['content'] = filter_xss(variable_get('sign_for_acknowledgement_is_expired', t('Can\'t sign for acknowledgement, deadline closed.')));
  $variables['user'] = user_load($GLOBALS['user']->uid);
  $variables['expire'] = _sign_for_acknowledgement_get_expiration_date();
  $variables['date'] = format_date(time(), 'long');
}
