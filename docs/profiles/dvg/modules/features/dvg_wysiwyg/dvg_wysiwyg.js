(function ($) {
  $(function () {
    // Disable auto-conversion of url-text to links for ie9 and up...
    if (typeof CKEDITOR != 'undefined' && CKEDITOR.env.ie && CKEDITOR.env.version >= 9) {
      document.execCommand('AutoUrlDetect', false, false);
    }
  });
})(jQuery);
