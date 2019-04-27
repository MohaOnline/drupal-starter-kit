(function ($) {

  Drupal.behaviors.office_hours = {
    attach: function (context, settings) {

      // Hide every item above the max blocks per day.
      $(".oh-hide", context).parent().hide();

      $(".oh-add-more-link", context).each(function (i) {
          $(this).parent().children("div.office-hours-block").hide();
          // If the previous row has an "Add more hours" link, and the office-hours-block is hidden, don't show this line.
          $this_tr = $(this).closest("tr");
          if ($this_tr.prev().find(".oh-add-more-link").length && !$this_tr.prev().find("div.office-hours-block:visible").length) {
            $this_tr.hide();
          }
        })
        .bind('click', show_upon_click);

      fix_striping();

      // Clear the content of this block, when user clicks "Clear/Remove".
      $('.oh-clear-link').bind('click', function (e) {
        $(this).parent().parent().find('.form-select').each(function () {
          $(this).val($("#target option:first").val());
        });
        e.preventDefault();
      });

      // Copy values from previous day, when user clicks "Same as above".
      $('.oh-same-link').bind('click', function (e) {
        e.preventDefault();
        var current_day = parseInt($(this).parent().parent().attr('data-day'));
        var previous_day = current_day - 1;
        if (current_day == 0) {
          previous_day = current_day + 6;
        }

        // Select current table.
        var tbody = $(this).closest('tbody');
        // Div's from current day.
        var current_selector = tbody.find('div[data-day="' + current_day + '"]');
        // Div's from previous day.
        var previous_selector = tbody.find('div[data-day="' + previous_day + '"]');

        // Replace current day values with the ones from the previous day.
        previous_selector.find('.form-select:visible').each(function (index, value) {
          var previous_value = $(this).val();
          current_selector.find('.form-select').eq(index).val(previous_value);
          //"unhide" copied value using add more link
          current_selector.find('.form-select').eq(index).closest('tr').find('.oh-add-more-link').click();
        });
      });

      // Show an office-hours-block, when user clicks "Add more".
      function show_upon_click(e) {
        $(this).hide();
        $(this).parent().children("div.office-hours-block").fadeIn("slow");
        e.preventDefault();

        // If the next item has an "add more" link, show it.
        $next_tr = $(this).closest("tr").next();
        if ($next_tr.find(".oh-add-more-link").length) {
          $next_tr.show();
        }
        fix_striping();
        return false;
      }

      // Function to traverse visible rows and apply even/odd classes.
      function fix_striping() {
        $('tbody tr:visible', context).each(function (i) {
          if (i % 2 == 0) {
            $(this).removeClass('odd').addClass('even');
          } else {
            $(this).removeClass('even').addClass('odd');
          }
        });
      }
    }
  };

})(jQuery);
