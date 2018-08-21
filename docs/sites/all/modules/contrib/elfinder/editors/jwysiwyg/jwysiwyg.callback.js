/*
 * elFinder Integration
 *
 * Copyright (c) 2010-2018, Alexey Sukhotin. All rights reserved.
 */

function elfinder_jwysiwyg_callback(url) {
  window.opener.jQuery('.ui-dialog input[type=text][name=url]').val(url);
  window.close();
}