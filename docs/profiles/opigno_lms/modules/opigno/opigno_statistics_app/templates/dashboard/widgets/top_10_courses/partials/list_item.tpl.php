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
  <td><?php print $course['group_title']; ?></td>
  <td class="center"><?php print $course['page_views']; ?></td>
  <td class="center"><?php print $course['nb_members']; ?></td>
  <td class="center"><?php print $course['number_passed']; ?></td>
  <td class="center"><?php print l(t('View statistics'), $course['stats_link']); ?></td>
</tr>