Visual Mapper

Changelog:
1.7.1 (03.10.2018)
- Fixed multiple bugs when updating the concept directly via ID in the tree
  view, data gets merged correctly now.

1.7 (04.09.2018)
- Made multiple VisualMapper instances on the same page possible by converting
  all IDs and general selections into classes and instance-specific selections
  wherever possible and made use of a new randomized ID everywhere else.
- Added the Visual Mapper instance as the first argument for every existing
  listener.
- Added a getDOMElement()-method in order the get the element a VisualMapper
  instance is attached to.
- Improved having multiple VisualMapper instances on the same page.
- Fixed d3js error messages thrown for uninitialized angles in paths.
- Added a new display type 'Tree View' and improved the display type switcher.
  This new display type comes with the "treeView" settings group, including
  setting "barHeight" to set the height of each bar in pixel,
  "collapseCircleRadius" to set the size of the circles to open or collapse a
  concept and "conceptFontSize".
- Made the duration of transitions configurable with the "transitionDuration"
  parameter.

1.6.1 (11.08.2017)
- Renamed setting "spiderChart.chartMarginLeft" to
  "spiderChart.chartShiftHorizontal" and setting "spiderChart.chartMarginTop"
  to "spiderChart.chartShiftVertical".
- Added settings "spiderChart.legendShiftHorizontal" and
  "spiderChart.legendShiftVertical" to customize the position of the legend
  additionally to legendPositionX and legendPositionY. Positive values shift the
  legend to the right / up, negative values shift the legend to the left / down.
- Added settings "export.exportButtonShiftHorizontal" and
  "export.exportButtonShiftVertical" to customize the position of the export
  button additionally to exportButtonPositionX and exportButtonPositionY.
  Positive values shift the export button to the right / up, negative values
  shift the export button to the left / down.
- Fixed reusing CSS styles in the exported image.

1.6 (21.07.2017)
- Added the possibilty to export the visualisation as an image. The new export
  button can be enabled / disabled and customized via the new configuration
  property section "export", which includes properties "enabled",
  "exportButtonRadius", "exportButtonPositionX", "exportButtonPositionY",
  "exportButtonColor" and "exportFileName". To adapt the text of the export
  button tooltip a new property "exportText" was added inside the "wording"-
  section.

1.5 (25.04.2017)
- Fixed cleaning of containers for obsolete relations.

1.4 (06.04.2017)
- Fixed a crash of the VisualMapper caused by updating the concept too fast.
  From now on method updateConcept() is only available if all transitions
  are finished.

1.3 (22.03.2017)
- JSON data returned by the server can now update the VisualMapper settings by
  setting the "settings" property of the root object. The structure of the
  settings here is the same as the "settings"-parameter of initVisualMapper().

1.2 (09.09.2015)
- Two new configuration parameters were added inside the
  spiderChart-configuration: "chartMarginLeft" and "chartMarginTop". These
  parameters make it possible to give the legend more space if required.
- If property "image" is available in the received JSON data for the currently
  viewed concept and it is a valid URL to an image, this property will be used
  as the background image of the root circle.
- Setting "loadParams" was added. This setting allows you to provide a set of
  "key --> value" pairs, which will be used as parameters in every GET-request
  additionally to parameters "uri" and "lang" (so don't use these two keys in
  your loadParams object).

1.1 (23.04.2015)
- Listener "visibleConceptsChanged" was added, which is triggered when the
  current root concept changes or the user switches any page.
- The relations to use can be specified in parameter "relations" of the settings
  for a higher flexibility now. This change causes settings "brightColors",
  "darkColors" and some sub-settings of settings "wording"
  ("legendConceptScheme", "legendParent", "legendChildren", "legendRelated") to
  be replaced by sub-settings of the new "relations" setting.
- The position of the legend can be customized with parameters "legendPositionX"
  (either "left" or "right") and "legendPositionY" (either "top" or "bottom")
  for spidercharts.
- A new settings parameter was added to chose the style of the legend:
  "legendStyle". It can be either "list" or "circle".
