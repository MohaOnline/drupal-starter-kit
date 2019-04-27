<?php

/**
 * @file
 * Default theme implementation for Audiofield wavesurfer.js.
 *
 * Available variables:
 * - $container_id: Unique ID for the player.
 * - $audio_title: Title of the audio file being rendered.
 *
 * @see template_preprocess()
 *
 * @ingroup themeable
 */
?>
<div class="audiofield">
  <div class="audiofield-wavesurfer" id="<?php print $container_id; ?>">
    <div class="waveform"></div>
    <div class="player-button playpause play">Play</div>
    <input type="range" class="volume" min="0" max="10" value="{{ settings.audio_player_initial_volume}}">
    <label><?php print $audio_title; ?></label>
  </div>
</div>
