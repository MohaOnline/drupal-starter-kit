<?php
/**
 * @file
 * Opigno statistics app - Course - Course lessons list item template file
 *
 * @param array $course_lesson
 *  $course_lesson['lesson_name']
 *  $course_lesson['number_of_interactions']
 *  $course_lesson['avg_number_of_interactions']
 *  $course_lesson['score']
 *  $course_lesson['avg_score']
 */
?>
<tr>
  <td><?php print $course_lesson['lesson_name']; ?></td>
  <td><?php print $course_lesson['number_of_interactions']; ?></td>
  <td><?php print $course_lesson['score'] . '%'; ?></td>
</tr>