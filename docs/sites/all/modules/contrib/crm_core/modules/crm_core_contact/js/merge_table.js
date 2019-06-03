(function ($) {
  Drupal.behaviors.crm_core_contact_merge_table = {
    attach: function(context) {
      $('#merge-contacts-table tr').each(function() {
        var context = $(this);
        var all_radios = $('input[type=radio]', context);
        $('input[type=radio]:not(.processed)', context).change(function() {
          all_radios.addClass('processed');
          all_radios.not(this).removeAttr('checked');
          all_radios.removeClass('processed');
        });
      });
    }
  }
})(jQuery);
