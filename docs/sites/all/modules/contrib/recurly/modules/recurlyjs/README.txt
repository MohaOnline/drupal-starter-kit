Description
-----------
This module provides FormAPI elements for subscriptions, updating billing
information, and one-time payments through Recurly.com.

IMPORTANT CAVEAT: Recurlyjs does not use Drupal to process sensitive
information, so it never touches your server (thus circumventing PCI compliance
requirements). This comes with the following caveats:

- Users must have JavaScript enabled in their browser to submit payment.
- Recurly.js is compatible with IE7 and higher.
- Recurly.js recommends jQuery 1.5.2 or higher. Drupal 7 comes with jQuery 1.4.4
  out of the box, however we are not aware of any issues with using Recurly.js
  with this older version of jQuery.

Requirements
------------
Drupal 7.x
Recurly
Libraries API

The recurlyjs library is dynamically included as of 7.x-2.x and no longer needs
to be downloaded.

The README.txt for the Recurly module describes the installation and
configuration for this module.

Support
-------
Please use the issue queue for filing bugs with this module at
http://drupal.org/project/issues/recurly?categories=All

WE DO NOT PROVIDE SUPPORT ON CUSTOM CODING. Please use the issue queue for
issues that are believed to be bugs or for requesting features.
