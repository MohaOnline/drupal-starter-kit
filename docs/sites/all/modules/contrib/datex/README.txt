WEBFORM, TRANSLATION
INTRODUCTION
------------

Datex is a zero-configuration, batteries-included, fire-and-forget, zero
dependency date localization and internationalization module using php-intl.
It supports Gregorian (doh!), Persian, and... bundled with a nice jquery date
picker.

It uses PHP-Intl but works without it too. To get popup support (for date
fields, views exposed forms, scheduler module, node edit form and ...), just enable
the datex_popup.


INSTALLATION
------------

  - Download and enabled datex as usual.
  - Fin.

Optionally (NOT required):
  - Enable locale module in the core.
  - Go to admin/config/regional/date-time/datex and configure schemas.
  - If you get wrong <i>time</i> values, set your site's timezone properly.
  - To get better support for views, enable date_views in date module.

JQUERY LIBRARY - POPUP DATEPICKER
--------------

It is not required to download any library, datex comes bundles with the great
calendar developed by (Babakhani)[https://github.com/babakhani/pwt.datepicker]
<b>BUT</b> It requires
(Jquery Update Module)[https://drupal.org/project/jquery_update] or a recent
version of jQuery loaded into the page.


FEATURES
----------

 - <b>Popup:</b> for date fields, node edit form, scheduler, views exposed form, ...
 - <b>Views Exposed Filters</b> works just fine as long as the date field works.
   node created is supported but popup for it is underway.
 - <b>Views Contextual Filter:</b> <i>node created date</i> can be set as a
 - <b>Date - Views Contextual Filter:</b> Support of date field
    contextual filters is fully implemented.
 - <b>Node/Comment</b> node and comment edit / add form are fully supported.
 - <b>Scheduler Module</b> is fully supported, with and without popup.
 - <b>Node admin page</b> is fully localized.
 - <b>Smaller Granularities:</b> date fields with granularity lesser than
   year-month-day (including year only or year and month only) are supported.
   Great care has been taken to support this without time offset drift.
   contextual filter. <b>year</b> and <b>year and month</b> are supported. more
   support is underway.
 - There is no need to patch the core.
 - Intl-fallback: in case php-intl is missing a fallback calendar will be used.
 - Easy admin interface, with no footprint in the database.


FEATURE REQUESTS
----------------

Datex has a very clean readable code base, so if you wish to have something
added to datex, feel free to create a pull request.

