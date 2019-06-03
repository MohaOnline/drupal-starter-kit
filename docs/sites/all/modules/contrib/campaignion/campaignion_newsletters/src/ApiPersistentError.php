<?php

namespace Drupal\campaignion_newsletters;

class ApiPersistentError extends ApiError {

  public function isPersistent() {
    return TRUE;
  }

}
