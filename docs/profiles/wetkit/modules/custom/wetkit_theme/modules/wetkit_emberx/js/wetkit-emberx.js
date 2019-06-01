(function ($) {
  var controls = '#panels-ipe-control-container';

  $(document).on('mousemove', function () {
    // Show the IPE menu when the mouse is actively in the window.
    $(controls).fadeIn();
  }).on('mouseleave', function () {
    // Hide the menu when the mouse leaves the window.
    $(controls).delay(200).fadeOut();
  });

})(jQuery);
