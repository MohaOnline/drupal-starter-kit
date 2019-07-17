CONTENTS OF THIS FILE
---------------------
   
 * Introduction
 * Requirements
 * Recommended modules
 * Installation
 * Configuration
 * Troubleshooting
 * Maintainers

INTRODUCTION
------------
The Views Slideshow: Swiper module bridges Views Slideshow and the external
library, Swiper, so that the touch slider is available as a slideshow type.
Swiper is a "modern mobile touch slider with hardware accelerated transitions"
that is "intended to be used in mobile websites, mobile web apps, and mobile
native/hybrid apps."

It works on iOS, "the latest Android, Windows Phone 8 and modern Desktop browsers"
with the following highlighted features:

 * Ability to customize pagination, navigation buttons, and create parallax effects.
 * Native 100% RTL support with correct layout.
 * Modern parallax transitions that can be used on any element inside of Swiper.
 * Automatically re-initilise/calculate parameters due to DOM or Swiper changes.
 * Optional image lazy loading.

 * For a full description of the module, visit the project page:
   https://drupal.org/project/views_slideshow_swiper

 * To submit bug reports and feature suggestions, or to track changes:
   https://drupal.org/project/issues/views_slideshow_swiper

REQUIREMENTS
------------
This module requires the following modules:
 * Views (https://drupal.org/project/views)
 * Views UI (https://drupal.org/project/views)
 * Views Slideshow (https://drupal.org/project/views_slideshow)
 * swiperjs (https://drupal.org/project/swiperjs)
 * Libraries API (https://drupal.org/project/libraries)

INSTALLATION
------------
 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

 * Download the latest release of the Swiper library from the following
   main or download link:

   http://idangero.us/swiper
   https://github.com/nolimits4web/Swiper/archive/v4.4.2.tar.gz

   Extract the Swiper library and rename it to Swiper (capital S) , it is case sensitive.
   Place it in your sites/all/libraries or sites/default/libraries folder.
   When in doubt, check the drupal status report page to confirm if the library is properly installed.
   Otherwise, check the javascript console for a TypeError message, if you have this, something is wrong, debug it.

CONFIGURATION
-------------
 * The Views UI module must be enabled to configure a View to use the
   slideshow type provided by this module.

TROUBLESHOOTING
---------------
 * If the Swiper slideshow type doesn't appear as an option in the Format Settings
   after Slideshow has been selected as the way to style the View, use these checks
   to identify where the issue may be:

   - Does Drupal recognise that the Swiper library has been installed? Navigate to
     the Administration => Reports => Status report page to confirm that the Swiper
     library is recognised and its version is detected.

MAINTAINERS
-----------
Current maintainers:
 * James An (jamesan) - https://drupal.org/user/322251

This project has been sponsored by:
 * OneMethod
   A Toronto-based digital design agency that has cultivated a reputation for producing
   “original and unexpected” work. Visit https://onemethod.com for more information.
