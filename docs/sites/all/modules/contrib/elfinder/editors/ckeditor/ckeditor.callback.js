

function elfinder_ckeditor_callback(arg1) {

  var url = arg1;

  if (typeof arg1 == 'object') {
    url = arg1.url;
  }
  funcNum = window.location.search.replace(/^.*CKEditorFuncNum=(\d+).*$/, "$1");
  window.opener.CKEDITOR.tools.callFunction(funcNum, url, function() {
    // adapted from http://docs.ckeditor.com/#!/guide/dev_file_browser_api
    var dialog = this.getDialog();
    if (dialog.getName() == 'link' ) {
      var element = dialog.getContentElement('info', 'linkDisplayText');
      var display_text = element.getValue();
      // If display text is blank, insert the filename.
      if (element && !display_text) {
        element.setValue(arg1.name);
      }
    }
  });

  // Avoid beforeunload event when selecting an image.
  // See https://github.com/Studio-42/elFinder/issues/1340
  // Maybe remove this when elfinder js library gets updated.
  $(window).off('beforeunload');

  window.close();
}
