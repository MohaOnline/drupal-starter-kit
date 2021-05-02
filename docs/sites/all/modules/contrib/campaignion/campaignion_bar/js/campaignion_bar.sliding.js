/**
 * needs jQuery, jQuery UI, effects.core.js, effects.sliding.js
 * expects to be called on a div container with an id (used to namespace submenu-ids)
 * expects a single ul.menu inside the container
 * expects <a>s inside <li>s inside <ul.menu>s
 * sets ids on ul.menu
 * sets .has-submenu class on li which have submenus
 * creates back li.menu-back>a for menu navigation
 */

(function($) {
  'use strict';
  $.fn.slidingMenus = function() {
    var container = $(this);
    var arraySubmenuIds = []; // storing submenu ids which will be generated

    // namespacing the submenus
    var ns = '';
    ns = container.attr('id');

    // set id on toplevel
    var toplevelMenu = container.children('ul.menu').first();
    toplevelMenu.attr('id', ns + '-toplevel-menu');

    // marking submenus
    var withSubmenu = toplevelMenu.find('li > ul.menu');
    withSubmenu.each(function() {
      $(this).parent().addClass('has-submenu');
    });

    // preparing DOM
    withSubmenu.each(function(index) {
      var me = $(this);
      // generate id for link
      var linkId = ns + '-submenu-' + index;
      me.attr('id', linkId);
      // set link
      me.siblings('a').attr('id', 'go-to-' + linkId);
      // store ids
      arraySubmenuIds.push(linkId);
      // hide submenus
      me.hide();
      // create back links
      var parentMenu = me.parent().closest('ul.menu');
      if (!parentMenu.attr('id')) {
        parentMenu.attr('id', 'toplevel-menu');
      }
      me.prepend('<li class="menu-back"><a id="go-to-' + parentMenu.attr('id') + '" href="">Back</a></li>');
    });

    // move the submenus to the same level as the toplevel menu
    var i, submenuCount;
    for (i = 0, submenuCount = withSubmenu.length; i < submenuCount; i += 1) {
      // move submenu out of toplevel menu
      var submenu = $('#' + arraySubmenuIds[i]).detach();
      toplevelMenu.after(submenu);
    }

    // handlers
    // submenu link
    container.find('li.has-submenu > a').click(function(e) {
      e.preventDefault();
      // get id
      var myId = $(this).attr('id').replace('go-to-', ''); // TODO undef?
      // to hide myself
      $(this).closest('ul.menu').hide('slide', {
        direction: 'left'
      }, 200);
      // to show submenu
      $('#' + myId).show('slide', {
        direction: 'right'
      }, 200);
    });
    // back link
    container.find('li.menu-back > a').click(function(e) {
      e.preventDefault();
      var myId = $(this).attr('id').replace('go-to-', ''); // TODO undef?
      $(this).closest('ul.menu').hide('slide', {
        direction: 'right'
      }, 200);
      $('#' + myId).show('slide', {
        direction: 'left'
      }, 200);
    });
  };
}(jQuery));
