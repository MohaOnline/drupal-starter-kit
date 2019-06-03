<?php

namespace Drupal\campaignion_email_to_target;

use Drupal\little_helpers\System\FormRedirect;

/**
 * Common datastructure for handling protest messages.
 */
class Exclusion extends MessageTemplateInstance {

  public $message;
  public $url;
  protected $tokenEnabledFields = ['message', 'url'];

  /**
   * Get a renderable array for this exclusion message.
   */
  public function renderable() {
    $message = $this->message;
    if (!is_array($message)) {
      $message = ['#markup' => _filter_autop(check_plain($message))];
    }
    return $message;
  }

  /**
   * Return a redirect if configured.
   */
  public function redirect() {
    if ($this->url) {
      return FormRedirect::fromFormStateRedirect($this->url);
    }
  }

}
