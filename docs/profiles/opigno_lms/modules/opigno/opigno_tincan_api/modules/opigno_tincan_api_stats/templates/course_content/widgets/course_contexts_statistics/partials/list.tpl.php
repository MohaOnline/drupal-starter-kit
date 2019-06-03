<?php
/**
 * @file
 * Opigno Learning Record Store stats - Course content - Course contexts statistics list template file
 *
 * @param array $course_contexts_statistics
 */
?>
<table>
  <tr>
    <th><?php print t('Course'); ?></th>
    <th class="center"><?php print t('Number of visits'); ?></th>
    <th class="center"><?php print t('Number of users'); ?></th>
    <th class="center"><?php print t('Percentage of users'); ?></th>
  </tr>
  <?php foreach($course_contexts_statistics as $course_context_id => $course_context_statistics) print theme('opigno_lrs_stats_course_content_widget_course_contexts_statistics_list_item', compact('course_context_id', 'course_context_statistics')); ?>
</table>