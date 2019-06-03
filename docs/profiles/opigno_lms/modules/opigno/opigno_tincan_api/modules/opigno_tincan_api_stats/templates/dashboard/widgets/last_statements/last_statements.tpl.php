<?php
/**
 * @file
 * Opigno Learning Record Store stats - Dashboard - Last statements template file
 *
 * @param array $last_statements
 */
?>
<div class="lrs-stats-widget col col-3-out-of-6" id="lrs-stats-widget-dashboard-last-statements">
  <h2><?php print t('Last statements'); ?></h2>
  <?php print theme('opigno_lrs_stats_statements_list', array('statements' => $last_statements)); ?>
</div>