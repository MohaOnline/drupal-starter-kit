/**
 * Image Lazy Loader
 *
 * Initialise the Lozad plugin when required.
 */
(function($) {
    Drupal.behaviors.ImageLazyLoader = {
      attach: function (context) {
        if ($(".lozad").length) {
          if (!Drupal.behaviors.ImageLazyLoader.lozad) {
            Drupal.behaviors.ImageLazyLoader.lozad = lozad(".lozad", {
              load: function (el) {
                if($(el).data("background-image")) {
                  var bgImage = "url(" + $(el).attr("data-background-image");
                  $(el).css("background-image", bgImage);
                }
                el.src = el.dataset.src;
                el.onload = function () {
                  el.classList.add(el.getAttribute("data-animation"));
                }
              }
            })
          }
          Drupal.behaviors.ImageLazyLoader.lozad.observe();
        }
      }
    };
  }
)(jQuery);
