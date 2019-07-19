(function ($) {
Drupal.behaviors.powertagging_field = {
  attach: function (context, settings) {

    // Show/Hide additional fields if a checkbox is enabled/disabled
    $("#field-ui-field-edit-form").bind("state:visible", function(e) {
      if(e.trigger) {
        $(e.target).closest(".form-wrapper")[e.value ? "slideDown" : "slideUp"]();
        e.stopPropagation();
      }
    });

    var powertagging_vm = {};
    $("div.field-type-powertagging").once(function() {
      var field_id = $(this).attr("id").substr(5).replace(/-/g, "_");
      var pt_field = Drupal.settings.powertagging[field_id][Object.keys(Drupal.settings.powertagging[field_id])[0]];

      $(this).find(".powertagging-get-tags").click(function(event) {
        event.preventDefault();
        var field_id = $(this).closest('.field-type-powertagging').attr("id").substr(5).replace(/-/g, "_");
        var html_id = "#edit-" + field_id.replace(/_/g, "-");
        var data = collect_content(Drupal.settings.powertagging[field_id][Object.keys(Drupal.settings.powertagging[field_id])[0]]);
        loading_extracted_tags(html_id);

        $.post(Drupal.settings.basePath + "powertagging/extract", data, function (tags) {
          renderResult(field_id, tags);
        }, "json");
      });

      if (pt_field.settings.browse_concepts_charttypes.length > 0) {
        $(this).find('.powertagging-browse-tags-area').attr('id', $(this).attr('id') + '-browse-tags');
        $(this).find(".powertagging-browse-tags").click(function (event) {
          event.preventDefault();
          var powertagging_field = $(this).closest('.field-type-powertagging');
          var field_id = powertagging_field.attr("id").substr(5).replace(/-/g, "_");
          var pt_field = Drupal.settings.powertagging[field_id][Object.keys(Drupal.settings.powertagging[field_id])[0]];
          var html_id = "#edit-" + field_id.replace(/_/g, "-");
          var browse_tags_area = $('#' + powertagging_field.attr("id") + '-browse-tags');

          var settings = {
            "enabled": 1,
            "width": 680,
            "height": 680,
            "chartTypes": pt_field.settings.browse_concepts_charttypes,
            "headerColor": "#dee4db",
            "spiderChart": {
              "rootOuterRadius": 260,
              "legendPositionX": "right",
              "legendStyle": "circle"
            },
            export: {
              "enabled": false
            },
            "relations": {
              "parents": {
                "colors": {"bright": "#B7C9CD", "dark": "#4B7782"},
                "wording": {"legend": "Broader"}
              },
              "children": {
                "colors": {"bright": "#C6E2D3", "dark": "#70B691"},
                "wording": {"legend": "Narrower"}
              },
              "related": {
                "colors": {"bright": "#FFC299", "dark": "#FF6600"},
                "wording": {"legend": "Related"}
              }
            }
          };
          powertagging_vm[field_id] = browse_tags_area.children(".powertagging-browse-tags-vm").empty().initVisualMapper(settings, {"conceptLoaded": [addConceptButton]});
          powertagging_vm[field_id].load(Drupal.settings.basePath + "powertagging/get-visualmapper-data/" + pt_field.settings.powertagging_id, "", pt_field.settings.entity_language);

          // The dialog gets opened the first time.
          if (powertagging_field.find('.powertagging-browse-tags-area').length > 0) {
            browse_tags_area.dialog({
              title: Drupal.t('Browse tags'),
              resizable: false,
              height: "auto",
              width: 900,
              modal: true,
              open: function(event, ui) {
                $('.ui-widget-overlay').bind('click', function() {
                  $(this).siblings('.ui-dialog').find('.ui-dialog-content').dialog('close');
                });
              }
            });
          }
          // The dialog was already opened before.
          else {
            browse_tags_area.find('.powertagging-browse-tags-selection-results').empty();
            browse_tags_area.dialog("open");
          }

          browse_tags_area.find('.powertagging-browse-tags-selection-cancel').unbind('click')
            .click(function () {
              $(this).closest('.powertagging-browse-tags-area').dialog('close');
            });

          browse_tags_area.find('.powertagging-browse-tags-selection-save').unbind('click')
            .click(function () {
              var browse_tags_area = $(this).closest('.powertagging-browse-tags-area');
              var field_id_full = browse_tags_area.attr('id').slice(0, -12);
              var field_id = field_id_full.substr(5).replace(/-/g, "_");
              var pt_field = Drupal.settings.powertagging[field_id][Object.keys(Drupal.settings.powertagging[field_id])[0]];
              var concepts = [];
              $(this).siblings('.powertagging-browse-tags-selection-results').children().each(function () {
                concepts.push({
                  uri: $(this).data('uri'),
                  prefLabel: $(this).data('label')
                });
              });
              var data = {settings: pt_field.settings, concepts: concepts};
              $.post(Drupal.settings.basePath + "powertagging/get-concept-tids", data, function (result_tags) {
                // Add the tags to the result.
                result_tags.forEach(function (result_tag) {
                  result_tag.label = result_tag.prefLabel;
                  result_tag.type = 'concept';
                  addTagToResult('#' + field_id_full, result_tag);
                });

                browse_tags_area.dialog('close');
              }, "json");
            });

          var autocomplete_box = browse_tags_area.find('.powertagging-browse-tags-search-ac');
          autocomplete_box.autocomplete({
            minLength: 2,
            source: Drupal.settings.basePath + "powertagging/autocomplete-tags/" + pt_field.settings.powertagging_id + '/' + pt_field.settings.entity_language,
            focus: function (event, ui) {
              this.value = ui.item.label;
              return false;
            },
            select: function (event, ui) {
              var browse_tags_area = $(this).closest('.powertagging-browse-tags-area');
              var field_id_full = browse_tags_area.attr('id').slice(0, -12);
              var field_id = field_id_full.substr(5).replace(/-/g, "_");
              if (ui.item) {
                powertagging_vm[field_id].updateConcept({id: ui.item.uri});
              }
              $(this).val("");
              return false;
            }
          });

          // Set the custom popup menu for the autocomplete field.
          var autocomplete = autocomplete_box.data("ui-autocomplete");
          var data_item = "ui-autocomplete-item";
          if (typeof autocomplete === "undefined") {
            autocomplete = autocomplete_box.data("autocomplete");
            data_item = "item.autocomplete";
          }
          autocomplete._renderItem = function (ul, item_data) {
            var field_id_full = $(this.element.context).closest('.powertagging-browse-tags-area').attr('id').slice(0, -12);
            var field_id = field_id_full.substr(5).replace(/-/g, "_");
            var pt_field = Drupal.settings.powertagging[field_id][Object.keys(Drupal.settings.powertagging[field_id])[0]];

            var item = $("<a>" + item_data.label + "</a>");
            if (pt_field.settings.ac_add_matching_label && item_data.matching_label.length > 0) {
              $("<div class='ui-menu-item-metadata'>")
                .append("<span class='ui-menu-item-metadata-label'>" + Drupal.t("Matching label") + ":</span>")
                .append("<span class='ui-menu-item-metadata-value'>" + item_data.matching_label + "</span>")
                .appendTo(item);
            }
            if (pt_field.settings.ac_add_context && item_data.context.length > 0) {
              $("<div class='ui-menu-item-metadata'>")
                .append("<span class='ui-menu-item-metadata-label'>" + Drupal.t("Context") + ":</span>")
                .append("<span class='ui-menu-item-metadata-value'>" + item_data.context + "</span>")
                .appendTo(item);
            }

            return $("<li>").data(data_item, item_data).append(item).appendTo(ul);
          };
        });

        var addConceptButton = function (vm, data) {
          var chart_header = $(vm.getDOMElement()).children('.chart-header');
          var $add_concept = chart_header.find(".add-concept-button");

          if ($add_concept.length === 0) {
            chart_header.prepend('<div class="add-concept-button button"></div>');
            $add_concept = chart_header.find(".add-concept-button");
          }

          // Check if the tag is already in use.
          var browse_tags_area = $add_concept.closest('.powertagging-browse-tags-area');
          var field_id = browse_tags_area.attr('id').slice(0, -12);
          var pt_field_id = field_id.substr(5).replace(/-/g, "_");
          var pt_field = Drupal.settings.powertagging[pt_field_id][Object.keys(Drupal.settings.powertagging[pt_field_id])[0]];

          // Data is available for the concept.
          if (data.hasOwnProperty('type')) {
            // Normal concept.
            if (data.type !== "project" && data.type !== "conceptScheme") {
              var tag_active = ($('#' + field_id + ' .powertagging-tag-result .powertagging-tag[data-uri="' + data.id + '"]').length > 0 || browse_tags_area.find('.powertagging-browse-tags-selection-results').children('.powertagging-browse-tags-tag[data-uri="' + data.id + '"]').length > 0);

              $add_concept.html('<a class="powertagging-browse-tags-add-concept' + (tag_active ? ' active' : '') + '" data-label="' + data.name + '" data-uri="' + data.id + '" href="#">Add Concept</a>');
              $add_concept.children('a').click(function (e) {
                e.preventDefault();
                if ($(this).hasClass('active')) {
                  return false;
                }

                var tag_selection = $(this).closest('.powertagging-browse-tags-area').find('.powertagging-browse-tags-selection-results');
                tag_selection.append('<div class="powertagging-browse-tags-tag" data-label="' + $(this).data('label') + '" data-uri="' + $(this).data('uri') + '">' + $(this).data('label') + '</div>');
                $(this).addClass('active');

                tag_selection.find('.powertagging-browse-tags-tag').unbind('click')
                  .click(function () {
                    // Check if the add concept button has to be made active again.
                    var chart_header = $(this).closest('.powertagging-browse-tags-area').find(".chart-header");
                    var add_concept_button = chart_header.find(".add-concept-button > a");

                    if (add_concept_button.length > 0 && add_concept_button.data('uri') === $(this).data('uri')) {
                      add_concept_button.removeClass('active');
                    }

                    // Remove the item from the selection list.
                    $(this).remove();
                  });
              });
            }
            // Concept scheme.
            else {
              $add_concept.html("");
            }
          }
          // Data has to be fetched first.
          else {
            // Empty the area first.
            $add_concept.html("");

            // Then load the data.
            $.ajax({
              dataType: "json",
              url: Drupal.settings.basePath + "powertagging/get-visualmapper-data/" + pt_field.settings.powertagging_id,
              data: {
                uri: data.id,
                lang: vm.language
              },
              success: function (concept) {
                if (concept) {
                  // Since data gets fetched via AJAX, the JS for the connected content
                  // needs to be updated here.
                  addConceptButton(vm, concept);
                }
              }
            });
          }
        };
      }
    });

    $(document).ready(function () {
      Object.getOwnPropertyNames(Drupal.settings.powertagging).forEach(function(field_id) {
        renderResult(field_id, {});
      });
    });

    /**
     * Collect all the data into an object for the extraction
     */
    function collect_content (pt_field) {
      var field_id = "#edit-" + pt_field.settings.field_name.replace(/_/g, "-");

      var data = {settings: pt_field.settings, content: "", files: [], entities: {}};
      // Build the text content to extract tags from.
      $.each(pt_field.fields, function(field_index, field) {
        switch (field.module) {
          case "text":
            var text_content = collect_content_text(field.field_name, field.widget) || "";
            if (text_content.length > 0) {
              data.content += (data.content.length > 0 ? " " : "") + text_content;
            }
            break;

          case "file":
          case "media":
            var files = collect_content_file(field.field_name, field.widget);
            if (files.length > 0) {
              data.files = data.files.concat(files);
            }
            break;

          case "entityreference":
            var entites = collect_referenced_entities(field.field_name, field.widget);
            if (entites.length > 0) {
              data.entities[field.field_name] = entites;
            }
            break;
        }
      });
      return data;
    }

    /**
     * Collect the data from different text field types
     */
    function collect_content_text (field, widget) {
      var field_id = "#edit-" + field.replace(/_/g, "-");
      switch (widget) {
        case "text_textfield_title":
          return $(field_id).val();

        case "text_textfield":
          return $(field_id + " input").val();

        case "text_textarea":
        case "text_textarea_with_summary":
          var content = "";
          // Get the text from the summary and the full text.
          $(field_id + " textarea").each(function() {
            var textarea_id = $(this).attr("id");
            // CkEditor.
            if (typeof(CKEDITOR) !== "undefined" && CKEDITOR.hasOwnProperty("instances") && CKEDITOR.instances.hasOwnProperty(textarea_id)) {
              content += CKEDITOR.instances[textarea_id].getData();
            }
            // TinyMCE.
            else if (typeof(tinyMCE) !== "undefined" && tinyMCE.hasOwnProperty("editors") && tinyMCE.editors.hasOwnProperty(textarea_id)) {
              content += tinyMCE.editors[textarea_id].getContent({format : "raw"});
            }
            // No text editor or an unsupported one.
            else {
              content += $(this).val();
            }
          });
          return content;
      }
      return "";
    }

    /**
     * Collect the data from different file field types
     */
    function collect_content_file (field, widget) {
      var field_id = "#edit-" + field.replace(/_/g, "-");
      switch (widget) {
        case "file_generic":
        case "media_generic":
          var files = [];
          $(field_id + " input[type=hidden]").each(function() {
            if ($(this).attr("name").indexOf("[fid]") > 0 && $(this).val() > 0) {
              files.push($(this).val());
            }
          });
          return files;
      }
      return [];
    }

    /**
     * Collect the selected entities referenced in a entityreference field.
     */
    function collect_referenced_entities (field, widget) {
      var field_id = "#edit-" + field.replace(/_/g, "-");
      var entities = [];
      switch (widget) {
        case "entityreference_autocomplete":
          $(field_id + " input[type=text]").each(function() {
            var field_value = $(this).val();
            if (field_value.length > 0) {
              var entity_id_start = field_value.lastIndexOf("(");
              var entity_id_end = field_value.lastIndexOf(")");
              if (entity_id_start !== -1 && entity_id_end !== -1 && entity_id_end > entity_id_start) {
                var entity_id = field_value.substr(entity_id_start + 1,( entity_id_end - entity_id_start) - 1);
                if (!isNaN(entity_id) && parseInt(Number(entity_id)) == entity_id && !isNaN(parseInt(entity_id, 10))) {
                  entities.push(entity_id);
                }
              }
            }
          });
          break;

        case "entityreference_autocomplete_tags":
          var field_value = $(field_id + " input[type=text]").val();
          if (field_value.length > 0) {
            var all_values = field_value.split(', ');
            all_values.forEach(function(single_value) {
              var entity_id_start = single_value.lastIndexOf("(");
              var entity_id_end = single_value.lastIndexOf(")");
              if (entity_id_start !== -1 && entity_id_end !== -1 && entity_id_end > entity_id_start) {
                var entity_id = single_value.substr(entity_id_start + 1,( entity_id_end - entity_id_start) - 1);
                if (!isNaN(entity_id) && parseInt(Number(entity_id)) == entity_id && !isNaN(parseInt(entity_id, 10))) {
                  entities.push(entity_id);
                }
              }
            });
          }
          break;
      }
      return entities;
    }

    /**
     * Render the PowerTagging data
     */
    function renderResult (field_id, tags) {
      var settings = Drupal.settings.powertagging[field_id][Object.keys(Drupal.settings.powertagging[field_id])[0]];
      field_id = "#edit-" + field_id.replace(/_/g, "-");

      check_entity_language(field_id, settings.settings);

      // Close the extracted tags container on the beginning
      if (!tags.hasOwnProperty("suggestion")) {
        hide_extracted_tags(field_id);
      }

      if (!tags.hasOwnProperty('messages') || tags.messages.length === 0) {
        // Render the content tags.
        if (tags.hasOwnProperty('content')) {
          var content_tags = tags.content.concepts.concat(tags.content.freeterms);
          if (content_tags.length > 0) {
            var html_tags = [];

            content_tags.forEach(function(tag) {
              html_tags.push(renderTag(tag));
            });

            $(field_id + " .powertagging-extracted-tags").append('<div class="powertagging-extracted-tags-area"><div class="powertagging-extraction-label">' + Drupal.t("Form fields") + "</div><ul><li>" + html_tags.join("</li><li>") + "</li></ul></div>");
            show_extracted_tags(field_id);
          }
        }

        // Render the file tags.
        if (tags.hasOwnProperty('files')) {
          for(var file_name in tags.files) {
            var file_tags = tags.files[file_name].concepts.concat(tags.files[file_name].freeterms);
            if (file_tags.length > 0) {
              var html_tags = [];

              file_tags.forEach(function(tag) {
                html_tags.push(renderTag(tag));
              });

              $(field_id + " .powertagging-extracted-tags").append('<div class="powertagging-extracted-tags-area"><div class="powertagging-extraction-label">' + Drupal.t('Uploaded file "%file"', {'%file': file_name}) + "</div><ul><li>" + html_tags.join("</li><li>") + "</li></ul></div>");
              show_extracted_tags(field_id);
            }
          }
        }

        // Initially add the tags for the results area.
        var result_tags = [];

        // There are already tags connected with this node.
        if (settings.hasOwnProperty("selected_tags") && settings.selected_tags.length > 0) {
          settings.selected_tags.forEach(function(tag) {
            result_tags.push(tag);
          });
        }
        // No tags connected with this node yet, use the suggestion.
        else if (tags.hasOwnProperty("suggestion")) {
          var suggestion_tags = tags.suggestion.concepts.concat(tags.suggestion.freeterms);
          if (suggestion_tags.length > 0) {
            suggestion_tags.forEach(function(tag) {
              result_tags.push(tag);
            });
          }
        }

        // Add the tags to the result.
        result_tags.forEach(function(result_tag) {
          addTagToResult(field_id, result_tag);
        });
      }
      // There are errors or infos available --> show them instead of the tags.
      else {
        var messages_html = '';
        tags.messages.forEach(function(message){
          messages_html += '<div class="messages ' + message.type + '">' + message.message + '</div>';
        });
        $(field_id + " .powertagging-extracted-tags").html(messages_html);
        show_extracted_tags(field_id);
      }

      // Add the click handlers to the tag-elements.
      $(field_id + " .powertagging-extracted-tags .powertagging-tag").click(function() {
        if ($(this).hasClass('disabled')) {
          removeTagFromResult(field_id, tagElementToObject($(this)));
        }
        else {
          addTagToResult(field_id, tagElementToObject($(this)));
        }
      });
      $(field_id + " .powertagging-tag-result .powertagging-tag").click(function() {
        removeTagFromResult(field_id, tagElementToObject($(this)));
      });

      // Manually add an existing concept or freeterm to the result.
      var autocomplete_box = $(field_id + " .powertagging-tag-result").closest(".field-type-powertagging").find("input.powertagging_autocomplete_tags");
      autocomplete_box.autocomplete({
        source: Drupal.settings.basePath + "powertagging/autocomplete-tags/" + settings.settings.powertagging_id + '/' + settings.settings.entity_language,
        minLength: 2,
        select: function( event, ui ) {
          event.preventDefault();
          addTagToResult(field_id, {tid: ui.item.tid, uri: ui.item.uri, label: ui.item.label, type: ui.item.type});
          $(this).val("");
        }
      });

      // Set the custom popup menu for the autocomplete field.
      var autocomplete = autocomplete_box.data("ui-autocomplete");
      var data_item = "ui-autocomplete-item";
      if (typeof autocomplete === "undefined") {
        autocomplete = autocomplete_box.data("autocomplete");
        data_item = "item.autocomplete";
      }
      autocomplete._renderItem = function (ul, item_data) {
        var field_id = $(this.element.context).closest('.field-type-powertagging').attr("id").substr(5).replace(/-/g, "_");
        var pt_field = Drupal.settings.powertagging[field_id][Object.keys(Drupal.settings.powertagging[field_id])[0]];

        var item = $("<a>" + item_data.label + "</a>");
        if (pt_field.settings.ac_add_matching_label && item_data.matching_label.length > 0) {
          $("<div class='ui-menu-item-metadata'>")
            .append("<span class='ui-menu-item-metadata-label'>" + Drupal.t("Matching label") + ":</span>")
            .append("<span class='ui-menu-item-metadata-value'>" + item_data.matching_label + "</span>")
            .appendTo(item);
        }
        if (pt_field.settings.ac_add_context && item_data.context.length > 0) {
          $("<div class='ui-menu-item-metadata'>")
            .append("<span class='ui-menu-item-metadata-label'>" + Drupal.t("Context") + ":</span>")
            .append("<span class='ui-menu-item-metadata-value'>" + item_data.context + "</span>")
            .appendTo(item);
        }

        return $("<li>").data(data_item, item_data).append(item).appendTo(ul);
      };

      // Manually add a new freeterm to the result if it is allowed.
      if (settings.settings["custom_freeterms"]) {
        autocomplete_box.keyup(function (e) {
          if (e.keyCode === 13) {
            var field_value = jQuery.trim($(this).val());
            if (field_value.length > 0) {
              addTagToResult(field_id, {
                tid: 0,
                uri: "",
                label: field_value,
                type: "freeterm"
              });
              $(this).autocomplete("close");
              $(this).val("");
            }
          }
        });
      }

      // Update the language of the entity.
      if ($(field_id).closest('form').find('#edit-language').length > 0) {
        $(field_id).closest('form').find('#edit-language').change(function() {
          // Language selection as a select-element.
          var language = "";
          if ($(this).is('select')) {
            language = $(this).val();
          }
          // Language selection as radio buttons.
          else if ($(this).find('input:checked').length > 0) {
            language = $(this).find('input:checked').val();
          }

          Object.getOwnPropertyNames(Drupal.settings.powertagging).forEach(function(field_id) {
            Drupal.settings.powertagging[field_id][Object.keys(Drupal.settings.powertagging[field_id])[0]].settings.entity_language = language;
            var html_field_id = "#edit-" + field_id.replace(/_/g, '-');
            var field_settings = Drupal.settings.powertagging[field_id][Object.keys(Drupal.settings.powertagging[field_id])[0]].settings;
            check_entity_language(html_field_id, field_settings);
            
            // Update the the autocomplete path.
            $(html_field_id + " .powertagging-tag-result").closest(".field-type-powertagging").find("input.powertagging_autocomplete_tags").autocomplete(
              'option', 'source', Drupal.settings.basePath + "powertagging/autocomplete-tags/" + settings.settings.powertagging_id + '/' + settings.settings.entity_language
            );
          });
        });
      }
    }

    /**
     * Create a JS-object out of a jQuery element for a powertagging tag.
     */
    function tagElementToObject(tag_object) {
      var type = tag_object.hasClass('concept') ? 'concept' : 'freeterm';
      return {tid: tag_object.attr("data-tid"), uri: tag_object.attr("data-uri"), label: tag_object.attr("data-label"), type: type};
    }

    /**
     * Get the HTML-code of a single powertagging tag.
     *
     * @param tag
     *   The tag with properties "tid" and "label"
     *
     * @return string
     *   The HTML-output of the tag
     */
    function renderTag(tag) {
      var score = tag.score ? ' (' + tag.score + ')' : '';
      return '<div class="powertagging-tag ' + tag.type + '" data-tid="' + tag.tid + '" data-label="' + tag.label + '" data-uri="' + tag.uri + '">' + tag.label + score + '</div>';
    }

    function addTagToResult(field_id, tag) {
      // Only add tags, that are not already inside the results area.
      if ((parseInt(tag.tid) > 0 && $(field_id + ' .powertagging-tag-result .powertagging-tag[data-tid="' + tag.tid + '"]').length === 0) ||
          (parseInt(tag.tid) === 0 && ($(field_id + ' .powertagging-tag-result .powertagging-tag[data-label="' + tag.label + '"]').length === 0 || tag.uri !== ""))) {

        // Add a new list if this is the first tag to add.
        if ($(field_id + " .powertagging-tag-result ul").length === 0) {
          $(field_id + " .powertagging-tag-result").append("<ul></ul>");
        }

        // Remove freeterms with the same string for concepts.
        if (tag.type === 'concept') {
          $(field_id + ' .powertagging-tag-result .powertagging-tag[data-label="' + tag.label + '"]').parent("li").remove();
        }

        // Add a new list item to the result.
        $(field_id + " .powertagging-tag-result ul").append("<li>" + renderTag(tag) + "</li>");

        // Add a click handler to the new result tag.
        $(field_id + " .powertagging-tag-result li:last-child .powertagging-tag").click(function() {
          removeTagFromResult(field_id, tagElementToObject($(this)));
        });

        // Update the field value to save.
        updateFieldValue(field_id);
      }

      // Disable already selected tags in the extraction area.
      if (parseInt(tag.tid) > 0) {
        $(field_id + ' .powertagging-extracted-tags .powertagging-tag[data-tid="' + tag.tid + '"]').addClass("disabled");
      }
      else {
        $(field_id + ' .powertagging-extracted-tags .powertagging-tag[data-label="' + tag.label + '"]').addClass("disabled");
      }
    }

    function removeTagFromResult(field_id, tag) {
      var settings = Drupal.settings.powertagging[field_id.replace('#edit-', '').replace(/-/g, '_')];
      settings = settings[Object.keys(settings)[0]];

      // Enable tags in the extraction area again and remove the list item.
      if (parseInt(tag.tid) > 0) {
        $(field_id + ' .powertagging-tag-result .powertagging-tag[data-tid="' + tag.tid + '"]').parent("li").remove();
        $(field_id + ' .powertagging-extracted-tags .powertagging-tag[data-tid="' + tag.tid + '"]').removeClass("disabled");
      }
      else {
        $(field_id + ' .powertagging-tag-result .powertagging-tag[data-label="' + tag.label + '"]').parent("li").remove();
        $(field_id + ' .powertagging-extracted-tags .powertagging-tag[data-label="' + tag.label + '"]').removeClass("disabled");
      }

      // No empty ULs are allowed, remove them.
      if ($(field_id + " .powertagging-tag-result li").length === 0) {
        $(field_id + " .powertagging-tag-result ul").remove();
      }

      // Also remove the tag from the selected tags in the Drupal settings.
      if (settings.hasOwnProperty("selected_tags") && settings.selected_tags.length > 0) {
        for (var tag_index = 0; tag_index < settings.selected_tags.length; tag_index++) {
          if (settings.selected_tags[tag_index].label === tag.label) {
            settings.selected_tags.splice(tag_index, 1);
            break;
          }
        }
      }

      // Update the field value to save.
      updateFieldValue(field_id);
    }

    function updateFieldValue(field_id) {
      var tags_to_save = [];
      // Use tid for existing terms and label for new free terms.
      $(field_id + " .powertagging-tag-result .powertagging-tag").each(function() {
        if ($(this).attr("data-tid") > 0) {
          tags_to_save.push($(this).attr("data-tid"));
        }
        else {
          tags_to_save.push($(this).attr("data-label").replace(',', ';') + '|' + $(this).attr("data-uri"));
        }
      });

      $(field_id + " .powertagging-tag-result").closest(".field-type-powertagging").find("input.powertagging_tag_string").val(tags_to_save.join(','));
    }

    /**
     * Check if the currently selected entity language is allowed
     *
     * @param field_id
     *   The ID of the DOM element of the powertagging field
     * @param settings
     *   An array of settings of the powertagging field
     */
    function check_entity_language(field_id, settings) {
      var language_error_element = $('#' + $(field_id).children('fieldset').attr('id') + '-language-error');
      // The currently selected entity language is allowed.
      if ($.inArray(settings.entity_language, settings.allowed_languages) > -1) {
        language_error_element.hide();
      }
      // The currently selected entity language is not allowed.
      else {
        language_error_element.show();
      }
    }

    /**
     * Show loading image
     *
     * @param field_id
     *   The ID of the DOM element of the powertagging field
     */
    function loading_extracted_tags(field_id) {
      // Clear the extracted tags area
      $(field_id + " .powertagging-extracted-tags").slideUp().html("");
      $(field_id + " .powertagging-extracted-tags").prev().slideDown();
      $(field_id + " .powertagging-extracted-tags").parent().slideDown();
    }

    /**
     * Show extracted tags
     *
     * @param field_id
     *   The ID of the DOM element of the powertagging field
     */
    function show_extracted_tags(field_id) {
      $(field_id + " .powertagging-extracted-tags").show();
      $(field_id + " .powertagging-extracted-tags").prev().slideUp();
      $(field_id + " .powertagging-extracted-tags").slideDown();
    }

    /**
     * Hide extracted tags
     *
     * @param field_id
     *   The ID of the DOM element of the powertagging field
     */
    function hide_extracted_tags(field_id) {
      $(field_id + " .powertagging-extracted-tags").parent().hide();
      $(field_id + " .powertagging-extracted-tags").prev().hide();
      $(field_id + " .powertagging-extracted-tags").hide();
    }

  }
}
})(jQuery);
