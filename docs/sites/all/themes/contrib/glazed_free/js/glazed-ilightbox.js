(function ($, Drupal, window, document, undefined) {
  Drupal.behaviors.glazedIlightbox = {
    attach: function(context, settings) {
      var counter = $('.ilightbox').length;
      var thumbs = true;
      var arrows = true;
      $('.ilightbox').iLightBox({
        skin: 'metro-black',
        path: 'horizontal',
        // linkId: deeplink,
        infinite: false,
        //fullViewPort: 'fit',
        smartRecognition: false,
        fullAlone: false,
        //fullStretchTypes: 'flash, video',
        overlay: {
          opacity: .96
        },
        controls: {
          arrows: (counter > 1 ? arrows : false),
          fullscreen: true,
          thumbnail: thumbs,
          slideshow: (counter > 1 ? true : false)
        },
        show: {
          speed: 200
        },
        hide: {
          speed: 200
        },
        social: {
          start: false,
          // buttons: social
        },
        caption: {
          start: true
        },
        styles: {
          nextOpacity: 1,
          nextScale: 1,
          prevOpacity: 1,
          prevScale: 1
        },
        effects: {
          switchSpeed: 400
        },
        slideshow: {
          pauseTime: 5000
        },
        thumbnails: {
          maxWidth: 60,
          maxHeight: 60,
          activeOpacity: .6
        },
        html5video: {
          preload: true
        }
      });
    }
  }
})(jQuery, Drupal, this, this.document);