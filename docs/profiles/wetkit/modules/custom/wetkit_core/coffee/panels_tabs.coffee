###
 @file
 Custom JS for administering WetKit Menu.
###

###
 JS to assist in creating Panels Tabs.
###

(($) ->

  Drupal.behaviors.panelsTabs =
    attach: (context) ->
      tabsID = Drupal.settings.panelsTabs.tabsID
      for key of Drupal.settings.panelsTabs.tabsID
        $("#" + tabsID[key] + ":not(.tabs-processed)", context).addClass("tabs-processed").tabs()

) jQuery
