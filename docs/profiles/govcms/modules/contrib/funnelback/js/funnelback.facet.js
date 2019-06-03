(function ($) {
  Drupal.behaviors.funnelbackFacetBehavior = {
    attach: function (context, settings) {
      $('.facet input[type=checkbox], .facet input[type=radio]', context).on('click', function () {
        // Go to the URL in the link after this.
        window.location.href = $(this).attr('redirect');
      });
    }
  };
})(jQuery);
