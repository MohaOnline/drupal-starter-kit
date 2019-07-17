INTRODUCTION
------------
The TinyPNG on Upload module will send all uploaded PNG's and JPG's to the
TinyPNG optimization service using the TinyPNG file api.  TinyPNG uses smart
lossy compression techniques to reduce the file size of your image files.
By selectively decreasing the number of colors in the image, fewer bytes are
required to store the data. The effect is nearly invisible but it makes a HUGE
 difference in file size!  By optimizing an image at upload time, all
 derivative images will already be optimized making for a much lighter image
 footprint.


 * For a full description of the module, visit the project page:
   https://www.drupal.org/sandbox/thebenji/2460295

REQUIREMENTS
------------
None.

INSTALLATION
------------
 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

CONFIGURATION
-------------
You will need an API key from https://tinypng.com/developers.  You will be
 able to optimize 500 images per month for free.  Paid subscriptions also
 available.

MAINTAINERS
-----------
Current maintainers:
 * Benji Regan (TheBenji) - https://www.drupal.org/u/thebenji
