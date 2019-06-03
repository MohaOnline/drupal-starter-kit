<?php
/**
 * @file
 * Opigno Learning Record Store stats - Dashboard - Top 10 viewed page template file
 *
 * @param array $top_10_viewed_pages
 */
?>
<div class="lrs-stats-widget col col-3-out-of-6" id="lrs-stats-widget-dashboard-top-10-most-viewed-pages">
  <h2><?php print t('Top 10 most viewed pages'); ?></h2>
  <?php print theme('opigno_lrs_stats_dashboard_widget_top_10_viewed_pages_list', compact('top_10_viewed_pages')); ?>
</div>