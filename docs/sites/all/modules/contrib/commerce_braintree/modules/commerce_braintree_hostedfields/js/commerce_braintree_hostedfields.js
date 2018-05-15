/**
 * @file
 * Defines behaviors for the Braintree Hosted Fields payment method form.
 */

(function($) {
  Drupal.behaviors.commerceBraintreeHostedFields = {
    attach: function(context, settings) {
      if (typeof settings.commerceBraintreeHostedFields !== 'undefined') {
        var $hostedfieldsForm = $(context).find('.braintree-form').once();
        if ($hostedfieldsForm.length) {
          var waitForSdk = setInterval(function () {
            if (typeof braintree !== 'undefined') {
              clearInterval(waitForSdk);
              var $form = $hostedfieldsForm.closest('form');
              Drupal.braintreeHostedFields = new Drupal.commerceBraintreeHostedFields($form, settings.commerceBraintreeHostedFields);
              Drupal.braintreeHostedFields.bootstrap();
            }
          }, 100);
        }
      }

      // Braintree hijacks all submit buttons for this form. Simulate the back
      // button to make sure back submit still works.
      $('.checkout-cancel, .checkout-back', context).click(function(e) {
        e.preventDefault();
        window.history.back();
      });
    }
  };

  Drupal.commerceBraintreeHostedFields = function($form, settings) {
    this.settings = settings;
    this.$form = $form;
    this.fromId = this.$form.attr('id');
    this.$submit = this.$form.find('[name=op]');
    return this;
  };

  Drupal.commerceBraintreeHostedFields.prototype.bootstrap = function() {
    var options = this.getOptions();
    braintree.setup(this.settings.clientToken, 'custom', options);
  };

  Drupal.commerceBraintreeHostedFields.prototype.resetSubmitBtn = function() {
    $('.checkout-processing', this.$form).addClass('element-invisible');
    this.$submit.next('.checkout-continue').removeAttr('disabled');
  };

  Drupal.commerceBraintreeHostedFields.prototype.jsValidateErrorHandler = function(response) {
    var message = this.errorMsg(response);
    this.showError(message);
  };

  Drupal.commerceBraintreeHostedFields.prototype.errorMsg = function(response) {
    var message;

    switch (response.message) {
      case 'User did not enter a payment method':
        message = Drupal.t('Please enter your credit card details.');
        break;

      case 'Some payment method input fields are invalid.':
        var fieldName = '';
        var fields = [];
        var invalidFields = this.$form.find('.braintree-hosted-fields-invalid');

      function getFieldName(id) {
        return id.replace('-', ' ');
      }

        if (invalidFields.length > 0) {
          invalidFields.each(function(index) {
            var id = $(this).attr('id');

            fields.push(Drupal.t(getFieldName(id)));
          });

          if (fields.length > 1) {
            var last = fields.pop();
            fieldName = fields.join(', ');
            fieldName += ' and ' + Drupal.t(last);
            message = Drupal.t('The @field you entered are invalid', {'@field': fieldName});
          }
          else {
            fieldName = fields.pop();
            message = Drupal.t('The @field you entered is invalid', {'@field': fieldName});
          }

        }
        else {
          message = Drupal.t('The payment details you entered are invalid');
        }

        message += Drupal.t(', please check your details and try again.');

        break;

      default:
        message = response.message;
    }

    return message;
  };

  Drupal.commerceBraintreeHostedFields.prototype.showError = function(message) {
    this.resetSubmitBtn();
  };

  Drupal.commerceBraintreeHostedFields.prototype.getOptions = function() {
    var options = {
      onReady: $.proxy(this.onReady, this),
      onError: $.proxy(this.onError, this),
      id: this.fromId,
      hostedFields: {
        onFieldEvent: this.onFieldEvent
      }
    };
    // Set up hosted fields selector
    options = $.extend(options, this.settings);

    if (this.settings.advancedFraudTools === true) {
      options.dataCollector = {
        kount: {environment: this.settings.environment}
      };
    }
    return options;
  };

  Drupal.commerceBraintreeHostedFields.prototype.onPaymentMethodReceived = function (payload) {
    this.$submit.removeAttr('disabled');
    this.$form.submit();
  };

  Drupal.commerceBraintreeHostedFields.prototype.onReady = function (integration) {
    this.integration = integration;
    var deviceData = integration.deviceData;
    if (typeof deviceData !== 'undefined') {
      this.$form.find('[name="commerce_payment[payment_details][braintree][device_data]"]').val(deviceData);
    }
  };

  // Global event callback
  Drupal.commerceBraintreeHostedFields.prototype.onError = function (response) {
    if (response.type === 'VALIDATION') {
      this.jsValidateErrorHandler(response);
    }
    else {
      console.log('Other error', arguments);
    }
  };

  /**
   * You can subscribe to events using the onFieldEvent callback. This
   * allows you to hook into focus, blur, and fieldStateChange.
   *
   * @param event
   *
   * @see https://developers.braintreepayments.com/javascript+php/guides/hosted-fields/events
   */
  Drupal.commerceBraintreeHostedFields.prototype.onFieldEvent = function (event) {
  };

})(jQuery);
