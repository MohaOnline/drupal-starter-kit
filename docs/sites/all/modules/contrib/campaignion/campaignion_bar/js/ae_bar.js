/*jslint browser:true, jquery:true, white:true, maxlen:200 */
/*jslint bitwise:false, camelcase:true, curly:true, eqeqeq:true, forin:true,
         immed:true, indent:2, latedef:false, newcap:true, noempty:true,
         nonew:true, undef:true, unused:true, strict:true, trailing:true */
/*jslint evil:true */
/*jslint devel:true */
/*global Drupal:false */

(function ($) {
  'use strict';

  Drupal.behaviors.aeBar = {
    attach: function (context, settings) {
      // we are using aetoolbar (but not in iframes)
      var body = $('body');
      var isInIframe = (window.location != window.parent.location) ? true : false;
      var doNotLoadAeBar = body.hasClass('no-aetoolbar');
      if (!isInIframe && !doNotLoadAeBar) {
        body.addClass('aetoolbar');

        $('#ae-bar').once('aebar', function() {
          // tooltips
          $('#ae-bar .tooltipped, #ae-menu-show .tooltipped').tooltipsy({
            offset: [0, 10]
          });

          Drupal.aeBar.init();

          // call window.load (ie. when all images are loaded) to set
          // the correct calculated sizes
          $(window).load(function () {
            setTimeout(Drupal.aeBar.setDimensions, 1);
          });
        });
      }
    }
  };

  Drupal.aeBar = Drupal.aeBar || {};

  /**
   * variables
   */
  Drupal.aeBar.barHeightAbsolute = '62px'; // 61px + 1px border
  Drupal.aeBar.sideWidth = '320px';
  Drupal.aeBar.animationDuration = 200;

  /**
    * initialize the toolbar on page load/reload.
    */
  Drupal.aeBar.init = function () {
    var hidden;
    var sideState;

    Drupal.aeBar.setDimensions();
    // update heights/widths
    $("#ae-popups").css('min-height', $(window).height() - 62 + 'px');
    $("#ae-popups").css('height', Drupal.aeBar.sideHeight);

    // stop following links of bar buttons
    // bind click handlers to toolbar buttons
    $('#ae-menu-for-manage').click(function () {
      Drupal.aeBar.toggleSide('manage', {
        'animate': true
      });
    });
    $('#ae-menu-for-monitor').click(function () {
      Drupal.aeBar.toggleSide('monitor', {
        'animate': true
      });

    });
    $('#ae-menu-for-new').click(function () {
      Drupal.aeBar.toggleWide('new', {
        'animate': true
      });
    });

    // bind toggling to show/hide buttons
    $('#ae-menu-show').click(function () {
      Drupal.aeBar.showBar({
        'animate': true
      });
    });
    $('#ae-menu-hide').click(function () {
      Drupal.aeBar.hideBar({
        'animate': true
      });
    });

    // enable sliding menus
    $('#ae-popup-monitor').slidingMenus();
    $('#ae-popup-manage').slidingMenus();

    // TODO margin with overlay + admin toolbar

    // on resize recalculate sideHeight
    $(window).bind('resize', Drupal.aeBar.setDimensions);

    // set the state to the new (sub)menu when we navigate inside the sidemenu
    $('li.has-submenu a, li.menu-back a').bind('click', function (e) {
      Drupal.aeBar.setState('sidepopup', $(e.target).attr('id').substr(15));
    });

    // TODO on hash change
    // this is hardcoded :/ this is baaaad...
    $(window).bind('hashchange', function () {
      if ($.bbq.getState().hasOwnProperty('monitor')) {
        if (!$('#ae-menu-for-monitor').hasClass('active')) {
          //                    Drupal.aeBar.clearView();
          //                    Drupal.aeBar.showSide('monitor', {animate: false});
        }
      }
      if ($.bbq.getState().hasOwnProperty('manage')) {
        if (!$('#ae-menu-for-manage').hasClass('active')) {
          //                    Drupal.aeBar.clearView();
          //                    Drupal.aeBar.showSide('manage', {animate: false});
        }
      }
    });

    // reset the state
    // should the aeBar be visiible? check state
    hidden = Drupal.aeBar.getState('hidden');
    sideState = Drupal.aeBar.getState('sidepopup');

    if (hidden && hidden === '1') {
      Drupal.aeBar.hideBar();
    } else { // show
      Drupal.aeBar.showBar();
      if (sideState && sideState.length > 0) {
        Drupal.aeBar.showSide(sideState, {animation: false});
      }
    }

    // set right size after fully loaded images, etc
    //setTimeout(Drupal.aeBar.setDimensions, 500);


    // clear new popup active/active-trail if an overlay is opening
    $(document).bind('drupalOverlayOpen', function() {
      Drupal.aeBar.hideWide('all');
      $('#ae-menu-for-new').removeClass('active');
      $('#ae-menu-for-new a').removeClass('active');
    });
  };

  /**
   * sets/updates the dimension of the bar, e.g. after resize.
   */
  Drupal.aeBar.setDimensions = function () {
    // what to resize
    var myContainer = $('#ae-popups');
    var borderWidth = 1;

    // get current dimensions
    Drupal.aeBar.barHeight = parseFloat(Drupal.aeBar.barHeightAbsolute) / parseFloat($('#ae-bar').css('font-size'));
    Drupal.aeBar.topPadding = parseFloat(Drupal.aeBar.barHeight) * parseFloat($('#ae-bar').css('font-size')) / parseFloat($('body').css('font-size')) + 'em';
    Drupal.aeBar.minSideHeight = $(window).height() - parseFloat(Drupal.aeBar.barHeightAbsolute) + 'px'; // window - (bar+border)
    Drupal.aeBar.sideFontSize = parseFloat($('#ae-popups').css('font-size'), 10);
    Drupal.aeBar.sideHeight = ($(document).height() / Drupal.aeBar.sideFontSize) - Drupal.aeBar.barHeight +  'em'; // window - (bar+border)

    myContainer.css('height', Drupal.aeBar.sideHeight);
    myContainer.css('min-height', $(window).height() - Drupal.aeBar.barHeight - borderWidth + 'px');
  };

  /**
   * set a cookie to remember states by a key
   * setState considered friendly as it should not be used with any
   * user input as arguments
   *
   * @param {string} key the key.
   * @param {string} state the state to save e.g. fragment id to remember, or ''.
   */
  Drupal.aeBar.setState = function (key, state) {
    // from admin module: admin.toolbar.js
    var existing = Drupal.aeBar.getState(key);
    if (existing !== state) {
      Drupal.aeBar.state[key] = state;
      var query = [], i;
      for (i in Drupal.aeBar.state) {
        query.push(i + '=' + Drupal.aeBar.state[i]);
      }
      $.cookie('DrupalAeBar', query.join('&'), {
        expires: 7,
        path: '/'
      });
    }
  };

  /**
   * get the state by key which is saved in a cookie
   *
   * @param {string} key the key for the saved state to query.
   * @return {string} the saved state.
   */
  Drupal.aeBar.getState = function (key) {
    // from admin module: admin.toolbar.js
    if (!Drupal.aeBar.state) {
      Drupal.aeBar.state = {};
      var cookie = $.cookie('DrupalAeBar');
      var query = cookie ? cookie.split('&') : [];
      if (query) {
        for (var i in query) {
          // Extra check to avoid js errors in Chrome, IE and Safari when
          // combined with JS like twitter's widget.js.
          // See http://drupal.org/node/798764.
          if (typeof(query[i]) == 'string' && query[i].indexOf('=') != -1) {
            var values = query[i].split('=');
            if (values.length === 2) {
              Drupal.aeBar.state[values[0]] = values[1];
            }
          }
        }
      }
    }
    return Drupal.aeBar.state[key] ? Drupal.aeBar.state[key] : false;
  };

  /**
   * TODO clears the view.
   * closes open sidebars (saves state), widebars, active classes.
   */
  Drupal.aeBar.clearView = function () {
    // remove all other .active classes
    $('#ae-menu li, #ae-menu a').each(function () {
      $(this).removeClass('active').blur();
    });
    Drupal.aeBar.hideSide('all', {animate: false, clearState: false});
    $('#ae-popups div').each(function () {
      $(this).hide();
    });
    $('#ae-popups').hide();
    $('#ae-widepopups').hide();
    // $('#ae-bar .button-wide').blur();
  };

  /**
   * shows the top bar.
   *
   * @param {Object} options the options for the animation
   */
  Drupal.aeBar.showBar = function (options) {
    var defaults = {
      'animate': false
    };
    var settings = $.extend({}, defaults, options);

    if (settings.animate === true) {
      $('body.aetoolbar').animate({
        paddingTop: Drupal.aeBar.topPadding
      }, 200, 'easeOutExpo');
      $('#ae-bar').slideDown(200, 'easeOutExpo', function () {
        $('#ae-menu-show').fadeOut(200);
      });
    } else {
      $('body.aetoolbar').css({
        paddingTop: Drupal.aeBar.topPadding
      });
      $('#ae-bar').show();
      $('#ae-menu-show').hide();
    }

    this.setState('hidden', '0');

    Drupal.aeBar.showSide('last', {animate: settings.animate});
  };

  /**
   * hides the top bar.
   *
   * @param {Object} options the options for the animation
   */
  Drupal.aeBar.hideBar = function (options) {
    var defaults = {
      'animate': false
    };
    var settings = $.extend({}, defaults, options);

    if (settings.animate === true) {
      $('#ae-bar').slideUp(200, 'easeInCubic', function () {
        $('#ae-menu-show').fadeIn(200);
      });
      $('body.aetoolbar').animate({
        paddingTop: '0px'
      }, 200, 'easeInCubic');
    } else {
      $('body.aetoolbar').css({
        paddingTop: '0px'
      });
      $('#ae-bar').hide();
      $('#ae-menu-show').show();
    }

    this.setState('hidden', '1');

    Drupal.aeBar.hideSide('all', {animate: false, clearState: false});
    Drupal.aeBar.hideWide('all', {animate: false});
  };

  /**
   * toggles the visibility of a sidepopup
   *
   * @param {string} which the name of the sidepopup to toggle.
   * @param {Object} options the options for the animation
   */
  Drupal.aeBar.toggleSide = function (which, options) {
    var defaults = {
      'animate': false
    };
    var settings = $.extend({}, defaults, options);
    var myButton = $('#ae-menu-for-' + which);

    if (myButton.hasClass('active')) {
      this.hideSide(which, settings);
    } else {
      this.showSide(which, settings);
      this.hideWide('all', {animate: false});
    }
  };


  /**
   * shows a side bar or the last on.
   *
   * @param {string} which the name of the sidepopup to toggle or 'last'.
   * @param {Object} options the options for the animation
   */
  Drupal.aeBar.showSide = function (which, options) {
    var defaults = {
      'animate': false
    };

    // set which to the state if 'last' is specified
    if (which === '') {
      return;
    } else if (which === 'last') {
      which = Drupal.aeBar.getState('sidepopup');
      if (!(which && which.length > 0)) {
        return;
      }
    }

    var settings = $.extend({}, defaults, options);
    var myContainer = $('#ae-popups');
    var myMenu = $('#ae-popup-' + which);
    // var popups = $('#ae-popups .popup');
    var anyPopupOpen = $('#ae-popups .sidepopup:visible').length;
    // the manage or monitor sidemenu
    var sideMenu = myMenu.closest('.sidepopup');
    var sideMenuName = (sideMenu.length > 0) ? sideMenu.attr('id').substr(9) : '';

//    var barHeight = $('#ae-bar').height();
//    $('#ae-popups, #ae-widepopups').css('top', barHeight + 1 + 'px');

    // if the which is the same as the sideMenuName we want the toplevel (sub)menu
    if (sideMenuName === which) {
      myMenu = $('#ae-popup-' + sideMenuName + '-toplevel-menu');
    }
    var myButton = $('#ae-menu-for-' + sideMenuName);
    // var myId = myButton.attr('id');

    // remove all other .active classes
    $('#ae-menu li, #ae-menu a').each(function () {
      $(this).removeClass('active');
    });
    $('#ae-popups div').each(function () {
      $(this).hide();
    });

    if (myButton.length > 0 && myMenu.length > 0) { // a valid identifier
      // set the heights
      myContainer.css('min-height', Drupal.aeBar.minSideHeight);
      //myContainer.css('height', DaDrupal.aeBar.sideHeight);
//      myContainer.css('height', $('html').height() - 63 + 'px');
      myContainer.css('width', Drupal.aeBar.sideWidth);

      // open container
      // checking whether another sidepopup is visible
      // then subsitute the already open sidepopup
      if (!(anyPopupOpen > 0)) { // if sidebar not open
        if (settings.animate === true) {
          myContainer.show('slide', {
            direction: 'left'
          }, 200);
          $('body.aetoolbar').animate({
            paddingLeft: this.sideWidth
          }, 200, 'easeInQuint', function () {
            myButton.addClass('active');
            myButton.find('a').toggleClass('active');
          });
        } else {
          $('body.aetoolbar').css('padding-left', this.sideWidth);
          myContainer.show();
          myButton.addClass('active');
          myButton.find('a').toggleClass('active');
        }
      } else {
        // set button active
        myButton.addClass('active');
        myButton.find('a').addClass('active');
      }

      // show side (first hide all submenus, then show the right one
      sideMenu.children('.menu').hide();
      sideMenu.show();
      myMenu.show();

      // set state
      this.setState('sidepopup', which);
    }
  };

  /**
   * hides a side bar or all.
   *
   * @param {string} which the name of the sidepopup to toggle or 'all'.
   * @param {Object} options the options for the animation
   */
  Drupal.aeBar.hideSide = function (which, options) {
    var defaults = {
      'animate': false,
      'clearState': true
    };
    var settings = $.extend({}, defaults, options);
    var myButton = $('#ae-menu-for-' + which);
    var myContainer = $('#ae-popups');
    var myMenu = $('#ae-popup-' + which);
    $('#ae-bar .active').each(function () {
      $(this).removeClass('active').blur();
    });

    // hide container
    if (which === 'all' || (myButton.length > 0 && myMenu.length > 0)) { // a valid identifier
      // set the heights
      myContainer.css('min-height', Drupal.aeBar.minSideHeight);
      //myContainer.css('height', Drupal.aeBar.sideHeight);
      myContainer.css('height', Drupal.aeBar.sideHeight);
      myContainer.css('width', Drupal.aeBar.sideWidth);

      if (settings.animate === true) {
        myContainer.hide('slide', {
          direction: 'left'
        }, 200);
        $('body.aetoolbar').animate({
          paddingLeft: '0px'
        }, 200, 'easeOutQuint');
      } else {
        myContainer.hide();
        $('body.aetoolbar').css({
          paddingLeft: '0px'
        });
      }
    }

    if (myButton.length > 0 && myMenu.length > 0) { // a valid identifier
      myMenu.hide();
    }

    // clear the state
    if (settings.clearState === true) {
      this.setState('sidepopup', '');
    }
  };

  /**
   * toggles the visibility of a widepopup
   *
   * @param {string} which the name of the widepopup to toggle.
   * @param {Object} options the options for the animation
   */
  Drupal.aeBar.toggleWide = function (which, options) {
    var defaults = {
      'animate': false
    };
    var settings = $.extend({}, defaults, options);
    var myButton = $('#ae-menu-for-' + which);

    if (myButton.hasClass('active')) {
      this.hideWide(which, settings);
      this.showSide('last', {animate: false});
    } else {
      this.hideSide('all', {animate: false, clearState: true});
      this.showWide(which, settings);
    }
  };

  /**
   * shows a widepopup or the last one.
   *
   * @param {string} which the name of the widepopup to show or 'last'.
   * @param {Object} options the options for the animation
   */
  Drupal.aeBar.showWide = function (which, options) {
    var defaults = {
      'animate': false
    };

    var settings = $.extend({}, defaults, options);
    var myButton = $('#ae-menu-for-' + which);
    // var myId = myButton.attr('id');
    var myContainer = $('#ae-widepopups');
    var myMenu = $('#ae-popup-' + which);

    if (myButton.length > 0 && myMenu.length > 0) { // a valid identifier
      myButton.addClass('active');
      myButton.find('a').addClass('active');

      // TODO animate
      if (settings.animate === true) {
        myContainer.slideDown(500);
      } else {
        myContainer.show();
      }

      myMenu.show();

      // show the campaignion-dialog-wrapper as grayed out area
      $('.campaignion-dialog-wrapper').show().addClass('visible');
    }
  };

  /**
   * hides a widepopup or all.
   *
   * @param {string} which the name of the widepopup to hide or 'all'.
   * @param {Object} options the options for the animation
   */
  Drupal.aeBar.hideWide = function (which, options) {
    var defaults = {
      'animate': false
    };

    var settings = $.extend({}, defaults, options);
    var myButton = $('#ae-menu-for-' + which);
    // var myId = myButton.attr('id');
    var myContainer = $('#ae-widepopups');
    var myMenu = $('#ae-popup-' + which);

    if (which === 'all' || (myButton.length > 0 && myMenu.length > 0)) { // a valid identifier
      if (myButton.hasClass('active')) {
        myButton.removeClass('active').blur();
        myButton.find('a').removeClass('active').blur();
      }

      myMenu.hide();

      // TODO animate
      if (settings.animate === true) {
        myContainer.hide();
      } else {
        myContainer.hide();
      }

      // hide the campaignion-dialog-wrapper again
      $('.campaignion-dialog-wrapper').hide().removeClass('visible');
    }
  };
}(jQuery));
