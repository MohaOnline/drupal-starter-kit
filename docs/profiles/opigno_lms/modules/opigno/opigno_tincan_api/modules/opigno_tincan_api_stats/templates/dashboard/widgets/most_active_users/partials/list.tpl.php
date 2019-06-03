<?php
/**
 * @file
 * Opigno Learning Record Store stats - Dashboard - Most active users list template file
 *
 * @param array $most_active_users
 */
?>
<table>
  <tr>
    <th class="center"><?php print t('#'); ?></th>
    <th><?php print t('Username'); ?></th>
    <th class="center"><?php print t('Number of statements'); ?></th>
  </tr>
  <?php foreach($most_active_users as $index => $user) print theme('opigno_lrs_stats_dashboard_widget_most_active_users_list_item', compact('index', 'user')); ?>
</table>