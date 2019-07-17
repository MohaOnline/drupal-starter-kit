<?php

/**
 * @file media_bliptv/includes/themes/media-bliptv-video.tpl.php
 *
 * Template file for theme('media_bliptv_video').
 *
 * Variables available:
 *  $uri - The uri to the BlipTV video, such as bliptv://v/xsy7x8c9.
 *  $video_id - The unique identifier of the BlipTV video.
 *  $width - The width to render.
 *  $height - The height to render.
 *  $autoplay - If TRUE, then start the player automatically when displaying.
 *  $fullscreen - Whether to allow fullscreen playback.
 *
 * Note that we set the width & height of the outer wrapper manually so that
 * the JS will respect that when resizing later.
 */
?>
<div class="media-bliptv-outer-wrapper" id="media-bliptv-<?php print $id; ?>" style="width: <?php print $width; ?>px; height: <?php print $height; ?>px;">
  <div class="media-bliptv-preview-wrapper" id="<?php print $wrapper_id; ?>">
    <?php print $output; ?>
  </div>
</div>
