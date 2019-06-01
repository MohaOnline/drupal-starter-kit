###
 @file
 Custom JS for administering WetKit Bean.
###

###
 JS to assist in managing the Bean Back End.
###

(($) ->
  Drupal.behaviors.wetkitbeanAdmin = attach: (context, settings) ->
    unless $(context).hasClass("draggable")

      # Hide all current slide forms.
      $("table#field-bean-slide-collection-values tr.draggable:not(.bean-slide-processed)", context).each ->
        $form = $(this).addClass("bean-slide-processed").find("> td:nth-child(2)")
        Drupal.wetkitbeanAdmin.setupForm $form
        $form.before Drupal.wetkitbeanAdmin.createOverview($form)


      # Hide new slide form.
      $("table#field-bean-slide-collection-values tr.draggable:last", context).each ->
        unless $(this).hasClass("bean-slide-processed")
          $form = $(this).addClass("bean-slide-processed").find("> td:nth-child(2)")
          Drupal.wetkitbeanAdmin.setupForm $form
          $form.before Drupal.wetkitbeanAdmin.createAdd($form)


  Drupal.wetkitbeanAdmin = Drupal.wetkitbeanAdmin or {}

  # Initially hide individual slide forms and add "Collapse" button when form is
  # displayed.
  Drupal.wetkitbeanAdmin.setupForm = ($form) ->
    $form.addClass("slide-form").hide()
    $button = $("<div class=\"form-actions\"><input class=\"form-submit submit\" type=\"button\" value=\"Collapse\" /></div>").click(->
      $form.hide()
      Drupal.wetkitbeanAdmin.triggerChange()
      $form.before Drupal.wetkitbeanAdmin.createOverview($form)
      false
    )
    $form.append $button


  # Create overview page for the add new slide form.
  Drupal.wetkitbeanAdmin.createAdd = ($form) ->
    html = $(Drupal.theme("wetkitbeanNewOverview"))
    html.find("a.add").click ->
      $form.show()
      $(this).parents("td.overview").remove()
      false

    html


  # Create overview page for existing slides.
  Drupal.wetkitbeanAdmin.createOverview = ($form) ->
    html = $(Drupal.theme("wetkitbeanOverview", $form))
    html.find(".edit").click ->
      $form.show()
      $(this).parents("td.overview").remove()
      false

    html


  # Borrow the changed warning system from tabledrag to indicate to user that
  # form still needs to be saved.
  Drupal.wetkitbeanAdmin.triggerChange = ->
    if Drupal.tableDrag["field-bean-slide-collection-values"].changed is false
      $(Drupal.theme("tableDragChangedWarning")).insertBefore(Drupal.tableDrag["field-bean-slide-collection-values"].table).hide().fadeIn "slow"
      Drupal.tableDrag["field-bean-slide-collection-values"].changed = true

  Drupal.theme::wetkitbeanNewOverview = ->
    "<td class=\"overview\"><ul class=\"links\"><li><a class=\"add\" href=\"#\">New Slide</a></li></ul></td>"

  Drupal.theme::wetkitbeanOverview = ($form) ->
    html = "<td class=\"overview\"><div class=\"overview-column\">"
    html += $("div.media-thumbnail", $form).html()  if $("div.media-thumbnail", $form).html()
    html += "</div><div class=\"overview-column headline\">"
    if $(".field-name-field-slide-headline input", $form).val()
      html += $(".field-name-field-slide-headline input", $form).val()
    else

      # Allow the div to have width, so that the Expand button will line up
      # with slides that *do* have a headline.
      html += "&nbsp;"
    html += "</div>"
    html += "<div class=\"overview-column form-actions form-wrapper\"><input type=\"button\" class=\"form-submit edit\" value=\"Expand\" /></div>"
    html += "</td>"
    html
) jQuery
