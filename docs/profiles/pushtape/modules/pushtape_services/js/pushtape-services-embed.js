/**
 * Behaviors for the pop-up preview for configuring the embed code.
 */
(function ($) {
  Drupal.behaviors.pushtapeServicesEmbed = {
    attach: function (context, settings) {
      /**
       *  Grab the previewDiv code from the textarea markup and dynamically
       *  insert it below as a way to preview it.
       *
       *  See pushtape_services_embed_code() in pushtape_services.module.
       */
 
      // Grab the textarea containing the iFrame code
      var embedContainer = $('textarea.embed-code-container');      
      var previewContainer = $('.preview-container');

      // Create a new dummy object to render the iFrame
      // @TODO: use clone()?
      var previewDiv = $();
      previewDiv.add('<div>');
      previewDiv.addClass('previewDiv');

      embedContainer.once('embed-code-preview', function(){
        var $this = $(this);

        // Automatically select all to make it easier to copy and paste
        $this.bind('click.selectAll', function(e){
          $(this).select();  
        }).trigger('click.selectAll');

        var firstTime = true;
        // Function to update the preview
        var updatePreview = function() {
          var width = $('.embed-width').val();
          var height = $('.embed-height').val();

          // Append the iframe code in the textarea to our dummy object
          var val = $this.val();
          previewDiv.append(val);

          // We append ?iframe=1 because of a weird bug where browsers don't seem to like loading an iframe src tag that matches the current window.location   
          previewContainer.html(val);
          if (firstTime) {
            var src = previewContainer.find('iframe').attr('src');
            // Need to output ampersand if clean urls are off
            var cleanUrl = Drupal.settings.pushtapeServices.clean_url ? '?' : '&';
            // Workaround to output ampersand in attr
            // http://stackoverflow.com/questions/11591174/escaping-of-attribute-values-using-jquery-attr
            //var url = $('<div/>').html(src + cleanUrl + 'iframe=1').text(); 
            previewContainer.find('iframe').attr('src', src + cleanUrl + 'iframe=1');
            firstTime = false;
          }
          previewContainer.find('iframe').attr('width', width);
          previewContainer.find('iframe').attr('height', height);
          embedContainer.val(previewContainer.html()).select();
        }
        updatePreview();

        $('.embed-refresh').bind('click', function(e){
          e.preventDefault();
          updatePreview();
        });


      });

    }
  };
}(jQuery));