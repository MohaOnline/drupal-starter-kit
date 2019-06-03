(function($) {

Drupal.behaviors.campaignion_manage_translation_sets = {};
Drupal.behaviors.campaignion_manage_translation_sets.attach = function(context) {
  $('.campaignion-manage-content-listing .node-translations', context).hide().each(function() {
    var $translations = $(this);
    var $tset = $translations.prev();

    var $showHideLink = $('<a class="show" href="#">' + Drupal.t('show translations') + '</a>').click(function(event) {
      event.preventDefault();
      var $showHideLink = $(this);
      if ($translations.is(':visible')) {
        $translations.hide();
        $showHideLink.html(Drupal.t('show translations')).addClass('show').removeClass('hide');
      } else {
        $translations.show();
        $showHideLink.html(Drupal.t('hide translations')).addClass('hide').removeClass('show');
      }
    }).appendTo($tset.find('.campaignion-manage'));
  });
};

})(jQuery);
