<?php
/**
 * @file
 * Opigno statistics app - Course - Course lessons template file
 *
 * @param array $course_lessons
 */
?>
<div class="opigno-statistics-app-widget" id="opigno-statistics-app-course-widget-course-lessons">
  <h2><?php print t('Course lessons'); ?></h2>
  <?php print theme('opigno_statistics_app_course_widget_course_lessons_list', compact('course_lessons')); ?>
</div>