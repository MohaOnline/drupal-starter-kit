(function($) {
$(function() {

Drupal.campaignion_tracking = {eventsFired: {}};
$.each(Drupal.settings.campaignion_tracking.events, function (i, event_name) {
  Drupal.campaignion_tracking.eventsFired[event_name] = new Promise(function (resolve) {
    var resolved = false;
    $(document).on(event_name, function(e) {
      if (!resolved) {
        resolved = true;
        resolve(e);
      }
    });
  });
});

});
})(jQuery)
