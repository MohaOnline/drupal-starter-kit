<?php
/**
 * @file
 * Quizzes template file
 *
 * @param array $quizzes
 */
?>
<div id="lrs-stats-quizzes">
  <div class="lrs-stats-widget" id="lrs-stats-widget-quizzes-widget">
    <?php print theme('opigno_lrs_stats_quizzes_list', compact('quizzes')); ?>
  </div>
</div>