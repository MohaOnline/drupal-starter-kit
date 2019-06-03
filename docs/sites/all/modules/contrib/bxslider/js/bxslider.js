(function ($) {
  Drupal.behaviors.bxslider = {
    attach: function (context, settings) {

      if (!settings.bxslider || context == '#cboxLoadedContent') {
        return;
      }
      for (var slider_id in settings.bxslider) {

        $('#' + slider_id, context).once('bxslider-' + slider_id, function () {

          var slider_settings = settings.bxslider[slider_id].slider_settings;

          if (slider_settings.buildPager) {
            slider_settings.buildPager = new Function('slideIndex', slider_settings.buildPager);
            slider_settings.pagerCustom = null;
          }

          var indexCorrection = 0;
          if(slider_settings.infiniteLoop) {
            // If enabled Infinite Loop, then slide index increased on 1;
            indexCorrection = 1;
          }

          slider_settings.onSlideAfter = function($slideElement, oldIndex, newIndex){
            $(this).find('li.active-slide').removeClass("active-slide");
            $(this).find('li').eq(newIndex + indexCorrection).addClass("active-slide");
          }

          var slider = $('#' + slider_id + ' .bxslider', context).show().bxSlider(slider_settings);

          var current = slider.getCurrentSlide();
          slider.find('li').eq(current + indexCorrection).addClass("active-slide");
        });
      }
    }
  };
}(jQuery));
