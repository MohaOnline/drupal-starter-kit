###
 @file
 Custom JS for administering WetKit Menu.
###

###
 In admin menu edit, this hides and closes the WET config depending on
 whether a minipanel is selected.
###

(($) ->
  Drupal.behaviors.menuMiniPanelsAdmin = attach: (context, settings) ->

    # Hide hover settings unless a minipanel is selected.
    toggleHoverSettings = ->
      if $("#edit-options-minipanel").val() is "" and $("#menu-minipanels-hover-settings").is(":visible")
        $("#menu-minipanels-hover-settings").slideUp 500
      else $("#menu-minipanels-hover-settings").slideDown 500  unless $("#edit-options-minipanel").val() is ""

    $("#edit-options-minipanel").change (e) ->
      toggleHoverSettings()


    # Set appropriate on load.
    toggleHoverSettings()
) jQuery
