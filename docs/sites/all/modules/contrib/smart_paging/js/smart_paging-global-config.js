// $Id$

(function ($) {
  Drupal.behaviors.smartPagingGlobalConfig = {
    attach: function (context, settings) {
      // Make sure "Allow smart paging to set the canonical link" is checked if
      // "Use the unpaged version as canonical" is enabled
      $(':input[name="smart_paging_use_nopaging_canonical"]', context).click(function (e) {
        if ($(this).is(':checked')) {
          $(':input[name="smart_paging_use_link_canonical"]', context).attr('checked', true);
        }
      });
      // Uncheck the "Use the unpaged version as canonical" option if
      // "Allow smart paging to set the canonical link" is disabled
      $(':input[name="smart_paging_use_link_canonical"]', context).click(function (e) {
        if (!$(this).is(':checked')) {
          $(':input[name="smart_paging_use_nopaging_canonical"]', context).attr('checked', false);
        }
      });
    }
  };
})(jQuery);