<?php
/**
 * @file
 * Opigno statistics app - Class - Total number of page view template file
 *
 * @param $total_number_of_page_view
 *  $total_number_of_page_view['graph_config']
 */
?>
<div class="opigno-statistics-app-widget col col-3-out-of-4 clearfix" id="opigno-statistics-app-class-widget-number-page-view">
  <div class="opigno-statistics-app-widget-class-number-page-view-header clearfix">
    <h2><?php print t('Total number of page views'); ?></h2>
  </div>
  <div id="opigno-statistics-app-class-widget-page-view-chart" style="height: 250px;"></div>
  <script type="text/javascript">
    new Morris.Line(<?php print json_encode($total_number_of_page_view['graph_config']); ?>);
  </script>
</div>