<?php

/**
 * @file
 * Default theme implementation for Audiofield Soundmanager2 360 player.
 *
 * Available variables:
 * - $id: unique ID for the player.
 * - $audio_file: Path to the audio file being rendered.
 *
 * @see template_preprocess()
 *
 * @ingroup themeable
 */
?>
<div id="<?php print $id; ?>" class="ui360">
  <a href="<?php print $audio_file; ?>"></a>
</div>
