(function () {

  'use strict';

  Drupal.behaviors.quicklink = {
    attach: function attachQuicklink(context, settings) {
      var debug = settings.quicklink.debug;

      function hydrateQuicklinkConfig() {
        settings.quicklink.quicklinkConfig = settings.quicklink.quicklinkConfig || {};
        settings.quicklink.ignoredSelectorsLog = settings.quicklink.ignoredSelectorsLog || [];

        var quicklinkConfig = settings.quicklink.quicklinkConfig;

        quicklinkConfig.ignores = [];

        // Loop through all the patters to ignore, and generate rules to ignore URL patterns.
        for (var i = 0; i < settings.quicklink.url_patterns_to_ignore.length; i++) {
          var pattern = settings.quicklink.url_patterns_to_ignore[i];

          (function (i, pattern) {
            if (pattern.length) {
              quicklinkConfig.ignores.push(function (uri, elem) {
                var ruleName = 'Pattern found in href. See ignored selectors log.';
                var ruleFunc = uri.includes(pattern);

                outputDebugInfo(ruleFunc, ruleName, uri, elem, pattern);

                return ruleFunc;
              });
            }
          })(i, pattern);
        }

        if (settings.quicklink.ignore_admin_paths) {
          var adminLinkContainerPatterns = settings.quicklink.admin_link_container_patterns.join();

          quicklinkConfig.ignores.push(function (uri, elem) {
            var ruleName = 'Exists in admin element container.';
            var ruleFunc = elem.matches(adminLinkContainerPatterns);

            outputDebugInfo(ruleFunc, ruleName, uri, elem);

            return ruleFunc;
          });
        }

        if (settings.quicklink.ignore_ajax_links) {
          quicklinkConfig.ignores.push(function (uri, elem) {
            var ruleName = 'Link has "use-ajax" CSS class.';
            var ruleFunc = elem.classList.contains('use-ajax');

            outputDebugInfo(ruleFunc, ruleName, uri, elem);

            return ruleFunc;
          });

          quicklinkConfig.ignores.push(function (uri, elem) {
            var ruleName = 'Link has "/ajax" in url.';
            var ruleFunc = uri.includes('/ajax');

            outputDebugInfo(ruleFunc, ruleName, uri, elem);

            return ruleFunc;
          });
        }

        if (settings.quicklink.ignore_file_ext) {
          quicklinkConfig.ignores.push(function (uri, elem) {
            var ruleName = 'Contains file extension at end of href.';
            var ruleFunc = uri.match(/(\.[^\/]{1,5}\?)|(\.[^\/]{1,5}$)/);

            outputDebugInfo(ruleFunc, ruleName, uri, elem);

            return ruleFunc;
          });
        }

        quicklinkConfig.ignores.push(function (uri, elem) {
          var ruleName = 'Contains noprefetch attribute.';
          var ruleFunc = elem.hasAttribute('noprefetch');

          outputDebugInfo(ruleFunc, ruleName, uri, elem);

          return ruleFunc;
        });

        quicklinkConfig.ignores.push(function (uri, elem) {
          var ruleName = 'Contains download attribute.';
          var ruleFunc = elem.hasAttribute('download');

          outputDebugInfo(ruleFunc, ruleName, uri, elem);

          return ruleFunc;
        });

        quicklinkConfig.origins = (settings.quicklink.allowed_domains) ? settings.quicklink.allowed_domains : false;
        quicklinkConfig.urls = (settings.quicklink.prefetch_only_paths) ? settings.quicklink.prefetch_only_paths : false;
      }

      function outputDebugInfo(ruleFunc, ruleName, uri, elem, pattern) {
        if (debug && ruleFunc) {
          var debugMessage = ruleName + ' Link ignored.';
          var thisLog = {};
          var urlPattern = pattern || false;

          elem.classList.add('quicklink-ignore');
          elem.textContent += '🚫';
          elem.dataset.quicklinkMatch = debugMessage;

          thisLog.ruleName = ruleName;
          thisLog.uri = uri;
          thisLog.elem = elem;
          thisLog.message = debugMessage;

          if (urlPattern) {
            thisLog.urlPattern = urlPattern;
          }

          (function (thisLog) {
            settings.quicklink.ignoredSelectorsLog.push(thisLog);
          })(thisLog);
        }
      }

      function loadQuicklink() {
        var urlParams = new URLSearchParams(window.location.search);
        var noprefetch = urlParams.get('noprefetch') !== null || window.location.hash === '#noprefetch' ;

        if (noprefetch && debug) {
          // eslint-disable-next-line no-console
          console.info('The "noprefetch" parameter or hash exists in the URL. Quicklink library not loaded.');
        }

        return window.quicklink && !noprefetch;
      }

      if (!settings.quicklink.quicklinkConfig) {
        hydrateQuicklinkConfig();
      }

      settings.quicklink.quicklinkConfig.el = (settings.quicklink.selector) ? context.querySelector(settings.quicklink.selector) : context;

      if (debug) {
        console.info('Quicklink config object', settings.quicklink.quicklinkConfig); // eslint-disable-line no-console
        console.info('Quicklink module debug log', settings.quicklink.debug_log); // eslint-disable-line no-console
        console.info('Quicklink ignored selectors', settings.quicklink.ignoredSelectorsLog); // eslint-disable-line no-console
      }

      if (loadQuicklink()) {
        quicklink(settings.quicklink.quicklinkConfig);
      }
    }
  };
})();
