<?php
/**
 * @file
 * Opigno statistics app - Class - Students results template file
 *
 * @param array $students_results
 */
?>
<div class="opigno-statistics-app-widget" id="opigno-statistics-app-class-widget-students-results">
  <h2><?php print t('Students results'); ?></h2>
  <?php print theme('opigno_statistics_app_class_widget_students_results_list', compact('students_results')); ?>
</div>