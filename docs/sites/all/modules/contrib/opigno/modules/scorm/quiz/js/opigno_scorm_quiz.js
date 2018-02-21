/**
 * @file
 * JS Quiz logic for SCORM player.
 */

;(function($, Drupal, window, undefined) {

  Drupal.behaviors.opignoScormQuiz = {

    attach: function(context, settings) {
      // SCORM 2004 API
      if (window.API_1484_11 !== undefined) {
        try {
          // Add '_children' properties, as we cannot set them server-side through PHP.
          window.API_1484_11.data.cmi.objectives._children = 'id,score,success_status,completion_status,progress_measure,description';
        }
        catch (e) { }
      }
      // SCORM 1.2 API
      if (window.API !== undefined) {
        try {
          // Add '_children' properties, as we cannot set them server-side through PHP.
          window.API.data.cmi.objectives._children = 'id,score,status';
        }
        catch (e) { }
      }
    }

  };

})(jQuery, Drupal, window);
