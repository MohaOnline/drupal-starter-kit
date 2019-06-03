<?php

/**
 * @file
 * Views handler: Withdrawal operations.
 */

/**
 * Returns the available operations for withdrawal requests.
 */
class CommerceFundsHandlerFieldWithdrawalOperations extends views_handler_field {

  /**
   * Loads additional fields.
   */
  public function query() {
    $this->field_alias = 'operations';
  }

  /**
   * Alter rendering.
   */
  public function pre_render(&$values) {
    foreach ($values as $result) {
      $this->output = _commerce_funds_withdrawal_operations($this, $result);
      $result->commerce_funds_withdrawal_operations = $this->output;
    }
  }

  /**
   * Render the handler.
   */
  public function render($values) {
    $output = $values->commerce_funds_withdrawal_operations;
    return $output;
  }

}

/**
 * Defines the operation links for a withdrawal.
 *
 * @return array
 *   A list of available operations for the request.
 */
function _commerce_funds_withdrawal_operations($handler, $result) {
  $request_id = $result->request_id;
  $status = $result->commerce_funds_withdrawal_requests_status;
  $current_display = $handler->view->current_display;
  $links = array();

  if (user_access('administer withdraw requests')) {
    if ($status == 'Pending approval') {
      $links['approve'] = array(
        'title' => 'Approve',
        'href' => 'admin/commerce/funds/withdrawals/approve/' . $request_id,
        'attributes' => array('class' => array('commerce-funds-approve'), 'title' => 'Approve'),
      );
      $links['decline'] = array(
        'title' => 'Decline',
        'href' => 'admin/commerce/funds/withdrawals/decline/' . $request_id,
        'attributes' => array('class' => array('commerce-funds-decline'), 'title' => 'Decline'),
      );
    }
    else {
      return t('None');
    }
  }

  $attributes = array('class' => 'commerce-funds-links');

  return theme_links(
    array(
      'links' => $links,
      'attributes' => $attributes,
      'heading' => array(),
    )
  );
}
