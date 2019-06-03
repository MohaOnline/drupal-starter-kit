<?php
/**
 * @file
 * Opigno statistics app - Dashboard - Top 10 courses list item template file
 *
 * @param int $index
 * @param array $course
 *  $course['nid']
 *  $course['title']
 *  $course['number_of_visits']
 *  $course['number_of_users']
 *  $course['number_passed']
 */
?>
<tr>
  <td class="center"><?php print $index+1; ?></td>
  <td><?php print $class['group_title']; ?></td>
  <td class="center"><?php print $class['page_views']; ?></td>
  <td class="center"><?php print $class['nb_members']; ?></td>
  <td class="center"><?php print l(t('View statistics'), $class['stats_link']); ?></td>
</tr>