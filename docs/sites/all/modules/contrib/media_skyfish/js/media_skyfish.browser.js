/**
 * @file
 * Handles the JS for the file browser.
 *
 * Note that this does not currently, support multiple file selection.
 */

(function ($) {
  'use strict';

  Drupal.behaviors.media_skyfish = {
    attach: function (context, settings) {

      // --- Checkboxes ---
      var active_class = 'media-skyfish-active';

      if ($('.media-skyfish-list', context).length > 0) {
        $('.media-skyfish-list', context).once().click(function () {
          var image = $('img', this).attr('data-image');
          var checkbox = 'input[name=' + image + ']';

          uncheck_skyfish_checkboxes(image);

          $(checkbox).attr( "checked", true );
          if ($(checkbox).is(':checked')) {
            $(this).addClass(active_class);
          }
          else {
            $(this).removeClass(active_class);
          }
        });
      }

      function uncheck_skyfish_checkboxes(image) {
        $('.media-skyfish-checkbox').each(function (index, value) {
          var this_image = $(this).attr('name');
          if ($(this).is(':checked')) {
            $('img[data-image=' + this_image + ']').parent().removeClass(active_class);
            $(this).attr( "checked", false );
          }
        });
      }

      // --- Info dialog ---
      $('#media-skyfish-form .media-skyfish-list .form-item .show-more').once().click(function () {
        var id = $(this).attr('data-image');
        var dialog = $('#media-skyfish-form #skyfish-dialog-wrapper');
        var content = '#media-skyfish-form .media-skyfish-list .form-item .dialog-info-' + id;
        $('#skyfish-dialog-content-inner', dialog).html($(content).html());
        dialog.addClass('active');
      });

      $('#media-skyfish-form #skyfish-dialog-wrapper #skyfish-info-dialog #skyfish-dialog-content #skyfish-dialog-close', context).once().click(function () {
        $('#media-skyfish-form #skyfish-dialog-wrapper', context).removeClass('active');
      });

      // --- Submit button ---
      $('#media-skyfish-form-submit', context).once().click(function () {
        var $throbber = $('<div class="ajax-progress ajax-progress-throbber media-skyfish-throbber"><div class="throbber">&nbsp;</div></div>');
        $(this)
          .addClass('progress-disabled')
          .after($throbber);
      });
    }

  };

}(jQuery));
