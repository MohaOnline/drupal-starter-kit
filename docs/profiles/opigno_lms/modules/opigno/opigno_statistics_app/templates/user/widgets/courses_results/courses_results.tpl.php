<?php
/**
 * @file
 * Opigno statistics app - User - Course results template file
 *
 * @param array $course_results
 */
?>
<div class="opigno-statistics-app-widget" id="opigno-statistics-app-user-widget-courses-results">
  <h2><?php print t('Courses results'); ?></h2>
  <?php print theme('opigno_statistics_app_user_widget_courses_results_list', compact('courses_results')); ?>
</div>