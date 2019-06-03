<?php
/**
 * @file
 * Opigno statistics app - Course - Course lessons list template file
 *
 * @param array $course_lessons
 */
?>
<table>
  <tr>
    <th><?php print t('Lesson'); ?></th>
    <th><?php print t('Interactions'); ?></th>
    <th><?php print t('Avg score'); ?></th>
  </tr>
  <?php foreach($course_lessons as $course_lesson) print theme('opigno_statistics_app_course_widget_course_lessons_list_item', compact('course_lesson')); ?>
</table>