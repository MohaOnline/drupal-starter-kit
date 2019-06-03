(function($) {
    Drupal.behaviors.campaignion_google_analytics_donation = {};
    Drupal.behaviors.campaignion_google_analytics_donation.attach = function(context, settings) {

      function unique(el, index, array) {
        return array.indexOf(el) === index;
      }

      if (typeof ga === 'undefined') { return; }
      // guard against missing window.sessionStorage
      // this prevents errors, but degrades functionality on systems missing
      // sessionStorage
      if (typeof window.sessionStorage === 'undefined') {
        window.sessionStorage = {};
        window.sessionStorage['setItem'] = function () {};
        window.sessionStorage['getItem'] = function () {};
        window.sessionStorage['removeItem'] = function () {};
      }

      var config =  settings.campaignion_google_analytics;
      if (typeof config === "undefined") {
        return
      }

      var tracker = new tracking.Tracker({defaultCurrency: 'EUR'});
      tracker.initializeDonation();

      var actions = config.actions.filter(unique);

      // if some donation teasers have been rendered on the page
      // config.impressions will be set and we only have to send
      // them to GA as impressions
      if (actions.indexOf('impression') >= 0) {
        $.each(config.impression, function(i, impression) {
          tracker.sendImpression(impression);
        });
        // clear state after, as we are done sending
        delete actions[actions.indexOf('impression')];
        delete config.impression;
      }

      if (actions.indexOf('view') >= 0) {
        tracker.initializeDonation({ currency: config.currency });
        tracker.sendView(config.product);
        // clear state after, as we are done sending
        delete actions[actions.indexOf('view')];
      }

      if (actions.indexOf('add') >= 0) {
        tracker.initializeDonation({ currency: config.currency });
        tracker.sendAdd(config.product);
        // clear state after, as we are done sending
        delete actions[actions.indexOf('add')];
      }

      if (actions.indexOf('checkoutBegin') >= 0) {
        tracker.initializeDonation({ currency: config.currency });
        tracker.sendBeginCheckout(config.product, { eventLabel: config.title + " [" + config.nid + "]"});
        // clear state after, as we are done sending
        delete actions[actions.indexOf('checkoutBegin')];
      }

      if (actions.indexOf('checkoutEnd') >= 0) {
        tracker.initializeDonation({ currency: config.currency });

        // bind on click of last step if there is an paymethod select form
        // submit is a complex option, as webform_ajax and clientside_validation
        // are involved
        $form = $('.webform-client-form #payment-method-all-forms', context).closest('form.webform-client-form', document);

        // the current webform page, does not contain a paymethod-selector.
        if ($form.length) {
          var form_id = $form.attr('id');
          var form_num = form_id.split('-')[3];
          var $button = $form.find('#edit-webform-ajax-submit-' + form_num);

          if ($button.length === 0) { // no webform_ajax.
            $button = $form.find('input.form-submit');
          }

          $button.click(function() {
            var $controller = $('#' + form_id + ' .payment-method-form:visible');
            // get the label of the selected paymethod controller
            // fallback to id
            var controller = $controller.find('> legend').length > 0 ? $controller.find('> legend').text() : $controller.attr('id');
            var $issuer = $('[name*=\"[issuer]\"]', $controller);
            var methodOption;
            if ($issuer.length > 0) {
              methodOption = controller + " [" + $issuer.val() + "]";
            } else {
              methodOption = controller;
            }

            tracker.sendEndCheckout(config.product, { eventLabel: config.title + " [" + config.nid + "]"}, { option: methodOption });
          });
        }

        // clear state after, as we are done sending
        delete actions[actions.indexOf('checkoutEnd')];
      }
    }
})(jQuery);
