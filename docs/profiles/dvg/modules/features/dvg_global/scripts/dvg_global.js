(function ($) {

  $(document).bind('state:required', function(e) {
    if (e.trigger) {
      // Remove additional .form-required *'s.
      $(e.target).closest('.form-item, .form-wrapper').find('label .form-required:not(:first)').remove();
    }
  });

})(jQuery);
