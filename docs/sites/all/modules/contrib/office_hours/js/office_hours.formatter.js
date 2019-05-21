(function ($) {

  /**
   * Determine the open/closed status dynamically, to avoid errors due to caching.
   * @todo: this should also be done for the 'Next' value,
   */
  Drupal.behaviors.office_hours = {
    attach: function (context, settings) {
      if ($('.oh-current-wrapper', context).length == 0) {
        return;
      }

      // Only run if output is enabled (wrapper exists in DOM)
      $('.oh-current-wrapper', context).each(function () {
        var current_context_id = $(this).data('oh-current-context_id');
        var slots = Drupal.settings.office_hours.instances[current_context_id];
        if (!slots) {
          return;
        }
        var $instance = $(this);
        var open = false;
        var dateObj = new Date();
        var today = dateObj.getDay();
        var now = (dateObj.getHours() > 9 ? dateObj.getHours().toString() : '0' + dateObj.getHours().toString()) + (dateObj.getMinutes() > 9 ? dateObj.getMinutes().toString() : '0' + dateObj.getMinutes().toString());
        jQuery.each(slots.days, function (day_index, day_value) {
          var start_day = day_value.startday;
          if (!day_value.times) {
            return;
          }
          jQuery.each(day_value.times, function (time_index, time_value) {
            var start_time = time_value.start;
            var end_time = time_value.end;
            if (start_day - today == -1 || (start_day - today == 6)) {
              if (start_time >= end_time && end_time >= now) {
                open = true;
              }
            }
            else if (start_day == today) {
              if (start_time <= now) {
                if ((start_time > end_time)
                  || (start_time == end_time)
                  || ((start_time < end_time) && (end_time > now))) {
                  open = true;
                }
              }
            }
          });
        });
        if (open) {
          $instance.children('.oh-current-open:first').removeClass('element-invisible');
          $instance.children('.oh-current-closed:first').addClass('element-invisible');
        }
        else {
          $instance.children('.oh-current-closed:first').removeClass('element-invisible');
          $instance.children('.oh-current-open:first').addClass('element-invisible');
        }
      });
    }
  };

})(jQuery);
