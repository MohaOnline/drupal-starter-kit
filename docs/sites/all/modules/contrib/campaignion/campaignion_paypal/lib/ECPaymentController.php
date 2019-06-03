<?php

namespace Drupal\campaignion_paypal;

class ECPaymentController extends \PayPalPaymentECPaymentMethodController {

  // callback for user form
  public $payment_configuration_form_elements_callback = 'campaignion_paypal_payment_method_form';

  /**
   * Implements PaymentMethodController::execute().
   */
  function execute(\Payment $payment) {
    // Prepare the PayPal checkout token.
    $authentication = NULL;
    if ($payment->pid) {
      $authentication = $this->loadAuthentication($payment->pid);
    }
    if (!$authentication) {
      entity_save('payment', $payment);
      $authentication = $this->setExpressCheckout($payment);
      if ($authentication) {
        $this->saveAuthentication($authentication);
      }
    }

    // Start checkout.
    if ($authentication) {
      $context = $payment->contextObj;
      $context->redirect($this->checkoutURL($payment->method->controller_data['server'], $authentication->token));
    }
    else {
      $payment->setStatus(new \PaymentStatusItem(PAYMENT_STATUS_FAILED));
    }
  }

  /**
   * Default to credit-card payment.
   */
  public function paymentNVP(\Payment $payment) {
    $nvp = parent::paymentNVP($payment);
    $nvp['USERSELECTEDFUNDINGSOURCE'] = 'CreditCard';
    $nvp['LANDINGPAGE'] = 'Billing';
    $nvp['SOLUTIONTYPE'] = 'Sole';
    return $nvp;
  }
}
