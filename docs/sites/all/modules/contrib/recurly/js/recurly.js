/**
 * @file
 * JavaScript provided to enhance UI elements of the Recurly module.
 */
(function ($) {

/**
 * Behavior to change the radio buttons on the change subscription form.
 */
Drupal.behaviors.recurlyPlanSelect = {};
Drupal.behaviors.recurlyPlanSelect.attach = function(context, settings) {
  $('.plan-signup a.plan-select', context).each(function() {
    var link = this;
    var $link = $(this);
    $link.parents('.plan:first').click(function(e) {
      if (e.target !== link) {
        // Click on the link if it's a real URL and not just an anchor.
        if (!link.href.match(/#$/)) {
          window.location = link.href;
        }
        // Otherwise trigger the handler which selects the radio button.
        else {
          $(this).find('a.plan-select').click();
        }
      }
    });
    $link.parents('.plan:first').hover(function() {
      $(this).addClass('plan-hover');
    }, function() {
      $(this).removeClass('plan-hover');
    });
  });
};

})(jQuery);
