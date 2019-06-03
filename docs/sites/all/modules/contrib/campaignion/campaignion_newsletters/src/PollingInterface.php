<?php

namespace Drupal\campaignion_newsletters;

interface PollingInterface {

  /**
   * Poll one batch of subscribers.
   *
   * @return bool
   *   Whether or not polling should continue for this provider if time allows.
   */
  public function poll();

}
