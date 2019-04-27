/**
 * @file
 *
 * JavaScript functionalities for the Semantic Connector backend.
 */

(function ($) {
  Drupal.behaviors.semanticConnectorAdmin = {
    attach: function (context) {

      // Sorting function for the connection overview.
      var semantic_connector_sort_extraction = function(node) {
        var $node = $(node);
        // Normal text cell content.
        if ($node.children().length == 0) {
          return $node.text();
        }
        // List projects with available configurations first.
        else if ($node.children('ul').length > 0) {
          return 'a';
        }
        // List any unsupported project at the end.
        else if ($node.children('.semantic-connector-italic').length > 0) {
          return 'c';
        }
        // List anything else (most probably supported projects) in the middle.
        else {
          return 'b_'+ $node.text();
        }
      };

      // Make the project tables and the sparql endpoint tables sortable if
      // tablesorter is available.
      if ($.isFunction($.fn.tablesorter)) {
        $("table.pp-server-projects-table").tablesorter({
          widgets: ["zebra"],
          widgetOptions: {
            zebra: ["odd", "even"]
          },
          sortList: [[0, 0]],
          textExtraction: semantic_connector_sort_extraction
        });
        $("table#sparql-endpoints-table").tablesorter({
          widgets: ["zebra"],
          widgetOptions: {
            zebra: ["odd", "even"]
          },
          sortList: [[0, 0]],
          headers: {
            2: { sorter: false }
          },
          textExtraction: semantic_connector_sort_extraction
        });
      }
    }
  };
})(jQuery);
