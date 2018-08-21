/*
 * elFinder Integration
 *
 * Copyright (c) 2010-2018, Alexey Sukhotin. All rights reserved.
 */

function elfinder_fckeditor_callback(url) {
  window.opener.SetUrl(url);
  window.close();
}