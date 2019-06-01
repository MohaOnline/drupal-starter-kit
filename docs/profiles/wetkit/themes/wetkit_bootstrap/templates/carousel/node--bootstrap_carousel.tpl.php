<?php

/**
 * @file
 * Template for bootstrap carousel
 */
?>
<section>
  <div class="wb-tabs carousel-s2" data-speed="slow">
    <?php
      $carousel_items = field_get_items('node', $node, 'field_slides');

      $control_options = field_get_items('node', $node, 'field_control_options');
      $arrow_enabled = FALSE;
      $bullets_enabled = FALSE;

      if (is_array($control_options)) :
        foreach($control_options as $control) :
          switch($control['value']) :
            case '1':
              $arrow_enabled = TRUE;
              break;

            case '2':
              $bullets_enabled = TRUE;
              break;

          endswitch;
        endforeach;
      endif;
    ?>

      <?php if (is_array($carousel_items)) : ?>
        <ul role="tablist">
        <?php foreach ($carousel_items as $id => $carousel_slide) : ?>
          <li class="<?php ($id == '0') ? print 'active' : print ''; ?>">
            <a href="#panel<?php print $id; ?>" title="panel<?php print $id; ?>">
              <?php if(!empty($carousel_slide['carousel_image'])) : ?>
                <?php $img_url = file_create_url(file_load($carousel_slide['carousel_image'])->uri); ?>
                <img src="<?php print $img_url ?>" alt="<?php print $carousel_slide['image_alt_text'];?>"/>
              <?php endif; ?>
            </a>
          </li>
        <?php endforeach; ?>
        </ul>
      <?php endif; ?>

    <?php if (is_array($carousel_items)) : ?>
      <?php foreach ($carousel_items as $id => $carousel_slide) : ?>
        <div class="<?php ($id == '0') ? print 'in ' : print 'out '; ?>fade" id="panel<?php print $id; ?>" role="tabpanel">
          <figure>
          <?php if(!empty($carousel_slide['carousel_image'])) : ?>
            <?php $img_url = file_create_url(file_load($carousel_slide['carousel_image'])->uri); ?>
            <img src="<?php print $img_url ?>" alt="<?php print $carousel_slide['image_alt_text'];?>"/>
          <?php endif; ?>

          <?php if(!empty($carousel_slide['carousel_video'])) : ?>
            <div class="video-wrapper">
              <div class="video-container">
                <div class="ytplayer" id="ytplayer-<?php print $carousel_slide['carousel_video']; ?>" data-videoid="<?php print $carousel_slide['carousel_video']; ?>">
                </div>
              </div>
            </div>
          <?php endif; ?>

          <?php if (strip_tags($carousel_slide['carousel_caption']) != ''): ?>
            <figcaption>
              <?php print $carousel_slide['carousel_caption']; ?>
            </figcaption>
          <?php endif; ?>
          </figure>
        </div>

      <?php endforeach; ?>
    <?php endif; ?>

  </div>
</section>
