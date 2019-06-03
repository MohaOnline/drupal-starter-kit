<?php

/**
 * Specify available contact types. These types extend the functionality of
 * redhen contacts by some features. Namely importers and exporters.
 *
 * @return array
 *   An array keyed by redhen_contact->type. Foreach contact-type it contains a
 *   class name that points us to the class representing the contact-type
 *   The classes must implement the @see \Drupal\campaignion\CRM\ContactTypeInterface .
 */
function hook_campaignion_contact_type_info() {
  $types['supporter'] = '\\Drupal\\campaignion_supporter\\ContactType';
  return $types;
}

/**
 * Alter defined contact types.
 *
 * @param array $types
 *   Reference to the combined array of all implementations of
 *   @see hook_campaignion_contact_type_info().
 */
function hook_campaignion_contact_type_info_alter(&$types) {
  $types['supporter']['other'] = '\\Drupal\\other\\SupporterExporter';
}
