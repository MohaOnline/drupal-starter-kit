(function ($) {
  Drupal.behaviors.audiofield_wavesurfer_player = {
    attach: function (context, settings) {
      $(".audiofield-wavesurfer").each(function() {
        var id = $(this).attr('id');
        var wavesurfer = WaveSurfer.create({
          container: '#' + id + ' .waveform',
        });
        wavesurfer.load(settings.audiofield[id]);

        $('#' + id + ' .player-button.playpause').bind('click', function (event) {
          wavesurfer.playPause();
          var button = $('#' + id + ' .player-button.playpause');
          if (wavesurfer.isPlaying()) {
            $('#' + id).addClass('playing');
            button.html('Pause');
          } else {
            $('#' + id).removeClass('playing');
            button.html('Play');
          }
        });

        $('#' + id + ' .volume').bind('change', function (event) {
          wavesurfer.setVolume($(event.currentTarget).val() / 10);
        });
      });
    }
  };
})(jQuery);
