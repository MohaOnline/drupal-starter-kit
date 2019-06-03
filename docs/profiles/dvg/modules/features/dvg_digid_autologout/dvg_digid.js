(function ($) {
  var timeout = 0,
    autologout_block;

  // Override Dialog defaults to disable resizing and dragging for the autologout dialog.
  if ($.ui && $.ui.dialog) {
    $.extend($.ui.dialog.prototype.options, {
      modal: true,
      resizable: false,
      draggable: false
    });
  }

  Drupal.behaviors.dvgDigidScript = {
    attach: function (context, settings) {
      $('.block-dvg-digid-autologout', context).once('digid-autologout', function() {
        // Make sure these strings are translated.
        Drupal.t('seconds');
        Drupal.t('second');
        Drupal.t('minutes');
        Drupal.t('minute');

        autologout_block = $(this);
        timeout = getTimeout();

        var timeout_span = autologout_block.find('.timeout');
        autologout_block.find('.refresh').toggleClass('element-invisible', !settings.autologout.can_refresh);
        autologout_block.find('.norefresh').toggleClass('element-invisible', settings.autologout.can_refresh);

        if (timeout_span.length) {
          showTime(timeout_span, timeout);
          setInterval(function () {
            timeout--;
            showTime(timeout_span, timeout);
          }, 1000);
        }

        function showTime(timeout_span, timeout) {
          if (timeout < 0) {
            timeout = 0;
          }

          var numMinutes = (timeout % 3600) / 60,
            numMinutes_floor = Math.floor(numMinutes),
            numMinutes_ceil = Math.ceil(numMinutes),
            text = numMinutes_ceil,
            min_sec = Drupal.formatPlural(numMinutes_ceil, 'minute', 'minutes');
          // Countdown seconds for the last 3 minutes.
          if (numMinutes_floor < 3) {
            var numSeconds = Math.round(timeout % 60);
            if (numMinutes_floor > 0) {
              text = numMinutes_floor + ':';
            }
            else {
              // During the last minute, only show the seconds.
              text = '';
              min_sec = Drupal.formatPlural(numSeconds, 'second', 'seconds');
            }
            text += (numMinutes_floor > 0 && numSeconds < 10 ? '0' : '') + numSeconds;
          }
          timeout_span.text(text + ' ' + min_sec);
        }
      });

      function getTimeout() {
        var block;
        block = $('#timer .interval');
        return parseInt(block.text());
      }

      timeout = getTimeout();
    }
  }
}(jQuery));
