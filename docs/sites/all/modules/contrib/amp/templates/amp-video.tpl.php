<?php
/**
 * @file
 * Template for an amp-video.
 *
 * Available variables:
 * - video_attributes: The HTML attributes for the amp-video element.
 * - src: A URL for a video file.
 * - scheme: A string for the scheme of the video src URL, i.e. http or https.
 * - fallback_text: A string that will render if the browser is incompatible
 *   with the HTML5 video element.
 *
 * @see template_preprocess_amp_video()
 */
?>
<?php if ($scheme == 'https'): ?>
  <amp-video <?php print $video_attributes; ?> src="<?php print $src; ?>" layout="responsive" controls>
    <div fallback>
      <p><?php print $fallback_text; ?></p>
    </div>
  </amp-video>
<?php endif; ?>
