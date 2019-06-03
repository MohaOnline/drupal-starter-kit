<?php
/**
 * @file
 * Opigno statistics app - User - Total number of page view template file
 *
 * @param $total_number_of_page_view
 *  $total_number_of_page_view['graph_config']
 */
?>
<div class="opigno-statistics-app-widget col col-3-out-of-4 clearfix" id="opigno-statistics-app-user-widget-number-page-view">
  <div class="opigno-statistics-app-user-widget-number-page-view-header clearfix">
    <h2><?php print t('Total number of page views'); ?></h2>
  </div>
  <?php
    $form = drupal_get_form('opigno_statistics_app_user_filter_form');
    print drupal_render($form);
  ?>
  <div id="opigno-statistics-app-user-widget-page-view-chart" style="height: 250px;"></div>
  <script type="text/javascript">
    new Morris.Line(<?php print json_encode($total_number_of_page_view['graph_config']); ?>);
  </script>
</div>