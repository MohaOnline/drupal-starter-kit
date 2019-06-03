<?php
/**
 * @file
 * Opigno statistics app - Class - Students results list template file
 *
 * @param array $students_results
 */
?>
<table>
  <tr>
    <th><?php print t('Students'); ?></th>
    <th><?php print t('Interactions'); ?></th>
    <th><?php print t('Avg interactions'); ?></th>
    <th><?php print t('Avg score'); ?></th>
    <th><?php print t('General avg score'); ?></th>
    <th><?php print t('Passed'); ?></th>
    <th><?php print t('Action'); ?></th>
  </tr>
  <?php foreach($students_results as $student_result) print theme('opigno_statistics_app_class_widget_students_results_list_item', compact('student_result')); ?>
</table>