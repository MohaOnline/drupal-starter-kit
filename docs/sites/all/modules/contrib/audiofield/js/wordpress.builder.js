(function ($) {
  AudioPlayer.setup('/sites/all/libraries/player/audio-player/player.swf', {
    width: 400,
    transparentpagebg: 'yes'
  });
  jQuery('.audiofield-wordpress-player') .each(function () {
    AudioPlayer.embed(jQuery(this).attr('id'), {soundFile: jQuery(this).attr('data-src')});
  });
})(jQuery);
