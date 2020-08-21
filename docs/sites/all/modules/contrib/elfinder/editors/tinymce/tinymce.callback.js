/*
 * elFinder Integration
 *
 * Copyright (c) 2010-2020, Alexey Sukhotin. All rights reserved.
 */

var elfinder_tinymce_callback = function(params) {
  var url = params;

  if (typeof params == 'object') {
    url = params.url;
  }

  var win;

  if (typeof tinyMCEPopup == 'object') {
    win = tinyMCEPopup.getWindowArg("window");
    win.document.getElementById(tinyMCEPopup.getWindowArg("input")).value = url;

    // are we an image browser
    if (typeof (win.ImageDialog) != "undefined") {
    // we are, so update image dimensions...
      if (win.ImageDialog.getImageData)
        win.ImageDialog.getImageData();

      // ... and preview if necessary
      if (win.ImageDialog.showPreviewImage) {
        win.ImageDialog.showPreviewImage(url);
      }
    }

    tinyMCEPopup.close();
  } else {
    window.parent.postMessage({'selectedFile': params},"*");
  }
}
