<?php
/**
 * @file
 * Search summary template.
 *
 * Available variables:
 * - $summary: An array of summary information.
 */
?>
<div id="funnelback-summary">
  <?php print $summary['start'] ?> - <?php print $summary['end'] ?>
  search results of <?php print $summary['total'] ?>
  for <strong><?php print $summary['query']; ?></strong>
</div>
