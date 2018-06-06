// $Id: smart_paging-admin.js,v 1.1.2.2 2010/12/08 22:42:42 arpeggio Exp $

(function ($) {
  Drupal.behaviors.smartPagingAdmin = {
    attach: function (context, settings) {
      $('fieldset.smart-paging-settings', context).drupalSetSummary(function (context) {
        var values = new Array();
        var t = new Array();
        var perm = new Array();
        if ($(':input[name="smart_paging_use_default"]', context).attr('checked')) {
          t['%method'] = settings.smart_paging.default_method;
          values.push(Drupal.t('Using default: %method', t));
        }
        else {
          // Show status of the selected page break method
          t['!method'] = $(':input[name="smart_paging_method"] option[selected=true]', context).text();
          if ($(':input[name="smart_paging_method"]').attr('value') == -1) {
            t['@value'] = '';
          }
          else {
            if ($(':input[name="smart_paging_pagebreak"]').parent().css('display') != 'none') {
              t['@value'] = Drupal.t(' (placeholder="') + $(':input[name="smart_paging_pagebreak"]').attr('value') + '")';
            }
            if ($(':input[name="smart_paging_character_count"]').parent().css('display') != 'none') {
              t['@value'] = ' (' + $(':input[name="smart_paging_character_count"]').attr('value') + ' characters)';
            }
            if ($(':input[name="smart_paging_word_count"]').parent().css('display') != 'none') {
              t['@value'] = ' (' + $(':input[name="smart_paging_word_count"]').attr('value') + ' words)';
            }
          }
          values.push(Drupal.t('!method@value', t));
          if ($(':input[name="smart_paging_title_display_suffix"]').attr('checked')) {
            t['@value'] = $(':input[name="smart_paging_title_suffix"]').attr('value');
            values.push(Drupal.t('Title suffix="@value"', t));
          }
          else {
            values.push(Drupal.t('No title suffix', t));
          }
        }
        
        return values.join(', ');
      });
      if ($(':input[name="smart_paging_pagebreak"]').attr('class').indexOf('error') != -1) {
        $(':input[name="smart_paging_pagebreak"]').parent().show();
      }
      if ($(':input[name="smart_paging_character_count"]').attr('class').indexOf('error') != -1) {
        $(':input[name="smart_paging_character_count"]').parent().show();
      }
      if ($(':input[name="smart_paging_word_count"]').attr('class').indexOf('error') != -1) {
        $(':input[name="smart_paging_word_count"]').parent().show();
      }
    }
  };
})(jQuery);