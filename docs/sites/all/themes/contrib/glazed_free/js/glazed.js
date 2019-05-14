/**
 * @file
 * A JavaScript file that styles the page with bootstrap classes.
 *
 * @see sass/styles.scss for more info
 */
(function ($, Drupal, window, document, undefined) {
var glazedMenuState = '';

Drupal.behaviors.fullScreenSearch = {
    attach: function(context, settings) {
        function clearSearchForm() {
            $searchForm.toggleClass("hidden"),
            setTimeout(function() {
                $searchFormInput.val("")
            }, 350)
        }
        var $searchButton = $(".full-screen-search-button")
          , $searchForm = $(".full-screen-search-form")
          , $searchFormInput = $searchForm.find(".search-query")
          , escapeCode = 27;
        $searchButton.on("touchstart click", function(event) {
            event.preventDefault(),
            $searchForm.toggleClass("hidden"),
            $searchFormInput.focus()
        }),
        $searchForm.on("touchstart click", function($searchButton) {
            $($searchButton.target).hasClass("search-query") || clearSearchForm()
        }),
        $(document).keydown(function(event) {
            event.which === escapeCode && !$searchForm.hasClass("hidden") && clearSearchForm()
        })
    }
}

Drupal.behaviors.glazed = {
  attach: function(context, settings) {
    var windowHeight = $(window).height();
    glazedMenuGovernor(context);

    // Fix for conditions where Glazed Controls are hidden behind menu
    if (($('#block-system-main .glazed-editor').length)
        && ($('#page-title').length == 0)
        && ($('.glazed-header--top.glazed-header--overlay,.glazed-header--top.glazed-header--fixed').length)) {

        var controlsTop = $('#block-system-main .glazed-editor').scrollTop() - 35;
        var menuBottom = $('.glazed-header--overlay, .glazed-header--fixed').scrollTop() + $('.glazed-header--overlay, .glazed-header--fixed').height();
        var marginTop = menuBottom - controlsTop;
        if (controlsTop < menuBottom) {
            $('#block-system-main .glazed-editor > .controls').css('margin-top', marginTop);
            $('#block-system-main .glazed-editor > .az-section').first().find('> .controls').css('margin-top', marginTop);
        }
    }

    // Helper classes
    $('.glazed-util-full-height', context).css('min-height', windowHeight);

    // User page
    $('.page-user .main-container', context).find('> .row > .col-sm-12')
        .once('glazed_free')
        .removeClass('col-sm-12')
        .addClass('col-sm-8 col-md-offset-2');

    // Main content layout
    $('.glazed-util-content-center-4-col .main-container', context).find('> .row > .col-sm-12')
        .once('glazed_free')
        .removeClass('col-sm-12')
        .addClass('col-sm-4 col-md-offset-4');

    $('.glazed-util-content-center-6-col .main-container', context).find('> .row > .col-sm-12')
        .once('glazed_free')
        .removeClass('col-sm-12')
        .addClass('col-sm-6 col-md-offset-3');

    $('.glazed-util-content-center-8-col .main-container', context).find('> .row > .col-sm-12')
        .once('glazed_free')
        .removeClass('col-sm-12')
        .addClass('col-sm-8 col-md-offset-2');

    $('.glazed-util-content-center-10-col .main-container', context).find('> .row > .col-sm-12')
        .once('glazed_free')
        .removeClass('col-sm-12')
        .addClass('col-sm-8 col-md-offset-1');

    // Breadcrumbs
    $('.breadcrumb a', context)
        .once('glazed_free')
        .after(' <span class="glazed-breadcrumb-spacer">/</span> ');

    // Comments
    $('#comments .links a', context)
        .once('glazed_free')
        .addClass('btn-sm');

    $('#comments .links .comment-reply a', context)
        .once('glazed_free')
        .addClass('btn-primary');

    // Sidebar nav blocks
    $('.region-sidebar-first .block .view ul, .region-sidebar-second .block .view ul', context)
        .once('glazed_free')
        .addClass('nav');

    // Portfolio content

    // Blog styling

    // Events styling
    $('.node-event [class^="field-event-"]', context)
        .once('glazed_free').each(function() {
          $(this).prev().andSelf().wrapAll('<div class="row">');
        });

    $('.node-event .field-label', context)
        .once('glazed_free')
        .addClass('col-sm-3');

    $('.node-event [class^="field-event-"]', context)
        .once('glazed_free')
        .addClass('col-sm-9');

    $('.node-event .field-event-location', context)
        .once('glazed_free')
        .wrapInner('<a href="https://maps.google.com/?q=' + $('.node-event .field-event-location').text() + '">');
  }
};

// Create underscore debounce function if it doesn't exist already
if(typeof _ != 'function'){
  window._ = {};
  window._.debounce = function(func, wait, immediate) {
    var timeout, result;

    var later = function(context, args) {
      timeout = null;
      if (args) result = func.apply(context, args);
    };

    var debounced = restArgs(function(args) {
      var callNow = immediate && !timeout;
      if (timeout) clearTimeout(timeout);
      if (callNow) {
        timeout = setTimeout(later, wait);
        result = func.apply(this, args);
      } else if (!immediate) {
        timeout = _.delay(later, wait, this, args);
      }

      return result;
    });

    debounced.cancel = function() {
      clearTimeout(timeout);
      timeout = null;
    };

    return debounced;
  };
  var restArgs = function(func, startIndex) {
    startIndex = startIndex == null ? func.length - 1 : +startIndex;
    return function() {
      var length = Math.max(arguments.length - startIndex, 0);
      var rest = Array(length);
      for (var index = 0; index < length; index++) {
        rest[index] = arguments[index + startIndex];
      }
      switch (startIndex) {
        case 0: return func.call(this, rest);
        case 1: return func.call(this, arguments[0], rest);
        case 2: return func.call(this, arguments[0], arguments[1], rest);
      }
      var args = Array(startIndex + 1);
      for (index = 0; index < startIndex; index++) {
        args[index] = arguments[index];
      }
      args[startIndex] = rest;
      return func.apply(this, args);
    };
  }
  _.delay = restArgs(function(func, wait, args) {
    return setTimeout(function() {
      return func.apply(null, args);
    }, wait);
  });

}

$(window).resize(_.debounce(function(){
    if ($('#glazed-main-menu .menu').length == 0) {
      return false;
    }
    glazedMenuGovernorBodyClass(document);
    glazedMenuGovernor(document);
}, 250));

function glazedMenuGovernor(context) {
  // Bootstrap dropdown multi-column smart menu
  var navBreak = 1200;
  if('glazedNavBreakpoint' in window) {
    navBreak = window.glazedNavBreakpoint;
  }
  if (($('.body--glazed-header-side').length == 0) && $(window).width() > navBreak) {
    if (glazedMenuState == 'top') {
      return false;
    }
    $('.glazed-header--side').removeClass('glazed-header--side').addClass('glazed-header--top');
    $('#glazed-main-menu .menu__breadcrumbs').remove();
    $('.menu__level').removeClass('menu__level').css('margin-top', 0).css('height', 'auto');
    $('.menu__item').removeClass('menu__item');
    $('[data-submenu]').removeAttr('data-submenu');
    $('[data-menu]').removeAttr('data-menu');

    var bodyWidth = $('body').innerWidth();
    var margin = 10;
    $('#glazed-main-menu .menu .dropdown-menu', context)
      .each(function() {
        var width = $(this).width();
        if ($(this).find('.glazed-megamenu__heading').length > 0) {
          var columns = $(this).find('.glazed-megamenu__heading').length;
        }
        else {
          var columns = Math.floor($(this).find('li').length / 8) + 1;
        }
        if (columns > 2) {
          $(this).css({
              'width' : '100%', // Full Width Mega Menu
              'left:' : '0',
          }).parent().css({
              'position' : 'static',
          }).find('.dropdown-menu >li').css({
              'width' : 100 / columns + '%',
          });
        }
        else {
          var $this = $(this);
          if (columns > 1) {
            // Accounts for 1px border.
            $this
              .css('min-width', width * columns + 2)
              .find('>li').css('width', width)
          }
          // Workaround for drop down overlapping.
          // See https://github.com/twbs/bootstrap/issues/13477.
          var $topLevelItem = $this.parent();
          // Set timeout to let the rendering threads catch up.
          setTimeout(function() {
            var delta = Math.round(bodyWidth - $topLevelItem.offset().left - $this.outerWidth() - margin);
            // Only fix items that went out of screen.
            if (delta < 0) {
              $this.css('left', delta + 'px');
            }
          }, 0)
        }
      });
    glazedMenuState = 'top';
  }
  // Mobile Menu with sliding panels and breadcrumb
  // @dsee glazed-mobile-nav.js
  else {
    if (glazedMenuState == 'side') {
      return false;
    }
    // Temporary hiding while settings up @see #290
    $('#glazed-main-menu').hide();
    // Set up classes
    $('.glazed-header--top').removeClass('glazed-header--top').addClass('glazed-header--side');
    // Remove split-megamenu columns
    $('#glazed-main-menu .menu .dropdown-menu, #glazed-main-menu .menu .dropdown-menu li').removeAttr('style');
    $('#glazed-main-menu .menu').addClass('menu__level');
    $('#glazed-main-menu .menu .dropdown-menu').addClass('menu__level');
    $('#glazed-main-menu .menu .glazed-megamenu').addClass('menu__level');
    $('#glazed-main-menu .menu a').addClass('menu__link');
    $('#glazed-main-menu .menu li').addClass('menu__item');
    // Set up data attributes
    $('#glazed-main-menu .menu a.dropdown-toggle').each(function( index ) {
        $(this).attr('data-submenu', $(this).text())
          .next().attr('data-menu', $(this).text());
      });
    $('#glazed-main-menu .menu a.glazed-megamenu__heading').each(function( index ) {
        $(this).attr('data-submenu', $(this).text())
          .next().attr('data-menu', $(this).text());
      });

      var bc = ($('#glazed-main-menu .menu .dropdown-menu').length > 0);
      var menuEl = document.getElementById('glazed-main-menu'),
          mlmenu = new MLMenu(menuEl, {
              breadcrumbsCtrl : bc, // show breadcrumbs
              initialBreadcrumb : 'menu', // initial breadcrumb text
              backCtrl : false, // show back button
              itemsDelayInterval : 10, // delay between each menu item sliding animation
              // onItemClick: loadDummyData // callback: item that doesn´t have a submenu gets clicked - onItemClick([event], [inner HTML of the clicked item])
          });
      // mobile menu toggle
      $('#glazed-menu-toggle').once('glazedMenuToggle').click(function() {
        $(this).toggleClass( 'navbar-toggle--active' );
        $(menuEl).toggleClass( 'menu--open' );
        $('body').toggleClass( 'body--glazed-nav-mobile--open' );
      });
      $('#glazed-main-menu').show();

      // See if logo  or block content overlaps menu and apply correction
      if ($('.wrap-branding').length > 0) {
        var brandingBottom = $('.wrap-branding').position().top + jQuery('.wrap-branding').height();
      }
      else {
        var brandingBottom = 0;
      }
      var $lastBlock = $('#glazed-main-menu .block:not(.block-menu)').last();
      if ($lastBlock.length > 0) {
        var lastBlockBottom = $lastBlock.position().top + $lastBlock.height();
      }
      else {
        var lastBlockBottom = 0;
      }

      // Show menu after completing setup
      // See if blocks overlap menu and apply correction
      if (($('.body--glazed-header-side').length > 0) && ($(window).width() > navBreak) && ($lastBlock.length > 0) && (brandingBottom > 0)) {
        $('#glazed-main-menu').css('padding-top', brandingBottom + 40);
      }
      if (($lastBlock.length > 0) && (lastBlockBottom > 0)) {
        $('.menu__breadcrumbs, .menu__level').css('margin-top', lastBlockBottom-40);
        var offset = 40 + lastBlockBottom;
        $('.glazed-header--side .menu__level').css('height', 'calc(100vh - ' + offset + 'px)');
      }
      else if (($('.body--glazed-header-side').length > 0) && ($('.wrap-branding').length > 0) && (brandingBottom > 120)) {
        $('.menu__breadcrumbs, .menu__level').css('margin-top', brandingBottom-40);
        var offset = 40 + brandingBottom;
        $('.glazed-header--side .menu__level').css('height', 'calc(100vh - ' + offset + 'px)');
      }


    glazedMenuState = 'side';
  }

}

function glazedMenuGovernorBodyClass() {
  var navBreak = 1200;
  if('glazedNavBreakpoint' in window) {
    navBreak = window.glazedNavBreakpoint;
  }
  if ($(window).width() > navBreak) {
    $('.body--glazed-nav-mobile').removeClass('body--glazed-nav-mobile').addClass('body--glazed-nav-desktop');
  }
  else {
    $('.body--glazed-nav-desktop').removeClass('body--glazed-nav-desktop').addClass('body--glazed-nav-mobile');
  }
}

})(jQuery, Drupal, this, this.document);
