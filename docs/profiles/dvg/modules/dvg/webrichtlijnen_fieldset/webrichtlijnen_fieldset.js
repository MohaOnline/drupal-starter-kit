/**
 * @file
 * JavaScript behaviors for the front-end display of fieldsets.
 */

(function ($) {

  "use strict";

  Drupal.behaviors.webrichtlijnen_fieldset = {
    attach: function (context, settings) {

      $('.webform-conditional-processed', context).bind('change', function () {
        Drupal.webrichtlijnen_fieldset.doSync(context);
      });

      Drupal.webrichtlijnen_fieldset.doSync(context);
    }
  };

  Drupal.webrichtlijnen_fieldset = Drupal.webrichtlijnen_fieldset || {};

  /**
   * Sync conditional effects on webform components with fieldset wrapper.
   * @param context
   */
  Drupal.webrichtlijnen_fieldset.doSync = function (context) {
    var $components = $('.webform-component-radios, .webform-component-checkboxes', context);

    $components.each(function () {
      if ($(this).hasClass('webform-conditional-hidden')) {
        $(this).closest('fieldset').hide();
      }
      else {
        $(this).closest('fieldset').show();
      }
    });
  }
})(jQuery);
