(function ($) {
  Drupal.behaviors.dvgAuthenticationAutoLogout = {
    attach: function(context, settings) {

      if (context !== document) {
        return;
      }

      // Start pulse for regular updates and delayed activity triggers.
      setInterval(pulse, 60000);

      var keepAliveTimer;
      var logoutTimer;

      resetTimers();

      // Register send alive and pulse functions globally for use from other scripts.
      window.auto_logout = {
        sendStillAlive: sendStillAlive,
        pulse: pulse
      };

      // Activity is a boolean used to detect a user has
      // interacted with the page.
      var activity = false;

      // Bind to page interaction events to send activity pings.
      if (!Drupal.settings.auto_logout.keep_alive) {
        var $body = $('body');
        // Bind to form events.
        $body.bind('formUpdated', function(event) {
          $(event.target).trigger('preventAutoLogout');
        });

        // Support for CKEditor.
        if (typeof CKEDITOR !== 'undefined') {
          CKEDITOR.on('instanceCreated', function(e) {
            e.editor.on('contentDom', function() {
              // Keyup event in ckeditor should prevent auto logout.
              e.editor.document.on('keyup', function(event) {
                $(event.target).trigger('preventAutoLogout');
              });
            });
          });
        }

        // Register custom event to unify handling of page interaction activity.
        $body.bind('preventAutoLogout', function(event) {
          // Send quick alive ping when the end of all time is near.
          if (Drupal.settings.auto_logout.time_remaining < 60) {
            sendStillAlive();
          }
          else {
            activity = true;
          }
        });
      }

      function resetTimers() {
        if (Drupal.settings.auto_logout.keep_alive) {
          // On pages that cannot be logged out of, send activity pings.
          clearTimeout(keepAliveTimer);
          keepAliveTimer = setTimeout(sendStillAlive, Drupal.settings.auto_logout.time_remaining * 500);
        }
        else {
          // On pages where the user can be logged out.
          clearTimeout(logoutTimer);
          logoutTimer = setTimeout(logout, Drupal.settings.auto_logout.time_remaining * 1000);
        }
      }

      // Fired every 60 seconds.
      function pulse() {
        if (activity) {
          // The user has been active on the page.
          activity = false;
          sendStillAlive();
        }
        else if (Drupal.settings.auto_logout.time_remaining < 0) {
          logout();
        }
        else {
          // Periodically refresh remaining time in case the user is
          // navigating in another tab.
          fetchTimeRemaining();
        }
      }

      var isLoggingOut = false;
      function logout() {
        if (!isLoggingOut) {
          isLoggingOut = true;
          window.location = '/auto-logout/logout?destination=' + encodeURI(window.location.pathname);
        }
      }

      function sendStillAlive() {
        fetchSettings('/auto-logout/still-alive');
      }

      function fetchTimeRemaining() {
        fetchSettings('/auto-logout/time-remaining');
      }

      function fetchSettings(endpoint) {
        $.ajax({
          url: endpoint,
          success: function (data) {
            // Old version of merging object properties, due to IE support.
            for (var key in data) {
              Drupal.settings.auto_logout[key] = data[key]
            }
            // Reset timers as the remaining time might have changed.
            resetTimers();
          },
          error: logout
        })
      }

      // Check if the page was loaded via a back button click.
      var $dirty_bit = $('#auto-logout-cache-check-bit');
      if ($dirty_bit.length !== 0) {
        if ($dirty_bit.val() === '1') {
          // Page was loaded via a back button click, register the activity.
          sendStillAlive();
        }

        $dirty_bit.val('1');
      }
    }
  };
})(jQuery);
