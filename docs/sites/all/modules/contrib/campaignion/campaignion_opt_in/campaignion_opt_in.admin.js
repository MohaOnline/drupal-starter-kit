/**
 * @file
 * Change some labels according to the display setting.
 */

(function ($) {

  "use strict";

  /**
   * Set “no is opt-out” label according to the display setting.
   */
  Drupal.behaviors.campaignionOptInLabels = {
    attach: function (context, settings) {
      context = $(context).get(0);
      if (!settings.campaignionOptIn || !settings.campaignionOptIn.labels) {
        return;
      }
      for (var checkbox_id in settings.campaignionOptIn.labels) {
        var s = settings.campaignionOptIn.labels[checkbox_id];
        var checkbox = context.querySelector(checkbox_id);
        if (checkbox) {
          var display = checkbox.form.querySelector(s.display_id);
          var labels = checkbox.form.querySelectorAll('label[for="' + checkbox.id + '"]');
          display.addEventListener('change', function() {
            labels.forEach(function(label) {
              if (!label.classList.contains('itoggle')) {
                label.innerHTML = s.labels[display.value];
              }
            });
          });

          // Hack to only show the “disable opt-in” checkbox when (all of):
          // - Display is not radio.
          // - “no is opt-out“ is checked.
          // #states is not capable of such complex logic.
          var disable_optin = checkbox.form.querySelector('[name="extra[disable_optin]"]').parentNode;
          var setVisibility = function() {
            var visible = checkbox.checked && display.value != 'radios';
            disable_optin.style.display = visible ? 'block' : 'none';
          };
          display.addEventListener('change', setVisibility);
          checkbox.addEventListener('change', setVisibility);
          setVisibility();
        }
      }
    }
  };

})(jQuery);
