/*
 * elFinder Integration
 *
 * Copyright (c) 2010-2018, Alexey Sukhotin. All rights reserved.
 */

function elfinder_drupalnodeattach_callback(url) {
  parent.jQuery('input#edit-attach-url').val(url);
  alert(url);
}