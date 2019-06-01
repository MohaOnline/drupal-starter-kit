/**
 *  @file
 *  Module overrides.
 */

(function ($) {

/**
 * Default jQuery dialog options used when creating the Linkit modal.
 */
if(typeof(Drupal.linkit) != "undefined"){
  Drupal.linkit.modalOptions = function() {
    return {
      dialogClass: 'linkit-wrapper',
      modal: true,
      draggable: false,
      resizable: false,
      width: 520,
      position: ['center', 90],
      minHeight: 0,
      zIndex: 210000,
      close: Drupal.linkit.modalClose,
      open: function (event, ui) {
        // Change the overlay style.
        $('.ui-widget-overlay').css({
          opacity: 0.5,
          filter: 'Alpha(Opacity=50)',
        });
      },
      title: 'Linkit'
    };
  };
}
})(jQuery);
