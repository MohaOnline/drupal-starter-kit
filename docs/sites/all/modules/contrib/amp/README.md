# Accelerated Mobile Pages (AMP)

## Introduction

## Requirements

* [AMP Theme](https://www.drupal.org/project/amptheme)
* [AMP PHP Library](https://github.com/Lullabot/amp-library)
* [Token](https://www.drupal.org/project/token)
* [Ctools](https://www.drupal.org/project/ctools)
* PHP version 5.5.+

## Introduction

The AMP module is designed to convert Drupal pages into pages that comply with the AMP standard. Initially only node pages will be converted. Other kinds of pages will be enabled at a later time.

When the AMP module is installed, AMP can be enabled for any node type. At that point, AMP content becomes available on URLs such as `node/1?amp` or `node/article-title?amp`. There are also special AMP formatters for text, image, and video fields.

The [AMP Theme](https://www.drupal.org/project/amptheme) is designed to produce the very specific markup that the AMP HTML standard requires. The AMP theme is triggered for any node delivered on an `?amp` path. As with any Drupal theme, the AMP theme can be extended using a subtheme, allowing publishers as much flexibility as they need to customize how AMP pages are displayed. This also makes it possible to do things like place AMP ad blocks on the AMP page using Drupal's block system.

The [AMP PHP Library](https://github.com/Lullabot/amp-library) analyzes HTML entered by users into rich text fields and reports issues that might make the HTML non-compliant with the AMP standard.  The library does its best to make corrections to the HTML, where possible, and automatically converts images and iframes into their AMP HTML equivalents. More automatic conversions will be available in the future. The PHP Library is CMS agnostic, designed so that it can be used by both the Drupal 8 and Drupal 7 versions of the Drupal module, as well as by non-Drupal PHP projects.

We have done our best to make this solution as turnkey as possible, but the module, in its current state, is not feature complete. At this point only node pages can be converted to AMP HTML. The initial module supports AMP HTML tags such as `amp-ad`, `amp-pixel`, `amp-img`, `amp-video`, `amp-analytics`, and `amp-iframe`, but we plan to add support for more of the extended components in the near future. For now the module supports Google Analytics, AdSense, and DoubleClick
for Publisher ad networks, but additional network support is forthcoming.


## Supported AMP Components

- [amp-ad](https://www.ampproject.org/docs/reference/amp-ad.html)
- [amp-pixel](https://www.ampproject.org/docs/reference/amp-pixel.html)
- [amp-img](https://www.ampproject.org/docs/reference/amp-img.html)
- [amp-video](https://www.ampproject.org/docs/reference/amp-video.html)
- [amp-analytics](https://www.ampproject.org/docs/reference/extended/amp-analytics.html)
- [amp-iframe](https://www.ampproject.org/docs/reference/extended/amp-iframe.html)

Support for additional [extended components](https://www.ampproject.org/docs/reference/extended.html) is forthcoming.


## Module Architecture Overview

The module will be responsible for the basic functionality of providing an AMP version of Drupal pages. It will:

- Create an AMP view mode, so users can identify which fields in which order should be displayed on the AMP version of a page.
- Create an AMP route, which will display the AMP view mode on an AMP path (i.e. `node/1?amp`).
- Create AMP formatters for common fields, like text, image, video, and iframe that can be used in the AMP view mode to display AMP-compatible markup for those fields.
- Create AMP ad blocks that can be placed by the theme.
- The theme can place AMP pixel items in the page markup where appropriate, based on the configuration options.
- Create an AMP configuration page where users can identify which ad and analytics systems to use, and identify which theme is the AMP theme.
- Create a way for users to identify which content types should provide AMP pages, and a way to override individual nodes to prevent them from being displayed as AMP pages (to use for odd pages that wouldn’t transform correctly).
- Create an AMP Metadata configuration page where users can provide information necessary for an AMP page to appear in Google Top Stories carousels.
- Make sure that paths that should not work as AMP pages generate 404s instead of broken pages.
- Make sure that aliased paths work correctly, so if `node/1` has an alias of `my-page`, `node/1?amp` has an alias of `my-page?amp`.
- Create a system so the user can preview the AMP page.

The body field presents a special problem, since it is likely to contain lots of invalid markup, especially embedded images, videos, tweets, and iframes. There is no easy way to convert a blob of text with invalid markup into AMP-compatible markup. At the same time, this is a common problem that other projects will run into. Our solution is to create a separate, stand-alone, [AMP PHP Library](https://github.com/Lullabot/amp-library) to transform that markup, as best it can, from non-compliant HTML to AMP HTML. The AMP formatter for the body will use that library to render the body in the AMP view mode.


## Installation with Drush
* Download the theme, module, and composer manager: `drush dl amp, amptheme, composer_manager`
* Enable Composer Manager and the AMP Theme: `drush en composer_manager, amptheme, ampsubtheme_example`
* Composer Manager writes a file to `sites/default/files/composer`
* Enable AMP: `drush en amp`
* As long as Composer Manager is enabled, the required dependencies will be added to `sites/all/vendor` as soon as you enable the AMP module
* If you don't see any dependencies download, try `drush composer-json-rebuild` followed by `drush composer-manager install` when in docroot
* Check `/admin/config/system/composer-manager` to ensure it's all green
* Tip: If you ever want to update your composer dependencies to a more recent version (while respecting versioning constraints) try `drush composer-manager update`
* Tip: See composer manager drupal documentation to understand how this all works


## Configuration
* Go to the AMP configuration screen at `/admin/config/content/amp`

### Theme
* The AMP Theme project provides an AMP Base theme that takes care of converting some of the larger parts of the page into AMP
* The AMP Theme provides a subtheme aptly named the ExAMPle theme that demonstrates how to customize the appearance of AMP pages with custom styles
* It is also possible to create your own custom subtheme with your own styles
* Make sure to configure the blocks for the AMP theme you have selected

### Content Types
* Find the list of your content types at the top
* Click the link to "Enable AMP in Custom Display Settings"
* Open "Custom Display Settings" fieldset, check AMP, click Save button (this brings you back to the AMP config form)
* Click "Configure AMP view mode"
* Set your Body field to use the `AMP text` format (and any other fields you want to configure)
* Click Save button (this brings you back to the AMP config form)

### Analytics (optional)
* Go to the AMP configuration screen at `/admin/config/content/amp/analytics`
* Enter your Google Analytics Web Property ID and click save configuration

### Adsense (optional)
* Enter your Google AdSense Publisher ID and click save
* Visit `/admin/structure/block` to configure add Adsense blocks to your layout (currently up to 4)

### DoubleClick (optional)
* Enter your Google DoubleClick for Publishers Network ID and click save
* Visit `/admin/structure/block` to configure add DoubleClick blocks to your layout (currently up to 4)

### AMP Pixel (optional)
* Check the "Enable amp-pixel" checkbox
* Fill out the domain name and query path boxes
* Click save

## AMP Metadata configuration
* Go to the AMP Metadata configuration screen at `/admin/config/content/amp/metadata`

### Organization information (required)
* Provide organization name (can use a token to use the site name) and a specially-formatted organization logo (should be 600x60).

### Content information (required)
* Below the Content Information heading, each AMP-enabled content type has an 'Edit AMP metadata settings' link.
* Follow this link, then find the AMP Metadata vertical tab.
* Ensure all fields are completed with token values appropriate for that content type.
* Some fields have character length restrictions to keep in mind. Tokens like [node:title] and [node:summary] will be automatically truncated to meet those character limits. If you want more control, you may want to create fields on your content type where editors can provide short titles and summaries.
* Take special note of the image field, as that typically varies per content type. You must provide an image field for each content type if you want that content type to appear in Top Stories listings.
* After reviewing and completing all fields, save the content type settings.
* Each AMP-enabled content types must have AMP Metadata settings reviewed and saved in order for that content type to appear in Top Stories listings.

### View AMP Metadata JSON
* After all settings are completed, view a node for an AMP-enabled content type that has content necessary for AMP Metadata (such as an image field).
* Make sure you are using the most recent version of AMP Theme.
* When you view source on that node, you should see JSON in the head section of your HTML.
* Compare the JSON with the guidelines available at https://developers.google.com/search/docs/data-types/articles.
* You can copy the script element into the Structured Data Testing tool to verify that all information meets the requirements: https://search.google.com/structured-data/testing-tool.

## Current maintainers:

- Matthew Tift - https://www.drupal.org/u/mtift
- Marc Drummond - https://www.drupal.org/u/mdrummond
- Sidharth Kshatriya - https://www.drupal.org/u/sidharth_k
