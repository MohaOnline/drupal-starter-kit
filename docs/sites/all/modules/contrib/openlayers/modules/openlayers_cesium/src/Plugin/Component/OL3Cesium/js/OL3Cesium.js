Drupal.openlayers.pluginManager.register({
  fs: 'openlayers.Component:OL3Cesium',
  init: function(data) {
    var ol3d = new olcs.OLCesium({map: data.map});
    ol3d.setEnabled(true);
    data.map.set('ol3d', ol3d);
  }
});
