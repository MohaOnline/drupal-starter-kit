<?php

namespace Drupal\campaignion_email_to_target;

use Drupal\campaignion_action\ActionBase;
use Drupal\campaignion_action\ActionType;
use Drupal\campaignion_email_to_target\Api\Client;
use Drupal\campaignion_email_to_target\Channel\Email;
use Drupal\little_helpers\Services\Container;
use Drupal\little_helpers\Webform\Submission;

/**
 * Defines special behavior for email to target actions.
 *
 * Mainly deals with the configuration and with selecting messages / exclusion.
 */
class Action extends ActionBase {

  protected $options;
  protected $api;
  protected $parameters;

  /**
   * Create a new action instance.
   *
   * @param array $parameters
   *   Additional action parameters.
   * @param object $node
   *   The action’s node.
   * @param \Drupal\campaignion_email_to_target\Api\Client $api
   *   Api client for the e2t_api serivce.
   */
  public function __construct(array $parameters, $node, Client $api = NULL) {
    parent::__construct($parameters + [
      'channel' => Email::class,
    ], $node);
    $this->options = $this->getOptions();
    $this->api = $api ?? Container::get()->loadService('campaignion_email_to_target.api.Client');
  }

  /**
   * Choose an appropriate message for a given target.
   */
  public function getMessage($target) {
    $is_stub = empty($target['email']);
    $templates = MessageTemplate::byNid($this->node->nid);
    foreach ($templates as $t) {
      if ((!$is_stub || $t->type == 'exclusion') && $t->checkFilters($target)) {
        return $t->createInstance();
      }
    }
    watchdog('campaignion_email_to_target', 'No message found for target');
    return NULL;
  }

  /**
   * Get options for this action.
   */
  public function getOptions() {
    $field = $this->parameters['email_to_target']['options_field'];
    $items = field_get_items('node', $this->node, $field);
    return ($items ? $items[0] : []) + [
      'dataset_name' => 'mp',
      'users_may_edit' => '',
      'selection_mode' => 'one_or_more',
    ];
  }

  /**
   * Get configured no target message.
   */
  public function defaultExclusion() {
    $field = $this->parameters['email_to_target']['no_target_message_field'];
    $renderable = field_view_field('node', $this->node, $field, ['label' => 'hidden']);
    return new Exclusion(['message' => $renderable]);
  }

  /**
   * Get the selected dataset for this action.
   */
  public function dataset() {
    return $this->api->getDataset($this->getOptions()['dataset_name']);
  }

  /**
   * Create a link to view the action in test-mode.
   */
  public function testLink($title, $query = [], $options = []) {
    return $this->_testLink($title, $query, $options);
  }

  /**
   * Build selector for querying targets.
   *
   * For the moment the chosen selector as well as the filter mapping is
   * hard-coded.
   *
   * @param \Drupal\little_helpers\Webform\Submission $submission
   *   A webform submission object used to determine the selector values.
   *
   * @return string[]
   *   Query parameters used for filtering targets.
   *
   * @TODO: Make the selector configurable for datasets with more than one
   * possible selector.
   * @TODO: Make the mapping of form_keys to filter values configurable.
   */
  public function buildSelector(Submission $submission) {
    $dataset = $this->api->getDataset($this->options['dataset_name']);
    $selector_metadata = reset($dataset->selectors);
    $selector = [];
    foreach (array_keys($selector_metadata['filters']) as $filter_name) {
      $selector[$filter_name] = $submission->valueByKey($filter_name);
    }
    if (isset($selector['postcode'])) {
      $selector['postcode'] = preg_replace('/[ -]/', '', $selector['postcode']);
    }
    return $selector;
  }

  /**
   * Generate target message pairs for a submission.
   *
   * @param \Drupal\little_helpers\Webform\Submission $submission_o
   *   A submisson object.
   * @param bool $test_mode
   *   Whether to replace all target email addresses.
   *
   * @return array|\Drupal\campaignion_email_to_target\Exclusion
   *   Either an array of target messages pairs or an exclusion if no targets
   *   were found or all targets were excluded.
   */
  public function targetMessagePairs(Submission $submission_o, $test_mode = FALSE) {
    $email_override = $test_mode ? $submission_o->valueByKey('email') : NULL;

    $pairs = [];
    $exclusion = NULL;
    $token_defaults = [
      'first_name' => '',
      'last_name' => '',
      'title' => '',
      'salutation' => '',
    ];

    $selector = $this->buildSelector($submission_o);
    $contacts = $this->api->getTargets($this->options['dataset_name'], $selector);

    foreach ($contacts as $target) {
      if ($message = $this->getMessage($target)) {
        if ($email_override && isset($target['email'])) {
          $target['email'] = $email_override;
        }
        $message->replaceTokens($target + $token_defaults, $submission_o, TRUE);
        if ($message instanceof Exclusion) {
          // The first exclusion-message is used.
          if (!$exclusion) {
            $exclusion = $message;
          }
        }
        else {
          $pairs[] = [$target, $message];
        }
      }
    }

    if (empty($pairs)) {
      watchdog('campaignion_email_to_target', 'The API found no targets (dataset=@dataset, selector=@selector).', [
        '@dataset' => $this->options['dataset_name'],
        '@selector' => drupal_http_build_query($selector),
      ], WATCHDOG_WARNING);
      return $exclusion ? $exclusion : $this->defaultExclusion();
    }

    return $pairs;
  }

  /**
   * Get the configured channel.
   */
  public function channel() {
    return $this->pluginInstance($this->parameters['channel']);
  }

  /**
   * Create a new plugin instance based on a specification.
   *
   * @param mixed $spec
   *   A spec can either be a fully qualified class name or an array with at
   *   least one member 'class' which must be a fully qualified class name.
   *
   * @return mixed
   *   A new plugin instance.
   */
  protected function pluginInstance($spec) {
    if (!is_array($spec)) {
      $spec = ['class' => $spec];
    }
    $class = $spec['class'];
    return $class::fromConfig($spec);
  }

}
