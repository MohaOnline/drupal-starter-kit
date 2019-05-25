
/**
 * @file
 * Custom Menu Toolbar JS.
 *
 * Created by: Topsitemakers
 * http://www.topsitemakers.com/
 */
(function ($) {
  Drupal.behaviors.w3b4Toolbar = {
    attach: function (context, settings) {

      $('#custom-menu-toolbar').once('toggle', function() {
        $('#custom-menu-toolbar-toggle').click(function() {
          $('#custom-menu-toolbar').toggleClass('open');
          $('body').toggleClass('custom-menu-toolbar-open');
        });
        $('#custom-menu-toolbar li.expanded > a').click(function() {
          if ($(window).width() < 979) {
            $(this).closest('ul').toggleClass('open');
            return false;
          }
        });
      });

    }
  };
})(jQuery);
