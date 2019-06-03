<?php

/**
 * @file
 * Views handler: Escrow operations.
 */

/**
 * Returns the available operations for escrow payments.
 */
class CommerceFundsHandlerFieldEscrowOperations extends views_handler_field {

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
      $this->output = _commerce_funds_escrow_operations($this, $result);
      $result->commerce_funds_escrow_operations = $this->output;
    }
  }

  /**
   * Render the handler.
   */
  public function render($values) {
    $output = $values->commerce_funds_escrow_operations;
    return $output;
  }

}

/**
 * Definition of _commerce_funds_escrow_operations().
 */
function _commerce_funds_escrow_operations($handler, $result) {
  $transaction_id = $result->transaction_id;
  $status = $result->commerce_funds_transactions_status;
  $current_display = $handler->view->current_display;
  $links = array();

  if ($current_display == "incoming_escrow_payments") {
    if ($status == 'Pending') {
      $links['cancel'] = array(
        'title' => 'Cancel',
        'href' => 'user/funds/escrow-payments/cancel/' . $transaction_id,
        'attributes' => array('class' => array('commerce-funds-cancel'), 'title' => 'Cancel'),
      );
    }
    else {
      return t('None');
    }
  }

  if ($current_display == "outgoing_escrow_payments") {
    if ($status == 'Pending') {
      $links['cancel'] = array(
        'title' => 'Cancel',
        'href' => 'user/funds/escrow-payments/cancel/' . $transaction_id,
        'attributes' => array('class' => array('commerce-funds-cancel'), 'title' => 'Cancel'),
      );
      $links['release'] = array(
        'title' => 'Release',
        'href' => 'user/funds/escrow-payments/release/' . $transaction_id,
        'attributes' => array('class' => array('commerce-funds-cancel'), 'title' => 'Release'),
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
