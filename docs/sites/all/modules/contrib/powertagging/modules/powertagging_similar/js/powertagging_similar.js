(function ($) {
  Drupal.behaviors.powertagging_similar = {
    attach: function (context, settings) {
      $(document).ready(function () {
        $(".powertagging-similar-widget-accordion").accordion({
          autoHeight: false
        });
      });
    }
  }
})(jQuery);
