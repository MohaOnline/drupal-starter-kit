<?php

/**
 * @file
 * Views handler: Product price field.
 */

/**
 * Returns a formatted price value to display in the View.
 */
class CommerceFundsHandlerFieldMoneyAmount extends views_handler_field {

  /**
   * Overrides views_handler_field::render().
   */
  public function render($values) {
    return commerce_currency_format($values->{$this->field_alias}, commerce_default_currency());
  }

}
