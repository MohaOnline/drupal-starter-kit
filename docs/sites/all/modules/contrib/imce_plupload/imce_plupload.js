imce.uploadValidate = function (data, form, options) {  
  jQuery.each(data, function (index) {
    var msg = data[index].name;
    if (msg.indexOf('_name') === -1) return;
    var path = data[index].value;
    if (!path) return;
    if (imce.conf.extensions != '*') {
      var ext = path.substr(path.lastIndexOf('.') + 1);
      if ((' '+ imce.conf.extensions +' ').indexOf(' '+ ext.toLowerCase() +' ') == -1) {
        imce.setMessage(Drupal.t('Only files with the following extensions are allowed: %files-allowed.', {'%files-allowed': imce.conf.extensions}), 'error');
      }
    }
    var sep = path.indexOf('/') == -1 ? '\\' : '/';
    // Make url contain current dir
    options.url = imce.ajaxURL('upload');
    imce.fopLoading('upload', true);
  });
  
  return true;
};

/**
 * Resets the upload form after a succesful upload 
 */
jQuery(document).ready(function() {
  var uploader = jQuery('.plupload-element').pluploadQueue();
  
  uploader.bind('UploadComplete', function() {
    if (uploader.total.uploaded == uploader.files.length) {
      jQuery(".plupload_buttons").css("display", "inline");
      jQuery(".plupload_upload_status").css("display", "inline");
      
      // Give IMCE some time to finish the uploading.
      setTimeout(function () {
        uploader.splice();
      }, 200);
    }
  });
  
});