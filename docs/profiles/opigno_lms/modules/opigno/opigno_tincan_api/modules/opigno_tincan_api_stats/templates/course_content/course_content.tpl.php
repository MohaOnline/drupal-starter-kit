<?php
/**
 * @file
 * Opigno Learning Record Store stats - course content template file
 *
 * @param array $general_statistics
 * @param array $course_contexts_statistics
 *
 */
?>
<div id="lrs-stats-course-content">
  <?php print theme('opigno_lrs_stats_course_content_general_statistics', compact('general_statistics')); ?>
  <?php print theme('opigno_lrs_stats_course_content_course_contexts_statistics', compact('course_contexts_statistics')); ?>
</div>