(function($){

  /**
   * CodeMirror behavior
   * @type {Object}
   */

  Drupal.behaviors.codemirror_field = {
    attach: function (context, settings) {
      var $textareas = $('textarea.codemirror');

      $.each ($textareas, function (i, textarea) {

        //This is handy if an ajax request reattaches the behaviors, we won't attach codemirror again.
        if ($(textarea).hasClass('codemirror-processed')) {
          return true;
        }

        //Get the instance id from the classes.
        instance = $.grep(this.className.split(" "), function (v, i) {
          return v.indexOf('codemirror-instance') === 0;
        }).join();

        if (typeof settings.codemirror.settings[instance] != 'undefined') {

          //Fix numeric values in the settings object.
          $.each(settings.codemirror.settings[instance], function(key, value) {
            if(!isNaN(value)) {
              settings.codemirror.settings[instance][key] = parseInt(value);
            }
          });

          //Add gutter for linter.
          if (typeof settings.codemirror.settings[instance]['lint'] != 'undefined'
            && settings.codemirror.settings[instance]['lint'] == 1) {
            settings.codemirror.settings[instance]['lint'] = true;
            settings.codemirror.settings[instance]['gutters'] = ["CodeMirror-lint-markers"];
          }

          //Configure match tags.
          if (typeof settings.codemirror.settings[instance]['matchTags'] != 'undefined'
            && settings.codemirror.settings[instance]['matchTags'] == 1) {
            settings.codemirror.settings[instance]['matchTags'] = {
              bothTags: true
            };
          }

          //Attach CodeMirror with the instance settings.
          CodeMirror.fromTextArea(textarea, settings.codemirror.settings[instance]);
        }

        $('.CodeMirror').next('.grippie').remove();

        //Textarea processed.
        $(textarea).addClass('codemirror-processed');

      });
    }
  };
}(jQuery));
