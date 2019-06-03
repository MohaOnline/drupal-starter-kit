<?php
/**
 * @file
 * Opigno statistics app - Course - Students results template file
 *
 * @param array $students_results
 */
?>
<div class="opigno-statistics-app-widget" id="opigno-statistics-app-course-widget-students-results">
  <h2><?php print t('Students results'); ?></h2>
  <?php print theme('opigno_statistics_app_course_widget_students_results_list', compact('students_results')); ?>
</div>