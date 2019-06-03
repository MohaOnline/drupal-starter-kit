<?php
/**
 * @file
 * Opigno statistics app - User - Courses results list item template file
 *
 * @param array $course_result
 *  $course_result['uid']
 *  $course_result['course_nid']
 *  $course_result['course_name']
 *  $course_result['number_of_interactions']
 *  $course_result['avg_number_of_interactions']
 *  $course_result['score']
 *  $course_result['avg_score']
 *  $course_result['status']
 *
 */
?>
<tr>
  <td><?php print $course_result['course_name']; ?></td>
  <td><?php print $course_result['number_of_interactions']; ?></td>
  <td><?php print $course_result['avg_number_of_interactions']; ?></td>
  <td><?php print $course_result['score'] . '%'; ?></td>
  <td><?php print $course_result['avg_score'] . '%'; ?></td>
  <td><?php print ($course_result['status']? t('Yes') : t('No')); ?></td>
</tr>