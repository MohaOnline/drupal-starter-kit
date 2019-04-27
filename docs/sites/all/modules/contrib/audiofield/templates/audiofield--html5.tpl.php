<?php

/**
 * @file
 * Default theme implementation for Audiofield built-in HTML5 player.
 *
 * Available variables:
 * - $audio_file: Path to the audio file being rendered.
 * - $download_access: flag indicating if user should be allowed to download.
 *
 * @see template_preprocess()
 *
 * @ingroup themeable
 */
?>
<audio src="<?php print $audio_file; ?>" controls<?php print ($variables['download_access'] ? '' : ' controlsList="nodownload"'); ?>></audio>
