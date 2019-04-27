/**
 * @file
 *
 * JavaScript functionalities for the Semantic Connector frontend.
 */

(function ($) {
  Drupal.behaviors.semanticConnectorConceptDestinations = {
    attach: function (context) {
      // Check if the individual PP servers are available.
      $("div.semantic-connector-led").each(function() {
        setLed($(this));
      });

      // Add all the actions required for the concept destinations menu.
      $(".semantic-connector-concept").each(function() {
        if ($(this).find('ul.semantic-connector-concept-destination-links').length > 0) {
          $(this).find('a.semantic-connector-concept-link').unbind('click').click(function () {
            $(this).siblings('ul.semantic-connector-concept-destination-links').show();
            return false;
          });
        }
      });
      $(".semantic-connector-concept").unbind('mouseover').mouseover(function() {
        $(this).find('ul.semantic-connector-concept-destination-links').show();
      });
      $(".semantic-connector-concept").unbind('mouseout').mouseout(function() {
        $(this).find('ul.semantic-connector-concept-destination-links').hide();
      });
    }
  };

  var setLed = function(item) {
    if (typeof item.data("server-type") != 'undefined' && typeof item.data("server-id") != 'undefined') {
      var url = Drupal.settings.basePath + "admin/config/semantic-drupal/semantic-connector/connections/" + item.data("server-type") + "/" + item.data("server-id") + "/available";
      $.get(url, function (data) {
        var led = "led-red";
        var title = Drupal.t("Service NOT available");
        if (data == 1) {
          led = "led-green";
          title = Drupal.t("Service available");
        }
        item.addClass(led);
        item.attr("title", title);
      });
    }
  };
})(jQuery);


