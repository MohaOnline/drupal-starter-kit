;(function($) {

  /**
   * Theme function to create an overlay iframe element.
   * The allowfullscreen attribute is added to allow the fullscreen rendering.
   */
  Drupal.theme.overlayElement = function (url) {
    return '<iframe class="overlay-element" frameborder="0" scrolling="auto" allowtransparency="true" allowfullscreen="true" webkitallowfullscreen="true" mozallowfullscreen="true"></iframe>';
  };


})(jQuery);