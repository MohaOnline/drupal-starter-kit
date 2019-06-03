(function ($) {
  Drupal.behaviors.dvgAppointments = {
    attach: function (context, settings) {
      $('.wa-product-product', context).once('dvgAppointments').change(function (e) {
        var $this = $(this);
        $this.parents('.fieldset-wrapper').find('.messages.error').hide();
        $this.parents('.fieldset-wrapper').find('input.error').removeClass('error');
        // Hide the global form error message only if all other field-specific error messages are gone.
        var allErrorsHidden = true;
        $(".error.messages-inline:visible").each(function () {
          if ($(this).length === 1) {
            allErrorsHidden = false;
          }
        });
        if (allErrorsHidden) {
          $(".messages.error").hide();
        }

        // When changing the selected product, add/replace the product_code in the form action url.
        var form = $this.parents('form:first');
        var action = form.attr('action');

        action = action.replace(
          new RegExp("([?&]product_code(?=[=&#]|$)[^#&]*|(?=#|$))"), "&product_code="+encodeURIComponent($this.val())
        ).replace(/^([^?&]+)&/, "$1?");

        form.attr('action', action);

      });

      // When selecting a new time, unselect all other appointment times selectors.
      $('.time-selector-appointments select', context).change(function (e) {
        $('.time-selector-appointments select').not(this).prop('selectedIndex', 0);
      });
      // Resets the value of the count-field when a product with max_persons
      // being 1 is selected. The count-field will be hidden by 'states' and
      // 'state:visible'-event will be triggered.
      $(document).bind('state:visible', function(event) {
        if (!event.value && Drupal.settings.dvg_appointments) {
          var name = event.target.name;
          if (name && Drupal.settings.dvg_appointments[name]) {
            $(':input[name="' + name + '"]').val(1);
          }
        }
      });

      $('#dvg-show-more-dates').click(function(e) {
        $('.dvg-appointments-date-selection tr').removeClass('element-invisible');
        $(this).remove();
      });
    }
  };

}(jQuery));
