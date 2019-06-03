/**
 * @file
 * Configures Raven.js with the public DSN, options and context.
 */

(function (Drupal, Raven) {

  'use strict';

  Raven.config(Drupal.settings.raven.dsn, Drupal.settings.raven.options).install();
  Raven.setUserContext(Drupal.settings.raven.user);

})(Drupal, window.Raven);
