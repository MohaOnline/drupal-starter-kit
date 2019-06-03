(function($) {

Drupal.behaviors.formBuilderSubmitButton = {};

// Virtually submit buttons labels to the end of the form.
Drupal.behaviors.formBuilderSubmitButton.attach = function(context) {
  var $stepForm = $('.wizard-form', context);
  var $formBuilder = $('#form-builder', context);
  
  if ($stepForm.length > 0 && $formBuilder.length > 0) {
    $wrapper = $stepForm.find('#edit-submit-buttons');
    $wrapper.find('input').each(function() {
      var $field = $(this);
      var $hidden = $('<input type="hidden" name="' + $field.attr('name') + '" />')
        .insertBefore($wrapper);
      $stepForm.submit(function() {
        $hidden.val($field.val());
      });
    });
    $formBuilder.parent().append($wrapper);
  }
};

})(jQuery);
