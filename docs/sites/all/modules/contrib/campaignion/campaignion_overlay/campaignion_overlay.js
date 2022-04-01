(function($) {
  Drupal.behaviors.campaignionOverlay = {
    attach: function(context, settings) {
      var overlay = $(".campaignion-overlay-options", context).first();
      if (overlay.length) {
        // Add jQuery functions for animations
        jQuery.fn.extend({
          campaignionOverlayShow: Drupal.behaviors.campaignionOverlay.show,
          campaignionOverlayClose: Drupal.behaviors.campaignionOverlay.close
        });

        overlay.dialog({
          dialogClass: "campaignion-overlay",
          maxWidth: $(window).width() * 0.9,
          maxHeight: $(window).height() * 0.9,
          modal: "true",
          resizable: false,
          width: "auto",
          show: "campaignionOverlayShow",
          hide: "campaignionOverlayClose"
        });

        // Add custom 'close' button.
        var close = $('<a href="#" class="close" title="' + Drupal.t("Close") + '">x</a>');
        close.click(function() {
          overlay.dialog("close");
        });
        overlay.prepend(close);

        // generic class to use with e.g. custom buttons
        $('.campaignion-overlay-close', context).click(function(event) {
          overlay.dialog("close");
        });

        // Center dialog on window resize.
        $(window).resize(function() {
          overlay.dialog("option", "position", {
            my: "center",
            at: "center",
            of: window
          });

          overlay.dialog("option", "maxWidth", $(window).width() * 0.9);
          overlay.dialog("option", "maxHeight", $(window).height() * 0.9);
        });
      }
    }
  };
  /**
   * Provides a fade in type animation.
   *
   * The object is moved up from 5% below its actual `top` property
   * and its opacity is toggled.
   */
  Drupal.behaviors.campaignionOverlay.show = function() {
    var top_ = parseFloat($(this).css('top').replace('px', ''));
    var move = $(window).height()/100 * 5;
    var new_top = (top_+move).toString().concat('px');

    $(this).css('top', new_top);
    $(this).animate({
      top: top_.toString().concat('px'),
      opacity: "toggle"
    }, 750, "easeInCubic");
  };
  /**
   * Provides a fade out type animation.
   *
   * The object is moved up to 5% above its actual `top` property
   * and its opacity is toggled.
   */
  Drupal.behaviors.campaignionOverlay.close = function() {
    var top_ = parseFloat($(this).css('top').replace('px', ''));
    var move = $(window).height()/100 * 5;
    var new_top = (top_-move).toString().concat('px');

    $(this).animate({
      top: new_top,
      opacity: "toggle"
    }, 750, "easeOutQuad", function() {
      $(this).hide();
    });
  };
})(jQuery);
