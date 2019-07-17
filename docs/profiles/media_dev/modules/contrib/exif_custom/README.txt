EXIF Custom
-----------
This module allows images to be parsed for EXIF, XMP and IPTC meta data and have
it saved as fields on the image record.


Features
--------------------------------------------------------------------------------
Supports the following image meta data:
* EXIF, using PHP's exif_read_data() function.
* XMP, using custom logic.
* IPTC, using PHP's getimagesize() and iptcparse() functions.

Also included is EXIF Data Display, a sub module for display all of an image's
meta data on the image edit form.


Configuration
--------------------------------------------------------------------------------



Credits / contact
--------------------------------------------------------------------------------
Originally written by edwbaker [1], currently maintained by Joestp Olstad [2]
and Damien McKenna [3].

The best way to contact the authors is to submit an issue, be it a support
request, a feature request or a bug report, in the project issue queue:
  https://www.drupal.org/project/issues/exif_custom


References
--------------------------------------------------------------------------------
1: https://www.drupal.org/u/edwbaker
2: https://www.drupal.org/u/josepholstad
3: https://www.drupal.org/u/damienmckenna
