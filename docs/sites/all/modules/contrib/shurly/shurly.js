(function($){
$(document).ready(function(){
  if ($('#edit-result').length) {
    var clip = new ZeroClipboard.Client();
    clip.setText($('#edit-result').val());
    clip.setHandCursor( true );
    clip.glue('shurly-copy', 'shurly-copy-container');
    var origBg = $('#edit-result').css('backgroundColor');
    clip.addEventListener( 'onComplete', function() {
      $('#edit-result').css('backgroundColor', '#FFFF3F').fadeTo(300, .1, function(){
        $(this).fadeTo(1, 1).css('backgroundColor', origBg);
      });
    });
    
    $('#edit-result')
      .focus()
      .focus(function(){
        $(this).select();
      })
      .mouseup(function(){
        // fix for select problem in WebKit
        return false;
      });
  }
});
})(jQuery);
