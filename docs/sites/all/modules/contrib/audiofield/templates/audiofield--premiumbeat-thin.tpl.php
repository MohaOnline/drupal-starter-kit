<?php

/**
 * @file
 * Default theme implementation for Audiofield Premium Beat Thin player.
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
  <embed src="<?php print $player_path;?>"  width="220" height="21" float="left" wmode="transparent" flashvars="mediaPath=<?php print $audio_file;?>&defaultVolume=100" autostart="true" loop="false" controller="true" bgcolor="#FF9900" pluginspage="http://www.macromedia.com/go/getflashplayer" ></embed>
</object>
