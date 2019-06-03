<?php

namespace Drupal\dvg_authentication_dummy;

use Drupal\dvg_authentication\ExternalUserBase;

/**
 * Class DummyUser, for use with the dummy authentication provider.
 */
class DummyUser extends ExternalUserBase {

  /**
   * {@inheritdoc}
   */
  public function getValue($field_name) {
    // Check if any dummy data is available.
    $user_data = $this->authenticationProvider->getConfig('dummy_user');
    if (isset($user_data[$field_name])) {
      $value = $user_data[$field_name];
      if ($field_name === 'date_of_birth' && is_array($value)) {
        $date_of_birth = new \DateObject();
        $date_of_birth->setDate($value['year'], $value['month'], $value['day']);
        $value = date_format_date($date_of_birth, 'custom', 'd-m-Y');
      }
    }
    else {
      $value = parent::getValue($field_name);
    }
    return $value;
  }

}
