<?php

namespace Drupal\campaignion_facebook_pixel;

use Drupal\campaignion_opt_in\Values;
use Drupal\little_helpers\Webform\Submission;

/**
 * Class to manage attached JS needed for the Facebook pixel.
 */
class Config {

  const EVENT_MAP = [
    'Lead' => 'l',
    'CompleteRegistration' => 'r',
    'ViewPage' => 'v',
  ];

  /**
   * FB-pixel codes keyed by node IDs.
   *
   * @var string[]
   */
  protected $mapping;

  /**
   * Construct a new config object.
   *
   * @param string[] $mapping
   *   FB-pixel codes keyed by node IDs.
   */
  public function __construct(array $mapping = []) {
    $this->mapping = $mapping;
  }

  /**
   * Encode pixel events as fragment string.
   *
   * @param string $pixel_id
   *   The facebook pixel ID.
   * @param string[] $events
   *   The name of the tracking events that should be sent.
   *
   * @return string
   *   Events encoded as fragment string.
   */
  protected static function encodeFragment($pixel_id, array $events) {
    $encoded = [];
    foreach ($events as $e) {
      $encoded[] = isset(static::EVENT_MAP[$e]) ? static::EVENT_MAP[$e] : $e;
    }
    return "fbq:$pixel_id=" . implode(',', $encoded);
  }

  /**
   * Get events based on a completed webform submission.
   *
   * @param \Drupal\little_helpers\Webform\Submission $submission
   *   Completed webform submission.
   *
   * @return string
   *   Events encoded as fragment string to be sent for this submission.
   */
  public function submissionFragment(Submission $submission) {
    $node = $submission->node;
    if (isset($this->mapping[$node->nid])) {
      $pixel_id = $this->mapping[$node->nid];
      $events = ['CompleteRegistration'];
      if (Values::submissionHasOptIn($submission, 'email')) {
        $events[] = 'Lead';
      }
      return static::encodeFragment($pixel_id, $events);
    }
  }

  /**
   * Attach the Facebook Pixel JavaScript and settings to the $node->content.
   *
   * @param object $node
   *   The node object which’s content is being generated.
   */
  public function attach($node) {
    if (isset($this->mapping[$node->nid])) {
      $settings['pixels'][$this->mapping[$node->nid]] = ['PageView'];
      $settings = ['campaignion_facebook_pixel' => $settings];
      $node->content['#attached']['js'][] = [
        'data' => $settings,
        'type' => 'setting',
      ];
    }
  }

}
