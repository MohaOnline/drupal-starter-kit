<?php

/**
 * @file
 * Default theme implementation for Audiofield Wordpress Audio player.
 *
 * Available variables:
 * - $id: unique ID for the audio player.
 * - $audio_file: Path to the audio file being rendered.
 *
 * @see template_preprocess()
 *
 * @ingroup themeable
 */
?>
<div id="<?php print $id; ?>" class="audiofield-wordpress-player" data-src="<?php print $audio_file; ?>"></div>
