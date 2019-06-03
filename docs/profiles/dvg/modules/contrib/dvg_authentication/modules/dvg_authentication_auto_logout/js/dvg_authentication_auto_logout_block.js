(function ($) {
  Drupal.behaviors.dvgAuthenticationAutoLogoutBlock = {
    attach: function (context, settings) {
      $('.dvgautologout', context).once('auto-logout-once', function() {
        var autoLogoutBlock = $(this);

        var timeoutSpan = autoLogoutBlock.find('.timeout');
        if (timeoutSpan.length) {
          showTime(timeoutSpan);
          setInterval(function () {
            Drupal.settings.auto_logout.time_remaining--;
            if (Drupal.settings.auto_logout.time_remaining < 0) {
              // Send pulse to trigger auto logout.
              window.auto_logout.pulse();
            }
            showTime(timeoutSpan);
          }, 1000);
        }

        function showTime(timeoutSpan) {
          var timeRemaining = Drupal.settings.auto_logout.time_remaining;
          if (timeRemaining < 0) {
            timeRemaining = 0;
          }

          var minutesRemaining = Math.floor(timeRemaining / 60);

          var timeText = '';
          if (minutesRemaining > 0) {
            timeText = minutesRemaining + ' '
              + Drupal.formatPlural(minutesRemaining, 'minute', 'minutes');
          }

          // Countdown seconds for the last 3 minutes.
          if (minutesRemaining < 3) {
            var secondsRemaining = timeRemaining - (60 * minutesRemaining);
            // Ugly string padding method due to IE support.
            if (secondsRemaining < 10) {
              secondsRemaining = '0' + secondsRemaining;
            }
            timeText += ' ' + secondsRemaining + ' '
              + Drupal.formatPlural(secondsRemaining, 'second', 'seconds');
          }
          timeoutSpan.text(timeText);
        }
      });
      $('#auto-logout-refresh-link').on('click', function(event) {
        event.preventDefault();
        if (window.auto_logout.sendStillAlive) {
          window.auto_logout.sendStillAlive();
        }
        else {
          $(event.target).trigger('preventAutoLogout');
        }
      });
      $('#auto-logout-logout-link').on('click', function() {
        Drupal.settings.auto_logout.time_remaining = 0;
      })
    }
  }
}(jQuery));
