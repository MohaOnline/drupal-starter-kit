<?php
/**
 * @file
 * Opigno Learning Record Store stats - Dashboard - Top 10 viewed page list item template file
 *
 * @param int $index
 * @param array $page
 *  $page['title']
 *  $page['href']
 *  $page['view_count']
 *  $page['user_view_count'];
 */
?>
<tr>
  <td class="center"><?php print $index+1; ?></td>
  <td><?php print l($page['title'], $page['href']); ?></td>
  <td class="center"><?php print $page['view_count']; ?></td>
  <td class="center"><?php print $page['user_view_count']; ?></td>
</tr>