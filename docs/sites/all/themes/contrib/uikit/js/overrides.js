/**
 * @file
 * Attaches behaviors to the UIkit theme.
 */

(function ($) {
  'use strict';

  $(function() {
    var navbarLink = $('.uk-navbar-nav [href="#"]');

    // Prevent navbar parent links from being used as a link when clicked.
    navbarLink.click(function (e) {
      e.preventDefault();
    });

    // Initialize alert closing actions.
    UIkit.alert('#messages');
  });

  Drupal.behaviors.uikitComments = {
    attach: function () {
      var comments = $('.uk-comment-list');
      var permalink = comments.find(':regex(id,comment-[0-9])');

      // First move all permalinks inside their list item.
      permalink.each(function () {
        $(this).prependTo($(this).next('li'));
      });

      // Now move the indented comments into the previous list item.
      var indented = comments.find('.indented');
      var comment = '<ul>' + indented.html() + '</ul>';
      var sibling = indented.prev('li');
      $(comment).appendTo(sibling);

      // We don't want the original indented comment, so we remove it.
      $(indented).replaceWith();
    }
  }
})(jQuery);

/**
 * Regular expression selector filter.
 */
jQuery.expr[':'].regex = function(elem, index, match) {
  var matchParams = match[3].split(','),
    validLabels = /^(data|css):/,
    attr = {
      method: matchParams[0].match(validLabels) ?
        matchParams[0].split(':')[0] : 'attr',
      property: matchParams.shift().replace(validLabels,'')
    },
    regexFlags = 'ig',
    regex = new RegExp(matchParams.join('').replace(/^\s+|\s+$/g,''), regexFlags);
  return regex.test(jQuery(elem)[attr.method](attr.property));
};
