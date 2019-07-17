<?php

$prevEl = $variables['prevEl'];
$nextEl = $variables['nextEl'];
$paginationEl = $variables['paginationEl'];
/**
 * @file
 * Modified from the Views Slideshow project file: contrib/views_slideshow_cycle/theme/views-slideshow-cycle-main-frame.tpl.php
 * Added variables for pagination and navigation.
 * This allows supporting more than one pagination swiper buttons on the same page.
 * They need to have a different classname but have to adhere to the Swiper library standard.
 * Compatible with v4.4.1 of Swiper however recommend v4.4.2 or higher see README.md.
 */
?>
<ul id="views_slideshow_swiper_<?php print $vss_id; ?>" class="<?php print $classes; ?>">
  <?php print $rendered_rows; ?>
</ul>
<?php if (!empty($prevEl) && !empty($nextEl)): ?>
  <span class="<?php print $prevEl; ?>"></span><span class="<?php print $nextEl; ?>"></span>
<?php endif; ?>

<?php if (!empty($paginationEl)): ?>
  <div class="pagination-wrap"><div class="<?php print $paginationEl; ?>"></div></div>
<?php endif; ?>
