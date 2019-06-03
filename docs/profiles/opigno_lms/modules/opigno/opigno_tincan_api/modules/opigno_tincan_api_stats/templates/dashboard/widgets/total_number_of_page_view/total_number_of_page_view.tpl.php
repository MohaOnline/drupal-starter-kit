<?php
/**
 * @file
 * Opigno Learning Record Store stats - Dashboard - Total number of page view template file
 */
?>
<div class="lrs-stats-widget" id="lrs-stats-widget-dashboard-number-page-view">
  <div class="lrs-stats-widget-dashboard-number-page-view-header clearfix">
    <h2 class="pull-left"><?php print t('Number of page views'); ?></h2>
    <div class="pull-right">
      <?php
        $form = drupal_get_form('opigno_lrs_stats_dashboard_total_number_of_page_view_form');
        print drupal_render($form);
      ?>
    </div>
  </div>
  <div id="lrs-stats-widget-dashboard-page-view-chart" style="height: 250px;"></div>
  <script>
    jQuery.get('dashboard/total-number-of-page-view.json', function(config){
      new Morris.Line(config);
    })
  </script>
</div>