<?php
/**
 * @file
 * Opigno Learning Record Store stats - Course content - General statistics template file
 *
 * @param array $general_statistics
 *  $general_statistics['total_number_of_page_view']
 *  $general_statistics['total_number_of_active_users']
 *  $general_statistics['total_number_of_activities']
 */
?>
<div class="lrs-stats-widget clearfix" id="lrs-stats-widget-course-content-general-statistics">
  <h2 class="pull-left"><?php print t('General statistics'); ?></h2>
  <div class="pull-right">
    <?php
      $form = drupal_get_form('lrs_stats_course_content_general_statistics_form');
      print drupal_render($form);
    ?>
  </div>
  <table>
    <tr>
      <th class="center"><?php print t('Total number of page views'); ?></th>
      <th class="center"><?php print t('Total number of active users'); ?></th>
      <th class="center"><?php print t('Total number of activities'); ?></th>
      <th class="center"><?php print t('Avg score'); ?></th>
    </tr>
    <tr>
      <td class="center"><?php print $general_statistics['total_number_of_page_view']; ?></td>
      <td class="center"><?php print $general_statistics['total_number_of_active_users']; ?></td>
      <td class="center"><?php print $general_statistics['total_number_of_activities']; ?></td>
      <td class="center"><?php print $general_statistics['avg_score'] . '%'; ?></td>
    </tr>
  </table>
</div>