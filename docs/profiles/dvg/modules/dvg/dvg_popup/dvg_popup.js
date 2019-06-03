(function ($) {
  Drupal.behaviors.dvgPopup = {
    attach: function(context, settings) {
      if (Drupal.settings.dvg_popup && Drupal.settings.dvg_popup.dvg_popup_cookie_name) {
        var cookie_name = Drupal.settings.dvg_popup.dvg_popup_cookie_name;

        if (document.cookie.indexOf(cookie_name) == -1) {
          $('.popup-content, .popup-overlay').show();
        }

        $('.popup-content .close', context).click(function() {
          $('.popup-content, .popup-overlay').hide();
          // Set the cookie (expires now + x days) to keep the popup hidden.
          var date = new Date(),
            days = 365;

          if (Drupal.settings.dvg_popup.dvg_popup_cookie_expiry != undefined) {
            days = parseInt(Drupal.settings.dvg_popup.dvg_popup_cookie_expiry);
          }
          if (days != 0) {
            date.setDate(date.getDate() + days);
            var expires = "expires=" + date.toUTCString() + ";";
          }
          document.cookie = cookie_name + "=1;" + expires + "path=" + Drupal.settings.basePath;
        });
      }
    }
  };
}(jQuery));
