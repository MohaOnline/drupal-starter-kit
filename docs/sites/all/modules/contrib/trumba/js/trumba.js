/**
 * @file
 * Javascript for creating Trumba Calendar Spuds.
 */

(function ($, window) {
  'use strict';

  // Create a Trumba Spud.
  Drupal.behaviors.TrumbaAddSpud = {
    attach: function (context, settings) {

      // Do nothing if the external Trumba script has not loaded.
      if (!window.$Trumba) {
        return;
      }

      // Find each Trumba spud and init once.
      $('.trumba-spud', context).once('trumba-init', function () {
        $(this).each(function () {
          var spudId = $(this).data('trumba-spud');

          // Clone the object so we don't wreck the original settings.
          var spud = $.extend(true, {}, settings.trumba[spudId]);

          // Create the calendar.
          $Trumba.addSpud(spud);
        });
      });

    }
  };

})(jQuery, window);
