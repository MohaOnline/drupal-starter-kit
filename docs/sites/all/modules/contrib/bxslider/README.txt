BxSlider module integrates the bxSlider library (bxslider.com) with Fields.

Why bxSlider?

    Fully responsive - will adapt to any device
    Horizontal, vertical, and fade modes
    Slides can contain images, video, or HTML content
    Advanced touch / swipe support built-in
    Uses CSS transitions for slide animation (native hardware acceleration!)
    Full callback API and public methods
    Small file size, fully themed, simple to implement
    Browser support: Firefox, Chrome, Safari, iOS, Android, IE7+
    Tons of configuration options


DEPENDENCIES

 BxSlider Library
 https://github.com/stevenwanderski/bxslider-4/archive/v4.2.15.zip

 jQuery Update - https://drupal.org/project/jquery_update
    - this module is used, because BxSlider libraries require the jQuery
      library of 1.8 version or higher.

 Libraries API - https://drupal.org/project/libraries


INSTALLATION

 1. Download the library
    https://github.com/stevenwanderski/bxslider-4/archive/v4.2.15.zip

 2. Unzip and put the content of the archive to the
    sites/all/libraries/bxslider (create required directories). Only
    bxslider-4-4.2.15/dist/ directory is needed to be extracted
    to sites/all/libraries/bxslider directory.

    NOTE: the file jquery.bxslider.min.js must be accessible by the path
    sites/all/libraries/bxslider/jquery.bxslider.min.js.

 3. Download and enable this module and dependent modules.

 4. Select some content type, then select 'Manage display' and select a
    formatter "BxSlider" for required images field. Then click to the
    formatter settings for filling BxSlider settings

    Select the formatter "BxSlider - Thumbnail slider", if needed a
    carouser thumbnail pager.

    For example go to /admin/structure/types/manage/article/display ,
    select a formatter BxSlider for an Images field and click 'the gear'
    at the right side of the page for required image field.


MORE

 For development of a carouser thumbnail pager was used
 http://stackoverflow.com/questions/19326160
 /bxslider-how-is-it-possible-to-make-thumbnails-like-a-carousel

 If needed integration with Views, use BxSlider - Views slideshow integration
 (https://drupal.org/project/bxslider_views_slideshow)
