(function ($) {
Drupal.behaviors.powertagging_admin = {
  attach: function (context) {

    // Show/Hide additional fields if a checkbox is enabled/disabled
    $("#powertagging-index-form, #powertagging-update-taxonomy-form").bind("state:visible", function(e) {
      if(e.trigger) {
        $(e.target).closest(".form-item, .form-submit, .form-wrapper")[e.value ? "slideDown" : "slideUp"]();
        e.stopPropagation();
      }
    });

    if ($("fieldset#edit-batch-jobs").length > 0) {
      $("input#edit-batch-jobs-index").click(function(e) {
        e.preventDefault();
        
        $(this).siblings("table").children("tbody").find("input:checkbox:checked").each(function() {
          $(this).attr("checked", false);
          $(this).closest("table").children("thead").find("input:checkbox:checked").attr("checked", false);
          $(this).closest("tr").removeClass("selected");

          // Check if indexing or synchronization of this PowerTagging
          // configuration already runs.
          $.getJSON(Drupal.settings.basePath + 'powertagging/index', function(result_data) {
            if (result_data.success) {
              console.log(result_data.message);
            }
            else {
              console.log(result_data.message);
            }
          });
        });
      });
    }

    // Make the project tables sortable if tablesorter is available.
    if ($.isFunction($.fn.tablesorter)) {
      $("table#powertagging-configurations-table").tablesorter({
        widgets: ["zebra"],
        widgetOptions: {
          zebra: ["odd", "even"]
        },
        sortList: [[0, 0]],
        headers: {
          3: {sorter: false},
          4: {sorter: false}
        }
      });
    }

    if ($("form#powertagging-connection-form").length > 0) {
      $('#edit-load-connection').change(function() {
        var connection_value = (jQuery(this).val());
        if (connection_value.length > 0) {
          var connection_details = connection_value.split('|');
          jQuery('#edit-server-title').val(connection_details[0]);
          jQuery('#edit-url').val(connection_details[1]);
          jQuery('#edit-name').val(connection_details[2]);
          jQuery('#edit-pass').val(connection_details[3]);
        }
        return false;
      });
    }
  }
};
})(jQuery);
