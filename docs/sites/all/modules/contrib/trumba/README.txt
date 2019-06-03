CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Maintainers


INTRODUCTION
------------

The Trumba module provides a way to place Trumba calendar elements, referred to
by Trumba as "spuds", on a panels/panelized page.

 * For a full description of the module visit:
   https://www.drupal.org/project/trumba

 * To submit bug reports and feature suggestions, or to track changes visit:
   https://www.drupal.org/project/issues/trumba


REQUIREMENTS
------------

This module requires the following outside of Drupal core:

 * Chaos tool suite (ctools) - https://www.drupal.org/project/ctools
 * Panels - https://www.drupal.org/project/panels
 * Trumba account - https://www.trumba.com/t.aspx?z=SignIn


INSTALLATION
------------

Install the Trumba module as you would normally install a contributed Drupal
module. Visit https://www.drupal.org/node/895232 for further information.


CONFIGURATION
-------------

    1. Navigate to Administration > Modules and enable the module and its
       dependencies.
    2. Navigate to Administration > Configuration > System > Trumba to set
       the Default Web Name. This will pre-populate this setting in the panels
       settings forms.
    3. Go to a panels page or a panelized piece of content and Add Content. The
       various spud types are located in the Calendar section.
    4. Select the desired spud type (defined below), fill in the fields based on
       your settings for your calendar and spud in Trumba. Save.


Definition of Spud Types:

 * Main Calendar Spud: For use in the main content area of a page instead of
   sidebars, etc. This is basically just the main calendar for your Trumba
   web name.

 * Promo and Control Spud: Provides a promotional or control type of Trumba
   Spud. Examples of these are the Date Finder, Filter by Category and Day
   Summary spuds.

 * Open Spud: Provides an arbitrary type of Trumba Spud. Useful if the type you
   have is not in he pre-defined promo and control spuds.


MAINTAINERS
-----------

 * Carson Black (carsonblack) - https://www.drupal.org/u/carsonblack
