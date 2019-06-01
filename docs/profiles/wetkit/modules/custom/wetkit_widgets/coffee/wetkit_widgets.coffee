###
@file
Attaches the behaviors for the WetKit Widgets module.
###

###
 Form behavior for Tabbed Interface
###

Drupal.settings.tabbed_interface_settings = Drupal.settings.tabbed_interface_settings or {}

(($) ->

  Drupal.behaviors.wetkitTabbed_Interface = attach: (context, settings) ->
    if $(".ipe_tabs_interface .tabs").length
      loading_finished = "wb-init-loaded"
      $(document).on loading_finished, ->

      pe.wb_load
        plugins:
          tabbedinterface: $(".ipe_tabs_interface", context)
      , loading_finished

) jQuery
