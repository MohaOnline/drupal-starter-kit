<?php
/**
 * @file
 * course template file
 *
 * @param array $general_statistics
 * @param array $total_number_of_page_view
 * @param array $number_of_interactions
 * @param array $students_results
 */
?>
<div id="opigno-statistics-app-class">
  <?php
    $form = drupal_get_form('opigno_statistics_app_group_filter_form');
    print drupal_render($form);
  ?>
  <div class="col col-4-out-of-4 clearfix">
    <?php print theme('opigno_statistics_app_class_widget_general_statistics', compact('general_statistics')); ?>
    <?php print theme('opigno_statistics_app_class_widget_total_number_of_page_view', compact('total_number_of_page_view')); ?>
  </div>
  <div class="col col-4-out-of-4">
    <?php print theme('opigno_statistics_app_class_widget_number_of_interactions', compact('number_of_interactions')); ?>
  </div>
  <div class="col col-4-out-of-4">
    <?php print theme('opigno_statistics_app_class_widget_students_results', compact('students_results')); ?>
  </div>
</div>