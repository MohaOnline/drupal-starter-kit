(function($) {

    // Disable form_builder scroll
    Drupal.behaviors.formBuilderBlockScroll = {};

    // Only enable sidebar left region scroll once
    var scrollEnabled = false;

    Drupal.behaviors.surveyBuilder = {
        attach: function (context, settings) {

            // Remove default field palette group title
            $('#form-builder-field-palette .item-list:not(:first)').remove();

            // Add text search to existing questions block
            var $question_links = $('.block-views-questions-block .form-builder-fields li a');

            $('#block-views-questions-block .view-questions', context).once('surveyBuilder', function() {
                $('<input type="text" class="form-text" size="30">')
                    .bind('keydown keyup change focus blur', function() {
                        $question_links
                            .show()
                            .filter(':not(:containsCI("' + $(this).val() + '"))')
                                .hide();
                    })
                    .prependTo($(this));
            });

            // Add "Create Survey" form overlay
            $('#block-survey-builder-new-survey-form').overlay({
              top: 260,
              mask: {
                color: '#000',
                loadSpeed: 500,
                opacity: 0.5
              },
              load: true
            });

            // Add "Clone Survey" form overlay
            var $cloneForm = $('#block-survey-builder-clone-survey-form').hide();

            $('.survey-builder-clone-button').click(function() {
                console.log($cloneForm);
              $cloneForm.show().overlay({
                top: 260,
                mask: {
                  color: '#000',
                  loadSpeed: 500,
                  opacity: 0.5
                },
                load: true
              });
            });

            // Sidebar Left Region scroll
            if (scrollEnabled) {
                return;
            }
            scrollEnabled = true;

            var $block = $('#region-sidebar-first');

            if ($block.length) {
                $block.css('position', 'relative');
                var blockScrollStart = $block.offset().top;

                var $questions = $block.find('.survey-questions');
                $questions.height($(window).height() - 400);

                function blockScroll() {
                    // Do not move the palette while dragging a field.
                    if (Drupal.formBuilder.activeDragUi) {
                        return;
                    }

                    var windowOffset = $(window).scrollTop();
                    var blockHeight = $block.height();
                    var formBuilderHeight = $('#form-builder').height();
                    if (windowOffset - blockScrollStart > 0) {
                        // Do not scroll beyond the bottom of the editing area.
                        var newTop = Math.min(windowOffset - blockScrollStart + 20, formBuilderHeight - blockHeight);
                        $block.animate({ top: (newTop + 'px') }, 'fast');
                    }
                    else {
                        $block.animate({ top: '0px' }, 'fast');
                    }
                }

                var timeout = false;
                function scrollTimeout() {
                    if (timeout) {
                        clearTimeout(timeout);
                    }
                    timeout = setTimeout(blockScroll, 50);
                }

                $(window).scroll(scrollTimeout);
            }

        }
    };

    // Case insensitive contains
    $.expr[':'].containsCI = function(obj, index, match, stack) {
      return (obj.textContent || obj.innerText || $(obj).text() || "").toLowerCase().indexOf((match[3] || "").toLowerCase()) >= 0;
    };
})(jQuery);
