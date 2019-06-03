<?php
/**
 * @file
 * Display the progress bar for multipage forms
 *
 * Available variables:
 * - $node: The webform node.
 * - $progressbar_page_number: TRUE if the actual page number should be
 *   displayed.
 * - $progressbar_percent: TRUE if the percentage complete should be displayed.
 * - $progressbar_bar: TRUE if the bar should be displayed.
 * - $progressbar_pagebreak_labels: TRUE if the page break labels shoud be
 *   displayed.
 * - $page_num: The current page number.
 * - $page_count: The total number of pages in this form.
 * - $page_labels: The labels for the pages. This typically includes a label for
 *   the starting page (index 0), each page in the form based on page break
 *   labels, and then the confirmation page (index number of pages + 1).
 * - $percent: The percentage complete.
 */
?>
<div class="webform-progressbar">
  <?php if ($progressbar_bar): ?>
            <?php for ($n = 1; $n <= $page_count; $n++): ?>
        <div class="webform-progressbar-page<?php if ($n < $page_num) { print ' completed'; }; ?><?php if ($n == $page_num) { print ' current'; }; ?>">
          <?php if ($progressbar_pagebreak_labels): ?>
          <div class="webform-progressbar-page-label">
            <?php print check_plain($page_labels[$n - 1]); ?>
          </div>
          <?php endif; ?>
        </div>
      <?php endfor; ?>
  <?php endif; ?>
</div>
