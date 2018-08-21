function elfinder_ffs_callback(arg1) {

          var fieldName = Drupal.settings.elfinder.field_name;
          var fieldId = Drupal.settings.elfinder.filepath_id;

          var url = arg1;

          if (typeof arg1 == 'object') {
            url = arg1.url;
          }
          
          var filePath = url;
          
          /* Needs rework: must support both classic single file selection and multiple selection */
          //var filePath =  arg1.join('%%');

          window.opener.jQuery('input#'+fieldId).val(filePath).change();
          window.opener.focus();

          // Avoid beforeunload event when selecting an image.
          // https://github.com/Studio-42/elFinder/issues/1340
          // Maybe remove this when elfinder js library gets updated.
          //$(window).off('beforeunload');

          window.close();
}
