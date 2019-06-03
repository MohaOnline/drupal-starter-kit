(function ($) {
  Drupal.behaviors.pushtapePlayerConfig = {
    attach: function (context, settings) {
      // Load once
      if (context == document) {
        soundManager.setup({
          url:  Drupal.settings.pushtapePlayerSM2.url,
          debugMode: Drupal.settings.pushtapePlayerSM2.debug,
          useHighPerformance: true, // keep flash on screen, boost performance
          preferFlash: true, // for visualization effects (smoother scrubber)
          flashVersion: 9,
          wmode: 'transparent', // transparent SWF, if possible
          onready: function() {
            // Initialize pushtape player when SM2 is ready
            pushtapePlayer = new PushtapePlayer();
            pushtapePlayer.init(Drupal.settings.pushtapePlayerConfig);
          },
          ontimeout: function() {
            // Could not start. Missing SWF? Flash blocked? Show an error, etc.?
            console.log('Error initializing the Pushtape player.');
          }
        });
      }
    }
  };
})(jQuery);