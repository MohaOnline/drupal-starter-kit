Drupal.openlayers.pluginManager.register({
  fs: 'openlayers.Source:GPX',
  init: function(data) {
    data.opt.format = new ol.format.GPX();
    return new ol.source.Vector(data.opt);
  }
});
