Drupal.openlayers.pluginManager.register({
  fs: 'openlayers.Layer:Group',
  init: function(data) {
    var layers = [];

    for (var i in data.opt.grouplayers) {
      if (data.objects.layers[data.opt.grouplayers[i]] !== undefined) {
        layers[i] = data.objects.layers[data.opt.grouplayers[i]];
        data.map.removeLayer(layers[i]);
      }
    }

    return new ol.layer.Group({
      title: data.opt.grouptitle,
      layers: layers.reverse()
    });
  }
});
