
(function ($) {

Drupal.behaviors.flux = {
  attach: function (context, settings) {
    //START
    $('#block-menu-menu-pushtape h2').click(function(){
      $('#block-menu-menu-pushtape ul.menu:first-child').toggleClass('expanded');
      $(this).toggleClass('open');
    });
    //END
  }
};

}(jQuery));
