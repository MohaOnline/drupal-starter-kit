<?php
/**
 * @file
 * Opigno statistics app - User - Courses results list template file
 *
 * @param array $courses_results
 */
?>
<table>
  <tr>
    <th><?php print t('Courses'); ?></th>
    <th><?php print t('Interactions'); ?></th>
    <th><?php print t('Avg interactions'); ?></th>
    <th><?php print t('Score'); ?></th>
    <th><?php print t('Avg score'); ?></th>
    <th><?php print t('Passed'); ?></th>
  </tr>
  <?php foreach($courses_results as $course_result) print theme('opigno_statistics_app_user_widget_courses_results_list_item', compact('course_result')); ?>
</table>