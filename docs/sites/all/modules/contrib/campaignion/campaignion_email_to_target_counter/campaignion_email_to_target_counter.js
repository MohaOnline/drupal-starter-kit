(function($) {

"use strict";
  
var formatNumber = function(now) {
  var num, rest, zeros;
  num = '';
  zeros = 0;
  now = Math.round(now);
  if (now === 0) {
    return '0';
  }
  while (now > 0) {
    while (zeros > 0) {
      num = '0' + num;
      zeros -= 1;
    }
    rest = now % 1000;
    zeros = 3 - rest.toString().length;
    num = rest + ',' + num;
    now = (now - rest) / 1000;
  }
  return num.slice(0, num.length - 1);
};

/**
 * Counter object constructor.
 *
 * @constructor
 */
var Counter = (function() {
  function Counter(settings1, elements) {
    this.settings = settings1;
    this.current = 0;
    this.counter = elements;
  }

  /**
   * Register a callback for the polling registry.
   */
  Counter.prototype.poll = function() {
    var callback, registry;
    registry = Drupal.behaviors.polling.registry;
    callback = (function(_this) {
      return function(data) {
        var to_abs;
        to_abs = data.campaignion_email_to_target_counter[_this.settings.target_id]['count'];
        if (to_abs !== _this.current) {
          _this.animate(to_abs);
        }
      };
    })(this);
    return registry.registerUrl(this.settings.pollingURL, this.settings.id, callback);
  };

  /**
   * Animate values for a new value.
   */
  Counter.prototype.animate = function(to_abs, from_abs) {
    var diff, duration, resetCounters;
    if (from_abs == null) {
      from_abs = this.current;
    }
    diff = (to_abs - from_abs) / to_abs;
    this.counter.html(formatNumber(from_abs));
    duration = 500 + 1000 * diff;
    resetCounters = (function(_this) {
      return function(num, fx) {
        return _this.counter.html(formatNumber(num));
      };
    })(this);
    this.counter.animate({
      val: to_abs
    }, {
      duration: duration,
      step: resetCounters
    });
    return this.current = to_abs;
  };

  /**
   * Trigger the initial animation (counting up from 0).
   */
  Counter.prototype.animateInitially = function() {
    var animation;
    this.counter.html(formatNumber(this.current));
    animation = (function(_this) {
      return function() {
        _this.animate(_this.settings.current);
      };
    })(this);
    return window.setTimeout(animation, 2000);
  };

  return Counter;

})();

/**
 * Static constructor for a Counter object.
 *
 * Finds configuration based on a HTML element and builds the counter object.
 *
 * @constructor
 */
Counter.fromElement = function($element) {
  var id, settings;
  id = $element.attr('data-counter-id');
  settings = Drupal.settings.campaignion_email_to_target_counter[id];
  settings['id'] = id;
  return new Counter(settings, $element);
};

/**
 * Attaches counter objects to HTML elements.
 *
 * @type {Drupal~behavior}
 *
 * @prop {Drupal~behaviorAttach} attach
 *   Search for matching elements, attach counter objects and trigger initial
 *   animation.
 */
Drupal.behaviors.campaignion_email_to_target_counter = {};
Drupal.behaviors.campaignion_email_to_target_counter.attach = function(context, settings) {
  var ids = {};
  var $elements = $('.campaignion-email-to-target-counter[data-counter-id]', context);
  $elements.each(function() {
    ids[$(this).attr('data-counter-id')] = true;
  });
  for (var id in ids) {
    var item;
    item = Counter.fromElement($elements.filter('[data-counter-id="' + id + '"]'));
    item.animateInitially();
    item.poll();
  }
};

})(jQuery)
