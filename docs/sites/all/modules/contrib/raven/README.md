Raven Sentry integration for Drupal
===================================

Raven module integrates the
[Sentry PHP](https://github.com/getsentry/sentry-php) and
[Raven.js](https://github.com/getsentry/raven-js) clients for
[Sentry](https://sentry.io/) into Drupal.

[Sentry](https://sentry.io/) is a realtime event logging and aggregation
platform. It specializes in monitoring errors and extracting all the information
needed to do a proper post-mortem without any of the hassle of the standard user
feedback loop.


## Features

This module logs errors in a few ways:

* Register error handler for uncaught exceptions
* Register error handler for fatal errors
* Handle watchdog messages
* Register error handler for PHP errors (deprecated - use watchdog instead)
* Handle JavaScript exceptions via Raven.js.

You can choose which errors you want to catch by enabling
desired error handlers and selecting error levels.


## Installation for Drupal 7


### Option 1: Install Sentry PHP via composer

1. Choose a module to autoload your site's composer dependencies; for example,
   [Composer Autoloader](https://www.drupal.org/project/composer_autoloader)
   module may work well for you.
2. Install Sentry PHP via composer, for example `composer require drupal/raven`
   to install both this module and Sentry PHP, or
   `composer require sentry/sentry ^1.10.0` if you install this module manually.


### Option 2: Install Sentry PHP manually

1. Download and install [Libraries API 2](http://drupal.org/project/libraries),
   [X Autoload 5](http://drupal.org/project/xautoload) and Raven modules.
2. Then download version 1.x (not 2.x) of the
   [Sentry PHP library](https://github.com/getsentry/sentry-php/releases),
   unpack and rename the Sentry library directory to `sentry-php` and place it
   inside the `sites/all/libraries` directory.
3. Make sure the path to the library files becomes like this:
   `sites/all/libraries/sentry-php/lib/Raven/Client.php`.


## Dependencies

* The [Sentry PHP library](https://github.com/getsentry/sentry-php) version 1.x
(not 2.x) installed by composer or in `sites/all/libraries`. In the former case,
you will also need a module to autoload your composer dependencies, such as
[Composer Autoloader](https://www.drupal.org/project/composer_autoloader). In
the latter case, [Libraries API 2](http://drupal.org/project/libraries) and
[X Autoload 5](http://drupal.org/project/xautoload) modules are also required.


## Information for developers

You can attach an extra information to error reports (logged in user details,
modules versions, etc). See `raven.api.php` for examples.


## Drush integration

The `drush raven-capture-message` command sends a message to Sentry.


## Known issues

If you have code that generates thousands of PHP notices—for example processing
a large set of data, with one notice for each item—you may find that storing and
sending the errors to Sentry requires a large amount of memory and execution
time, enough to exceed your configured memory_limit and max_execution_time
settings. This could result in a stalled or failed request. The work-around for
this case would be to disable sending PHP notices to Sentry.


## Sponsors

This project was originally sponsored by [Seenta](http://seenta.ru/) and is
now sponsored by [EFF](https://www.eff.org/).
