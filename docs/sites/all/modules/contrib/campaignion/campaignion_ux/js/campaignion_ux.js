/* jshint */

(function ($) {
  'use strict';

  Drupal.behaviors.campaignion_ux = {};
  Drupal.behaviors.campaignion_ux.attach = function(context) {
    // generate an dialog
    // used fo graying out the content while using the "new" action
    if($('.campaignion-dialog-wrapper').length < 1) {
      var $dialog = $('<div class="campaignion-dialog-wrapper"><div class="camapignion-dialog-content"></div></div>');
      $dialog.appendTo($('body'));
    }
  };

$(document).ready(function() {

// Behaviors are executed in the order they are added to the behaviors object.
// We want this behavior to execute last.
Drupal.behaviors.campaignion_ux_webform_ajax_scroll = {};
Drupal.behaviors.campaignion_ux_webform_ajax_scroll.attach = function(context, settings) {
  if (settings.campaignion_ux && settings.campaignion_ux.skip_scroll) {
    return;
  }
  // Scroll to top of the form + padding if we are below or more than the
  // toleratedOffset above it.
  var padding = 12;
  var toleratedOffset = 50;
  if ($(context).is('[id^=webform-ajax-wrapper]')) {
    // This is the result of an AJAX submit.
    var $wrapper = $(context);
    var wrapperTop = $wrapper.offset().top;
    var diff = wrapperTop - $(document).scrollTop();
    if (diff < 0 || diff > toleratedOffset) {
      $('body, html').animate({ scrollTop: (wrapperTop - padding)}, 'slow');
    }
  }
};

});

}(jQuery));


