<?php
/**
 * @file
 * Opigno Learning Record Store stats - Dashboard - Most active users list item template file
 *
 * @param array $user
 *    $user['username']
 *    $user['url']
 *    $user['statement_count']
 */
?>
<tr>
  <td class="center"><?php print $index+1; ?></td>
  <td><?php print l($user['username'], $user['url']); ?></td>
  <td class="center"><?php print $user['statement_count']; ?></td>
</tr>