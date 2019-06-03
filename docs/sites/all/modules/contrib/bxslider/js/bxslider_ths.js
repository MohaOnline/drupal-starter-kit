(function($) {
  Drupal.behaviors.bxslider_ths = {
    attach: function(context, settings) {
      if (!settings.bxslider_ths || context == '#cboxLoadedContent') {
        return;
      }
      for (var slider_id in settings.bxslider_ths) {

        $('#' + slider_id, context).once('bxslider-ths-' + slider_id, function () {

          var slider_settings = settings.bxslider_ths[slider_id].slider_settings;
          var thumbnail_settings = settings.bxslider_ths[slider_id].thumbnail_slider_settings;
          
          slider_settings.onSlideBefore = function ($slideElement, oldIndex, newIndex) {
            changeRealThumb(realThumbSlider, newIndex);
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

          var realSlider = $('#' + slider_id + ' .bxslider').show().bxSlider(slider_settings);

          var current = realSlider.getCurrentSlide();
          realSlider.find('li').eq(current + indexCorrection).addClass("active-slide");

          var realThumbSlider = $('#' + slider_id + " .bxslider-ths").show().bxSlider(thumbnail_settings);

          linkRealSliders(realSlider, realThumbSlider);

          $('#' + slider_id + ' .bxslider-ths').find('li[slideIndex="0"]').addClass("active");

          if ($('#' + slider_id + " .bxslider-ths li").length <= thumbnail_settings.maxSlides) {
              $('#' + slider_id + " .bxslider-ths .bx-next").hide();
          }

          function linkRealSliders(bigS, thumbS) {
            $('#' + slider_id + " ul.bxslider-ths").on("click", "a", function (event) {
              event.preventDefault();
              var newIndex = $(this).parent().attr("slideIndex");
              bigS.goToSlide(newIndex);
            });
          }

          function changeRealThumb(slider, newIndex) {
            var thumbS = $('#' + slider_id + ' .bxslider-ths');

            thumbS.find('.active').removeClass("active");
            thumbS.find('li[slideIndex="' + newIndex + '"]').addClass("active");

            var maxSlides = thumbnail_settings.maxSlides;
            var moveSlides = thumbnail_settings.moveSlides;

            // Seems that a number from MoveSlides become like a one slide in goToSlide() function.
            slider.goToSlide(Math.floor(newIndex / maxSlides) * (maxSlides / moveSlides));
          }          

        });
      }
    }
  };
}(jQuery));
