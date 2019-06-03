!function(f,b,e,v,n,t,s){if(f.fbq)return;n=f.fbq=function(){n.callMethod?
n.callMethod.apply(n,arguments):n.queue.push(arguments)};if(!f._fbq)f._fbq=n;
n.push=n;n.loaded=!0;n.version='2.0';n.queue=[];t=b.createElement(e);t.async=!0;
t.src=v;s=b.getElementsByTagName(e)[0];s.parentNode.insertBefore(t,s)}(window,
document,'script','https://connect.facebook.net/en_US/fbevents.js');

// Read the URL fragment and send all events.
(function () {
  var eventCodes, hash, newHash, readFragmentParts, readPixelFragment, send;

  eventCodes = {
    l: 'Lead',
    r: 'CompleteRegistration',
    v: 'ViewContent'
  };

  readPixelFragment = function readPixelFragment(part, pixels) {
    var code, j, k, len, len1, p, pixelId, pixelStr, ref, ref1;
    pixels = pixels || Drupal.settings.campaignion_facebook_pixel.pixels;
    ref = part.split('&');
    for (j = 0, len = ref.length; j < len; j++) {
      pixelStr = ref[j];
      p = pixelStr.indexOf('=');
      pixelId = pixelStr.substring(0, p);
      if (!(pixelId in pixels)) {
        pixels[pixelId] = [];
      }
      ref1 = pixelStr.substring(p + 1).split(',');
      for (k = 0, len1 = ref1.length; k < len1; k++) {
        code = ref1[k];
        pixels[pixelId].push(code in eventCodes ? eventCodes[code] : code);
      }
      pixels[pixelId] = pixels[pixelId].filter(function (value, index, self) {
        return self.indexOf(value) === index;
      });
    }
    return pixels;
  };

  readFragmentParts = function readFragmentParts(hash) {
    var j, len, new_parts, part, ref;
    hash = hash || window.location.hash.substr(1);
    if (!hash) {
      return '';
    }
    new_parts = [];
    ref = hash.split(';');
    for (j = 0, len = ref.length; j < len; j++) {
      part = ref[j];
      if (part.substr(0, 4) === 'fbq:') {
        readPixelFragment(part.substring(4));
      } else {
        new_parts.push(part);
      }
    }
    return new_parts.join(';');
  };

  send = function send() {
    var e, events, j, len, pixelId, ref;
    ref = Drupal.settings.campaignion_facebook_pixel.pixels;
    for (pixelId in ref) {
      events = ref[pixelId];
      fbq('init', pixelId);
      for (j = 0, len = events.length; j < len; j++) {
        e = events[j];
        fbq('trackSingle', pixelId, e);
      }
    }
  };

  if (((ref = Drupal.settings.campaignion_facebook_pixel) != null ? ref.pixels : void 0) == null) {
    Drupal.settings.campaignion_facebook_pixel = {
      pixels: {}
    };
  }

  hash = window.location.hash.substr(1);
  if (hash) {
    newHash = readFragmentParts(hash);
    if (newHash !== hash) {
      window.location.hash = '#' + newHash;
    }
  }

  send();
})();
