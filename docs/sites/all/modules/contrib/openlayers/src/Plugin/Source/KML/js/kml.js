Drupal.openlayers.pluginManager.register({
  fs: 'openlayers.Source:KML',
  init: function(data) {
    var extractStyles = (data.opt.extract_styles !== undefined) ? data.opt.extract_styles : false;
    data.opt.format = new ol.format.KML({
      extractStyles: extractStyles
    });
    return new ol.source.Vector(data.opt);
  }
});
