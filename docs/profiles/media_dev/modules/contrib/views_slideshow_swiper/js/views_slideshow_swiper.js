(function ($) {
  // List arguments for all of Swiper's callback function parameters.
  Drupal.settings.viewsSlideshowSwiperValues = {
    'prefixId': 'views_slideshow_swiper_main_',
    'callbacks': {
      'init': [],
      'beforeDestroy': [],
      'slideChange': [],
      'slideChangeTransitionStart': [],
      'slideChangeTransitionEnd': [],
      'slideNextTransitionStart': [],
      'slideNextTransitionEnd': [],
      'slidePrevTransitionStart': [],
      'slidePrevTransitionEnd': [],
      'transitionStart': [],
      'transitionEnd': [],
      'touchStart': ['event'],
      'touchMove(event)': ['event'],
      'touchMoveOpposite': ['event'],
      'sliderMove': ['event'],
      'touchEnd': ['event'],
      'click': ['event'],
      'tap': ['event'],
      'doubleTap': ['event'],
      'imagesReady': [],
      'progress': ['progress'],
      'reachBeginning': [],
      'reachEnd': [],
      'fromEdge': [],
      'setTranslate': ['translate'],
      'setTransition': ['transition'],
      'resize': [],
      'paginationRender': ['swiper', 'paginationEl'],
      'paginationUpdate': ['swiper', 'paginationEl'],
      'autoplayStart': [],
      'autoplayStop': [],
      'autoplay': [],
      'lazyImageLoad': ['slideEl', 'imageEl'],
      'lazyImageReady': []
    },
    'callbackAdditions': {
      'init': function(swiper) {
        // Register callback to save references to Swiper instances. Allows
        // Views Slideshow controls to affect the Swiper.
        Drupal.viewsSlideshowSwiper.active = Drupal.viewsSlideshowSwiper.active || {};
        Drupal.viewsSlideshowSwiper.active[Drupal.settings.viewsSlideshowSwiper] = swiper;
      },
    }
  };


  // This is called when the page first loads to bootstrap Swiper.
  Drupal.behaviors.viewsSlideshowSwiper = {
    attach: function (context) {
      $('.views_slideshow_swiper_main:not(.views_slideshow_swiper-processed)', context).addClass('views_slideshow_swiper-processed').each(function() {
        // Get the ID of the slideshow
        var fullId = '#' + $(this).attr('id');

        // Create settings container
        var settings = Drupal.settings.viewsSlideshowSwiper[fullId];
        if ('autoplay' in settings.options && (settings.options['autoplay'] === 0 || settings.options['autoplay'] === '')) {
          delete settings.options.autoplay;
        }
        if (typeof settings.options['on'] === 'undefined') {
          settings.options['on'] = [];
        }

        // Define function to create callback function objects from user-inputted function body strings.
        var createFunction = (function(args, body) {
          function F(args) {
            return Function.apply(this, args);
          }
          F.prototype = Function.prototype;

          return function(args, body) {
            return new F(args.concat(body));
          }
        })();

        // For each callback function, add to its function body a call to an additional function if it exists for that callback.
        Object.keys(Drupal.settings.viewsSlideshowSwiperValues.callbackAdditions).forEach(function(parameter) {
          // If the function body is empty, instantiate it as an empty string to be added to.
          if (!(parameter in settings.options['on'])) {
            settings.options['on'][parameter] = '';
          }
          // Derive the function call string parts.
          var functionName = "Drupal.settings.viewsSlideshowSwiperValues.callbackAdditions." + parameter;
          // Add the function call to the callback function body.
          settings.options['on'][parameter] += functionName + '(this);';
        });
        Object.keys(settings.options['on']).filter(function(value) {
          return value in Drupal.settings.viewsSlideshowSwiperValues.callbacks;
        }).forEach(function(parameter) {
          // Get user-defined code that comprises the callback function body.
          var functionCode = settings.options['on'][parameter];

          // Instantiate and add a callback function if there is a non-empty function body for that callback function parameter
          if (functionCode) {
            settings.options['on'][parameter] = createFunction(Drupal.settings.viewsSlideshowSwiperValues.callbacks[parameter], functionCode);
          }
          else {
            delete settings.options['on'][parameter];
          }
        });

        // Set that Swiper has yet to load.
        settings.loaded = false;

        // Finally, instantiate Swiper for this View.
        Drupal.viewsSlideshowSwiper.load(fullId);
      });
    }
  };

  // Initialize the swiperjs object
  Drupal.viewsSlideshowSwiper = Drupal.viewsSlideshowSwiper || {};

  // Load mapping from Views Slideshow to Swiper
  Drupal.viewsSlideshowSwiper.load = function(fullId) {
    var settings = Drupal.settings.viewsSlideshowSwiper[fullId];

    // Ensure the slider isn't already loaded
    if (!settings.loaded) {
      settings.swiper = new Swiper(fullId, settings.options);
      settings.loaded = true;
    }
  };

  // Define central action dispatcher to handle all accepted actions.
  Drupal.viewsSlideshowSwiper.action = function(options) {
    var settings = Drupal.settings.viewsSlideshowSwiper['#' + Drupal.settings.viewsSlideshowSwiperValues.prefixId + options.slideshowID];
    if (settings.loaded) {
      switch (options.action) {
        case 'goToSlide':
          settings.swiper.slideTo(options.slideNum);
          break;
        case 'nextSlide':
          settings.swiper.slideNext();
          break;
        case 'pause':
          settings.swiper.stopAutoplay();
          break;
        case 'play':
          settings.swiper.startAutoplay();
          break;
        case 'previousSlide':
          settings.swiper.slidePrev();
          break;
      }
    }
  }

  // Use a central action dispatcher for more normalised code.
  Drupal.viewsSlideshowSwiper.goToSlide = Drupal.viewsSlideshowSwiper.action;
  Drupal.viewsSlideshowSwiper.pause = Drupal.viewsSlideshowSwiper.action;
  Drupal.viewsSlideshowSwiper.play = Drupal.viewsSlideshowSwiper.action;
  Drupal.viewsSlideshowSwiper.nextSlide = Drupal.viewsSlideshowSwiper.action;
  Drupal.viewsSlideshowSwiper.previousSlide = Drupal.viewsSlideshowSwiper.action;
})(jQuery);
