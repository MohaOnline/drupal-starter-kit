# QUICKLINK DRUPAL MODULE

## INTRODUCTION

The Drupal Quicklink module loads the
[Quicklink library](https://github.com/GoogleChromeLabs/quicklink) and provides
a Drupal administrative interface to configure it.

## REQUIREMENTS

This module is tested on Drupal 7.

## INSTALLATION

### MANUAL INSTALLATION

1. Download the Drupal module and extract it to your modules folder.
2. Because of licensing restrictions, the Quicklink JavaScript library cannot
   be hosted on Drupal.org.

By default this module will load the Quicklink JavaScript library from a CDN at
`https://unpkg.com/quicklink@1.0.0/dist/quicklink.umd.js`.

If you place a copy of this file into your local filesystem at
`/sites/all/libraries/quicklink/quicklink.umd.js`, this module will serve the local
copy instead of the CDN copy.

## CONFIGURATION

The Quicklink module admin interface is located at
`admin/config/development/performance/quicklink`.

After enabling, the Quicklink module will work properly for most sites. The
options and descriptions within the configuration form should be
self-explanatory. However, [full documentation is available on Drupal.org](https://www.drupal.org/docs/8/modules/quicklink).

## BROWSER SUPPORT

Without polyfills, Quicklink supports:
Chrome, Firefox, and Edge.

With [Intersection Observer polyfill](https://github.com/w3c/IntersectionObserver/tree/master/polyfill):
Safari.
