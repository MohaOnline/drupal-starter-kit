
-- SUMMARY --

Easychart provides a way to create charts based on the powerful Highcharts library (http://www.highcharts.com/products/highcharts).

This module:

* Defines a new Content Type named 'Chart' that allows you to add one or more charts to your website.
* Creates a Chart widget that you can use in your own content types.
* Integrates with the WYSIWYG module and provides an editor button that allows you to add charts to your WYSIWYG content (CKeditor and TinyMCE are supported).
* Gives you an easy interface to configure your charts through the Easychart jQuery plugin.

Attention:

Highcharts is free for personal, school or non-profit projects under the Creative Commons Attribution - Non Commercial 3.0 License.
For commercial and governmental websites and projects, you need to buy a license. (But they're absolutely worth every penny.) See http://shop.highsoft.com/highcharts.html.

-- INSTALLATION --

1. Download and install the Easychart module: https://drupal.org/project/easychart

2. If you are using drush, then run 'drush easychart-dependencies' to install the latest Highcharts and Easychart plugins.

3. If you are not using drush, download the Highcharts plugin at http://www.highcharts.com/download and place it in /sites/all/libraries/highcharts

4. If you are not using drush, download the v3 Easychart plugin at https://github.com/bestuurszaken/easychart and place it in /sites/all/libraries/easychart. The result should be /sites/all/libraries/easychart/dist/ec.full.js.


-- WYSIWYG PLUGIN ---

To use the WYSIWYG plugin, you will need to do the following:

1. Download and install the WYSIWYG module at https://drupal.org/project/wysiwyg. Follow the install instructions to add the CKeditor or TinyMCE editors.

2. Enable the Easychart WYSIWYG sub-module.

3. Enable the Easychart button:
   Go to http://YOUR_SITE/admin/config/content/wysiwyg and click edit next to the text format(s) that you want to allow a chart to be added.
   Check 'Easychart' under 'Buttons and plugins' and save.

4. Enable the Easychart filter:
   Go to http://YOUR_SITE/admin/config/content/formats and click configure next to the Text Format(s) from step 2.
   Check 'Insert Easychart charts' and save.
   Note: if you don't do this, you will see [[chart-nid:x,chart-view-mode:x]] whe viewing your node. The filter will replace that placeholder with the actual Chart.

5. Make sure that DIV tags are allowed in your  Text Format.
   Go to http://YOUR_SITE/admin/config/content/formats and click configure next to the Text Format(s) from step 2.
   If 'Limit allowed HTML tags' is checked, than add 'div' to the allowed tags.


-- NOTES WHEN USING CSV DATA URL --

In the Easychart interface, you can choose to get your date from an external url. Make sure that there are no cross-domain issues otherwise this functionality wil not work.
For performance reasons, we cache the data from the url, and only update this data when cron runs. You can overwrite the frequency by setting the variable 'easychart_url_update_frequency'.

-- VIEWS INTEGRATION --

If you want to expose data from your nodes to Easychart, you can follow these steps:

1. Download and install the Views Data Export module at https://www.drupal.org/project/views_data_export.

2. Create a View with a 'Data Export' display and 'CSV file' as Export type.

3. Set the path for that View, and use that path, including your domain name, as the 'url CSV' in the Easychart plugin.


-- POSSIBLE ISSUES --

1. I get a javascript error: 'uncaught exception: Highcharts error #13: www.highcharts.com/errors/13'
   The chart needs a div to be printed in. If your Text Format does not allow DIV's to be printed, you will get the error above, and the chart will not be printed.
   See step 4 under 'WYSIWYG PLUGIN' for the solution.

2. I see [[chart-nid:x,chart-view-mode:x]] instead of the actual chart.
   See step 3 under 'WYSIWYG PLUGIN' for the solution.

-- ACCESSIBILITY --

See https://www.drupal.org/node/2728809 on how to make your charts accessible. We have chosen not to do this by default for performance reasons.

-- CREDITS --

This project is sponsored by The Government of Flanders: http://overheid.vlaanderen.be.

The following libraries made this module possible:

1. Highcharts: http://highcharts.com/

2. screenfull.js: https://github.com/sindresorhus/screenfull.js

3. Handsontable: https://github.com/handsontable/handsontable