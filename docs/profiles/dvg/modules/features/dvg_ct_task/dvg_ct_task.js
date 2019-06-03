(function ($) {
  Drupal.behaviors.dvgTaskCtTaskScript = {
    attach: function (context, settings) {

      // Get the HASH
      var hash = window.location.hash;

      // All tab events/triggers for content-type Task (front-end)
      if ($('body.node-type-task, .node-task', context).length) {

        $('.section-list', context).once('section-list', function() {
          // Enable JS styles
          $('.section-list, .field-name-field-sections', context).addClass('js');
          $('.section-list li.first a', context).click();

          var active_tab = $('<span class="element-invisible">' + Drupal.t('Active tab') + ': </span>');

          // Click event
          $('.section-list a', context).click(function (event, skipScroll) {
            event.preventDefault();
            var $this = $(this);
            var url = $this.attr('href');
            var hash_link = url.substring(url.indexOf("#") + 1);

            if ($('.section-list li').css('float') == 'left') {
              $('.section-list a').parent().removeClass('active');
              $this.prepend(active_tab);
              $this.parent().addClass('active');
              $('.field-name-field-sections .task-section').addClass('element-invisible');
              $('.field-name-field-sections .task-section#' + hash_link).removeClass('element-invisible');
            }
            else if (!skipScroll) {
              $('html, body').animate({
                scrollTop: $(this.hash).offset().top
              }, 500, function(){ $(this).stop(true, true); });
            }

            hash = hash_link;
          });
        });

        // Initial setup
        if ($('.section-list li', context).css('float') == 'left') {
          if (hash.length) {
            $('.section-list a[href$=' + hash + ']', context).click();
          }
          else {
            $('.section-list li.first a', context).click();
          }
        }

        // Resize Event
        $('body').once('dvg-ct-task-resize', function () {
          $(window).resize(function () {
            if ($('.section-list li', context).css('float') == 'left') {
              $('.field-name-field-sections .task-section', context).addClass('element-invisible');

              if (hash.length) {
                $('.section-list a[href$=' + hash + ']', context).trigger('click', true);
              }
              else {
                $('.section-list li.first a', context).trigger('click', true);
              }
            }
            else {
              $('.field-name-field-sections .task-section', context).removeClass('element-invisible');
              if (hash) {
                $('.section-list a[href$=' + hash + ']', context).trigger('click', true);
              }
            }
          });
        });
      }
    }
  }
}(jQuery));
