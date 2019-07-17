<?php

/**
 * @file
 * Views Slideshow hook implementations.
 */

/**
 * Implements hook_views_slideshow_slideshow_info().
 */
function views_slideshow_swiper_views_slideshow_slideshow_info() {
  $options = array(
    'views_slideshow_swiper' => array(
      'name' => t('Swiper'),
      'accepts' => array(
        'goToSlide',
        'nextSlide',
        'pause',
        'play',
        'previousSlide',
      ),
      'calls' => array(
        'transitionBegin',
        'transitionEnd',
        'goToSlide',
        'pause',
        'play',
        'nextSlide',
        'previousSlide',
      ),
    ),
  );

  return $options;
}

/**
 * Implements hook_views_slideshow_option_definition().
 */
function views_slideshow_swiper_views_slideshow_option_definition() {
  // Default options for Swiper
  $options['views_slideshow_swiper'] = array(
    'default' => array(
      'parameters' => array(
        'init' => TRUE,
        'initialSlide' => 0,
        'direction' => 'horizontal',
        'speed' => 300,
        'setWrapperSize' => FALSE,
        'virtualTranslate' => FALSE,
        'width' => '',
        'height' => '',
        'autoHeight' => FALSE,
        'roundLengths' => FALSE,
        'nested' => FALSE,
        'uniqueNavElements' => TRUE,
        'effect' => 'slide',
        'runCallbacksOnInit' => TRUE,
        'watchOverflow' => FALSE,
        'spaceBetween' => 0,
        'slidesPerView' => 1,
        'slidesPerColumn' => 1,
        'slidesPerColumnFill' => 'column',
        'slidesPerGroup' => 1,
        'centeredSlides' => FALSE,
        'slidesOffsetBefore' => 0,
        'slidesOffsetAfter' => 0,
        'normalizeSlideIndex' => TRUE,
        'grabCursor' => FALSE,
        'touchEventsTarget' => 'container',
        'touchRatio' => 1,
        'touchAngle' => 45,
        'simulateTouch' => TRUE,
        'shortSwipes' => TRUE,
        'longSwipes' => TRUE,
        'longSwipesRatio' => 0.5,
        'longSwipesMs' => 300,
        'followFinger' => TRUE,
        'allowTouchMove' => TRUE,
        'threshold' => 0,
        'touchMoveStopPropagation' => TRUE,
        'iOSEdgeSwipeDetection' => FALSE,
        'iOSEdgeSwipeThreshold' => 20,
        'touchReleaseOnEdges' => FALSE,
        'passiveListeners' => TRUE,
        'resistance' => TRUE,
        'resistanceRatio' => 0.85,
        'preventIntercationOnTransition' => FALSE,
        'allowSlidePrev' => TRUE,
        'allowSlideNext' => TRUE,
        'noSwiping' => TRUE,
        'noSwipingClass' => 'swiper-no-swiping',
        'noSwipingSelector' => '',
        'swipeHandler' => NULL,
        'preventClicks' => TRUE,
        'preventClicksPropagation' => TRUE,
        'slideToClickedSlide' => FALSE,
        'freeMode' => FALSE,
        'freeModeMomentum' => TRUE,
        'freeModeMomentumRatio' => 1,
        'freeModeMomentumVelocityRatio' => 1,
        'freeModeMomentumBounce' => TRUE,
        'freeModeMomentumBounceRatio' => 1,
        'freeModeMinimumVelocity' => 0.02,
        'freeModeSticky' => FALSE,
        'watchSlidesProgress' => FALSE,
        'watchSlidesVisibility' => FALSE,
        'preloadImages' => TRUE,
        'updateOnImagesReady' => TRUE,
        'loop' => FALSE,
        'loopAdditionalSlides' => 0,
        'loopedSlides' => NULL,
        'loopFillGroupWithBlank' => FALSE,
        'breakpoints' => array(
          'enabled' => FALSE,
          '480' => array(
            'slidesPerView' => 1,
            'slidesPerGroup' => 1,
            'spaceBetween' => 20,
          ),
          '768' => array(
            'slidesPerView' => 1,
            'slidesPerGroup' => 1,
            'spaceBetween' => 30,
          ),
          '992' => array(
            'slidesPerView' => 2,
            'slidesPerGroup' => 2,
            'spaceBetween' => 20,
          ),
          '1200' => array(
            'slidesPerView' => 3,
            'slidesPerGroup' => 3,
            'spaceBetween' => 20,
          ),
        ),
        'breakpointsInverse' => FALSE,
        'observer' => FALSE,
        'observeParents' => FALSE,
        'containerModifierClass' => 'swiper-container-',
        'slideClass' => 'swiper-slide',
        'slideActiveClass' => 'swiper-slide-active',
        'slideDuplicatedActiveClass' => 'swiper-slide-duplicate-active',
        'slideVisibleClass' => 'swiper-slide-visible',
        'slideDuplicateClass' => 'swiper-slide-duplicate',
        'slideNextClass' => 'swiper-slide-next',
        'slideDuplicatedNextClass' => 'swiper-slide-duplicate-next',
        'slidePrevClass' => 'swiper-slide-prev',
        'slideDuplicatedPrevClass' => 'swiper-slide-duplicate-prev',
        'wrapperClass' => 'swiper-wrapper',
      ),
      'navigation' => array(
        'enabled' => FALSE,
        'nextEl' => '.swiper-button-next',
        'prevEl' => '.swiper-button-prev',
        'hideOnClick' => FALSE,
        'disabledClass' => 'swiper-button-disabled',
        'hiddenClass' => 'swiper-button-hidden',
      ),
      'pagination' => array(
        'enabled' => FALSE,
        'el' => '.swiper-pagination',
        'type' => 'bullets',
        'bulletElement' => 'span',
        'dynamicBullets' => FALSE,
        'dynamicMainBullets' => 1,
        'hideOnClick' => TRUE,
        'clickable' => FALSE,
        'bulletClass' => 'swiper-pagination-bullet',
        'bulletActiveClass' => 'swiper-pagination-bullet-active',
        'modifierClass' => 'swiper-pagination-',
        'currentClass' => 'swiper-pagination-current',
        'totalClass' => 'swiper-pagination-total',
        'hiddenClass' => 'swiper-pagination-hidden',
        'progressbarFillClass' => 'swiper-pagination-progressbar-fill',
        'clickableClass' => 'swiper-pagination-clickable',
      ),
      'scrollbar' => array(
        'enabled' => FALSE,
        'el' => '.swiper-scrollbar',
        'hide' => TRUE,
        'draggable' => FALSE,
        'snapOnRelease' => FALSE,
        'dragSize' => 'auto',
        'lockClass' => 'swiper-scrollbar-lock',
        'dragClass' => 'swiper-scrollbar-drag',
      ),
      'autoplay' => array(
        'enabled' => FALSE,
        'delay' => 300,
        'stopOnLastSlide' => FALSE,
        'disableOnInteraction' => TRUE,
        'reverseDirection' => FALSE,
        'waitForTransition' => TRUE,
      ),
      'parallax' => array(
        'enabled' => FALSE,
      ),
      'lazy' => array(
        'enabled' => FALSE,
        'loadPrevNext' => FALSE,
        'loadPrevNextAmount' => 1,
        'loadOnTransitionStart' => FALSE,
        'elementClass' => 'swiper-lazy',
        'loadingClass' => 'swiper-lazy-loading',
        'loadedClass' => 'swiper-lazy-loaded',
        'preloaderClass' => 'swiper-lazy-preloader',
      ),
      'fadeEffect' => array(
        'crossFade' => FALSE,
      ),
      'coverflowEffect' => array(
        'slideShadows' => TRUE,
        'rotate' => 50,
        'stretch' => 0,
        'depth' => 100,
        'modifier' => 1,
      ),
      'flipEffect' => array(
        'slideShadows' => TRUE,
        'limitRotation' => TRUE,
      ),
      'cubeEffect' => array(
        'slideShadows' => TRUE,
        'shadow' => TRUE,
        'shadowOffset' => 20,
        'shadowScale' => 0.94,
      ),
      'zoom' => array(
        'enabled' => FALSE,
        'maxRatio' => 3,
        'minRatio' => 1,
        'toggle' => TRUE,
        'containerClass' => 'swiper-zoom-container',
        'zoomedSlideClass' => 'swiper-slide-zoomed',
      ),
      'keyboard' => array(
        'enabled' => FALSE,
        'onlyInViewport' => TRUE,
      ),
      'mousewheel' => array(
        'enabled' => FALSE,
        'forceToAxis' => FALSE,
        'releaseOnEdges' => FALSE,
        'invert' => FALSE,
        'sensitivity' => 1,
        'eventsTarged' => 'container',
      ),
      'hashNavigation' => array(
        'enabled' => FALSE,
        'watchState' => FALSE,
        'replaceState' => FALSE,
      ),
      'history' => array(
        'enabled' => FALSE,
        'replaceState' => FALSE,
        'key' => 'slides',
      ),
      'a11y' => array(
        'enabled' => TRUE,
        'prevSlideMessage' => 'Previous slide',
        'nextSlideMessage' => 'Next slide',
        'firstSlideMessage' => 'This is the first slide',
        'lastSlideMessage' => 'This is the last slide',
        'paginationBulletMessage' => 'Go to slide {{index}}',
        'notificationClass' => 'swiper-notification',
      ),
      'on' => array(
        'init' => '',
        'beforeDestroy' => '',
        'slideChange' => '',
        'slideChangeTransitionStart' => '',
        'slideChangeTransitionEnd' => '',
        'slideNextTransitionStart' => '',
        'slideNextTransitionEnd' => '',
        'slidePrevTransitionStart' => '',
        'slidePrevTransitionEnd' => '',
        'transitionStart' => '',
        'transitionEnd' => '',
        'touchStart' => '',
        'touchMove' => '',
        'touchMoveOpposite' => '',
        'sliderMove' => '',
        'touchEnd' => '',
        'click' => '',
        'tap' => '',
        'doubleTap' => '',
        'imagesReady' => '',
        'progress' => '',
        'reachBeginning' => '',
        'reachEnd' => '',
        'fromEdge' => '',
        'setTranslate' => '',
        'setTransition' => '',
        'resize' => '',
        'paginationRender' => '',
        'paginationUpdate' => '',
        'autoplayStart' => '',
        'autoplayStop' => '',
        'autoplay' => '',
        'lazyImageLoad' => '',
        'lazyImageReady' => ''
      ),
    ),
  );

  return $options;
}

function _views_slideshow_swiper_validate_integer_non_negative($element, &$form_state) {
  $value = $element['#value'];
  if ($value !== '' && (!is_numeric($value) || intval($value) != $value || $value < 0)) {
    form_error($element, t('%name must be a non-negative integer.', array('%name' => $element['#title'])));
  }
}

function _views_slideshow_swiper_validate_swiper_button_next($element, &$form_state) {
  $value = $element['#value'];
  if (stripos($value, 'swiper-button-next') !== 1) {
    form_error($element, t('%name must begin with .swiper-button-next , example ".swiper-button-next.swiper-three"  Description: "%desc"', array(
      '%name' => $element['#title'],
      '%desc' => $element['#description']
    )));
  }
}

function _views_slideshow_swiper_validate_swiper_button_prev($element, &$form_state) {
  $value = $element['#value'];
  if (stripos($value, 'swiper-button-prev') !== 1) {
    form_error($element, t('%name must begin with .swiper-button-prev , example ".swiper-button-prev.swiper-three"  Description: "%desc"', array(
      '%name' => $element['#title'],
      '%desc' => $element['#description']
    )));
  }
}

function _views_slideshow_swiper_validate_swiper_pagination($element, &$form_state) {
  $value = $element['#value'];
  if (stripos($value, 'swiper-pagination') !== 1) {
    form_error($element, t('%name must begin with .swiper-pagination , example ".swiper-pagination.swiper-two"  Description: "%desc"', array(
      '%name' => $element['#title'],
      '%desc' => $element['#description']
    )));
  }
}

/**
 * Configuration form within the Views Slideshow settings pane
 */
function views_slideshow_swiper_views_slideshow_slideshow_type_form(&$form, &$form_state, &$view) {
  $default_values = views_slideshow_swiper_views_slideshow_option_definition()['views_slideshow_swiper']['default'];

  $form['views_slideshow_swiper'] = array(
    '#type' => 'vertical_tabs',
  );
  $form['views_slideshow_swiper']['parameters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Parameters'),
    '#group' => 'views_slideshow_swiper',
  );
  $form['views_slideshow_swiper']['parameters']['init'] = array(
    '#type' => 'checkbox',
    '#description' => t("Whether Swiper should be initialised automatically when you create an instance. If disabled, then you need to init it manually by calling <code>mySwiper.init()</code>"),
  );
  $form['views_slideshow_swiper']['parameters']['initialSlide'] = array(
    '#type' => 'textfield',
    '#description' => t("Index number of initial slide."),
  );
  $form['views_slideshow_swiper']['parameters']['direction'] = array(
    '#type' => 'select',
    '#options' => drupal_map_assoc(array('horizontal', 'vertical')),
    '#description' => t("Could be 'horizontal' or 'vertical' (for vertical slider)."),
  );
  $form['views_slideshow_swiper']['parameters']['speed'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('element_validate_integer_positive'),
    '#description' => t("Duration of transition between slides (in ms)"),
  );
  $form['views_slideshow_swiper']['parameters']['setWrapperSize'] = array(
    '#type' => 'checkbox',
    '#description' => t("Enabled this option and plugin will set width/height on swiper wrapper equal to total size of all slides. Mostly should be used as compatibility fallback option for browser that don't support flexbox layout well"),
  );
  $form['views_slideshow_swiper']['parameters']['virtualTranslate'] = array(
    '#type' => 'checkbox',
    '#description' => t("Enabled this option and swiper will be operated as usual except it will not move, real translate values on wrapper will not be set. Useful when you may need to create custom slide transition"),
  );
  $form['views_slideshow_swiper']['parameters']['width'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('_views_slideshow_swiper_validate_integer_non_negative'),
    '#description' => t("Swiper width (in px). Parameter allows to force Swiper width. Useful <strong>only</strong> if you initialize Swiper when it is hidden."),
  );
  $form['views_slideshow_swiper']['parameters']['height'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('_views_slideshow_swiper_validate_integer_non_negative'),
    '#description' => t("Swiper height (in px). Parameter allows to force Swiper <strong>height</strong>. Useful only if you initialize Swiper when it is hidden."),
  );
  $form['views_slideshow_swiper']['parameters']['autoHeight'] = array(
    '#type' => 'checkbox',
    '#description' => t("Set to <strong>true</strong> and slider wrapper will adopt its height to the height of the currently active slide"),
  );
  $form['views_slideshow_swiper']['parameters']['roundLengths'] = array(
    '#type' => 'checkbox',
    '#description' => t("Set to <strong>true</strong> to round values of slides width and height to prevent blurry texts on usual resolution screens (if you have such)"),
  );
  $form['views_slideshow_swiper']['parameters']['nested'] = array(
    '#type' => 'checkbox',
    '#description' => t("Set to <strong>true</strong> on nested Swiper for correct touch events interception. Use only on nested swipers that use same direction as the parent one"),
  );
  $form['views_slideshow_swiper']['parameters']['uniqueNavElements'] = array(
    '#type' => 'checkbox',
    '#description' => t("If enabled (by default) and navigation elements' parameters passed as a string (like <code>\".pagination\"</code>) then Swiper will look for such elements through child elements first. Applies for pagination, prev/next buttons and scrollbar elements"),
  );
  $form['views_slideshow_swiper']['parameters']['effect'] = array(
    '#type' => 'select',
    '#options' => drupal_map_assoc(array(
      'slide',
      'fade',
      'cube',
      'coverflow',
      'flip',
    )),
    '#description' => t("Tranisition effect. Could be \"slide\", \"fade\", \"cube\", \"coverflow\" or \"flip\""),
  );
  $form['views_slideshow_swiper']['parameters']['runCallbacksOnInit'] = array(
    '#type' => 'checkbox',
    '#description' => t("Fire [Transition/SlideChange][Start/End] events on swiper initialization. Such events will be fired on initialization in case of your initialSlide is not 0, or you use loop mode"),
  );
  $form['views_slideshow_swiper']['parameters']['watchOverflow'] = array(
    '#type' => 'checkbox',
    '#description' => t("When enabled Swiper will be disabled and hide navigation buttons on case there are not enough slides for sliding"),
  );

  $form['views_slideshow_swiper']['parameters']['spaceBetween'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('element_validate_number'),
    '#description' => t("Distance between slides in px."),
  );
  $form['views_slideshow_swiper']['parameters']['slidesPerView'] = array(
    '#type' => 'textfield',
    '#description' => t("Number of slides per view (slides visible at the same time on slider's container)."),
  );
  $form['views_slideshow_swiper']['parameters']['slidesPerColumn'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('_views_slideshow_swiper_validate_integer_non_negative'),
    '#description' => t("Number of slides per column, for multirow layout"),
  );
  $form['views_slideshow_swiper']['parameters']['slidesPerColumnFill'] = array(
    '#type' => 'select',
    '#options' => drupal_map_assoc(array('column', 'row')),
    '#description' => t("Could be 'column' or 'row'. Defines how slides should fill rows, by column or by row"),
  );
  $form['views_slideshow_swiper']['parameters']['slidesPerGroup'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('_views_slideshow_swiper_validate_integer_non_negative'),
    '#description' => t("Set numbers of slides to define and enable group sliding. Useful to use with slidesPerView > 1"),
  );
  $form['views_slideshow_swiper']['parameters']['centeredSlides'] = array(
    '#type' => 'checkbox',
    '#description' => t("If true, then active slide will be centered, not always on the left side."),
  );
  $form['views_slideshow_swiper']['parameters']['slidesOffsetBefore'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('_views_slideshow_swiper_validate_integer_non_negative'),
    '#description' => t("Add (in px) additional slide offset in the beginning of the container (before all slides)"),
  );
  $form['views_slideshow_swiper']['parameters']['slidesOffsetAfter'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('_views_slideshow_swiper_validate_integer_non_negative'),
    '#description' => t("Add (in px) additional slide offset in the end of the container (after all slides)"),
  );
  $form['views_slideshow_swiper']['parameters']['normalizeSlideIndex'] = array(
    '#type' => 'checkbox',
    '#description' => t("Normalize slide index"),
  );
  $form['views_slideshow_swiper']['parameters']['grabCursor'] = array(
    '#type' => 'checkbox',
    '#description' => t("This option may a little improve desktop usability. If <strong>true</strong>, user will see the \"grab\" cursor when hover on Swiper"),
  );
  $form['views_slideshow_swiper']['parameters']['touchEventsTarget'] = array(
    '#type' => 'select',
    '#options' => drupal_map_assoc(array('container', 'wrapper')),
    '#description' => t("Target element to listen touch events on. Can be '<strong>container</strong>' (to listen for touch events on swiper-container) or '<strong>wrapper</strong>' (to listen for touch events on swiper-wrapper)"),
  );
  $form['views_slideshow_swiper']['parameters']['touchRatio'] = array(
    '#type' => 'textfield',
    '#description' => t("Touch ration"),
  );
  $form['views_slideshow_swiper']['parameters']['touchAngle'] = array(
    '#type' => 'textfield',
    '#description' => t("Allowable angle (in degrees) to trigger touch move"),
  );
  $form['views_slideshow_swiper']['parameters']['simulateTouch'] = array(
    '#type' => 'checkbox',
    '#description' => t("If true, Swiper will accept mouse events like touch events (click and drag to change slides)"),
  );
  $form['views_slideshow_swiper']['parameters']['shortSwipes'] = array(
    '#type' => 'checkbox',
    '#description' => t("Set to <strong>false</strong> if you want to disable short swipes"),
  );
  $form['views_slideshow_swiper']['parameters']['longSwipes'] = array(
    '#type' => 'checkbox',
    '#description' => t("Set to <strong>false</strong> if you want to disable long swipes"),
  );
  $form['views_slideshow_swiper']['parameters']['longSwipesRatio'] = array(
    '#type' => 'textfield',
    '#description' => t("Ratio to trigger swipe to next/previous slide during long swipes"),
  );
  $form['views_slideshow_swiper']['parameters']['longSwipesMs'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('_views_slideshow_swiper_validate_integer_non_negative'),
    '#description' => t("Minimal duration (in ms) to trigger swipe to next/previous slide during long swipes"),
  );
  $form['views_slideshow_swiper']['parameters']['followFinger'] = array(
    '#type' => 'checkbox',
    '#description' => t("If disabled, then slider will be animated only when you release it, it will not move while you hold your finger on it"),
  );
  $form['views_slideshow_swiper']['parameters']['allowTouchMove'] = array(
    '#type' => 'checkbox',
    '#description' => t("If false, then the only way to switch the slide is use of external API functions like <code>slidePrev</code> or <code>slideNext</code>"),
  );
  $form['views_slideshow_swiper']['parameters']['threshold'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('_views_slideshow_swiper_validate_integer_non_negative'),
    '#description' => t("Threshold value in px. If \"touch distance\" will be lower than this value then swiper will not move"),
  );
  $form['views_slideshow_swiper']['parameters']['touchMoveStopPropagation'] = array(
    '#type' => 'checkbox',
    '#description' => t("If enabled, then propagation of \"touchmove\" will be stopped"),
  );
  $form['views_slideshow_swiper']['parameters']['iOSEdgeSwipeDetection'] = array(
    '#type' => 'checkbox',
    '#description' => t("Enable to release Swiper events for swipe-to-go-back work in iOS UIWebView"),
  );
  $form['views_slideshow_swiper']['parameters']['iOSEdgeSwipeThreshold'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('_views_slideshow_swiper_validate_integer_non_negative'),
    '#description' => t("Area (in px) from left edge of the screen to release touch events for swipe-to-go-back in iOS UIWebView"),
  );
  $form['views_slideshow_swiper']['parameters']['touchReleaseOnEdges'] = array(
    '#type' => 'checkbox',
    '#description' => t("Enable to release touch events on slider edge position (beginning, end) to allow for further page scrolling"),
  );
  $form['views_slideshow_swiper']['parameters']['passiveListeners'] = array(
    '#type' => 'checkbox',
    '#description' => t("Passive event listeners will be used by default where possible to improve scrolling performance on mobile devices. But if you need to use `e.preventDefault` and you have conflict with it, then you should disable this parameter"),
  );
  $form['views_slideshow_swiper']['parameters']['resistance'] = array(
    '#type' => 'checkbox',
    '#description' => t("Set to <strong>false</strong> if you want to disable resistant bounds"),
  );
  $form['views_slideshow_swiper']['parameters']['resistanceRatio'] = array(
    '#type' => 'textfield',
    '#description' => t("This option allows you to control resistance ratio"),
  );
  $form['views_slideshow_swiper']['parameters']['preventIntercationOnTransition'] = array(
    '#type' => 'checkbox',
    '#description' => t("When enabled it won't allow to change slides by swiping or navigation/pagination buttons during transition"),
  );
  $form['views_slideshow_swiper']['parameters']['allowSlidePrev'] = array(
    '#type' => 'checkbox',
    '#description' => t("Set to <strong>false</strong> to disable swiping to previous slide direction (to left or top)"),
  );
  $form['views_slideshow_swiper']['parameters']['allowSlideNext'] = array(
    '#type' => 'checkbox',
    '#description' => t("Set to <strong>false</strong> to disable swiping to next slide direction (to right or bottom)"),
  );
  $form['views_slideshow_swiper']['parameters']['noSwiping'] = array(
    '#type' => 'checkbox',
    '#description' => t("Enable/disable swiping on elements matched to class specified in <code>noSwipingClass</code>"),
  );
  $form['views_slideshow_swiper']['parameters']['noSwipingClass'] = array(
    '#type' => 'textfield',
    '#description' => t("Specify <code>noSwiping</code>'s' element css class"),
  );
  $form['views_slideshow_swiper']['parameters']['noSwipingSelector'] = array(
    '#type' => 'textfield',
    '#description' => t("Can be used instead of <code>noSwipingClass</code> to specify elements to disable swiping on. For example <code>'input'</code> will disable swiping on all inputs"),
  );
  $form['views_slideshow_swiper']['parameters']['swipeHandler'] = array(
    '#type' => 'textfield',
    '#description' => t("String with CSS selector or HTML element of the container with pagination that will work as only available handler for swiping"),
  );
  $form['views_slideshow_swiper']['parameters']['preventClicks'] = array(
    '#type' => 'checkbox',
    '#description' => t("Set to <strong>true</strong> to prevent accidental unwanted clicks on links during swiping"),
  );
  $form['views_slideshow_swiper']['parameters']['preventClicksPropagation'] = array(
    '#type' => 'checkbox',
    '#description' => t("Set to <strong>true</strong> to stop clicks event propagation on links during swiping"),
  );
  $form['views_slideshow_swiper']['parameters']['slideToClickedSlide'] = array(
    '#type' => 'checkbox',
    '#description' => t("Set to <strong>true</strong> and click on any slide will produce transition to this slide"),
  );
  $form['views_slideshow_swiper']['parameters']['freeMode'] = array(
    '#type' => 'checkbox',
    '#description' => t("If <strong>true</strong> then slides will not have fixed positions"),
  );
  $form['views_slideshow_swiper']['parameters']['freeModeMomentum'] = array(
    '#type' => 'checkbox',
    '#description' => t("If <strong>true</strong>, then slide will keep moving for a while after you release it"),
  );
  $form['views_slideshow_swiper']['parameters']['freeModeMomentumRatio'] = array(
    '#type' => 'textfield',
    '#description' => t("Higher value produces larger momentum distance after you release slider"),
  );
  $form['views_slideshow_swiper']['parameters']['freeModeMomentumVelocityRatio'] = array(
    '#type' => 'textfield',
    '#description' => t("Higher value produces larger momentum velocity after you release slider"),
  );
  $form['views_slideshow_swiper']['parameters']['freeModeMomentumBounce'] = array(
    '#type' => 'checkbox',
    '#description' => t("Set to <strong>false</strong> if you want to disable momentum bounce in free mode"),
  );
  $form['views_slideshow_swiper']['parameters']['freeModeMomentumBounceRatio'] = array(
    '#type' => 'textfield',
    '#description' => t("Higher value produces larger momentum bounce effect"),
  );
  $form['views_slideshow_swiper']['parameters']['freeModeMinimumVelocity'] = array(
    '#type' => 'textfield',
    '#description' => t("Minimum touchmove-velocity required to trigger free mode momentum"),
  );
  $form['views_slideshow_swiper']['parameters']['freeModeSticky'] = array(
    '#type' => 'checkbox',
    '#description' => t("Set to <strong>true</strong> to enable snap to slides positions in free mode"),
  );
  $form['views_slideshow_swiper']['parameters']['watchSlidesProgress'] = array(
    '#type' => 'checkbox',
    '#description' => t("Enable this feature to calculate each slides progress"),
  );
  $form['views_slideshow_swiper']['parameters']['watchSlidesVisibility'] = array(
    '#type' => 'checkbox',
    '#description' => t("<code>watchSlidesProgress</code> should be enabled. Enable this option and slides that are in viewport will have additional visible class"),
  );
  $form['views_slideshow_swiper']['parameters']['preloadImages'] = array(
    '#type' => 'checkbox',
    '#description' => t("When enabled Swiper will force to load all images"),
  );
  $form['views_slideshow_swiper']['parameters']['updateOnImagesReady'] = array(
    '#type' => 'checkbox',
    '#description' => t("When enabled Swiper will be reinitialized after all inner images (<img> tags) are loaded. Required <code>preloadImages: true</code>"),
  );
  $form['views_slideshow_swiper']['parameters']['loop'] = array(
    '#type' => 'checkbox',
    '#description' => t("Set to <strong>true</strong> to enable continuous loop mode"),
  );
  $form['views_slideshow_swiper']['parameters']['loopAdditionalSlides'] = array(
    '#type' => 'textfield',
    '#description' => t("Addition number of slides that will be cloned after creating of loop"),
  );
  $form['views_slideshow_swiper']['parameters']['loopedSlides'] = array(
    '#type' => 'textfield',
    '#description' => t("If you use <code>slidesPerView:'auto'</code> with loop mode you should tell to Swiper how many slides it should loop (duplicate) using this parameter"),
  );
  $form['views_slideshow_swiper']['parameters']['loopFillGroupWithBlank'] = array(
    '#type' => 'checkbox',
    '#description' => t("Enable and loop mode will fill groups with insufficient number of slides with blank slides. Good to be used with <code>slidesPerGroup</code> parameter"),
  );
  $form['views_slideshow_swiper']['parameters']['breakpoints'] = array(
    '#type' => 'fieldset',
    '#title' => t('Breakpoints'),
    '#group' => 'views_slideshow_swiper',
  );
  $form['views_slideshow_swiper']['parameters']['breakpoints']['enabled'] = array(
    '#type' => 'checkbox',
    '#description' => t("Check to enable breakpoints."),
  );
  $form['views_slideshow_swiper']['parameters']['breakpoints']['480']['slidesPerView'] = array(
    '#type' => 'textfield',
    '#description' => t("How many slidesPerView below 480 pixel window width. Api docs here: http://idangero.us/swiper/api/#parameters."),
  );
  $form['views_slideshow_swiper']['parameters']['breakpoints']['480']['slidesPerGroup'] = array(
    '#type' => 'textfield',
    '#description' => t("How many slidesPerGroup below 480 pixel window width. Api docs here: http://idangero.us/swiper/api/#parameters."),
  );
  $form['views_slideshow_swiper']['parameters']['breakpoints']['480']['spaceBetween'] = array(
    '#type' => 'textfield',
    '#description' => t("How much space between items below 480 pixel window width."),
  );
  $form['views_slideshow_swiper']['parameters']['breakpoints']['768']['slidesPerView'] = array(
    '#type' => 'textfield',
    '#description' => t("How many slidesPerView below 768 pixel window width. Api docs here: http://idangero.us/swiper/api/#parameters."),
  );
  $form['views_slideshow_swiper']['parameters']['breakpoints']['768']['slidesPerGroup'] = array(
    '#type' => 'textfield',
    '#description' => t("How many slidesPerGroup below 768 pixel window width. Api docs here: http://idangero.us/swiper/api/#parameters."),
  );
  $form['views_slideshow_swiper']['parameters']['breakpoints']['768']['spaceBetween'] = array(
    '#type' => 'textfield',
    '#description' => t("How much space between items below 768 pixel window width."),
  );
  $form['views_slideshow_swiper']['parameters']['breakpoints']['992']['slidesPerView'] = array(
    '#type' => 'textfield',
    '#description' => t("How many slidesPerView below 992 pixel window width. Api docs here: http://idangero.us/swiper/api/#parameters."),
  );
  $form['views_slideshow_swiper']['parameters']['breakpoints']['992']['slidesPerGroup'] = array(
    '#type' => 'textfield',
    '#description' => t("How many slidesPerGroup below 992 pixel window width. Api docs here: http://idangero.us/swiper/api/#parameters."),
  );
  $form['views_slideshow_swiper']['parameters']['breakpoints']['992']['spaceBetween'] = array(
    '#type' => 'textfield',
    '#description' => t("How much space between items below 992 pixel window width."),
  );
  $form['views_slideshow_swiper']['parameters']['breakpoints']['1200']['slidesPerView'] = array(
    '#type' => 'textfield',
    '#description' => t("How many slidesPerView below 1200 pixel window width. Api docs here: http://idangero.us/swiper/api/#parameters."),
  );
  $form['views_slideshow_swiper']['parameters']['breakpoints']['1200']['slidesPerGroup'] = array(
    '#type' => 'textfield',
    '#description' => t("How many slidesPerGroup below 1200 pixel window width. Api docs here: http://idangero.us/swiper/api/#parameters."),
  );
  $form['views_slideshow_swiper']['parameters']['breakpoints']['1200']['spaceBetween'] = array(
    '#type' => 'textfield',
    '#description' => t("How much space between items below 1200 pixel window width."),
  );
  $form['views_slideshow_swiper']['parameters']['breakpointsInverse'] = array(
    '#type' => 'checkbox',
    '#description' => t("Set to <strong>true</strong> to enable inverse breakpoints.)"),
  );
  $form['views_slideshow_swiper']['parameters']['observer'] = array(
    '#type' => 'checkbox',
    '#description' => t("Set to <strong>true</strong> to enable Mutation Observer on Swiper and its elements. In this case Swiper will be updated (reinitialized) each time if you change its style (like hide/show) or modify its child elements (like adding/removing slides)"),
  );
  $form['views_slideshow_swiper']['parameters']['observeParents'] = array(
    '#type' => 'checkbox',
    '#description' => t("Set to <strong>true</strong> if you also need to watch Mutations for Swiper parent elements"),
  );
  $form['views_slideshow_swiper']['parameters']['containerModifierClass'] = array(
    '#type' => 'textfield',
    '#description' => t("The beginning of the modifier CSS class that can be added to swiper container depending on different parameters"),
  );
  $form['views_slideshow_swiper']['parameters']['slideClass'] = array(
    '#type' => 'textfield',
    '#description' => t("CSS class name of slide"),
  );
  $form['views_slideshow_swiper']['parameters']['slideActiveClass'] = array(
    '#type' => 'textfield',
    '#description' => t("CSS class name of currently active slide"),
  );
  $form['views_slideshow_swiper']['parameters']['slideDuplicatedActiveClass'] = array(
    '#type' => 'textfield',
    '#description' => t("CSS class name of duplicated slide which represents the currently active slide"),
  );
  $form['views_slideshow_swiper']['parameters']['slideVisibleClass'] = array(
    '#type' => 'textfield',
    '#description' => t("CSS class name of currently visible slide"),
  );
  $form['views_slideshow_swiper']['parameters']['slideDuplicateClass'] = array(
    '#type' => 'textfield',
    '#description' => t("CSS class name of slide duplicated by loop mode"),
  );
  $form['views_slideshow_swiper']['parameters']['slideNextClass'] = array(
    '#type' => 'textfield',
    '#description' => t("CSS class name of slide which is right after currently active slide"),
  );
  $form['views_slideshow_swiper']['parameters']['slideDuplicatedNextClass'] = array(
    '#type' => 'textfield',
    '#description' => t("CSS class name of duplicated slide which represents the slide next to active slide"),
  );
  $form['views_slideshow_swiper']['parameters']['slidePrevClass'] = array(
    '#type' => 'textfield',
    '#description' => t("CSS class name of slide which is right before currently active slide"),
  );
  $form['views_slideshow_swiper']['parameters']['slideDuplicatedPrevClass'] = array(
    '#type' => 'textfield',
    '#description' => t("CSS class name of duplicated slide which represents the slide previous to active slide"),
  );
  $form['views_slideshow_swiper']['parameters']['wrapperClass'] = array(
    '#type' => 'textfield',
    '#description' => t("CSS class name of slides' wrapper"),
  );

  $form['views_slideshow_swiper']['navigation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Navigation'),
    '#group' => 'views_slideshow_swiper',
  );
  $form['views_slideshow_swiper']['navigation']['enabled'] = array(
    '#type' => 'checkbox',
  );
  $form['views_slideshow_swiper']['navigation']['nextEl'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('_views_slideshow_swiper_validate_swiper_button_next'),
    '#description' => t("Must begin with .swiper-button-next , if you have more than one swiper view on same page put for example \".swiper-button-next.swiper-two\" for the second view and so on and so forth. it is a selector or HTML element of the element that will work like \"next\" button after click on it"),
  );
  $form['views_slideshow_swiper']['navigation']['prevEl'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('_views_slideshow_swiper_validate_swiper_button_prev'),
    '#description' => t("Must begin with .swiper-button-prev , if you have more than one swiper view on same page put for example \".swiper-button-prev.swiper-two\" for the second view and so on and so forth. it is a selector or HTML element of the element that will work like \"prev\" button after click on it"),
  );
  $form['views_slideshow_swiper']['navigation']['hideOnClick'] = array(
    '#type' => 'checkbox',
    '#description' => t("Toggle navigation buttons visibility after click on Slider's container"),
  );
  $form['views_slideshow_swiper']['navigation']['disabledClass'] = array(
    '#type' => 'textfield',
    '#description' => t("CSS class name added to navigation button when it becomes disabled"),
  );
  $form['views_slideshow_swiper']['navigation']['hiddenClass'] = array(
    '#type' => 'textfield',
    '#description' => t("CSS class name added to navigation button when it becomes hidden"),
  );


  $form['views_slideshow_swiper']['pagination'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pagination'),
    '#group' => 'views_slideshow_swiper',
  );
  $form['views_slideshow_swiper']['pagination']['enabled'] = array(
    '#type' => 'checkbox',
  );
  $form['views_slideshow_swiper']['pagination']['el'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('_views_slideshow_swiper_validate_swiper_pagination'),
    '#description' => t('String with CSS selector or HTML element of the container with pagination ex) ".swiper-pagination" OR ".swiper-pagination.swiper-two"'),
  );
  $form['views_slideshow_swiper']['pagination']['type'] = array(
    '#type' => 'select',
    '#options' => drupal_map_assoc(array(
      'bullets',
      'fraction',
      'progressbar',
      'custom',
    )),
    '#description' => t("String with type of pagination. Can be \"bullets\", \"fraction\", \"progressbar\" or \"custom\""),
  );
  $form['views_slideshow_swiper']['pagination']['bulletElement'] = array(
    '#type' => 'textfield',
    '#description' => t("Defines which HTML tag will be use to represent single pagination bullet. Only for <strong>bullets</strong> pagination type."),
  );
  $form['views_slideshow_swiper']['pagination']['dynamicBullets'] = array(
    '#type' => 'checkbox',
    '#description' => t("Good to enable if you use bullets pagination with a lot of slides. So it will keep only few bullets visible at the same time."),
  );
  $form['views_slideshow_swiper']['pagination']['dynamicMainBullets'] = array(
    '#type' => 'textfield',
    '#description' => t("The number of main bullets visible when <strong>dynamicBullets</strong> enabled."),
  );
  $form['views_slideshow_swiper']['pagination']['hideOnClick'] = array(
    '#type' => 'checkbox',
    '#description' => t("Defines which HTML tag will be use to represent single pagination bullet. Only for <strong>bullets</strong> pagination type."),
  );
  $form['views_slideshow_swiper']['pagination']['clickable'] = array(
    '#type' => 'checkbox',
    '#description' => t("If <strong>true</strong> then clicking on pagination button will cause transition to appropriate slide. Only for <strong>bullets</strong> pagination type"),
  );
  $form['views_slideshow_swiper']['pagination']['bulletClass'] = array(
    '#type' => 'textfield',
    '#description' => t("CSS class name of single pagination bullet"),
  );
  $form['views_slideshow_swiper']['pagination']['bulletActiveClass'] = array(
    '#type' => 'textfield',
    '#description' => t("CSS class name of currently active pagination bullet"),
  );
  $form['views_slideshow_swiper']['pagination']['modifierClass'] = array(
    '#type' => 'textfield',
    '#description' => t("The beginning of the modifier CSS class name that will be added to pagination depending on parameters"),
  );
  $form['views_slideshow_swiper']['pagination']['currentClass'] = array(
    '#type' => 'textfield',
    '#description' => t("CSS class name of the element with currently active index in \"fraction\" pagination"),
  );
  $form['views_slideshow_swiper']['pagination']['totalClass'] = array(
    '#type' => 'textfield',
    '#description' => t("CSS class name of the element with total number of \"snaps\" in \"fraction\" pagination"),
  );
  $form['views_slideshow_swiper']['pagination']['hiddenClass'] = array(
    '#type' => 'textfield',
    '#description' => t("CSS class name of pagination when it becomes inactive"),
  );
  $form['views_slideshow_swiper']['pagination']['progressbarFillClass'] = array(
    '#type' => 'textfield',
    '#description' => t("CSS class name of pagination progressbar fill element"),
  );
  $form['views_slideshow_swiper']['pagination']['clickableClass'] = array(
    '#type' => 'textfield',
    '#description' => t("CSS class name set to pagination when it is clickable"),
  );


  $form['views_slideshow_swiper']['scrollbar'] = array(
    '#type' => 'fieldset',
    '#title' => t('Scrollbar'),
    '#group' => 'views_slideshow_swiper',
  );
  $form['views_slideshow_swiper']['scrollbar']['enabled'] = array(
    '#type' => 'checkbox',
  );
  $form['views_slideshow_swiper']['scrollbar']['el'] = array(
    '#type' => 'textfield',
    '#description' => t("String with CSS selector or HTML element of the container with scrollbar."),
  );
  $form['views_slideshow_swiper']['scrollbar']['hide'] = array(
    '#type' => 'checkbox',
    '#description' => t("Hide scrollbar automatically after user interaction"),
  );
  $form['views_slideshow_swiper']['scrollbar']['draggable'] = array(
    '#type' => 'checkbox',
    '#description' => t("Set to <strong>true</strong> to enable make scrollbar draggable that allows you to control slider position"),
  );
  $form['views_slideshow_swiper']['scrollbar']['snapOnRelease'] = array(
    '#type' => 'checkbox',
    '#description' => t("Set to <strong>true</strong> to snap slider position to slides when you release scrollbar"),
  );
  $form['views_slideshow_swiper']['scrollbar']['dragSize'] = array(
    '#type' => 'textfield',
    '#description' => t("Size of scrollbar draggable element in px"),
  );
  $form['views_slideshow_swiper']['scrollbar']['lockClass'] = array(
    '#type' => 'textfield',
    '#description' => t("Scrollbar element additional CSS class when it is disabled"),
  );
  $form['views_slideshow_swiper']['scrollbar']['dragClass'] = array(
    '#type' => 'textfield',
    '#description' => t("Scrollbar draggable element CSS class"),
  );


  $form['views_slideshow_swiper']['autoplay'] = array(
    '#type' => 'fieldset',
    '#title' => t('Autoplay'),
    '#group' => 'views_slideshow_swiper',
  );
  $form['views_slideshow_swiper']['autoplay']['enabled'] = array(
    '#type' => 'checkbox',
  );
  $form['views_slideshow_swiper']['autoplay']['delay'] = array(
    '#type' => 'textfield',
    '#description' => t("Delay between transitions (in ms). If this parameter is not specified, auto play will be disabled"),
  );
  $form['views_slideshow_swiper']['autoplay']['stopOnLastSlide'] = array(
    '#type' => 'checkbox',
    '#description' => t("Enable this parameter and autoplay will be stopped when it reaches last slide (has no effect in loop mode)"),
  );
  $form['views_slideshow_swiper']['autoplay']['disableOnInteraction'] = array(
    '#type' => 'checkbox',
    '#description' => t("Set to false and autoplay will not be disabled after user interactions (swipes), it will be restarted every time after interaction"),
  );
  $form['views_slideshow_swiper']['autoplay']['reverseDirection'] = array(
    '#type' => 'checkbox',
    '#description' => t("Enables autoplay in reverse direction"),
  );
  $form['views_slideshow_swiper']['autoplay']['waitForTransition'] = array(
    '#type' => 'checkbox',
    '#description' => t("When enabled autoplay will wait for wrapper transition to continue. Can be disabled in case of using Virtual Translate when your slider may not have transition"),
  );


  $form['views_slideshow_swiper']['parallax'] = array(
    '#type' => 'fieldset',
    '#title' => t('Parallax'),
    '#group' => 'views_slideshow_swiper',
  );
  $form['views_slideshow_swiper']['parallax']['enabled'] = array(
    '#type' => 'checkbox',
  );


  $form['views_slideshow_swiper']['lazy'] = array(
    '#type' => 'fieldset',
    '#title' => t('Lazy loading'),
    '#group' => 'views_slideshow_swiper',
  );
  $form['views_slideshow_swiper']['lazy']['enabled'] = array(
    '#type' => 'checkbox',
  );
  $form['views_slideshow_swiper']['lazy']['loadPrevNext'] = array(
    '#type' => 'checkbox',
    '#description' => t("Set to \"true\" to enable lazy loading for the closest slides images (for previous and next slide images)"),
  );
  $form['views_slideshow_swiper']['lazy']['loadPrevNextAmount'] = array(
    '#type' => 'textfield',
    '#description' => t("Amount of next/prev slides to preload lazy images in. Can't be less than <code>slidesPerView</code>"),
  );
  $form['views_slideshow_swiper']['lazy']['loadOnTransitionStart'] = array(
    '#type' => 'checkbox',
    '#description' => t("By default, Swiper will load lazy images after transition to this slide, so you may enable this parameter if you need it to start loading of new image in the beginning of transition"),
  );
  $form['views_slideshow_swiper']['lazy']['elementClass'] = array(
    '#type' => 'textfield',
    '#description' => t("CSS class name of lazy element"),
  );
  $form['views_slideshow_swiper']['lazy']['loadingClass'] = array(
    '#type' => 'textfield',
    '#description' => t("CSS class name of lazy loading element"),
  );
  $form['views_slideshow_swiper']['lazy']['loadedClass'] = array(
    '#type' => 'textfield',
    '#description' => t("CSS class name of lazy loaded element"),
  );
  $form['views_slideshow_swiper']['lazy']['preloaderClass'] = array(
    '#type' => 'textfield',
    '#description' => t("CSS class name of lazy preloader"),
  );


  $form['views_slideshow_swiper']['fadeEffect'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fade effect'),
    '#group' => 'views_slideshow_swiper',
  );
  $form['views_slideshow_swiper']['fadeEffect']['crossFade'] = array(
    '#type' => 'checkbox',
    '#description' => t("By default, Swiper will load lazy images after transition to this slide, so you may enable this parameter if you need it to start loading of new image in the beginning of transition"),
  );

  $form['views_slideshow_swiper']['coverflowEffect'] = array(
    '#type' => 'fieldset',
    '#title' => t('Coverflow effect'),
    '#group' => 'views_slideshow_swiper',
  );
  $form['views_slideshow_swiper']['coverflowEffect']['slideShadows'] = array(
    '#type' => 'checkbox',
    '#description' => t("Enables slides shadows"),
  );
  $form['views_slideshow_swiper']['coverflowEffect']['rotate'] = array(
    '#type' => 'textfield',
    '#description' => t("Slide rotate in degrees"),
  );
  $form['views_slideshow_swiper']['coverflowEffect']['stretch'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('_views_slideshow_swiper_validate_integer_non_negative'),
    '#description' => t("Stretch space between slides (in px)"),
  );
  $form['views_slideshow_swiper']['coverflowEffect']['depth'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('_views_slideshow_swiper_validate_integer_non_negative'),
    '#description' => t("Depth offset in px (slides translate in Z axis)"),
  );
  $form['views_slideshow_swiper']['coverflowEffect']['modifier'] = array(
    '#type' => 'textfield',
    '#description' => t("Effect multipler"),
  );


  $form['views_slideshow_swiper']['flipEffect'] = array(
    '#type' => 'fieldset',
    '#title' => t('Flip effect'),
    '#group' => 'views_slideshow_swiper',
  );
  $form['views_slideshow_swiper']['flipEffect']['slideShadows'] = array(
    '#type' => 'checkbox',
    '#description' => t("Enables slides shadows"),
  );
  $form['views_slideshow_swiper']['flipEffect']['limitRotation'] = array(
    '#type' => 'checkbox',
    '#description' => t("Limit edge slides rotation"),
  );


  $form['views_slideshow_swiper']['cubeEffect'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cube effect'),
    '#group' => 'views_slideshow_swiper',
  );
  $form['views_slideshow_swiper']['cubeEffect']['slideShadows'] = array(
    '#type' => 'checkbox',
    '#description' => t("Enables slides shadows"),
  );
  $form['views_slideshow_swiper']['cubeEffect']['shadow'] = array(
    '#type' => 'checkbox',
    '#description' => t("Enables main slider shadow"),
  );
  $form['views_slideshow_swiper']['cubeEffect']['shadowOffset'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('_views_slideshow_swiper_validate_integer_non_negative'),
    '#description' => t("Main shadow offset in px"),
  );
  $form['views_slideshow_swiper']['cubeEffect']['shadowScale'] = array(
    '#type' => 'textfield',
    '#description' => t("Main shadow scale ratio"),
  );


  $form['views_slideshow_swiper']['zoom'] = array(
    '#type' => 'fieldset',
    '#title' => t('Zoom'),
    '#group' => 'views_slideshow_swiper',
  );
  $form['views_slideshow_swiper']['zoom']['enabled'] = array(
    '#type' => 'checkbox',
  );
  $form['views_slideshow_swiper']['zoom']['maxRatio'] = array(
    '#type' => 'textfield',
    '#description' => t("Maximum image zoom multiplier"),
  );
  $form['views_slideshow_swiper']['zoom']['minRatio'] = array(
    '#type' => 'textfield',
    '#description' => t("Minimal image zoom multiplier"),
  );
  $form['views_slideshow_swiper']['zoom']['toggle'] = array(
    '#type' => 'checkbox',
    '#description' => t("Enable/disable zoom-in by slide's double tap"),
  );
  $form['views_slideshow_swiper']['zoom']['containerClass'] = array(
    '#type' => 'textfield',
    '#description' => t("CSS class name of zoom container"),
  );
  $form['views_slideshow_swiper']['zoom']['zoomedSlideClass'] = array(
    '#type' => 'textfield',
    '#description' => t("CSS class name of zoomed in container"),
  );


  $form['views_slideshow_swiper']['keyboard'] = array(
    '#type' => 'fieldset',
    '#title' => t('Keyboard control'),
    '#group' => 'views_slideshow_swiper',
  );
  $form['views_slideshow_swiper']['keyboard']['enabled'] = array(
    '#type' => 'checkbox',
  );
  $form['views_slideshow_swiper']['keyboard']['onlyInViewport'] = array(
    '#type' => 'checkbox',
    '#description' => t("When enabled it will control sliders that are currently in viewport"),
  );


  $form['views_slideshow_swiper']['mousewheel'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mousewheel control'),
    '#group' => 'views_slideshow_swiper',
  );
  $form['views_slideshow_swiper']['mousewheel']['enabled'] = array(
    '#type' => 'checkbox',
  );
  $form['views_slideshow_swiper']['mousewheel']['forceToAxis'] = array(
    '#type' => 'checkbox',
    '#description' => t("Set to <strong>true</strong> to force mousewheel swipes to axis. So in horizontal mode mousewheel will work only with horizontal mousewheel scrolling, and only with vertical scrolling in vertical mode."),
  );
  $form['views_slideshow_swiper']['mousewheel']['releaseOnEdges'] = array(
    '#type' => 'checkbox',
    '#description' => t("Set to <strong>true</strong> and swiper will release mousewheel event and allow page scrolling when swiper is on edge positions (in the beginning or in the end)"),
  );
  $form['views_slideshow_swiper']['mousewheel']['invert'] = array(
    '#type' => 'checkbox',
    '#description' => t("Set to <strong>true</strong> to invert sliding direction"),
  );
  $form['views_slideshow_swiper']['mousewheel']['sensitivity'] = array(
    '#type' => 'textfield',
    '#description' => t("Multiplier of mousewheel data, allows to tweak mouse wheel sensitivity"),
  );
  $form['views_slideshow_swiper']['mousewheel']['eventsTarged'] = array(
    '#type' => 'textfield',
    '#description' => t("String with CSS selector or HTML element of the container accepting mousewheel events. By default it is swiper-container"),
  );


  // VIRTUAL is not used


  $form['views_slideshow_swiper']['hashNavigation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Hash navigation'),
    '#group' => 'views_slideshow_swiper',
  );
  $form['views_slideshow_swiper']['hashNavigation']['enabled'] = array(
    '#type' => 'checkbox',
  );
  $form['views_slideshow_swiper']['hashNavigation']['watchState'] = array(
    '#type' => 'checkbox',
    '#description' => t("Set to <strong>true</strong> to enable also navigation through slides (when hashnav is enabled) by browser history or by setting directly hash on document location"),
  );
  $form['views_slideshow_swiper']['hashNavigation']['replaceState'] = array(
    '#type' => 'checkbox',
    '#description' => t("Works in addition to hashnav to replace current url state with the new one instead of adding it to history"),
  );


  $form['views_slideshow_swiper']['history'] = array(
    '#type' => 'fieldset',
    '#title' => t('History navigation'),
    '#group' => 'views_slideshow_swiper',
  );
  $form['views_slideshow_swiper']['history']['enabled'] = array(
    '#type' => 'checkbox',
  );
  $form['views_slideshow_swiper']['history']['replaceState'] = array(
    '#type' => 'checkbox',
    '#description' => t("Works in addition to hashnav or history to replace current url state with the new one instead of adding it to history"),
  );
  $form['views_slideshow_swiper']['history']['key'] = array(
    '#type' => 'textfield',
    '#description' => t("Url key for slides"),
  );


  // CONTROLLER is not used


  $form['views_slideshow_swiper']['a11y'] = array(
    '#type' => 'fieldset',
    '#title' => t('Accessibility'),
    '#group' => 'views_slideshow_swiper',
  );
  $form['views_slideshow_swiper']['a11y']['enabled'] = array(
    '#type' => 'checkbox',
  );
  $form['views_slideshow_swiper']['a11y']['prevSlideMessage'] = array(
    '#type' => 'textfield',
    '#description' => t("Message for screen readers for previous button"),
  );
  $form['views_slideshow_swiper']['a11y']['nextSlideMessage'] = array(
    '#type' => 'textfield',
    '#description' => t("Message for screen readers for next button"),
  );
  $form['views_slideshow_swiper']['a11y']['firstSlideMessage'] = array(
    '#type' => 'textfield',
    '#description' => t("Message for screen readers for previous button when swiper is on first slide"),
  );
  $form['views_slideshow_swiper']['a11y']['lastSlideMessage'] = array(
    '#type' => 'textfield',
    '#description' => t("Message for screen readers for previous button when swiper is on last slide"),
  );
  $form['views_slideshow_swiper']['a11y']['paginationBulletMessage'] = array(
    '#type' => 'textfield',
    '#description' => t("Message for screen readers for single pagination bullet"),
  );
  $form['views_slideshow_swiper']['a11y']['notificationClass'] = array(
    '#type' => 'textfield',
    '#description' => t("CSS class name of a11 notification"),
  );


  $form['views_slideshow_swiper']['on'] = array(
    '#type' => 'fieldset',
    '#title' => t('Events'),
    '#group' => 'views_slideshow_swiper',
  );
  $form['views_slideshow_swiper']['on']['init'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#description' => t("Event will be fired right after Swiper initialization."),
  );
  $form['views_slideshow_swiper']['on']['beforeDestroy'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#description' => t("Event will be fired right beforey Swiper destoryed"),
  );
  $form['views_slideshow_swiper']['on']['slideChange'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#description' => t("Event will be fired when currently active slide is changed"),
  );
  $form['views_slideshow_swiper']['on']['slideChangeTransitionStart'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#description' => t("Event will be fired in the beginning of animation to other slide (next or previous)."),
  );
  $form['views_slideshow_swiper']['on']['slideChangeTransitionEnd'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#description' => t("Event will be fired after animation to other slide (next or previous)."),
  );
  $form['views_slideshow_swiper']['on']['slideNextTransitionStart'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#description' => t("Same as \"slideChangeTransitionStart\" but for \"forward\" direction only"),
  );
  $form['views_slideshow_swiper']['on']['slideNextTransitionEnd'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#description' => t("Same as \"slideChangeTransitionEnd\" but for \"forward\" direction only"),
  );
  $form['views_slideshow_swiper']['on']['slidePrevTransitionStart'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#description' => t("Same as \"slideChangeTransitionStart\" but for \"backward\" direction only"),
  );
  $form['views_slideshow_swiper']['on']['slidePrevTransitionEnd'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#description' => t("Same as \"slideChangeTransitionEnd\" but for \"backward\" direction only"),
  );
  $form['views_slideshow_swiper']['on']['transitionStart'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#description' => t("Event will be fired in the beginning of transition."),
  );
  $form['views_slideshow_swiper']['on']['transitionEnd'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#description' => t("Event will be fired after transition."),
  );
  $form['views_slideshow_swiper']['on']['touchStart'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#arguments' => 'event',
    '#description' => t("Event will be fired when user touch Swiper. Receives 'touchstart' event as an arguments."),
  );
  $form['views_slideshow_swiper']['on']['touchMove'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#arguments' => 'event',
    '#description' => t("Event will be fired when user touch and move finger over Swiper. Receives 'touchmove' event as an arguments."),
  );
  $form['views_slideshow_swiper']['on']['touchMoveOpposite'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#arguments' => 'event',
    '#description' => t("Event will be fired when user touch and move finger over Swiper in direction opposite to <code>direction</code> parameter. Receives 'touchmove' event as an arguments."),
  );
  $form['views_slideshow_swiper']['on']['sliderMove'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#arguments' => 'event',
    '#description' => t("Event will be fired when user touch and move finger over Swiper and move it. Receives 'touchmove' event as an arguments."),
  );
  $form['views_slideshow_swiper']['on']['touchEnd'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#arguments' => 'event',
    '#description' => t("Event will be fired when user release Swiper. Receives 'touchend' event as an arguments."),
  );
  $form['views_slideshow_swiper']['on']['click'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#arguments' => 'event',
    '#description' => t("Event will be fired when user click/tap on Swiper after 300ms delay. Receives 'touchend' event as an arguments."),
  );
  $form['views_slideshow_swiper']['on']['tap'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#arguments' => 'event',
    '#description' => t("Event will be fired when user click/tap on Swiper. Receives 'touchend' event as an arguments."),
  );
  $form['views_slideshow_swiper']['on']['doubleTap'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#arguments' => 'event',
    '#description' => t("Event will be fired when user double tap on Swiper's container. Receives 'touchend' event as an arguments"),
  );
  $form['views_slideshow_swiper']['on']['imagesReady'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#description' => t("Event will be fired right after all inner images are loaded. <code>updateOnImagesReady</code> should be also enabled"),
  );
  $form['views_slideshow_swiper']['on']['progress'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#arguments' => 'progress',
    '#description' => t("Event will be fired when Swiper progress is changed, as an arguments it receives <strong>progress</strong> that is always from 0 to 1"),
  );
  $form['views_slideshow_swiper']['on']['reachBeginning'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#description' => t("Event will be fired when Swiper reach its beginning (initial position)"),
  );
  $form['views_slideshow_swiper']['on']['reachEnd'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#description' => t("Event will be fired when Swiper reach last slide"),
  );
  $form['views_slideshow_swiper']['on']['fromEdge'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#description' => t("Event will be fired when Swiper goes from <code>beginning</code> or <code>end</code> position"),
  );
  $form['views_slideshow_swiper']['on']['setTranslate'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#arguments' => 'translate',
    '#description' => t("Event will be fired when swiper's wrapper change its position. Receives current translate value as an arguments"),
  );
  $form['views_slideshow_swiper']['on']['setTransition'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#arguments' => 'transition',
    '#description' => t("Event will be fired everytime when swiper starts animation. Receives current transition duration (in ms) as an arguments"),
  );
  $form['views_slideshow_swiper']['on']['resize'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#description' => t("Event will be fired on window resize right before swiper's onresize manipulation"),
  );
  $form['views_slideshow_swiper']['on']['paginationRender'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#arguments' => 'swiper, paginationEl',
    '#description' => t("Event will be fired after pagination rendered"),
  );
  $form['views_slideshow_swiper']['on']['paginationUpdate'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#arguments' => 'swiper, paginationEl',
    '#description' => t("Event will be fired when pagination updated"),
  );
  $form['views_slideshow_swiper']['on']['autoplayStart'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#description' => t("Event will be fired in when autoplay started"),
  );
  $form['views_slideshow_swiper']['on']['autoplayStop'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#description' => t("Event will be fired when autoplay stopped"),
  );
  $form['views_slideshow_swiper']['on']['autoplay'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#description' => t("Event will be fired when slide changed with autoplay"),
  );
  $form['views_slideshow_swiper']['on']['lazyImageLoad'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#description' => t("Event will be fired in the beginning of lazy loading of image"),
  );
  $form['views_slideshow_swiper']['on']['lazyImageReady'] = array(
    '#type' => 'textarea',
    '#size' => 2,
    '#arguments' => 'slideEl, imageEl',
    '#description' => t("Event will be fired when lazy loading image will be loaded"),
  );

  foreach (element_children($form['views_slideshow_swiper']) as $fieldset_name) {
    foreach (element_children($form['views_slideshow_swiper'][$fieldset_name]) as $element_name) {
      $current = $view->options['views_slideshow_swiper'][$fieldset_name][$element_name];
      $default = $default_values[$fieldset_name][$element_name];

      $form['views_slideshow_swiper'][$fieldset_name][$element_name]['#title'] = "<code>$element_name</code>";
      $form['views_slideshow_swiper'][$fieldset_name][$element_name]['#default_value'] = is_null($current) ? $default : $current;

      if ($fieldset_name == 'on') {
        $arguments = !empty($form['views_slideshow_swiper']['on'][$element_name]['#arguments'])
          ? $form['views_slideshow_swiper']['on'][$element_name]['#arguments']
          : '';

        $form['views_slideshow_swiper']['on'][$element_name]['#title'] = "<code>swiper.on('$element_name', function($arguments) {</code>";
        $form['views_slideshow_swiper']['on'][$element_name]['#field_suffix'] = "<strong><code>});</code></strong>";
      }
      if ($element_name == 'breakpoints') {
        foreach (element_children($form['views_slideshow_swiper'][$fieldset_name][$element_name]) as $breakpoint_setting) {
          $cur = $view->options['views_slideshow_swiper'][$fieldset_name][$element_name][$breakpoint_setting];
          if (isset($cur) && is_array($cur)) {
            $cur_slides = $cur['slidesPerView'];
            $cur_per_group = $cur['slidesPerGroup'];
            $cur_space = $cur['spaceBetween'];
            $def_slides = $default_values[$fieldset_name][$element_name][$breakpoint_setting]['slidesPerView'];
            $def_per_group = $default_values[$fieldset_name][$element_name][$breakpoint_setting]['slidePerGroup'];
            $def_space = $default_values[$fieldset_name][$element_name][$breakpoint_setting]['spaceBetween'];
            $val_slides = is_null($cur_slides) ? $def_slides : $cur_slides;
            $val_per_group = is_null($cur_per_group) ? $def_per_group : $cur_per_group;
            $val_space = is_null($cur_space) ? $def_space : $cur_space;
            $form['views_slideshow_swiper'][$fieldset_name][$element_name][$breakpoint_setting]['slidesPerView']['#default_value'] = $val_slides;
            $form['views_slideshow_swiper'][$fieldset_name][$element_name][$breakpoint_setting]['slidesPerGroup']['#default_value'] = $val_per_group;
            $form['views_slideshow_swiper'][$fieldset_name][$element_name][$breakpoint_setting]['spaceBetween']['#default_value'] = $val_space;
          } else if (isset($cur)) {
            $def = $default_values[$fieldset_name][$element_name][$breakpoint_setting];
            $val = is_null($cur) ? $def : $cur;
            $form['views_slideshow_swiper'][$fieldset_name][$element_name][$breakpoint_setting]['#default_value'] = $val;
          }
        }
      }
    }
  }
}

/**
 * Implements hook_views_slideshow_options_form_validate().
 */
function views_slideshow_swiper_views_slideshow_options_form_validate(&$form, &$form_state, &$view) {
  $settings =& $form_state['values']['style_options']['views_slideshow_swiper'];
  _views_slideshow_swiper_remove_default_values($settings);

  $breakpoints_temp = NULL;
  if (isset($settings['parameters']['breakpoints'])) {
    // Breakpoints is an array, cannot do a foreach as above, so attach it back to settings after foreach.
    $breakpoints_temp = $settings['parameters']['breakpoints'];
    unset($settings['parameters']['breakpoints']);
  }
  foreach ($settings as $group => $values) {
    foreach ($values as $key => $value) {
      if (is_string($value)) {
        $value = str_ireplace("\n", ' ', $value);
        $value = trim($value);

        if (is_numeric($value)) {
          if (floatval($value) > ((int) $value)) {
            $settings[$group][$key] = (float) $value;
          } else {
            $settings[$group][$key] = (int) $value;
          }
        }
        elseif (drupal_strtolower($value) == 'true') {
          $settings[$group][$key] = TRUE;
        }
        elseif (drupal_strtolower($value) == 'false') {
          $settings[$group][$key] = FALSE;
        }
        elseif (empty($value)) {
          $settings[$group][$key] = NULL;
        }
      }
    }
  }
  // Breakpoints is an array, need to seperately do what we did abovee, attach it back to settings after foreach.
  // Avoids Invalid argument supplied for foreach().
  if (!empty($breakpoints_temp)) {
    foreach ($breakpoints_temp as $key => $value) {
      if (is_string($value)) {
        $value = str_ireplace("\n", ' ', $value);
        $value = trim($value);

        if (is_numeric($value)) {
          $breakpoints_temp[$group][$key] = (int) $value;
        }
        elseif (drupal_strtolower($value) == 'true') {
          $breakpoints_temp[$group][$key] = TRUE;
        }
        elseif (drupal_strtolower($value) == 'false') {
          $breakpoints_temp[$group][$key] = FALSE;
        }
        elseif (empty($value)) {
          $breakpoints_temp[$group][$key] = NULL;
        }
      }
    }
    $settings['parameters']['breakpoints'] = $breakpoints_temp;
  }
}

function _views_slideshow_swiper_remove_default_values(&$new_values) {
  $default_values = views_slideshow_swiper_views_slideshow_option_definition()['views_slideshow_swiper']['default'];

  foreach ($new_values as $group => $values) {
    if (!is_array($values)) {
      continue;
    }

    $exclude = array('el', 'nextEl', 'prevEl');
    foreach ($values as $key => $value) {
      if (isset($default_values[$group][$key])) {
        $default_value = $default_values[$group][$key];

        settype($value, gettype($default_value));
        if (!in_array($key, $exclude) && $value === $default_value) {
          unset($new_values[$group][$key]);
        }
      }
    }

    if (empty($new_values[$group])) {
      unset($new_values[$group]);
    }
  }

  return $new_values;
}
