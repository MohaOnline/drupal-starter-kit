/**
 * @file
 * Default JavaScript file for Modal Page.
 */

(function ($) {
  'use strict';

  $(document).ready(function () {

    var modalPage = $('#js-modal-page-show-modal');

    if (modalPage.length) {
      modalPage.modal();
    }
  });

})(jQuery);
