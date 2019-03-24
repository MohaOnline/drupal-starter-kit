<?php

/**
 * @mainpage UIkit
 *
 * Welcome to the UIkit developer's documentation. Newcomers to Drupal 7 theme
 * development should review the
 * @link https://api.drupal.org/api/drupal/modules%21system%21theme.api.php/group/themeable/7.x Default theme implementations @endlink
 * API reference and
 * @link https://www.drupal.org/theming Theming and Front End Development with Drupal @endlink
 * before reading the heavily-documented UIkit topics below.
 *
 * @section about-uikit About UIkit
 * UIkit is a lightweight and modular front-end framework for developing fast
 * and powerful web interfaces. This API site contains documentation for the
 * UIkit Drupal 7 theme. To view the official UIkit framework documentation,
 * please see @link https://getuikit.com getuikit.com @endlink.
 *
 * UIkit gives you a comprehensive collection of HTML, CSS, and JS components
 * with configurable options built-in. This collection of small, responsive
 * components using consistent and conflict-free nameing conventions gives you
 * fine-grain control over the look and feel without conflicting with other
 * frameworks.
 *
 * With a mobile-first approach UIkit provides a consistent experience from
 * phones and tablets to desktops, without the need to worry about device sizes
 * and breakpoints.
 *
 * You can try out a demonstration of any release of UIkit by visiting
 * @link https://simplytest.me/project/uikit/7.x-3.0-rc3 simplytest.me @endlink.
 *
 * @section uikit-api UIkit API Topics
 * Here are some topics to help you get started developing with UIkit 7:
 * - @link getting_started Getting started with UIkit @endlink
 * - @link sub_theme Creating a UIkit sub-theme @endlink
 * - @link theme_settings UIkit theme settings @endlink
 * - @link uikit_themeable UIkit theme implementations @endlink
 * - @link /project-maintainers Project maintainers @endlink
 */

/**
 * @defgroup getting_started Getting started with UIkit
 * @{
 * @section basic-setup Basic setup and structure of UIkit 7
 * UIkit 7 does not come with the required UIkit framework files because, in
 * general,
 * @link https://www.drupal.org/node/422996 3rd party libraries and content are forbidden @endlink
 * from being committed to a reporsitory for projects hosted on
 * @link drupal.org drupal.org @endlink. We instead use
 * @link https://cdnjs.com cdnjs.com @endlink to retrieve the
 * @link https://cdnjs.com/libraries/uikit UIkit library @endlink.
 *
 * This also makes the footprint of our repository smaller. Simply follow the
 * instructions below to get started with using UIkit 7.
 *
 * @section requirements Requirements
 * The required UIkit libraries are retrieved automatically.
 *
 * @section download-uikit Download UIkit
 * First of all you need to download UIkit 7. There are three ways to do this:
 * - direct download from drupal.org project page (recommended)
 * - downloading via Drush (also recommended)
 * - cloning repository from git.drupal.org (for theme developers)
 *
 * Please read the
 * @link https://www.drupal.org/docs/7/extending-drupal/installing-themes Installing themes @endlink
 * article before installing UIkit 7. We only provide the download methods
 * below, not how to install themes.
 *
 * @subsection via-drupal-org via drupal.org (recommended for site administrators)
 * You can either visit
 * @link https://drupal.org/project/uikit drupal.org @endlink or use one of the
 * links below to download the project directly from drupal.org.
 *
 * @link https://ftp.drupal.org/files/projects/uikit-7.x-3.0-rc3.tar.gz UIkit 7.x-3.0-rc3.tar.gz @endlink
 * @link https://ftp.drupal.org/files/projects/uikit-7.x-3.0-rc3.zip UIkit 7.x-3.0-rc3.zip @endlink
 *
 * @subsection via-drush via Drush (recommended for site administrators comfortable with Drush)
 * Drush is a command line and shell scripting interface for Drupal. Use the
 * following command to download UIkit 7 with Drush.
 * @code drush dl uikit-7.x-3.0-rc3 @endcode
 *
 * Information on installing and using Drush can be found
 * @link http://www.drush.org/en/master/ here @endlink.
 *
 * @subsection via-git via git.drupal.org (recomended for theme developers only)
 * Use the following Git command to download the development release from the
 * 7.x-3.x branch. This will ensure you get the latest commited development
 * release.
 * @code git clone --branch 7.x-3.x https://git.drupal.org/project/uikit.git @endcode
 *
 * The development branch is where all new development resides and not
 * recommended for use on production sites. Work still needs done before a
 * release candidate can be released. We ask you use the
 * @link https://www.drupal.org/project/issues/uikit?categories=All issue queue @endlink
 * to report bugs, support or feature requests.
 *
 * @section recommendations Recommended Projects
 * The following modules are recommended and fully supported by UIkit 7.
 *
 * @link https://www.drupal.org/project/uikit_admin UIkit Admin @endlink:
 * An administration theme using UIkit 7 as a base theme.
 *
 * @link https://www.drupal.org/project/uikit_components UIkit Components @endlink:
 * Provides additional components and functionality to the UIkit base theme.
 *
 * Once you have finished implementing UIkit 7 into your Drupal site, take a
 * look at the @link sub_theme Creating a UIkit sub-theme @endlink to create a
 * UIkit sub-theme.
 * @}
 */

/**
 * @defgroup sub_theme Creating a UIkit sub-theme
 * @{
 * @section subtheme Create a custom theme by inheriting the UIkit 7 base theme.
 * Creating a custom theme utilizing UIkit is just like creating any other
 * theme. The only difference with creating a UIkit sub-theme is your custom
 * theme will automatically inherit all UIkit offers without having to reinvent
 * the wheel.
 *
 * @section manually Creating a sub-theme manually
 * UIkit for Drupal ships with a STARTERKIT to get you going quickly when
 * creating a UIkit sub-theme. If you're comfortable using the command line, we
 * recommend @ref drush instead.
 *
 * To get started you can copy the STARTERKIT folder in the root directory of
 * UIkit and paste it where you place your themes in your Drupal installation.
 * See
 * @link https://www.drupal.org/docs/7/extending-drupal/directory-precedence-and-multi-site-considerations Directory precedence and multi-site considerations @endlink
 * to learn where to place your themes in Drupal 7.
 *
 * The folder structure of the STARTERKIT looks like:
 * @code
 * |-css
 *  |  |-STARTERKIT.css
 *  |
 *  |-js
 *  |  |-STARTERKIT.js
 *  |
 *  |-favicon.ico
 *  |-logo.png
 *  |-screenshot.png
 *  |-STARTERKIT.info.text
 *  |-template.php
 *  |-theme-settings.php
 * @endcode
 *
 * Next you will need to replace all instances of STARTERKIT in the file names
 * and contents with your theme name. Remember to use the machine name for file
 * names and functions, i.e. "theme_name" and a human-readable name elsewhere,
 * i.e. "Theme name".
 *
 * Finally, one last change is needed in order for Drupal to recognize your new
 * sub-theme. Remove the .text extension from the theme info file, i.e.
 * "theme_name.info". We included the .text extension in STARTERKIT so Drupal
 * would not display STARTERKIT on the Appearance page.
 *
 * That's it! You are now ready to start making changes to your new sub-theme.
 * More information on customizing UIkit themes can be found in the
 * @link theme_settings UIkit theme settings @endlink topic.
 *
 * @section drush Creating a sub-theme using Drush
 * UIkit for Drupal comes equipped with an easy-to-use
 * @link http://www.drush.org/en/master/ Drush @endlink command to create a
 * sub-theme from the command line. This provides rapid development of your
 * UIkit sub-theme, creating the files necessary for you with one simple
 * command.
 *
 * The Drush command uikit-starterkit (alias uikit-sk) uses the STARTERKIT now
 * included with the project.
 *
 * @subsection uikit-sk-example Use example
 * @code drush uikit-sk machine_name "Theme name" --path=sites/default/themes --description="Awesome theme description." @endcode
 *
 * [machine_name], [--path] and [--description] are all optional; only the
 * theme name (wrapped in double-quotes) is required. Use
 * "drush uikit-sk --help" to view more detailed help information. If Drush
 * reports it cannot find the command, be sure to run "drush cc drush" to clear
 * Drush's cache.
 *
 * Once the sub-theme has been created you can begin customizing the sub-theme.
 * The file structure for the sub-theme mirrors the file structure
 * @link https://www.drupal.org/docs/7/theming/overview-of-theme-files Drupal recommends @endlink
 * to make it easy to find the files and functions you want to edit.
 *
 * @section theme-functions Theme functions
 * Theme functions are located in in template.php. We've included commonly
 * used functions to get you started.
 *
 * To learn more about what you can do with your UIkit sub-theme, read the
 * @link https://www.drupal.org/docs/7/theming Themeing Drupal 7 @endlink
 * documentation guide.
 * @}
 */

/**
 * @defgroup theme_settings UIkit theme settings
 * @{
 * @section settings Customizing UIkit 7 from the Drupal administration back-end.
 * UIkit comes with an extensive variety of theme settings to configure almost
 * all themeable aspects of your Drupal site. This topic provides a brief
 * overview of these theme settings to customize the look of your website.
 *
 * @subsection jump-to-section Jump to a section
 * - @ref mobile-settings
 * - @ref layout
 * - @ref navigations
 *
 * @section mobile-settings Mobile settings
 * Adjust the mobile layout settings to enhance your users' experience on
 * smaller devices.
 *
 * @subsection mobile-metadata Mobile Metadata
 * HTML5 has attributes that can be defined in meta elements. Here you can
 * control some of these attributes:
 * - charset: Specify the character encoding for the HTML document.
 * - x_ua_compatible IE Mode: In some cases, it might be necessary to restrict a
 *   webpage to a document mode supported by an older version of Windows
 *   Internet Explorer. Here we look at the x-ua-compatible header, which allows
 *   a webpage to be displayed as if it were viewed by an earlier version of the
 *   browser.
 *
 * @subsection viewport-metadata Viewport Metadata
 * Gives hints about the size of the initial size of the viewport. This pragma
 * is used by several mobile devices only.
 * - Device width ratio: Defines the ratio between the device width
 *   (device-width in portrait mode or device-height in landscape mode) and the
 *   viewport size. Literal device width is defined as device-width and is the
 *   recommended value. You can also specify a pixel width by selecting Other,
 *   such as 300.
 * - Device height ratio: Defines the ratio between the device height
 *   (device-height in portrait mode or device-width in landscape mode) and the
 *   viewport size. Literal device height is defined as device-height and is the
 *   recommended value. You can also specify a pixel height by selecting Other,
 *   such as 300.
 * - initial-scale: Defines the ratio between the device width (device-width in
 *   portrait mode or device-height in landscape mode) and the viewport size.
 * - maximum-scale: Defines the maximum value of the zoom; it must be greater or
 *   equal to the minimum-scale or the behavior is indeterminate.
 * - minimum-scale: Defines the minimum value of the zoom; it must be smaller or
 *   equal to the maximum-scale or the behavior is indeterminate.
 * - user-scalable: If set to no, the user is not able to zoom in the webpage.
 *   Default value is Yes.
 *
 * @section layout Layout
 * Apply our fully responsive fluid grid system and panels, common layout parts
 * like blog articles and comments and useful utility classes.
 *
 * @subsection page-layout Page Layout
 * Page layout settings are available for standard, tablet and mobile layouts,
 * allowing you to arrange the main content and sidebar regions in various ways.
 * Each layout is independent of the others, giving you many ways to present
 * your content to your users.
 *
 * Additional page layout settings:
 * - Page Container: Add the .uk-container class to the page container to give
 *   it a max-width and wrap the main content of your website. For large screens
 *   it applies a different max-width.
 * - Page Margin: Select the margin to add to the top and bottom of the page
 *   container. This is useful, for example, when using the gradient style with
 *   a centered page container and a navbar.
 *
 * @subsection region-layout Region Layout
 * Change region layout settings on a per region basis. You can currently apply
 * the following two components to each region (more will be added in the
 * future):
 * - Card
 *
 * @section navigations Navigations
 * UIkit offers different types of navigations, like navigation bars and side
 * navigations. Use breadcrumbs or a pagination to steer through articles.
 *
 * @subsection local-tasks Local Tasks
 * Configure settings for the local tasks menus.
 * - Primary tasks style: Select the style to apply to the primary local tasks.
 * - Secondary tasks style: Select the style to apply to the secondary local
 *   tasks.
 *
 * @subsection breadcrumbs Breadcrumbs
 * Configure settings for breadcrumb navigation.
 * - Display breadcrumbs: Check this box to display the breadcrumb.
 * - Display home link in breadcrumbs: Check this box to display the home link
 *   in breadcrumb trail.
 * - Display current page title in breadcrumbs: Check this box to display the
 *   current page title in breadcrumb trail.
 * @}
 */

/**
 * @defgroup uikit_themeable UIkit theme implementations
 * @{
 * @section implementations Functions and templates for the user interface to be implemented by UIkit 7.
 * Drupal's default template renderer is a simple PHP parsing engine that
 * includes the template and stores the output. Drupal's theme engines
 * can provide alternate template engines, such as XTemplate, Smarty and
 * PHPTal. The most common template engine is PHPTemplate (included with
 * Drupal and implemented in phptemplate.engine, which uses Drupal's default
 * template renderer. This is the template engine utilized by UIkit.
 *
 * UIkit implements hook overrides by the use of template files and an include
 * file, which are used to override the default implementations provided by
 * Drupal. The folder structure of UIkit helps identify whether the template is
 * overriding a default template or theme hook:
 * - templates: Overrides default templates
 * - includes/theme.inc: Overrides default theme hooks
 *
 * The templates folder is further divided into the modules which provided the
 * default template. This structure will make it easier to find a template file
 * during development of a sub-theme.
 * @}
 */
