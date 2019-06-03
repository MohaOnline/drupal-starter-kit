(function($) {
    Drupal.behaviors.campaignion_google_analytics = {};
    Drupal.behaviors.campaignion_google_analytics.attach = function(context, settings) {
        if (typeof ga === "undefined") { return; }

        var config =  settings.campaignion_google_analytics;
        if (typeof config === "undefined") {
          return
        }

        var tracker = new tracking.Tracker({defaultCurrency: 'EUR'});
        tracker.initializeDonation();

        var actions = config.actions;

        tracker.ga("send", "event", "webform", "submitted", config.title + " [" + config.nid + "]");

        if (actions.indexOf('purchase') >= 0) {
          tracker.initializeDonation({ currency: config.currency });
          tracker.sendPurchase(config.product, { eventLabel: config.title + " [" + config.nid + "]"}, config.purchase);
          // clear state after, as we are done sending
          delete actions[actions.indexOf('purchase')];
        }
    }
})(jQuery);
