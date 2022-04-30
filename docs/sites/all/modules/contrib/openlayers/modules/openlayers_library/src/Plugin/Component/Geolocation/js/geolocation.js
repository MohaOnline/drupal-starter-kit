Drupal.openlayers.pluginManager.register({
  fs: 'openlayers.Component:Geolocation',
  init: function(data) {
    var map = data.map;

    var geolocation = new ol.Geolocation({
      projection: map.getView().getProjection()
    });

    document.getElementById(data.opt.checkboxID).addEventListener('change', function() {
      geolocation.setTracking(this.checked);
    });

    // update the HTML page when the position changes.
    geolocation.on('change', function() {
      jQuery('#' + data.opt.positionAccuracyID).val(geolocation.getAccuracy() + ' [m]');
      jQuery('#' + data.opt.altitudeID).val(geolocation.getAltitude() + ' [m]');
      jQuery('#' + data.opt.altitudeAccuracyID).val(geolocation.getAltitudeAccuracy() + ' [m]');
      jQuery('#' + data.opt.headingID).val(geolocation.getHeading() + ' [rad]');
      jQuery('#' + data.opt.speedID).val(geolocation.getSpeed() + ' [m/s]');

      if (ol.hasOwnProperty('animation')) {
        //  Deprecated in v3.20.0 - map.beforeRender() and ol.animation functions
        var pan = ol.animation.pan({
          duration: 2000,
          source: map.getView().getCenter()
        });
        var zoom = ol.animation.zoom({
          duration: 2000,
          resolution: map.getView().getResolution(),
          source: (map.getView().getZoom())
        });

        map.beforeRender(pan, zoom);
        map.getView().setCenter(geolocation.getPosition());
        map.getView().setZoom(7);
      } else {
        //  Introduced in v3.20.0 - view.animate() instead of map.beforeRender() and ol.animation functions
        //  TODO - need to complete this section
        map.getView().animate({
          zoom: zoom,
          duration: data.opt.animations.zoom
        });
      }
    });

    geolocation.on('error', function(error) {
      var info = document.getElementById('info');
      info.innerHTML = error.message;
      info.style.display = '';
    });

    var accuracyFeature = new ol.Feature();
    geolocation.on('change:accuracyGeometry', function() {
      accuracyFeature.setGeometry(geolocation.getAccuracyGeometry());
    });

    var positionFeature = new ol.Feature();
    geolocation.on('change:position', function() {
      var coordinates = geolocation.getPosition();
      positionFeature.setGeometry(coordinates ?
        new ol.geom.Point(coordinates) : null);
    });

    var featuresOverlay = new ol.layer.Vector({
      map: map,
      source: new ol.source.Vector({
        features: [accuracyFeature, positionFeature]
      })
    });
  }
});
