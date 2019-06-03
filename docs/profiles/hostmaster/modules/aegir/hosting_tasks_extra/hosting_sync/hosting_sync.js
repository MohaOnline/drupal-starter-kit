$(document).ready( function() {
  $('#hosting-task-confirm-form, #hosting-sync-platform').before($("<div id='hosting-sync-comparison-inline'></div>").hide());
 // $("#hosting-sync-comparison-inline")
  $('a.hosting-package-comparison-link').click( function() {
    hostingSyncComparisonInline($(this));
    return false;
  });
});

function hostingSyncComparisonInline(elem) {
  var hostingSyncCallback = function(data, responseText) {
    $("#hosting-sync-comparison-inline").html(data).show();
    $('#hosting-task-confirm-form, #hosting-sync-platform').hide();
    hostingSyncToggleSize();
    $('.hosting-sync-comparison-return').click( function() {
      hostingSyncComparisonClose();
      return false;
    }
    );
  }
 
  $.get('/hosting/js' + $(elem).attr('href'), null, hostingSyncCallback );
}

function hostingSyncToggleSize() {
  if (parent.Drupal.modalFrame.isOpen) {

    var self = Drupal.modalFrameChild;
    // Tell the parent window to resize the Modal Frame if the child window
    // size has changed more than a few pixels tall or wide.
    var newWindowSize = {width: $(window).width(), height: $('body').height() + 25};
    if (Math.abs(self.currentWindowSize.width - newWindowSize.width) > 5 || Math.abs(self.currentWindowSize.height - newWindowSize.height) > 5) {
      self.currentWindowSize = newWindowSize;
      self.triggerParentEvent('childResize');
    }

  }
}

function hostingSyncComparisonClose() {
  $("#hosting-sync-comparison-inline").hide();
  $('#hosting-task-confirm-form, #hosting-sync-platform').show();
  hostingSyncToggleSize();
}
