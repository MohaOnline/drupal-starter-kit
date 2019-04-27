<?php

/**
 * @file
 * Default theme implementation for Audiofield Premium Beat Single Track player.
 *
 * Available variables:
 * - $player_path: path to audio player.
 * - $audio_file: Path to the audio file being rendered.
 *
 * @see template_preprocess()
 *
 * @ingroup themeable
 */
?>
<object>
  <param name="autoplay" value="true" />
  <param name="controller"value="true" />
  <embed src="<?php print $player_path; ?>"  width="192" height="80" float="left" wmode="transparent" flashvars="mediaPath=<?php print $audio_file; ?>" autostart="true" loop="false"  controller="true" bgcolor="#FF9900" pluginspage="http://www.macromedia.com/go/getflashplayer" ></embed>
</object>
