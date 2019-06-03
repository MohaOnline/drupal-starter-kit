<?php

/**
 * @file
 * Template for BxSlider.
 */
?>
<div id="<?php print $slider_id; ?>">
  <ul class="bxslider" style="display:none">
    <?php foreach($items as $item): ?>
      <li><?php print $item['slide']; ?></li>
    <?php endforeach; ?>
  </ul>

  <ul class="bxslider-ths" style="display:none">
    <?php foreach($thumbnail_items as $key => $thumbnail_item): ?>
      <li slideindex="<?php print $key ?>"><a href="#"><?php print $thumbnail_item['thumbnail']; ?></a></li>
    <?php endforeach; ?>
  </ul>
</div>
