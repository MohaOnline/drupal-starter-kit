/**
 * @file
 * A JavaScript file for the Horizontal Tabs Panels Style plugin.
 *
 */

(function ($) {
  Drupal.behaviors.bpht = {
    attach: function(context) {
      $('.bootstrap-panels-horizontal-tabs .tab-content').each(function () {
        $(this).find('.tab-pane').not(':first').removeClass('active');
      });
    }
  };
})(jQuery);
