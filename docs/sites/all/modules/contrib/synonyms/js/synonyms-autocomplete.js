(function ($) {

/**
 * Fix the autocomplete core undesired behavior.
 *
 * The core autocomplete only allows 1 entry per suggestion, i.e. you can't have
 * 2 suggestion entries suggest the same key. Synonyms module very well needs
 * such ability, since multiple synonyms may point to the same entity. In order
 * to bypass this limitation Synonyms module pads the suggestion entries with
 * extra spaces on the right until it finds a "free" spot. This JavaScript
 * right-trims the entries in order to cancel out the effect.
 */
Drupal.behaviors.synonymsAutocompleteWidget = {
  attach: function (context, settings) {
    $('input.form-autocomplete.synonyms-autocomplete', context).once('synonyms-autocomplete', function () {
      $(this).bind('autocompleteSelect', function() {
        var value = $(this).val();
        value = value.replace(/\s+$/, '');
        $(this).val(value);
      });
    });
  }
};
})(jQuery);
