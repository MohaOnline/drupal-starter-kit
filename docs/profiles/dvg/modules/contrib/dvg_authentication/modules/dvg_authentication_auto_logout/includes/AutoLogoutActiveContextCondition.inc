<?php

/**
 * @file
 * Handles the condition logic for the active status of auto logout.
 */

use Drupal\dvg_authentication_auto_logout\AutoLogout;

/**
 * Expose the auto logout active value as a context condition.
 */
class AutoLogoutActiveContextCondition extends context_condition {

  /**
   * {@inheritdoc}
   */
  public function condition_values() {
    $values = [];
    $values['active'] = t('Active');
    $values['inactive'] = t('Inactive');
    return $values;
  }

  /**
   * Executes context conditions.
   */
  public function execute($user) {
    $status = Autologout::getInstance()->isActive($user) ? 'active' : 'inactive';
    foreach ($this->get_contexts($status) as $context) {
      $this->condition_met($context);
    }
  }

}
