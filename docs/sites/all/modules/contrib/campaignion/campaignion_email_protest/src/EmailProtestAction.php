<?php

namespace Drupal\campaignion_email_protest;

use Drupal\campaignion_action\ActionBase;
use Drupal\campaignion_wizard\EmailProtestEmailStep;
use Drupal\little_helpers\Webform\Submission;

/**
 * Email protest action.
 */
class EmailProtestAction extends ActionBase {

  /**
   * Send protest emails for a submission.
   */
  public function sendEmail(Submission $submission) {
    $node = $this->node;
    if (isset($node->webform['emails'][EmailProtestEmailStep::WIZARD_PROTEST_EID])) {
      $email = &$node->webform['emails'][EmailProtestEmailStep::WIZARD_PROTEST_EID];
    }
    else {
      return;
    }

    $target_contact_id = $submission->valueByKey('email_protest_target');
    $targets = [];
    if ($target_contact_id) {
      // User selected a target.
      $targets[] = $this->emailByContactId($target_contact_id);
    }
    else {
      // No target selected. Send email to configured all targets.
      $field = $this->type->parameters['email_protest']['target_field'];
      $protest_targets = field_get_items('node', $node, $field);
      if ($protest_targets) {
        foreach($protest_targets as $target) {
          $targets[] = $this->emailByContactId($target['target_id']);
        }
      }
    }

    if ($targets) {
      $email['email'] = implode(',', array_filter($targets));
      $email['from_name'] = $submission->valueByKey('first_name') . ' ' . $submission->valueByKey('last_name');
      $email['template'] = $submission->valueByKey('email_body');
      $email['headers'] = array(
        'X-Mail-Domain' => variable_get('site_mail_domain', 'supporter.campaignion.org'),
        'X-Action-UUID' => $this->rootNodeUuid($node),
      );
    }
  }

  /**
   * Get email address by contact ID.
   */
  protected function emailByContactId($contact_id) {
    if ($contact = redhen_contact_load($contact_id)) {
      $items = field_get_items('redhen_contact', $contact, 'redhen_contact_email');
      return $items[0]['value'];
    }
  }

  /**
   * Get the UUID from the nodes translation source.
   */
  protected function rootNodeUuid($node) {
    if ($node->tnid != 0 && $node->tnid != $node->nid) {
      $node = node_load($node->tnid);
    }
    return $node->uuid;
  }

}
