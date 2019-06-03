<?php
/**
 * @file
 * Dashboard template file
 *
 * @param array $general_statistics
 * @param array $total_number_of_page_view
 * @param array $active_users_last_week
 * @param array $most_active_users
 * @param array $top_10_courses
 */
?>
<div id="opigno-statistics-app-dashboard">
  <?php
    $form = drupal_get_form('opigno_statistics_app_dashboard_filter_form');
    print drupal_render($form);
  ?>
  <div class="col col-3-out-of-4 clearfix">
    <?php print theme('opigno_statistics_app_dashboard_widget_general_statistics', compact('general_statistics')); ?>
    <?php print theme('opigno_statistics_app_dashboard_widget_total_number_of_page_view', compact('total_number_of_page_view')); ?>
  </div>
  <div class="col col-1-out-of-4 clearfix">
    <?php print theme('opigno_statistics_app_dashboard_widget_active_users_last_week', compact('active_users_last_week')); ?>
    <?php print theme('opigno_statistics_app_dashboard_widget_most_active_users', compact('most_active_users')); ?>
  </div>
  <div class="col col-4-out-of-4">
    <?php print theme('opigno_statistics_app_dashboard_widget_top_10_courses', compact('top_10_courses')); ?>
  </div>
  <div class="col col-4-out-of-4">
    <?php print theme('opigno_statistics_app_dashboard_widget_top_10_classes', compact('top_10_classes')); ?>
  </div>
</div>