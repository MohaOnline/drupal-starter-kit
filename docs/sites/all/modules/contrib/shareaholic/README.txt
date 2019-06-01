Welcome to Shareaholic for Drupal

== Installing Shareaholic ==

Copy the 'shareaholic' module directory in to your Drupal sites/all/modules directory as usual.

Navigate to administer >> build >> modules. Enable Shareaholic.

== For Help & Support ==

Visit https://support.shareaholic.com

== Changelog ==

= 7.x-3.31 =
* Server Side Share Count service upgrades
* Various Admin section cleanup
** Fixed footer links
** Updated Chat widget
** New Admin Header
* New: WeChat Share and Follow Buttons! For Social Sharing, on desktop, users scan a QR code to share in the WeChat mobile app. On mobile, users copy the link and tap “Open WeChat” to share. https://www.shareaholic.com/blog/introducing-the-wechat-social-media-buttons/

= 7.x-3.30 =
* Performance Enhancement: Settings file is now loaded from a globally distributed CDN, which will make Shareaholic load faster on your site and for your visitors
* Performance Enhancement: Added support for DNS Prefetch and Preload which will make Shareaholic EVEN FASTER!!

= 7.x-3.29 =
* THIS IS A MAJOR UPDATE! Highly recommended upgrade.
* Many performance updates!
* Enhancement: Better Cloudflare compatibility
* Enhancements: Dramatically improved image selection logic
* Enhancement: Exclude Base64 images from being set as Shareaholic and Open Graph images
* Various meta tag bug fixes
* Bugfix: We’ve further improved the server connectivity check. If you’ve been getting the ‘retry’ error message after installing Shareaholic, this one is for you.

= 7.x-3.28 =
* Bugfix: Updating social counts library to be php7 compatible

= 7.x-3.27 =
* Bugfix: Removed Delicious count call from server-side connectivity check

= 7.x-3.26 =
* Bugfix: Update to fix W3C HTML5 validation

= 7.x-3.25 =
* Bugfix: removed deprecated Twitter tweet count API call.

= 7.x-3.24 =
* Enhancement: updated Shareaholic JavaScript snippet - now a lot simpler!

= 7.x-3.23 =
* Enhancement: updated the reset plugin routine to not create new key

= 7.x-3.22 =
* Bugfix: fixed issue with disabling server side share counts option

= 7.x-3.21 =
* Bugfix: removed code causing new node content to not appear until cache flush

= 7.x-3.20 =
* Enhancement: added support for Yummly and Fancy server-side share counts

= 7.x-3.19 =
* Bugfix: prevent Share count calls on non-public pages

= 7.x-3.18 =
* Enhancement: Added Shareaholic navigation bar to the Shareaholic settings pages

= 7.x-3.17 =
* Enhancement: Modified curl multi to conserve on cpu usage for server side share counts
* Enhancement: Added Google API key to authenticate API calls for Google Plus share counts for improved the reliability

= 7.x-3.16 =
* New Feature: Additional settings for configuring share buttons and related and promoted content
* New Feature: Configure monetization settings from affiliate linking to post share ads

= 7.x-3.15 =
* New Feature: Now you can earn revenue from your existing product links with zero additional effort.
* New Feature: Added support for Shareaholic ads.

= 7.x-3.14 =
* Bugfix: Fixed issue with querying for non-existent database tables
* New Feature: clear FB cache when user creates and/or updates an piece of content

= 7.x-3.13 =
* New Feature: Added footer navigation for more information on Shareaholic products
* Bugfix: Prevent share counts from being called on private pages

= 7.x-3.12 =
* New Feature: display Site ID for better debugging and cross referencing
* Bugfix: Removed an unneeded conditional check from the server side Share Counts API to make it more reliable
* Bugfix: Removed modal from blocking the advanced settings page

= 7.x-3.11 =
* New Feature: Share Counts for Google+, Reddit, StumbleUpon, VK, Buffer, etc! This release features an optional and all new server side Share Counts API. Toggle this option under the "Advanced Settings" section.
* Major performance upgrade and speed boost! Your pages will load faster for your visitors as share count lookups are now consolidated to one single HTTP request per page load (vs a call for each sharing service). The share counts are also heavily cached on both the client and server for super fast lookups and page performance.
* Bug Fix: images in the configuration page do not load properly

= 7.x-3.10 =
* Bug fix: removing regex causing share buttons to not show

= 7.x-3.9 =
* New feature: automatically generate open graph tags (includes a global setting to turn it off or a local setting to turn it off for certain nodes)
* New feature: display connectivity status to Shareaholic servers
* Other: minor wording and CSS styling changes

= 7.x-3.8 =
* Miscellaneous bug fixes and performance enhancements

= 7.x-3.7 =
* Removing packaging info from shareaholic.info
* Adding social shares API library with endpoint

= 7.x-3.6 =
* Bug fix: incorrect use of l() function: remove the slash

= 7.x-3.5=
* Bug fix: call time pass by reference in shareaholic.module
* Bug fix: links do not work if site does not use clean urls

= 7.x-3.4 =
* New feature: per page edit options to hide share buttons or recommendations
* New feature: per page edit options to exclude that content from being recommended

= 7.x-3.3 =
* New feature: newly created content will be added to Shareaholic's recommendations
* Any updates or deleted content will also notify Shareaholic's recommendations

= 7.x-3.2 =
* Bug fix: call to non-static methods (PHP strict mode)

= 7.x-3.1 =
* No major updates were introduced in this version

= 7.x-3.0 =
* Huge update! The module has been completely re-written from the ground up to be faster, simpler to use
* Choose from snazzy new Related Post themes
* Related Posts now come mobile optimized and responsive out of the box - Shareaholic automagically determines how many Related Posts to show given how much screen width it is given
* Customize your "You may also like" Related Posts text
* Choose from new Share button themes! (including vertical share buttons!)
* Addition of new shareaholic:language tag to improve related content and recommendations computation
* Advanced Settings: ability to reset your module
* Advanced Settings: ability to disable analytics

= 7.x-3.x-dev =
* Revamp module to use new and improved share buttons with recommendations

= 7.x-2.x-dev =
* Initial 7.x module dev release
