INTRODUCTION
------------
This project provides <a href="skyfish.com">Skyfish</a> 
integration to media module.
It allows you to use any image uploaded to Skyfish via media browser. 
Chosen image form image browser are stored locally, 
that it could have full functionality as local images 
(styles, attributes and etc), 
and is automatically mapped to field or added to textarea. 
Site administrator can provide global api key and secret key, 
which will be used for all users if they won't provide their onw key and secret.


REQUIREMENTS
------------
This module requires the following module:
 * Views (https://drupal.org/project/media


 INSTALLATION
------------
 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


CONFIGURATION
-------------
 * Global key can be added at <strong>admin/config/media/media_skyfish</strong> 
 * by users who has a "Configure Global Media Skyfish settings" permission.
 * All users which has a "Configure own Media Skyfish settings" permission 
 * can create a Skyfish token here <strong>user/%uid/skyfish</strong>


MAINTAINERS
-----------
Current maintainers:
 * Andrius P. (andriuzss) - https://drupal.org/user/2938417

This project has been sponsored by:
 * Adapt A/S - https://www.drupal.org/node/1897408
