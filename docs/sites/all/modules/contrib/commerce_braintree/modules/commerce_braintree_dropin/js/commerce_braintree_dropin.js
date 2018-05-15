/**
 * @file
 * Defines behaviors for the Braintree Drop-in UI payment method form.
 */

(function($) {
    Drupal.behaviors.commerceBraintreeDropin = {
        attach: function (context, settings) {
            if (typeof settings.commerceBraintreeDropin !== 'undefined') {
                var $dropInForm = $(context).find('#commerce-braintree-dropin-container').once()
                if ($dropInForm.length) {
                    var waitForSdk = setInterval(function () {
                        if (typeof braintree !== 'undefined') {
                            clearInterval(waitForSdk);
                            var $form = $dropInForm.closest('form');
                            Drupal.braintreeDropIn = new Drupal.commerceBraintreeDropin($form, settings.commerceBraintreeDropin);
                            Drupal.braintreeDropIn.bootstrap();
                        }
                    }, 100);
                }

                // Braintree hijacks all submit buttons for this form. Simulate the back
                // button to make sure back submit still works.
                $('.checkout-cancel,.checkout-back', context).click(function (e) {
                    e.preventDefault();
                    window.history.back();
                });
            }
        }
    };

    Drupal.commerceBraintreeDropin = function ($form, settings) {
        this.settings = settings;
        this.$form = $form;
        this.fromId = this.$form.attr('id');
        this.$submit = this.$form.find('[name=op]');
        this.error = '';
        return this;
    };

    Drupal.commerceBraintreeDropin.prototype.bootstrap = function () {
        var options = this.getOptions();
        braintree.setup(this.settings.clientToken, 'dropin', options);
    };

    Drupal.commerceBraintreeDropin.prototype.resetSubmitBtn = function () {
        $('.checkout-processing', this.$form).addClass('element-invisible');
        this.$submit.next('.checkout-continue').removeAttr('disabled');
    };

    Drupal.commerceBraintreeDropin.prototype.errorMsg = function (response) {
       this.error = response.message;
    };

    Drupal.commerceBraintreeDropin.prototype.showError = function (message) {
        this.resetSubmitBtn();
    };

    Drupal.commerceBraintreeDropin.prototype.getOptions = function () {
        var options = {
            onReady: $.proxy(this.onReady, this),
            onError: $.proxy(this.onError, this),
            id: this.fromId,
            container: 'commerce-braintree-dropin-container'
        };

        options = $.extend(options, this.settings);
        return options;
    };

    Drupal.commerceBraintreeDropin.prototype.onPaymentMethodReceived = function (payload) {
        this.$submit.removeAttr('disabled');
        this.$form.submit();
    };

    Drupal.commerceBraintreeDropin.prototype.onReady = function (integration) {
        this.integration = integration;
    };

    // Global event callback.
    Drupal.commerceBraintreeDropin.prototype.onError = function (response) {
       this.errorMsg(response)
    };
})(jQuery);
