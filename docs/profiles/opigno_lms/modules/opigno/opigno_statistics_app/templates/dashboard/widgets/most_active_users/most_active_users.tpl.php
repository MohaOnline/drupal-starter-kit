<?php
/**
 * @file
 * Opigno statistics app - Dashboard - Most active users
 *
 * @param array $most_active_users
 */
?>
<div class="opigno-statistics-app-widget" id="opigno-statistics-app-dashboard-widget-most-active-users">
  <h2><?php print t('Most active users'); ?></h2>
  <?php print theme('opigno_statistics_app_dashboard_widget_most_active_users_list', compact('most_active_users')); ?>
</div>