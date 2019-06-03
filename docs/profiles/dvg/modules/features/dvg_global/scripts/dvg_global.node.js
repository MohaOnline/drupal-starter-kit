(function ($) {

  Drupal.behaviors.dvgGlobalnodeFieldsetSummaries = {
    attach: function (context) {
      $('fieldset.dvg-options', context).drupalSetSummary(function (context) {
        var vals = [];

        $('input:checked', context).parent().each(function () {
          vals.push(Drupal.checkPlain($.trim($(this).text())));
        });

        return vals.join(', ');
      });
    }
  };

})(jQuery);
