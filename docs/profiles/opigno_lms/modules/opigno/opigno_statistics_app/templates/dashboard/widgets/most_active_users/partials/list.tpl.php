<?php
/**
 * @file
 * Opigno statistics app - Dashboard - Most active users list item template file
 *
 * @param array $top_10_courses
 */
?>
<div class="opigno-statistics-app-widget-dashboard-most-active-users-list">
  <?php foreach($most_active_users as $active_user) print theme('opigno_statistics_app_dashboard_widget_most_active_users_list_item', compact('index', 'active_user')); ?>
</div>
