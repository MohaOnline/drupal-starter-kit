/**
 * @file
 * Defines Javascript behaviors for the tocify module.
 */

(function ($) {

  Drupal.behaviors.tocify = {
    attach: function (context, settings) {
      console.log(settings.tocify);
      $('#tocify').tocify(
        settings.tocify
      );
    }
  };

})(jQuery);
