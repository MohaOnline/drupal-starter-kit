/**
 * @file
 * A JavaScript file for address components.
 */

(function ($, Drupal, window, document, undefined) {
  $.fn.webformAddressInject = function (arguments) {
    // Parse the JSON argument.
    var data = JSON.parse(arguments);
    data.formKey = data.formKey.replace('_','-');

    // To prevent the user adding to the text field, lose its (possible) focus.
    $("input.ajax-callback-street-name--" + data.formKey).blur();
    $("input.ajax-callback-street-name--" + data.formKey).val(data.street);

    $("input.ajax-callback-city--" + data.formKey).blur();
    $("input.ajax-callback-city--" + data.formKey).val(data.city);

  };
  Drupal.behaviors.webformAddressInjectSubmit = {
    attach: function (context, settings) {
      $('form').submit(function (e) {
        $('.progress-disabled').removeAttr('disabled');
      });
    }
  }
})(jQuery, Drupal, this, this.document);
