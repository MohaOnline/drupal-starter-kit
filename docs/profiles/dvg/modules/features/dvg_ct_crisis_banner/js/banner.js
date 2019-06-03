(function ($) {
  Drupal.behaviors.dvgCrisisBanner = {
    attach: function (context, settings) {
      if (Drupal.settings.dvgCrisisBanner && Drupal.settings.dvgCrisisBanner.code) {
        var localstorageKey = 'banner-' + Drupal.settings.dvgCrisisBanner.code;
        var $node = $('#node-' + Drupal.settings.dvgCrisisBanner.nid, context);
        var $closeBtn = $node.find('.banner-close-button');
        var $cookieEnabled = !!navigator.cookieEnabled;

        // Adding a cookie check because Firefox throws a security error when
        // cookies are disabled and localStorage is called.
        if ($cookieEnabled) {
          if ($node.length) {
            var closed = localStorage.getItem(localstorageKey);
            if (typeof closed === 'undefined' || closed == null) {
              $closeBtn.once('dvgCrisisBanner').click(function (e) {
                e.preventDefault();
                closeNodeAction(localstorageKey);
                $('body').removeClass('with-crisis-banner');
                $node.hide();
              });
            }
            else {
              $('body').removeClass('with-crisis-banner');
              $node.hide()
            }
          }
        } else {
          $closeBtn.once('dvgCrisisBanner').click(function (e) {
            e.preventDefault();
            $('body').removeClass('with-crisis-banner');
            $node.hide();
          });
        }
      }

      function closeNodeAction(localstorageKey) {
        localStorage.setItem(localstorageKey, new Date().getTime());
      }
    }
  };
}(jQuery));
