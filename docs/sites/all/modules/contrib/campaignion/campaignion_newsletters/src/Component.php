<?php

namespace Drupal\campaignion_newsletters;

use Drupal\campaignion\CRM\Import\Source\WebformSubmission;
use Drupal\campaignion_opt_in\Values;
use Drupal\little_helpers\ArrayConfig;
use Drupal\little_helpers\Webform\Submission;

/**
 * Special functionality for the newsletter webform component.
 */
class Component {

  /**
   * The component array storing all configuration.
   *
   * @var array
   */
  protected $component;

  /**
   * Flag: Unsubscribe lists for which we don’t know of a subscription.
   *
   * @var bool
   */
  protected $unsubscribeUnknown;

  /**
   * All list_ids for this installation used for lazy-loading.
   *
   * @var int[]
   */
  protected $allListIds = NULL;

  /**
   * Get a new instance from a component array and reading the global config.
   *
   * @param array $component
   *   Component array as stored in `$node->webform['components']`.
   */
  public static function fromComponent(array $component) {
    $unsubscribe_unknown = variable_get_value('campaignion_newsletters_opt_out_unknown');
    return new static($component, $unsubscribe_unknown);
  }

  /**
   * Construct a new instance.
   *
   * @param array $component
   *   Component array as stored in `$node->webform['components']`.
   * @param bool $unsubscribe_unknown
   *   Whether to try to unsubscribe even from lists without a subscription.
   */
  public function __construct(array $component, $unsubscribe_unknown) {
    module_load_include('components.inc', 'webform', 'includes/webform');
    webform_component_defaults($component);
    $this->component = $component;
    $this->unsubscribeUnknown = $unsubscribe_unknown;
  }

  /**
   * Take appropriate actions when a webform submission is completed.
   *
   * @param string $email
   *   The email address found in this submission.
   * @param \Drupal\campaignion\CRM\Import\Source\WebformSubmission $source
   *   The webform submission that is being submitted.
   */
  public function submit($email, WebformSubmission $s) {
    if ($value = $s->valuesByCid($this->component['cid'])) {
      $value = Values::removePrefix($value);
      if ($value == 'opt-in') {
        $this->subscribe($email, $s);
      }
      elseif ($value == 'opt-out') {
        $this->unsubscribe($email);
      }
    }
  }

  /**
   * Unsubscribe the email address from configured/all newsletter lists.
   *
   * @param string $email
   *   The email address to unsubscribe.
   */
  public function unsubscribe($email) {
    $all_lists = !empty($this->component['extra']['optout_all_lists']);
    if ($all_lists) {
      if ($this->unsubscribeUnknown) {
        foreach ($this->getAllListIds() as $list_id) {
          $subscription = Subscription::byData($list_id, $email);
          if ($subscription->isNew()) {
            $subscription->queueUnsubscribe();
          }
        }
      }
      foreach (Subscription::byEmail($email) as $subscription) {
        $subscription->delete();
      }
    }
    else {
      $lists = $this->component['extra']['lists'];
      foreach (Subscription::byEmail($email) as $subscription) {
        if (!empty($lists[$subscription->list_id])) {
          $subscription->delete();
        }
      }
    }
  }

  /**
   * Subscribe an email address to all configured lists.
   *
   * @param string $email
   *   The email address to subscribe.
   * @param \Drupal\campaignion\CRM\Import\Source\WebformSubmission $source
   *   The importer source for further CRM action.
   */
  public function subscribe($email, WebformSubmission $source) {
    $extra = $this->component['extra'];
    $lists = array_keys(array_filter($extra['lists']));
    foreach ($lists as $list_id) {
      $subscription = Subscription::byData($list_id, $email, [
        'source' => $source,
        'needs_opt_in' => !$extra['opt_in_implied'],
        'send_welcome' => (bool) $extra['send_welcome'],
        'optin_statement' => $extra['optin_statement'],
        'optin_info' => FormSubmission::fromWebformSubmission($source),
      ]);
      $subscription->save();
    }
  }

  /**
   * Set the array of all list IDs. Usually only used for testing.
   *
   * @param int[]
   *   Array of list IDs.
   */
  public function setAllListIds(array $list_ids) {
    $this->allListIds = $list_ids;
  }

  /**
   * Get all list IDs.
   *
   * @return int[]
   *   Array of list IDs.
   */
  public function getAllListIds() {
    if (is_null($this->allListIds)) {
      $list_ids = [];
      foreach (NewsletterList::listAll() as $l) {
        $list_ids[] = $l->list_id;
      }
      $this->setAllListIds($list_ids);
    }
    return $this->allListIds;
  }

  /**
   * Remove list from all components.
   *
   * @param int $list_id
   *   The list_id that is to be removed.
   */
  public static function pruneList($list_id) {
    $node_controller = entity_get_controller('node');

    $result = db_select('webform_component', 'c')
      ->fields('c', ['nid', 'cid'])
      ->condition('type', 'opt_in')
      ->execute();
    foreach ($result as $row) {
      $node = node_load($row->nid);
      $component = $node->webform['components'][$row->cid];
      if (!empty($component['extra']['lists'][$list_id])) {
        unset($component['extra']['lists'][$list_id]);
        webform_component_update($component);
        $node_controller->resetCache([$row->nid]);
        $left = array_filter($component['extra']['lists']);
        if ($node->status) {
          watchdog('campaignion_newsletters', 'Removed list from published node#%nid(%type) “%title” lists left: %lists', [
            '%nid' => $node->nid,
            '%type' => $node->type,
            '%title' => $node->title,
            '%lists' => implode(', ', $left),
          ], $left ? WATCHDOG_NOTICE : WATCHDOG_WARNING);
        }
      }
    }
  }

}
