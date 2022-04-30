Drupal.openlayers.pluginManager.register({
  fs: 'openlayers.Layer:Graticule',
  init: function(data) {
    if (typeof(ol.layer.Graticule) === 'function') {
      var graticule = new ol.layer.Graticule({
        strokeStyle: new ol.style.Stroke({
          color: 'rgba(' + data.opt.rgba + ')',
          width: data.opt.width,
          lineDash: data.opt.lineDash.split(',').map(Number)
        }),
        showLabels: data.opt.showLabels
      });
      return graticule;
    }
  }
});
