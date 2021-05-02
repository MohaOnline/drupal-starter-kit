(function($) {
Drupal.behaviors.campaignion_tracking = {};
Drupal.behaviors.campaignion_tracking.attach = function(context, settings) {

  /**
   * Dispatch to campaignion_tracking.tracker.
   *
   * Nothing happens if the tracker is not available.
   */

  function gracefulDispatch (name, data, trackingContext) {
    // Build an msg object expected by campaignion_tracking:
    // It has a "name" for the event, "data" for the event and a
    // "context" for additional data.
    // NB: the event handler of the tracker might expect a certain data
    // structure.
    var msg = {
      'name': name,
      'data': data,
      'context': trackingContext
    }
    window.campaignion_tracking.tracker.publish('donation', msg)
  }

  /**
   * Donation related events.
   *
   * The events are only fired if a correct campaignion_tracking context is
   * available. They are only fired once for the Drupal context to prevent the
   * same messages being dispatch multiple times (behaviours can be called
   * multiple times).
   *
   * Events:
   * - 'setDonationProduct'
   * - 'checkoutBegin'
   * - 'checkoutEnd'
   */
  if (settings['campaignion_tracking'] && settings['campaignion_tracking']['context']) {
    settings['campaignion_tracking']['sent'] = settings['campaignion_tracking']['sent'] || [];

    var node = settings.campaignion_tracking.context['node'] || {};
    var donation = settings.campaignion_tracking.context['donation'] || {};
    var webform = settings.campaignion_tracking.context['webform'] || {};

    if (node['is_donation']) {
      if (donation['amount'] && donation['interval'] && donation['currency_code']) {
        var product = {
          name: `${node['title']} (${donation['description']})`,
          id: `${node['nid']}-${donation['amount_component']}`,
          price: String(donation['amount']),
          variant: String(donation['interval']),
          quantity: 1
        };

        var msg = {
          currencyCode: donation['currency_code'] || 'EUR',
          product: product
        };

        gracefulDispatch('setDonationProduct', msg, settings.campaignion_tracking.context);

        // Assume the checkout begings when we are on the second step or if
        // there is only one page.
        if (!settings['campaignion_tracking']['sent'].includes('checkoutBegin')) {
          if (webform['current_step'] === 2 || webform['total_steps'] === 1) {
            settings['campaignion_tracking']['sent'].push('checkoutBegin');
            gracefulDispatch('checkoutBegin', {}, settings.campaignion_tracking.context);
          }
        }

        // Assume the checkout ends on the last webform step
        if (!settings['campaignion_tracking']['sent'].includes('checkoutEnd')) {
          if (webform['current_step'] === webform['total_steps']) {
            settings['campaignion_tracking']['sent'].push('checkoutEnd');
            gracefulDispatch('checkoutEnd', {}, settings.campaignion_tracking.context);
          }
        }
      }
    }
  }
};
})(jQuery);
