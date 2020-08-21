/**
 * @file
 * Defines Javascript behaviors for the tocify module.
 */

(function ($) {

  Drupal.behaviors.tocify = {
    attach: function (context, settings) {
      $('#tocify').tocify(
        settings.tocify
      );
    }
  };

})(jQuery);
