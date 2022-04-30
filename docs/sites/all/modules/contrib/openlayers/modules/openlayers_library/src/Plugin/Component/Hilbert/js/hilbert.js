Drupal.openlayers.pluginManager.register({
  fs: 'openlayers.Component:Hilbert',
  init: function(data) {
    var layer = new ol.layer.Vector({
      source: new ol.source.Vector({
      })
    });

    var map = data.map;
    map.addLayer(layer);

    function makeFractal(depth) {
      var origin = new ol.geom.LineString([[0, 0]]);
      var point_count = (1 << (depth * 2));

      for (var i=0; i<point_count; i++) {
        var coords = hilbert.d2xy(depth, i).map(function(item) {
          return item * 10000000 / Math.sqrt(point_count);
        });
        origin.appendCoordinate(coords);
      }

      layer.getSource().clear();
      layer.getSource().addFeature(new ol.Feature(origin));

      document.getElementById('count').innerHTML = point_count - 1;
      document.getElementById('length').innerHTML = (point_count -1 )/Math.pow(2, depth);
    }

    var depthInput = document.getElementById('depth');

    function update() {
      makeFractal(Number(depthInput.value));
    }

    depthInput.onchange = function() {
      update();
    };

    update();
  }
});
