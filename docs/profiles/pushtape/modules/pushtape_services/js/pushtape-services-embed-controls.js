/**
 * This script is only used for the internal iFrame embed HTML.
 */
(function ($) {
  Drupal.behaviors.pushtapeServicesEmbedControls = {
    attach: function (context, settings) {
      $('html').once('html-play-pause', function(){
        $(this).click(function(event) { 
            if(event.target == $('body')[0]) {
              $('.pt-play-pause')[0].click();          
            }
        });      
      });
    }
  };
}(jQuery));