// $Id: smart_paging-config.js,v 1.1 2010/09/18 04:00:30 arpeggio Exp $

(function ($) {
  Drupal.behaviors.smartPagingConfig = {
    attach: function (context, settings) {
      var filter_html_name = settings.smart_paging.filter_html_name;
      if (!settings.smart_paging.text_processing) {
        $('fieldset.smart-paging-settings', context).hide();
        $('.vertical-tabs-list .vertical-tab-button strong', context).filter(function () {
          return this.innerHTML == settings.smart_paging.fieldset_label;
        }).closest('.vertical-tab-button', context).hide();
      }
      for (var i in filter_html_name) {
        $(':input[name="' + filter_html_name[i] + '"]', context).bind('click', function (event) {
          event.stopPropagation();
          $(this, context).bind('change', function (e) {
            var show_config = false;
            for (var filter in settings.smart_paging.smart_paging_filter) {
              if (filter == $(this).attr('value')) {
                show_config = true;
                break;
              }
            }
            if (show_config) {
              $('fieldset.smart-paging-settings', context).show();
              $('.vertical-tabs-list .vertical-tab-button strong', context).filter(function () {
                return this.innerHTML == settings.smart_paging.fieldset_label;
              }).closest('.vertical-tab-button', context).show();
            }
            else {
              $('fieldset.smart-paging-settings', context).hide();
              $('.vertical-tabs-list .vertical-tab-button strong', context).filter(function () {
                return this.innerHTML == settings.smart_paging.fieldset_label;
              }).closest('.vertical-tab-button', context).hide();
            }
          }); 
        });
      }
    }
  };
})(jQuery);