(function ($) {

var DrupalAjaxErrorBehavior = DrupalAjaxErrorBehavior || {};

DrupalAjaxErrorBehavior.coreDisplayAjaxError = Drupal.displayAjaxError;

/**
 * Overwrites Drupal's display ajax error handling.
 */
Drupal.displayAjaxError = function (message) {
  // @see Drupal.displayAjaxError() in drupal.js
  if (Drupal.settings.ajaxErrorBehavior.behavior == 'core') {
    DrupalAjaxErrorBehavior.coreDisplayAjaxError(message);
  }
  else if (Drupal.settings.ajaxErrorBehavior.behavior == 'alert') {
    alert(message);
  }
  else if (Drupal.settings.ajaxErrorBehavior.behavior == 'watchdog') {
    $.ajax({
      type: 'POST',
      url: Drupal.settings.ajaxErrorBehavior.watchdog_url,
      data: {message: message},
    });

    DrupalAjaxErrorBehavior.coreDisplayAjaxError(Drupal.settings.ajaxErrorBehavior.error);
  }
  else if (Drupal.settings.ajaxErrorBehavior.behavior == 'console') {
    if (typeof(window.console) !== 'undefined') {
      window.console.log(message);
    }
  }
};

})(jQuery);
