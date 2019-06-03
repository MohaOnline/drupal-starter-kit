<?php
/**
 * @file
 * Dashboard template file
 *
 * @param array $top_10_viewed_pages
 * @param array $most_active_users
 * @param array $last_statements
*/
?>
<div id="lrs-stats-dashboard">
  <?php print theme('opigno_lrs_stats_dashboard_widget_total_number_of_page_view'); ?>
  <?php print theme('opigno_lrs_stats_dashboard_widget_top_10_viewed_pages', compact('top_10_viewed_pages')); ?>
  <?php print theme('opigno_lrs_stats_dashboard_widget_most_active_users', compact('most_active_users')); ?>
  <?php print theme('opigno_lrs_stats_dashboard_widget_last_statements', compact('last_statements')); ?>
</div>