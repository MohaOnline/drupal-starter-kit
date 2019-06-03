(function ($) {
  Drupal.behaviors.dvgDigidScriptStyle = {

    // Add class if a digid user is logged in.
    attach: function (context, settings) {
      $('.block-dvg-digid-autologout', context).once('digid-autologout-style', function() {
        var $window = $(window, context);
        var $rTop = $('.r-top', context);
        var $body = $('body', context);
        var currentMargin = (typeof $body.css('margin-top') !== 'undefined') ? parseInt($body.css('margin-top')) : 0;

        $rTop.addClass('digid-active');
        $window.resize(function() {
          $body.css('margin-top', $rTop.outerHeight() + currentMargin + 'px');
        });
        $window.trigger('resize');
      });
    }
  }
}(jQuery));
