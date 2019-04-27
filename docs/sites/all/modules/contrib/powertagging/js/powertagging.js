(function ($) {
  Drupal.behaviors.powertagging = {
    attach: function (context, settings) {
      $('span.powertagging-display-entities').each(function() {
        $(this).parent().addClass('powertagging-entity-extraction-show');
        $(this).remove();
      });
    }
  }
})(jQuery);
