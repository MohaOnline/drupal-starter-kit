(function ($) {

Drupal.wysiwyg.plugins['smart_paging'] = {

  /**
   * Return whether the passed node belongs to this plugin.
   */
  isNode: function(node) {
    return ($(node).is('img.smart-paging'));
  },

  /**
   * Execute the button.
   */
  invoke: function(data, settings, instanceId) {
    if (data.format == 'html') {
      var content = this._getPlaceholder(settings);
    }
    else {
      var content = settings.smartPagingPagebreak;
    }
    if (typeof content != 'undefined') {
      Drupal.wysiwyg.instances[instanceId].insert(content);
    }
  },

  /**
   * Replace all <!--pagebreak--> tags with images.
   */
  attach: function(content, settings, instanceId) {
    // @see http://drupal.org/node/510552#comment-3879096
    // @todo Update this technique if that thread produces a better way to do
    // image replacement.

    var smartPagingPagebreak = settings.smartPagingPagebreak;
    var placeholder = this._getPlaceholder(settings);

    // Some WYSIWYGs (CKEditor) will strip the slash from single tags:
    // <foo /> becomes <foo>
    var pagebreak = smartPagingPagebreak.replace(/\/>/, '/?>').replace(/ /g, ' ?');

    // Remove unnecessary paragraph.
    var pattern = new RegExp('<p>' + pagebreak + '</p>', 'ig');
    content = content.replace(pattern, placeholder);
    // Move breaks starting at the beginning of paragraphs to before them.
    pattern = new RegExp('<p>' + pagebreak + '(<[^p])', 'ig');
    content = content.replace(pattern, placeholder + '<p>$1');
    // Move breaks starting at the end of to after the paragraphs.
    pattern = new RegExp('([^p]>)' + pagebreak + '<\/p>', 'ig');
    content = content.replace(pattern, '$1</p>' + placeholder);
    // Other breaks.
    pagebreak =  new RegExp(pagebreak, 'g');
    content = content.replace(pagebreak, placeholder);

    return content;
  },

  /**
   * Replace images with <!--pagebreak--> tags in content upon detaching editor.
   */
  detach: function(content, settings, instanceId) {
    // @see http://drupal.org/node/510552#comment-3879096
    // @todo Update this technique if that thread produces a better way to do
    // image replacement.

    var smartPagingPagebreak = settings.smartPagingPagebreak;

    // Some WYSIWYGs (CKEditor) will strip the slash from single tags:
    // <foo /> becomes <foo>
    var pagebreak = smartPagingPagebreak.replace(/\/>/, '/?>').replace(/ /g, ' ?');

    //   console.log('original:\n'+content);
    // Replace (duplicate) placeholders within p tags with a single break.
    var newContent = content.replace(/\s*<p[^>]*?>(?:\s*<img(?:\s*\w+=['"][^'"]*?['"]\s*)*?\s*class=['"][^'"]*?smart-paging[^'"]*?['"]\s*(?:\s*\w+=['"][^'"]*?['"]\s*)*?(?:\/)?>\s*)+<\/p>\s*/ig, smartPagingPagebreak);
    //    console.log('1\n'+newContent);
    // Replace all other placeholders.
    newContent = newContent.replace(/<img(?:\s*\w+=['"][^'"]*?['"]\s*)*?\s*class=['"][^'"]*?smart-paging[^'"]*?['"]\s*(?:\s*\w+=['"][^'"]*?['"]\s*)*?(?:\/)?>/ig, smartPagingPagebreak);
    //    console.log('2\n'+newContent);
    // Fix paragraphs opening just before breaks.
    var pattern = new RegExp('(?:' + pagebreak + ')*(<p[^>]*?>\s*)' + pagebreak, 'ig');
    newContent = newContent.replace(pattern, smartPagingPagebreak + '$1');
    //    console.log('3\n'+newContent);
    // Remove duplicate breaks and any preceding whitespaces.
    pattern = new RegExp('(?:\s*' + pagebreak + '){2,}' + pagebreak, 'ig');
    newContent = newContent.replace(pattern, smartPagingPagebreak);
    //    console.log('4\n'+newContent);
    // Fix paragraphs ending after breaks.
    pattern = new RegExp(pagebreak + '(\s*<\/p>)(?:' + pagebreak + ')*', 'ig');
    newContent = newContent.replace(pattern, '$1' + smartPagingPagebreak);
    //    console.log('5\n'+newContent);
    // Remove duplicate breaks with trailing whitespaces.
    pattern = new RegExp('(?:' + pagebreak + '\s*){2,}', 'ig');
    newContent = newContent.replace(pattern, smartPagingPagebreak);
    //    console.log('done\n'+newContent);
    return newContent;

  },

  /**
   * Helper function to return a HTML placeholder.
   */
  _getPlaceholder: function (settings) {
    return '<img src="' + settings.path + '/images/spacer.gif" alt="&lt;--pagebreak--&gt;" title="&lt;--pagebreak--&gt;" class="smart-paging drupal-content" />';
  }
};

})(jQuery);
