WxT Webform
===========

Provides Webform functionality in [Drupal WxT][drupalwxt].

Key Features
------------

* Creates a [Webform][webform] content type for basic form management.
* Multilingual support via [Webform Localization][webform_localization]
* Webform NID replaced with UUID via [Webform UUID][webform_uuid]
* Can be easily integrated with other content types.

Installation
------------
You must enable the Webform, Webform Localization, Webform UUID modules to install Wetkit Webform.
Either <i>Drush en</i> each module in turn, or use the Modules admin page (admin/modules) to enable Wetkit Webform and confirm Drupal should enable the other modules as well.


<b>(Optional) Further Integration Possible:</b>
------------
1 - Add webforms to the Basic Page content type(s) (/admin/structure/types/manage/wetkit-page)
 * Edit the <i>Webform</i> option, ensure <i>Enable webform functionality</i> is checked, then click <b>Save content type</b>.
 * Edit the Page called <i>node_view</i> (/admin/structure/pages/edit/node_view).
   * Edit the Page variant called <i>Node (WxT)</i>, and open the sub-section for <b>Content</b>. (/admin/structure/pages/nojs/operation/node_view/handlers/node_view_panel_context_4/content).
   * Use the gears widget in the top left corner of the Content region and choose <b>Add content</b>.
   * Select <i>Page Content</i> then click <b>Add</b> beside the <i>Webform</i> element.
   * Accept all of the default settings and click <b>Save</b>.
   * Save the updated Page settings by clicking on <b>Update and Save</b>.

2 - Repeat the steps for the Page variant called <i>Node</i> (/admin/structure/pages/nojs/operation/node_view/handlers/node_view_panel_context_3/content).
   * Use the gears widget in the top left corner of the Content region and choose <b>Add content</b>.
   * Select <i>Page Content</i> then click <b>Add</b> beside the <i>Webform</i> element.
   * Accept all of the default settings and click <b>Save</b>.
   * Save the updated Page settings by clicking on <b>Update and Save</b>.

<!-- Links Referenced -->

[drupalwxt]:               http://www.drupal.org/project/wetkit
[webform]:                 http://www.drupal.org/project/webform
[webform_uuid]:            http://www.drupal.org/project/webform_uuid
[webform_localization]:    http://www.drupal.org/project/webform_localization
