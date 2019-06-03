<?php

namespace Drupal\campaignion_newsletters;

use \Drupal\little_helpers\Webform\Submission;

/**
 * Model-class for Opt-In form submissions.
 *
 * For proper opt-in we need to track the URL, date, IP and submitted data of
 * the relevant form submission.
 */
class FormSubmission implements \Serializable {
  public $ip;
  public $userAgent;
  public $url;
  public $date;
  public $data;

  /**
   * Construct from a webform submission.
   */
  public static function fromWebformSubmission(Submission $submission, $user_agent = NULL) {
    $user_agent = $user_agent ? $user_agent : $_SERVER['HTTP_USER_AGENT'];
    $url = url('node/' . $submission->nid, ['absolute' => TRUE]);
    $ip = $submission->remote_addr;
    $date = $submission->submitted;

    // Reconstruct the name of the post-variables.
    $postname_cid = [];
    foreach ($submission->node->webform['components'] as $c) {
      $parent = $c['pid'] ? $postname_cid[$c['pid']] : 'submitted';
      $postname_cid[$c['cid']] = $parent . "[{$c['form_key']}]";
    }
    $data = [];
    foreach ($postname_cid as $cid => $n) {
      if ($v = $submission->valueByCid($cid)) {
        $data[$n] = $v;
      }
    }

    return new static($ip, $user_agent, $url, $date, $data);
  }

  /**
   * Actual constructor.
   */
  public function __construct($ip, $user_agent, $url, $date, $data) {
     $this->ip = $ip;
     $this->userAgent = $user_agent;
     $this->url = $url;
     $this->date = $date;
     $this->data = $data;
  }

  /**
   * Export into an array structure.
   */
  public function serialize() {
    return serialize([
      'ip' => $this->ip,
      'user_agent' => $this->userAgent,
      'url' => $this->url,
      'date' => $this->date,
      'data' => $this->data,
    ]);
  }

  public function unserialize($data) {
    $data = unserialize($data);
    $data += [
      'ip' => '',
      'user_agent' => '',
      'url' => '',
      'date' => REQUEST_TIME,
      'data' => [],
    ];
    $this->__construct($data['ip'], $data['user_agent'], $data['url'], $data['date'], $data['data']);
  }

}
