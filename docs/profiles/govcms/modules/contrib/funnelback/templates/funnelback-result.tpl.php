<?php
/**
 * @file
 * Single search result template.
 *
 * Available variables:
 * - $display_url: The url string for display.
 * - $live_url: The live url string.
 * - $title: The result title string.
 * - $date: The result date string.
 * - $summary: The result summary string.
 * - $metadata: An array of additional metadata with the result.
 */
?>
<h3>
  <a href="<?php print $display_url ?>" title="<?php print $live_url ?>"><?php print $title ?></a>
</h3>
<p>
  <?php if ($date != 'No Date'): ?>
    <span class="date"><?php print format_date($date/1000); ?></span>
  <?php endif; ?>
  <span class="summary"><?php print $summary; ?></span></p>
</p>
