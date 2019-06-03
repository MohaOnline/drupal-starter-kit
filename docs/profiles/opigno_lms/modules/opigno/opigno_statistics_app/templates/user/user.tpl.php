<?php
/**
 * @file
 * User template file
 *
 * @param array $general_informations
 * @param array $badges_earned
 * @param array $total_number_of_page_view
 * @param array $users_results
 */

 // print theme('opigno_statistics_app_user_widget_total_number_of_page_view', compact('total_number_of_page_view'));
?>
<div id="opigno-statistics-app-user">
  <div class="clearfix">
    <?php print theme('opigno_statistics_app_user_widget_general_informations', compact('general_informations')); ?>
  </div>
  <div class="separator mt-5"></div>
  <div class="clearfix">
    <?php print theme('opigno_statistics_app_user_widget_badges_earned', compact('badges_earned')); ?>
  </div>
  <div class="separator mt-5"></div>
  <div class="clearfix">
    <?php print theme('opigno_statistics_app_user_widget_courses_results', compact('courses_results')); ?>
  </div>
  <div class="separator mt-5"></div>
</div>
