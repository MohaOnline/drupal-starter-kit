
/**
 * Implementation of Drupal behavior.
 */
(function($) {
Drupal.behaviors.campaignion_manage_bulk = {};
Drupal.behaviors.campaignion_manage_bulk.attach = function(context) {
  $('.campaignion-manage-bulkops', context).each(function() {
    var $wrapper = $(this);
    var $dialog = $wrapper.find('.bulkops');
    var $dialogBg = $('.campaignion-dialog-wrapper');
    var defaultZ = $('.campaignion-dialog-wrapper').css('z-index');

    $dialog.addClass('bulk-dialog');

    // Button for opening the dialog.
    $('<a class="button" id="bulk-edit-button"  href="#">' + Drupal.t('Bulk edit') + '</a>')
    .click(function(e) {
      e.preventDefault();
      e.stopPropagation();

      $wrapper.show(0, function(){
        $('html').css('overflow-y', 'hidden')
      });
      $dialogBg.css('z-index', 500).show();
    }).appendTo($wrapper.parents('form').find('.bulkop-button-wrapper'));

    var $radioWrapper = $wrapper.find('div.bulkops-radios').parent();
    var $radios = $radioWrapper.find('input[type=radio]');
    var $bulkopsWrapper = $wrapper.find('.bulkops-ops');
    var $bulkops = $bulkopsWrapper.find('.bulkops-op');
    var $actions = $wrapper.find('.actions');
    $radios.change(function() {
      $radioWrapper.hide();
      $actions.show();
      $bulkopsWrapper.show();
      $bulkops.hide();
      $bulkopsWrapper.find('.bulkops-op-' + $(this).val()).show();
    });
    var $cancel = $('<input type="button" name="cancel" value="' + Drupal.t('Cancel') + '" class="form-submit" />')
    $cancel.click(function() {
      $radios.prop('checked', false);
      $bulkops.hide();
      $bulkopsWrapper.hide();
      $actions.hide();
      $radioWrapper.show();
    }).appendTo($actions).click();

    // Button to hide the dialog.
    $('<div id="bulk-dialog-close">' + Drupal.t('Close') + '</div>')
    .click(function(e) {
      $wrapper.hide(0, function(){
        $('html').css('overflow-y', 'auto')
      });
      $dialogBg.css('z-index', defaultZ).hide();
      $cancel.click();
    }).appendTo($dialog.children('legend'));

    $radios.siblings('label').after('<span class="bulk-question-mark">?</span>');
    if ($.fn.popover) {
      $radioWrapper.find('.bulk-question-mark').each(function() {
        var $self = $(this);
        var op = $self.siblings('input').attr('value')
        $self.popover({
          content: $wrapper.find('.bulkops-op-' + op).find('.help-text').hide().html(),
        });
      });
    }

    $radioWrapper.on('show.bs.popover', function(e) {
      var $self = $(e.target);
      $radioWrapper.find('.bulk-question-mark').not($self).popover('hide');
    });
  });
};
Drupal.behaviors.campaignion_manage_select_all = {};
Drupal.behaviors.campaignion_manage_select_all.attach = function(context) {
$('.bulkop-select-wrapper', context).each(function() {
  var $wrapper = $(this);
  var $matching = $wrapper.find('.bulkop-select-toggles .form-item-listing-bulkop-select-all-matching');
  $('<div class="form-item form-type-checkbox"><input type="checkbox" id="select_all_toggle" /><label for="select_all_toggle" class="option">' + Drupal.t('Select all') + '</label></div>')
  .insertBefore($matching);
  var $toggle = $wrapper.find('#select_all_toggle');
  $toggle.change(function() {
    if ($(this).prop('checked')) {
      $matching.show();
    }
    else {
      $matching.hide();
    }
  }).change();
  $matching.find('input').change(function() {
    $toggle.prop('disabled', $(this).attr('checked') == 'checked').change();
  });

  var $targets = $wrapper.find('.bulk-select-target');
  $toggle.change(function(event, noprop) {
    noprop = (typeof noprop == 'undefined') ? false : noprop;
    if (noprop) {
      return;
    }
    if ($(this).prop('checked')) {
      $targets.prop('checked', true);
    }
    else {
      $targets.prop('checked', false);
      $matching.prop('checked', false);
    }
  });
  $targets.change(function() {
    if (!$(this).prop('checked')) {
      $toggle.prop('checked', false).trigger('change', true);
    }
  });

  var allCount = $wrapper.attr('data-count');
  var $counter = $wrapper.parents('form').find('.bulkop-count');
  $toggle.add($targets).change(function() {
    var newVal = $targets.filter(':checked').length;
    $counter.html(newVal);
  })
  $toggle.change();
  $matching.change(function() {
    var newVal = $('input', this).prop('checked') ? allCount : $targets.filter(':checked').length;
    $counter.html(newVal);
  });

});
};
})(jQuery);
