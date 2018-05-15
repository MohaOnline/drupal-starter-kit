(function($) {
  Drupal.behaviors.commerceBraintree = {
    attach: function (context, settings) {
      var form = $('#commerce-braintree-tr-redirect-form', context);
      $('input[type="submit"]:not(.checkout-processed)', form).addClass('checkout-processed').click(function() {
        var $this = $(this);
        $this.clone().insertAfter(this).attr('disabled', true).next().removeClass('element-invisible');
        $this.hide();
      });
    }
  }
})(jQuery);
