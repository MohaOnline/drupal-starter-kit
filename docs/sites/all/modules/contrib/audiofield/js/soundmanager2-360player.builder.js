(function ($) {
  Drupal.behaviors.audiofield_soundmanager2_360player = {
    attach: function (context, settings) {
      soundManager.url = settings.audiofield.swf_path;
      soundManager.useFastPolling = true;
      soundManager.waitForWindowLoad = true;
      soundManager.preferFlash = true;

      soundManager.onready(function () {
        soundManager.stopAll();
        threeSixtyPlayer.init();
      });
    }
  };
})(jQuery);
