<?php
/**
 * @file
 * Opigno statistics app - Dashboard - Top 10 classes template file
 *
 * @param array $top_10_classes
 */
?>
<div class="opigno-statistics-app-widget" id="opigno-statistics-app-dashboard-widget-top-10-classes">
  <h2><?php print t('Top 10 classes according to nb of interactions'); ?></h2>
  <?php print theme('opigno_statistics_app_dashboard_widget_top_10_classes_list', compact('top_10_classes')); ?>
</div>