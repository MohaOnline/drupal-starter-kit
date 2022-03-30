Drupal.TBMegaMenu = Drupal.TBMegaMenu || {};

(function ($) {
  Drupal.TBMegaMenu.createTouchMenu = function(items) {
    items.children('a, .tb_nolink').each( function() {
      var $item = $(this);
      var tbitem = $(this).parent();

      $item.click( function(event){
        if ($item.hasClass('tb-megamenu-clicked')) {
          var $uri = $item.attr('href');
          if ($uri && $uri !== '#') {
            window.location.href = $uri;
          }
        }
        else {
          event.preventDefault();
          $item.addClass('tb-megamenu-clicked');
          if(!tbitem.hasClass('open')){	
            tbitem.addClass('open');
          }

          // Find any parent siblings that are open and close them.
          tbitem.siblings('.open').find('.tb-megamenu-clicked').removeClass('tb-megamenu-clicked');
          tbitem.siblings('.open').removeClass('open');

          $('body').addClass('tb-megamenu-open');
        }
      });
    });
  }
  
  Drupal.TBMegaMenu.eventStopPropagation = function(event) {
    if (event.stopPropagation) {
      event.stopPropagation();
    }
    else if (window.event) {
      window.event.cancelBubble = true;
    }
  }

  Drupal.behaviors.tbMegaMenuTouchAction = {
    attach: function(context) {
      var isTouch = window.matchMedia('(pointer: coarse)').matches;
      if(isTouch){
        $('html').addClass('touch');
        Drupal.TBMegaMenu.createTouchMenu($('.tb-megamenu ul.nav li.mega').has('.dropdown-menu'));

        // When the user touches anywhere outside of the open menu item, close
        // the open menu item.
        $(document).on('touchstart', function(event) {
          if ($('body').hasClass('tb-megamenu-open') && !$(event.target).closest('.mega.open').length) {
            $('.tb-megamenu ul.nav li.mega a, .tb-megamenu ul.nav li.mega .tb_nolink').removeClass('tb-megamenu-clicked');
            $('.tb-megamenu ul.nav li.mega').removeClass('open');
            $('body').removeClass('tb-megamenu-open');
         }
       });
      }
    }
  }
})(jQuery);
