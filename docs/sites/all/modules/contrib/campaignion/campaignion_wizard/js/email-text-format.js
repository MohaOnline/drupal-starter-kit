(function($) {

Drupal.behaviors.webformTextFormat = {};

// Virtually submit buttons labels to the end of the form.
Drupal.behaviors.webformTextFormat.attach = function(context) {
  $('.email-wrapper .text-format-wrapper').each(function() {
    var $wrapper = $(this);
    var $format = $wrapper.find('.filter-list');
    var $html = $wrapper.next().find('input[type=checkbox]');
    $html.change(function() {
      var newFormat = $html.is(':checked') ? Drupal.settings.webform.textFormat.html : Drupal.settings.webform.textFormat.plain;
      $format.val(newFormat).change();
    });
    $wrapper.find('.filter-wrapper').hide();
  });
}

})(jQuery);