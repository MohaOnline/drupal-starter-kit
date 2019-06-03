<?php
/**
 * @file
 * Opigno statistics app - Dashboard - Top 10 classes list template file
 *
 * @param array $top_10_classes
 */
?>
<table>
  <tr>
    <th class="center"><?php print t('#'); ?></th>
    <th><?php print t('Classes'); ?></th>
    <th class="center"><?php print t('Number of visits'); ?></th>
    <th class="center"><?php print t('Number of users'); ?></th>
    <th class="center"><?php print t('Action'); ?></th>
  </tr>
  <?php foreach($top_10_classes as $index => $class) print theme('opigno_statistics_app_dashboard_widget_top_10_classes_list_item', compact('index', 'class')); ?>
</table>