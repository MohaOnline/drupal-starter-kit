<?php
/**
 * @file
 * Opigno Learning Record Store stats - Dashboard - Top 10 viewed page list template file
 *
 * @param array $top_10_viewed_pages
 */
?>
<table>
  <tr>
    <th class="center"><?php print t('#'); ?></th>
    <th><?php print t('Page'); ?></th>
    <th class="center"><?php print t('Number of visits'); ?></th>
    <th class="center"><?php print t('Number of users'); ?></th>
  </tr>
  <?php foreach($top_10_viewed_pages as $index => $page) print theme('opigno_lrs_stats_dashboard_widget_top_10_viewed_pages_list_item', compact('index', 'page')); ?>
</table>