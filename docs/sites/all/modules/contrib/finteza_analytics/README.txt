CONTENTS OF THIS FILE
---------------------

* Introduction
* Requirements
* Installation
* Configuration
* FAQ


INTRODUCTION
------------

The official plugin for integrating Finteza web analytics with Drupal websites

What is Finteza?

The system features real-time web analytics:

* Tracking of visits, page views and events
* Incoming traffic quality and visitor behavior analysis
* Conversion funnels
* Intuitive interface
* No delay, no data sampling

For more information, visit
[the official Finteza website](https://www.finteza.com/).

REQUIREMENTS
------------

This module requires no modules outside of Drupal core.

To assign custom events via WYSIWYG editor, following modules are required:

 * CKEditor (https://www.drupal.org/project/ckeditor)


INSTALLATION
------------

Install as you would normally install a contributed Drupal module. Visit:
https://www.drupal.org/documentation/install/modules-themes/modules-7
for further information.


CONFIGURATION
--------------

Main module:
    1. Open Modules, scroll-down, toggle-up section Finteza Analytics
       and activate module Finteza Analytics.
    2. Open "Configuration > System > Finteza Analytics"
       and configure params of the Finteza Analytics.

Plugin for CKEditor:
    1. Open Modules, scroll-down, toggle-up section Finteza Analytics
       and activate module Finteza Analytics CKEditor Plugin.
    2. Open "Configuration > Content authoring > CKEditor",
       select the editor profile that supports HTML and click to Edit.
    3. Open "Editor appearance" and drag the button "Finteza Analytics"
       from "Available buttons" into "Current toolbar".
    4. Below editor profile configuration in the list "Plugins",
       activate "Finteza web analytics module for your website".


FAQ
-----------

= What is Finteza? =

The system features real-time web analytics. For more information,
visit [the official Finteza website](https://www.finteza.com/).

= Is it free? =

Yes, the web analytics system and the plugin are free.

= Where do I get the website ID? =

The ID will be provided to you after registration in Finteza:

* automatically during registration, in plugin settings, or
* in the platform panel

= How do I register in Finteza? =

* In plugin settings, after the plugin installation
* On the [platform website](https://www.finteza.com/en/register)

= Where do I view statistics? =

On the [Finteza panel](https://panel.finteza.com/). Log in using the email and
password specified during registration. If you forgot the password, use the
password recovery (https://panel.finteza.com/recovery) page
