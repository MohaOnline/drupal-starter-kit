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
 *     - class: The type class representing actions of this type.
 *       Defaults to \Drupal\campiagnion_action\TypeBase
 *     - parameters: For backwards compatibility. The values are merged into
 *       The main array.
 *     The whole config array is passed as $parameters to the class constructor.
 */
function hook_campaignion_action_info() {
  $types['webform'] = array(
    'class' => 'Drupal\\campaignion_action\\FlexibleForm',
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
 * @param $node The node object of the action.
 * @param $submissionObj The \Drupal\little_helpers\Webform\Submission object
 *   that can be used to obtain data from the submission.
 */
function hook_campaignion_action_taken($node, Submission $submissionObj) {
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
