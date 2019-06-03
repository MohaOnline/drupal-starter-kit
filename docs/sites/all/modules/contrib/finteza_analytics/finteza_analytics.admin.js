jQuery(document).ready(($) => {
  'use strict';

  $("a[href*='#finteza-analytics-']").bind('click', function (e) {
    const $target = $(this.hash);

    if ($target.length) {
      e.preventDefault();
      $('html, body').animate({scrollTop: $target.offset().top - 50}, 300);
      Drupal.toggleFieldset($('fieldset.collapsed', $target));
    }
  });
});
