// $Id: smart_paging-admin.js,v 1.1 2010/09/18 04:00:30 arpeggio Exp $

(function ($) {
  Drupal.behaviors.smartPagingPager = {
    attach: function (context, settings) {
      if ($('.smart-paging-pager-box').html() == undefined) {
        var custom_url_page = settings.smart_paging.custom_url_page;
        var current_page  = settings.smart_paging.current_page;
        var first_element = settings.smart_paging.first_element;
        var path_prefix   = settings.smart_paging.path_prefix;
        var first_page = $('.smart-paging-pager .pager-first a', context).attr('href');
        var last_page  = $('.smart-paging-pager .pager-last a', context).attr('href');
        var base_url   = settings.smart_paging.base_url;
        var current_url = settings.smart_paging.current_url;
        var total_page;
        var js_box     = '<li><select class="smart-paging-pager-box">';
        if (first_page != undefined) {
          first_page = first_page.split('?')[0];
        }
        if (last_page != undefined) {
          last_page = last_page.split('?')[0];
        }
        total_page = last_page == undefined ? current_page : last_page.replace(/.*\//i, '');
        for (var index in custom_url_page) {
          total_page = total_page.replace(('#' + custom_url_page[index]), '');
        }
        for (var i = 0; i <= total_page; ++i) {
          var value = '/' + path_prefix + '/' + first_element + '/' + i + ((custom_url_page[i] == undefined) ? '' : ('#' + custom_url_page[i]));
          var selected = '';
          if (i == 0 && first_element == 0) {
            value = '';
          }
          if (i == current_page) {
            selected = 'selected';
          }
          else {
            selected = '';
          }
          js_box += '<option ' + selected + ' value="' + base_url + '/' + current_url + value + '">' + Drupal.t('Page: ') + (i + 1) + '</option>';
        }
        js_box += '</select></li>';
        $('.smart-paging-pager .pager-item, .smart-paging-pager .pager-current, .smart-paging-pager .pager-ellipsis').remove();
        if (first_page == undefined) {
          $('.smart-paging-pager .pager-next').before(js_box);
        }
        else {
          $('.smart-paging-pager .pager-previous').after(js_box);
        }
        $('.smart-paging-pager-box').bind('change', function () {
          window.location = $(this).val();
        });
      }
    }
  };
})(jQuery);