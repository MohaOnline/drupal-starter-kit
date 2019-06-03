/**
 * @file
 * Define theme JS logic.
 */

;
(function($, Drupal, window, undefined) {

  Drupal.settings.platon = Drupal.settings.platon || {};

  Drupal.behaviors.platon = {

    attach: function(context) {

      // Manage homepage slider height
      viewport = function() {
        var e = window,
          a = 'inner';
        if (!('innerWidth' in window)) {
          a = 'client';
          e = document.documentElement || document.body;
        }
        return {
          width: e[a + 'Width'],
          height: e[a + 'Height']
        };
      }

      $('.field-multiple-table').each(function() {
        if ($(this).parent('.form-item').find('.field-add-more-submit').length) {
          var $add = $(this).parent('.form-item').find('.field-add-more-submit');
          $(this).append($add);
        }
      });

      if ($('.page-course-catalogue, .page-devel, .page-my-trainings, .page-node-add, .page-node-edit, .page-training-catalogue')) {
        $('.form-type-select').each(function() {
          $(this).find('.description').css('margin-top', '-10px');
          $(this).after($(this).find('.description'));
        });
      }

      var sliderHeight = viewport().height - ($('#site-header').height() + $('#site-footer').height() + parseInt($('#site-footer').css('padding-top')) + parseInt($('#site-footer').css('padding-bottom')));
      if (sliderHeight > 680) {
        $('body.platon-use-slider #second-sidebar, body.platon-use-slider #second-sidebar ul.homepage-slider > li').height(sliderHeight);
      }

      // Replace category select list
      if (($('.view-my-trainings').length || $('.view-opigno-course-catalgue').length) && $('ul.action-links').length) {
        $('ul.action-links').append($('.views-widget-filter-opigno_course_categories_tid'));
        $('.view-filters form').prepend($('ul.action-links'));
      }

      // Auto submit when category select change
      if ($('.view-my-trainings').length || $('.view-opigno-course-catalgue').length) {
        var label = $('.views-widget-filter-opigno_course_categories_tid > label').text();

        $('.views-widget-filter-opigno_course_categories_tid select option[value="All"]').text(label);
        $('.view-filters .views-submit-button').hide();

        $(document).delegate('.views-widget-filter-opigno_course_categories_tid select', 'change', function() {
          $('.view-filters form').submit();
        });
      }

      $('.view-my-trainings .views-row .content-more a, .view-opigno-course-catalgue .views-row .content-more a, .view-active-trainings .views-row .content-more a').click(function(e) {
        e.preventDefault();
        if (!$(this).closest('.views-row').hasClass('open')) {
          $(this).closest('.views-row').addClass('open');
        }
      });
      $('.view-my-trainings .close-btn, .view-opigno-course-catalgue .close-btn, .view-active-trainings .close-btn').click(function(e) {
        e.preventDefault();
        if ($(this).closest('.views-row').hasClass('open')) {
          $(this).closest('.views-row').removeClass('open');
        }
      });

      // homepage close menu
      $('body.platon-use-slider #first-sidebar #main-navigation-wrapper .title .close-menu a').click(function() {

        if ($(this).hasClass('open')) {
          $(this).removeClass('open');
          $('#main-navigation-wrapper .main-navigation-row').slideUp();
        } else {
          $(this).addClass('open');
          $('#main-navigation-wrapper .main-navigation-row').slideDown();
        }
      });

      // Homepage slider
      if ($('body.platon-use-slider').length) {

        $('.homepage-slider').slick({
          fade: true,
          autoplay: true,
          autoplaySpeed: 5000,
          speed: 1000,
          arrows: false,
          responsive: true,
          pauseOnHover: false,
        });

        $('.homepage-slider').bind('beforeChange', function(event, slick, currentSlide, nextSlide) {
          $('body.platon-use-slider #second-sidebar .slider-footer .slider-counter .top').text(nextSlide + 1);
        });
      }

      // Manage login block display
      $('a.trigger-block-user-login').click(function() {

        var $loginBlock = $('.header-user-tools #header-login .region-header-login');

        if ($loginBlock.hasClass('open')) {
          $loginBlock.removeClass('open').slideUp();
        } else {
          $loginBlock.addClass('open').slideDown();
        };

      });

      $('.header-user-tools #header-login, .header-user-tools #header-login .region-header-login').click(function(event) {
        event.stopPropagation();
      });

      $(document).click(function() {
        var $loginBlock = $('.header-user-tools #header-login .region-header-login');
        if ($loginBlock.hasClass('open')) {
          $loginBlock.removeClass('open').slideUp();
        }
      })

      // Remove no-js class from html.
      $('html').removeClass('no-js');

      if ($('#opigno-group-progress').length) // use this if you are using id to check
      {
        $("#second-sidebar #content").addClass("has-group-progress");
        $("#second-sidebar #tabs").addClass("has-group-progress");
        $("#second-sidebar .action-links").addClass("has-group-progress");
      }

      // Make search form appear on hover.
      var $headerSearch = $('#header-search', context);
      if ($headerSearch.length && !$headerSearch.hasClass('js-processed')) {
        // On mouseenter, show the form (if not already visible). Else, hide it - unless one of the child inputs has focus.
        $headerSearch.hover(
          function() {
            if (!(Modernizr && Modernizr.mq && Modernizr.mq('(min-width: 800px)'))) {
              return;
            }

            // Clear any past timeouts.
            if (this._timeOut) {
              clearTimeout(this._timeOut);
            }

            if (!$headerSearch.hasClass('opened')) {
              $headerSearch.addClass('opened').animate({
                width: '180px'
              });
            }
          },
          function() {
            if (!(Modernizr && Modernizr.mq && Modernizr.mq('(min-width: 800px)'))) {
              return;
            }

            // Clear any past timeouts.
            if (this._timeOut) {
              clearTimeout(this._timeOut);
            }

            // Wait for half a second before closing.
            this._timeOut = setTimeout(function() {
              // Only close if no child input has any focus.
              if (!$headerSearch.find('input:focus').length) {
                $headerSearch.animate({
                  width: '40px'
                }, {
                  complete: function() {
                    $headerSearch.removeClass('opened');
                  }
                });
              }
            }, 500);
          }
        );

        // If a child input is blurred, trigger the "mouseleave" event to see if we can close it.
        $headerSearch.find('input[type="text"], input[type="submit"]').blur(function() {
          $headerSearch.mouseleave();
        });

        // Add a placeholder text to the search input.
        $headerSearch.find('input[type="text"]').attr('placeholder', Drupal.t("Search") + '...');

        // Don't process it again.
        $headerSearch.addClass('js-processed');
      }

      // Make messages dismissable.
      $('div.messages', context).each(function() {
        var $message = $(this),
          $dismiss = $('span.messages-dismiss', this);
        if ($dismiss.length && !$dismiss.hasClass('js-processed')) {
          $dismiss.click(function() {
            $message.hide('fast', function() {
              $message.remove();
            });
          }).addClass('js-processed');
        }
      });

      // Make entire section in admin/opigno clickable and hoverable.
      var $adminSections = $('div.admin-panel dt', context);
      if ($adminSections.length) {
        $adminSections.each(function() {
          var $this = $(this);

          // Only process it once.
          if (!$this.hasClass('js-processed')) {
            $this._dd = $this.next('dd');

            // On hover, make entire section light up.
            $this.hover(
              function() {
                $this.addClass('hover');
                $this._dd.addClass('hover');
              },
              function() {
                $this.removeClass('hover');
                $this._dd.removeClass('hover');
              }
            );
            $this._dd.hover(
              function() {
                $this.mouseenter();
              },
              function() {
                $this.mouseleave();
              }
            );

            // On click, trigger the dt > a click.
            $this.click(function() {
              window.location.href = $this.find('a')[0].href;
            });
            $this._dd.click(function() {
              $this.click();
            });

            // Flag them as being processed.
            $this.addClass('js-processed');
            $this._dd.addClass('js-processed');
          }
        });
      }

      // Show the number of unread messages.
      if (typeof Drupal.settings.platon.unreadMessages !== 'undefined' && Drupal.settings.platon.unreadMessages) {
        var $messageLink = $('#main-navigation-item-messages', context);
        if ($messageLink.length && !$messageLink.hasClass('js-processed')) {
          $messageLink.find('a').prepend('<span id="messages-num-unread">' + Drupal.settings.platon.unreadMessages + '</span>');
          $messageLink.addClass('js-processed');
        }
      }

      // Make the entire tool "block" clickable for a better UX.
      $('.opigno-tool-block', context).each(function() {
        var $this = $(this);
        if (!$this.hasClass('js-processed')) {
          $this.click(function() {
            window.location = $this.find('a.opigno-tool-link').attr('href');
          }).addClass('js-processed');
        }
      });

      var $menu = $('#main-navigation-wrapper');
      var $menuTrigger = $('button.trigger');
      $menuTrigger.once().click(function() {
        if ($menuTrigger.hasClass('open')) {
          $menuTrigger.removeClass('open');
          $menu.removeClass('open');
          $menu.animate({
            paddingRight: 20
          }, 300);
          $.cookie('open-menu', 0);
        } else {
          $menuTrigger.addClass('open');
          $menu.addClass('open');
          $menu.animate({
            paddingRight: 215
          }, 300);
          $.cookie('open-menu', 1);
        }
      });
      if (typeof $.cookie != 'undefined' && $.cookie('open-menu') == 1) {
        $menuTrigger.addClass('open');
        $menu.addClass('open');
        $menu.animate({
          paddingRight: 215
        }, 300);
      }

      var labelText = $('#collaborative-workspace-node-form .form-item-field-classes-courses label').text();
      $('#collaborative-workspace-node-form .form-item-field-classes-courses select option[value="_none"]').text(labelText);


      // Make menu "toggleable" on mobile.
      var $menuToggle = $('#menu-toggle-link', context);
      if ($('body.platon-use-slider').length) {
        if (!$menuToggle.hasClass('js-processed')) {
          $menuToggle.click(function() {
            $('#main-navigation-wrapper').toggleClass('open');
          }).addClass('js-processed');
        }
      } else {
        if (!$menuToggle.hasClass('js-processed')) {
          $menuToggle.click(function() {
            $('#main-navigation-wrapper').toggleClass('menuOpen');
            $('#second-sidebar').toggleClass('menuOpen');
          }).addClass('js-processed');
        }
      }


    }

  };

})(jQuery, Drupal, window);
