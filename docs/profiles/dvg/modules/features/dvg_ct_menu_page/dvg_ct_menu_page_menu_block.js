(function ($) {
  Drupal.behaviors.dvgMenuPageMenuBlockScript = {
    attach: function (context, settings) {
      var blocks = $('.block-menu-block', context);
      var menus = blocks.find('ul.menu');

      if (menus.length) {
        // Make sure everything has the correct tabindex.
        $('a, input, select, button, textarea').each(function(index) {
          this.tabIndex = index + 1;
        });

        $.each(menus,function(index, item) {
          var column1 = $('<ul class="menu two-column"></ul>');
          var column2 = column1.clone();
          var menu = $(item);

          menu.find('li:odd').clone().appendTo(column2);
          menu.find('li:even').clone().appendTo(column1);
          menu.parent().append(column1, column2);
          menu.addClass('one-column');
        });

        var $oneColumnBlocks = blocks.find('.one-column');
        var $twoColumnBlocks = blocks.find('.two-column');

        function switchColumns() {
          if (menus.find('li:first-child').css('float') == 'none') {
            $oneColumnBlocks.show();
            $twoColumnBlocks.hide();
          }
          else {
            $oneColumnBlocks.hide();
            $twoColumnBlocks.show();
          }
        }

        switchColumns();

        $('body').once('dvg-ct-menu-page-resize', function() {
          $(window).resize(function() {
            switchColumns();
          });
        });
      }
    }
  }
}(jQuery));
