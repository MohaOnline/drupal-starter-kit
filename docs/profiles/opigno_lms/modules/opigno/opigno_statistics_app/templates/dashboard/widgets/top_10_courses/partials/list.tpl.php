<?php
/**
 * @file
 * Opigno statistics app - Dashboard - Top 10 courses list template file
 *
 * @param array $top_10_courses
 */
?>
<table>
  <tr>
    <th class="center"><?php print t('#'); ?></th>
    <th><?php print t('Courses'); ?></th>
    <th class="center"><?php print t('Number of visits'); ?></th>
    <th class="center"><?php print t('Number of users'); ?></th>
    <th class="center"><?php print t('Number passed'); ?></th>
    <th class="center"><?php print t('Action'); ?></th>
  </tr>
  <?php foreach($top_10_courses as $index => $course) print theme('opigno_statistics_app_dashboard_widget_top_10_courses_list_item', compact('index', 'course')); ?>
</table>