/**
 * @file
 * JavaScript for the Roost for Drupal module.
 *
 * JavaScript that aids in the visual appearance of the Roost admin
 * section within the Drupal admin.
 */

(function($) {
  Drupal.behaviors.roost = {
    attach: function (context, settings) {
      if ($('#roostLoginForm').length) {
        $('#branding').css('background-image', 'none');
      }
    }
  }
})(jQuery);
