Please read this file - they contain answers to many common questions.

** Description:
Module to allow administrators/developers pick if an image should be loaded
normally or lazy-loaded in the 'Manage display', choose the animation when
images appear on window scroll, the duration and speed up the website avoiding
massive images rendering.

** Benefits:
Speed up the loading time - images are not rendered when a page loads.
Cool animation/effects when images appear.

** Installation:
- Unpack the Image Lazy Loader folder and contents in the appropriate modules
directory of your Drupal installation. This is probably sites/all/modules/
- Enable the Image Lazy Loader module in the administration tools.

** Configuration
Module uses animate.css (library of CSS animations) and includes it automatically.
If this library was loaded in the theme you can disable it within
/admin/config/media/image-lazy-loader to avoid duplicates.

** Credits:
Module developed and mantained by Omar Corbacho and Patricia Cano - frontend developers at Zoocha Ltd.
It uses the following libraries:
  - Animate.css - A cross-browser library of CSS animations
    https://daneden.github.io/animate.css/
  - Lozad.js - Lazy loads elements performantly using pure JavaScript
    https://apoorv.pro/lozad.js/
