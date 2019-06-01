(($) ->
  Drupal.behaviors.PanelsAccordionStyle =
    attach: (context, settings) ->
      for region_id of Drupal.settings.accordion
        accordion = Drupal.settings.accordion[region_id]
        jQuery("#" + region_id).accordion accordion.options
) jQuery
