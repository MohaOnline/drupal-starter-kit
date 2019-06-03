<?php
/**
 * @file
 * Opigno Learning Record Store stats - Dashboard - Most active users template file
 *
 * @param array $most_active_users
 */
?>
<div class="lrs-stats-widget col col-3-out-of-6" id="lrs-stats-widget-dashboard-most-active-users">
  <h2><?php print t('Most active users'); ?></h2>
  <?php print theme('opigno_lrs_stats_dashboard_widget_most_active_users_list', compact('most_active_users')); ?>
</div>