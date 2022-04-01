<?php

/**
 * @file
 * Example implementations for all hooks that are
 * invoked by this module.
 */

use Drupal\campaignion\Contact;
use Drupal\little_helpers\Webform\Submission;

/**
 * @return array
 *   Config arrays indexed by (machine readable) content-type names:
 *     - action_class: The class representing actions of this type.
 *       Defaults to \Drupal\campiagnion_action\ActionBase
 *     - wizard_class: The class representing the wizard for creating nodes of
 *       this type.
 *     - parameters: For backwards compatibility. The values are merged into
 *       The main array.
 *     The whole config is passed as $parameters to the class constructors.
 */
function hook_campaignion_action_info() {
  $types['webform'] = array(
    'action_class' => '\\Drupal\\campaignion_action\\ActionBase',
    'wizard_class' => '\\Drupal\\campaignion_wizard\\WebformWizard',
    'thank_you_page' => array(
      'type' => 'thank_you_page',
      'reference' => 'field_thank_you_pages',
    ),
  );
  return $types;
}

/**
 * This hook is triggered asynchronously after an action has been taken.
 *
 * Use this whenever you do something lengthy based on an action. For example:
 *  - Import of supporter data into your CRM.
 *  - Calling external APIs.
 *
 * @param object $node The node object of the action.
 * @param \Drupal\little_helpers\Webform\Submission $submission The submission
 *   that can be used to obtain data from the submission.
 * @param int The timestamp of when the event happened that lead to this call.
 */
function hook_campaignion_action_taken($node, Submission $submissionObj, int $when) {
  $myCRM->import($node, $submissionObj);
}

/**
 * Change a contact after a submission has been imported.
 *
 * @param Drupal\campaignion\Contact $contact
 *   The contact that is modified/created during the import.
 * @param Drupal\campaignion\Submission $submission
 *   The submission being imported.
 * @param object $node
 *   The action.
 *
 * @return boolean
 *   TRUE if contact was changed by this hook implementation.
 */
function hook_campaignion_action_contact_alter(Contact $contact, Submission $submission, $node) {
}
