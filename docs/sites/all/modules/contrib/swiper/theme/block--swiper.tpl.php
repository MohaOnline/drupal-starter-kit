<?php
/**
 * @file
 * This file contains the basic swiper plugin markup implementation.
 *
 * To more details access:
 * http://www.idangero.us/sliders/swiper
 *
 * If you want to change this base implementation, simply, create a template
 * file with the same name in the folder of your current theme.
 */
?>
<?php $node_id = $variables['elements']['#node']->nid;?>
<div class="main-swiper-wrapper">
  <div class="swiper-container-nid-<?php print $node_id;?> swiper-container">
    <div class="swiper-wrapper">
      <!--Slides-->
      <?php
      foreach ($variables['elements']['#swiper_content'] as $content):
      ?>
      <div class="swiper-slide-nid-<?php print $node_id;?> swiper-slide">
        <?php print $content; ?>
      </div>
      <?php endforeach; ?>
      <!--End Slides-->
    </div>
  </div>
  <div class="pagination-nid-<?php print $node_id;?> pagination"></div>
</div>
