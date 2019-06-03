INTRODUCTION
------------
This module registers a CKEditor plugin, PerformX OpenAccess, with the Wysiwyg
module so that that CKEditor users who may have access to the plugin.

 * For a full description of the module, visit the project page:
   https://www.drupal.org/sandbox/yangli0516/2495451

 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/2495451


REQUIREMENTS
------------
This module requires the Wysiwyg module(https://www.drupal.org/project/wysiwyg)
and only works with CKEditor(http://ckeditor.com/). It also requires
PerformX OpenAccess(http://ckeditor.com/addon/performx) plugin installed as an
external library. The supported versions of CKEditor currently are 3.6 - 4.2.


INSTALLATION
------------
 * Follows the instruction provided with the Wysiwyg module
   (https://www.drupal.org/project/wysiwyg) to install both Wysiwyg and CKEditor
   on your site.

 * Install and enable the OpenAccess module.
 
 * Download the latest version of PerformX OpenAccess from
   http://ckeditor.com/addon/performx, and extract the zip file to
   sites/all/libraries. Make sure the plugin files are sitting under
   sites/all/libraries/performx (eg. the path of the file plugin.js should be
   sites/all/libraries/performx/plugin.js).

 * If the Libraries API module is installed, the plugin can also be placed under 
   profiles/[yourprofilename]/libraries or sites/example.com/libraries


CONFIGURATION
-------------
To add the PerformX OpenAccess plugin buttons:

 * Go to http://[your site]/admin/config/content/wysiwyg and edit the relevant
   Wysiwyg profile.

 * Under "Buttons and plugins" section, check "OpenAccess Template", "OpenAccess
   Table" and "OpenAccess Accessibility Checker" and save the change.
