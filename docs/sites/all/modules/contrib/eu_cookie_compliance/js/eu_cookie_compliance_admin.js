/**
 * @file
 * EU Cookie Compliance admin script.
 */

(function ($) {
  function showHideThankYouFields(showHide) {
    if (showHide) {
      $('.form-item-eu-cookie-compliance-popup-find-more-button-message, .form-item-eu-cookie-compliance-popup-hide-button-message, .form-item-eu-cookie-compliance-popup-hide-agreed').show();
      $('.form-item-eu-cookie-compliance-popup-agreed-value').parent().show();

      $('#edit-eu-cookie-compliance-popup-agreed-value').attr('required', true);
      $('#edit-eu-cookie-compliance-popup-find-more-button-message').attr('required', true);
      $('#edit-eu-cookie-compliance-popup-hide-button-message').attr('required', true);

      $('.form-item-eu-cookie-compliance-popup-agreed-value label, .form-item-eu-cookie-compliance-popup-find-more-button-message label, .form-item-eu-cookie-compliance-popup-hide-button-message label').append('<span class="form-required">*</span>');
    }
    else {
      $('.form-item-eu-cookie-compliance-popup-find-more-button-message, .form-item-eu-cookie-compliance-popup-hide-button-message, .form-item-eu-cookie-compliance-popup-hide-agreed').hide();
      $('.form-item-eu-cookie-compliance-popup-agreed-value').parent().hide();

      $('#edit-eu-cookie-compliance-popup-agreed-value').attr('required', false);
      $('#edit-eu-cookie-compliance-popup-find-more-button-message').attr('required', false);
      $('#edit-eu-cookie-compliance-popup-hide-button-message').attr('required', false);

      $('.form-item-eu-cookie-compliance-popup-agreed-value label span, .form-item-eu-cookie-compliance-popup-find-more-button-message label span, .form-item-eu-cookie-compliance-popup-hide-button-message label span').remove();
    }
  }

  $(function () {
    showHideThankYouFields(document.getElementById('edit-eu-cookie-compliance-popup-agreed-enabled').checked === true);

    $('#edit-eu-cookie-compliance-popup-agreed-enabled').click(function () {
      showHideThankYouFields(this.checked === true);
    });
  });

} (jQuery))
