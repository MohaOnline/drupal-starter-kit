(function($) {

  Drupal.behaviors.masked_input = {

    attach: function(context, settings) {
      var config = settings.masked_input;

      // Quick check for the library dependency.
      if ($.mask) {
        for (var element in config.elements) {
          $('#' + config.elements[element].id, context).once('masked_input', function(){

            // Register mask definitions from configurations and individual form elements.
            for (var character in config.definitions) {
              $.mask.definitions[character] = config.definitions[character];
            }

            // Apply masking behavior only when applicable.
            if (config.elements[element].mask.length) {
              $(this).mask(config.elements[element].mask, {
                placeholder: config.elements[element].placeholder,
                completed: Drupal.behaviors.masked_input.completedCallback
              });
            }
          });
        }
      }
    },

    completedCallback: function () {
      // Do nothing. But it's here in case other modules/themes want to override it.
    }

  }

})(jQuery);
