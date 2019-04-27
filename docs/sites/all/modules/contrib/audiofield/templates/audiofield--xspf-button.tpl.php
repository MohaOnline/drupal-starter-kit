<?php

/**
 * @file
 * Default theme implementation for Audiofield XSPF button player.
 *
 * Available variables:
 * - $player_path: path to audio player.
 * - $audio_file: Path to the audio file being rendered.
 * - $audio_title: Title of the audio file being rendered.
 *
 * @see template_preprocess()
 *
 * @ingroup themeable
 */
?>
<object type="application/x-shockwave-flash" width="17" height="17" data="<?php print $player_path; ?>?song_url=<?php print $audio_file; ?>&song_title=<?php print $audio_title; ?>">
  <param name="movie" value="<?php print $player_path; ?>?song_url=<?php print $audio_file; ?>&song_title=<?php print $audio_title; ?>" />
  <param name="wmode" value="transparent" />
</object>
