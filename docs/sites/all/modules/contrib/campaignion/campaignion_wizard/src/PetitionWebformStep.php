<?php

namespace Drupal\campaignion_wizard;

class PetitionWebformStep extends WebformStepUnique {
  public function status() {
    return array(
      'caption' => t('Your form is ready to go'),
      'message' => t('You have the following fields on your form: TODO.'),
    );
  }
}
