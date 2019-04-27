<?php

/**
 * @file
 * Default theme implementation for Audiofield Soundmanager2 Button player.
 *
 * Available variables:
 * - $audio_file: Path to the audio file being rendered.
 * - $audio_title: Title of the audio file being rendered.
 *
 * @see template_preprocess()
 *
 * @ingroup themeable
 */
?>
 <div class="audiofield"><a href="<?php print $audio_file; ?>" class="sm2_button"><?php print $audio_title; ?></a> <?php print $audio_title; ?></div>
