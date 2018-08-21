function elfinder_tinymce_browse_callback(field_name, url, type, win) {

    var cmsURL = tinymce.settings.file_browser_url;    // script URL - use an absolute path!
    if (cmsURL.indexOf("?") < 0) {
        //add the type as the only query parameter
        cmsURL = cmsURL + "?type=" + type;
    } else {
        //add the type as an additional query parameter
        // (PHP session ID is now included if there is one at all)
        cmsURL = cmsURL + "&type=" + type;
    }

    tinyMCE.activeEditor.windowManager.open({
        file: cmsURL,
        title: 'File Manager',
        width: 900,
        height: 450,
        resizable: "yes",
        inline: "yes", // This parameter only has an effect if you use the inlinepopups plugin!
        popup_css: false, // Disable TinyMCE's default popup CSS
        close_previous: "no"
    }, {
        window: win,
        input: field_name
    });
    return false;
}
