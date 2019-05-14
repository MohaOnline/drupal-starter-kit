(function ($, Drupal) {
  /*global jQuery:false */
  /*global Drupal:false */
  "use strict";

  /**
   * Provide vertical tab summaries for Bootstrap settings.
   */
  Drupal.behaviors.glazedAdmin = {
    attach: function (context) {
      var $context = $(context);
      var $toolbar = $('#toolbar, #navbar-bar, #admin-menu', context);
      if (($toolbar.length > 0) && (Drupal.settings.glazed.glazedPath)) {
        glazedButtonAdd($toolbar);
      }
      /**
       * Hook into Admin Menu Cached loading
       */
      if ('admin' in Drupal) {
        Drupal.admin.behaviors.glazedButton = function (context, settings, $adminMenu) {
          $toolbar = $('#admin-menu', context);
          glazedButtonAdd($toolbar);
        };
      }

      function glazedButtonAdd($toolbar) {
        var themeName = Drupal.settings.glazedDefaultTheme || 'glazed_free';
        var glazedLogoPath = Drupal.settings.basePath + Drupal.settings.glazed.glazedPath + 'glazed-favicon.png';
        var $glazedButton = $('<div id="glazed-button-wrapper">').html($('<a>',{
          text: Drupal.t('Theme Settings'),
          title: 'Theme Settings, Demo Import, and more',
          class: 'glazed-button',
          href: Drupal.settings.basePath + 'admin/appearance/settings/' + themeName
        }).prepend($('<img>',{
          src: glazedLogoPath,
          width: 15
        })));
        $('.toolbar-menu, #admin-menu-wrapper', $toolbar).once('glazed_free_button').prepend($glazedButton);
      }

    }
  };

})(jQuery, Drupal);
