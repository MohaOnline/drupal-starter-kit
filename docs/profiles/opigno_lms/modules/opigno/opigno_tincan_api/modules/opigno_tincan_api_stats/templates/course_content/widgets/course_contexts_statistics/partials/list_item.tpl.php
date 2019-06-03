<?php
/**
 * @file
 * Opigno Learning Record Store stats - Course content - Course contexts statistics list item template file
 *
 * @param int $course_context_id
 * @param array $course_context_statistics
 *  $course_context_statistics['number_of_visit']
 *  $course_context_statistics['number_of_users']
 *  $course_context_statistics['percentage_of_users']
 */
?>
<tr>
  <td><?php print $course_context_statistics['title']; ?></td>
  <td class="center"><?php print $course_context_statistics['number_of_visit']; ?></td>
  <td class="center"><?php print $course_context_statistics['number_of_users']; ?></td>
  <td class="center"><?php print $course_context_statistics['percentage_of_users']; ?>%</td>
</tr>