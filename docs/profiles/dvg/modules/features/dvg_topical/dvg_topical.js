(function ($) {
  Drupal.behaviors.dvgBlockScript = {
    attach: function (context, settings) {

      var blocks = $('.topical-block', context);
      var menus = blocks.find('.content .view-content .item-list > ul');

      $.each(menus, function(index, item) {
          var column1 = $('<ul class="topical two-column"></ul>');
          var column2 = column1.clone();
          var menu = $(item);

          menu.find('li.views-row-even').clone().appendTo(column2);
          menu.find('li.views-row-odd').clone().appendTo(column1);
          menu.parent().append(column1, column2);
          menu.addClass('one-column');
      });

      function switchColumns(blocks, menus) {
        if (menus.find('li').css('float') == 'none') {
          blocks.find('.one-column').show();
          blocks.find('.two-column').hide();
        } else {
          blocks.find('.one-column').hide();
          blocks.find('.two-column').show();
        }
      }

      switchColumns(blocks, menus);

      $('body').once('dvg-topical-resize', function () {
        $(window).resize(function () {
          switchColumns(blocks, menus);
        });
      });
    }
  }
}(jQuery));
