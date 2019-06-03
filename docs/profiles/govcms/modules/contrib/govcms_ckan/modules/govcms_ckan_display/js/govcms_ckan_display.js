/**
 * govCMS CKAN Display.
 */
(function ($){

  /**
   * Display charts based on selector passed from settings.
   */
  Drupal.behaviors.tableCharts = {
    attach: function (context, settings) {

      // Only auto add if we have settings.
      if (settings.govcmsCkanDisplay === undefined || !settings.govcmsCkanDisplay.tableChartSelectors) {
        return;
      }

      // Tables to act on.
      var $tables = $(settings.govcmsCkanDisplay.tableChartSelectors.join(','), context);

      // Add tableCharts, including export stylesheets.
      $tables.once('table-charts').tableCharts({
        exportStylesheets: settings.govcmsCkanDisplay.exportStylesheets
      });

      // Add bubble chart for multiple tables mode, which is used in CKAN Views bubble style.
      // This must run after above jQuery tablecCharts to get the 'tableChartsChart.bubbleSoruces' value.
      if (tableChartsChart.hasOwnProperty('bubbleSoruces')) {
        tableChartsChart.bubble(tableChartsChart.bubbleSoruces);
      }
    }
  };

})(jQuery);
