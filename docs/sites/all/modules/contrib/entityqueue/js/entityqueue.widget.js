/**
 * @file
 * Javascript functions for the Entityqueue module.
 */
(function($) {

  Drupal.behaviors.entityqueueWidget = {
    attach: function(context, settings) {
      for (var base in settings.tableDrag) {
        $('#' + base, context).once('entityqueue-tabledrag', function () {
          var tabledrag = Drupal.tableDrag[base],
              changed = settings.entityqueue_changed;

          if (changed && changed.hasOwnProperty(tabledrag.table.id)) {
            tabledrag.changed = true;
            var $warning = $(Drupal.theme('tableDragChangedWarning'));
            $warning.insertBefore(tabledrag.table);
          }
        });
      }
    }
  };

})(jQuery);
