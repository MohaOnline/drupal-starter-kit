<?php
/**
 * @file
 * Opigno statistics app - Dashboard - Top 10 courses template file
 *
 * @param array $top_10_courses
 */
?>
<div class="opigno-statistics-app-widget" id="opigno-statistics-app-dashboard-widget-top-10-courses">
  <h2><?php print t('Top 10 courses according to nb of interactions'); ?></h2>
  <?php print theme('opigno_statistics_app_dashboard_widget_top_10_courses_list', compact('top_10_courses')); ?>
</div>