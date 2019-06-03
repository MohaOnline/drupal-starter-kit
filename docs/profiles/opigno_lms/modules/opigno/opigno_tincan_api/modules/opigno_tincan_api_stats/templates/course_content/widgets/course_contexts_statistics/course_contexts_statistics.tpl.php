<?php
/**
 * @file
 * Opigno Learning Record Store stats - Course content - Course contexts statistics template file
 *
 * @param array $course_contexts_statistics
 */
?>
<div class="lrs-stats-widget" id="lrs-stats-widget-course-content-course-contexts-statistics">
  <h2><?php print t('Course context statistics'); ?></h2>
  <?php print theme('opigno_lrs_stats_course_content_widget_course_contexts_statistics_list', compact('course_contexts_statistics')); ?>
</div>