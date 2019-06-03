<?php

use \Drupal\campaignion_newsletters\Subscriptions;

/**
 * Administration form.
 */
function campaignion_newsletters_admin_settings() {
  $form = array();

  $form['poll'] = array(
    '#type' => 'button',
    '#value' => t('Update Lists from all external sources now!'),
    '#description' => t('Updates run automatically around every hour in the background'),
    '#weight' => 20,
    '#executes_submit_callback' => TRUE,
    '#submit' => array('campaignion_newsletters_admin_poll'),
  );

  // Leave actual implementation to submodules for now.
  return system_settings_form($form);
}

/**
 * Submit callback for the polling button in the admin interface.
 */
function campaignion_newsletters_admin_poll() {
  drupal_set_message(t('Updating newsletter data...'));
  _campaignion_newsletters_poll();
}

/**
 * Implementation of campaignion_newsletters_form_redhen_contact_contact_form_alter().
 */
function _campaignion_newsletters_form_redhen_contact_contact_form_alter(&$form, &$form_state) {
  $mails = array();
  foreach ($form_state['redhen_contact']->allEmail() as $entry) {
    $mails[] = $entry['value'];
  }

  if (!count($mails)) {
    return;
  }

  $subscriptions = Subscriptions::byContact($form_state['redhen_contact']);
  $form_state['redhen_contact']->newsletters = $subscriptions;

  $options = $subscriptions->optionsArray();

  $fieldset = array(
    '#type' => 'fieldset',
    '#title' => t('Subscriptions'),
    '#collapsible' => FALSE,
    '#weight' => 10,
  );

  foreach ($mails as $mail) {
    $id = drupal_clean_css_identifier($mail);
    $fieldset[$id] = array(
      '#type' => 'checkboxes',
      '#title' => $mail,
      '#options' => $options,
      '#default_value' => $subscriptions->values($mail),
    );
  }

  $form['newsletters_subscriptions'] = $fieldset;

  array_unshift($form['actions']['submit']['#submit'], 'campaignion_newsletters_redhen_contact_submit');
}

/**
 * Submit handler for redhen_contact_contact_form.
 *
 * Store all subscriptions in the $contact to be updated on entity_save().
 */
function campaignion_newsletters_redhen_contact_submit($form, &$form_state) {
  $contact = $form_state['redhen_contact'];

  $values = array();
  foreach ($contact->allEmail() as $mail) {
    $email = $mail['value'];
    $id = drupal_clean_css_identifier($email);
    if (!empty($form_state['values'][$id])) {
      $values[$email] = $form_state['values'][$id];
    }
  }
  $contact->newsletters->update($values);
}
