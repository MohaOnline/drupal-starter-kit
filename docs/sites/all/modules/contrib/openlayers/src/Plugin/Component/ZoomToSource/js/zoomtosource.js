Drupal.openlayers.pluginManager.register({
  fs: 'openlayers.Component:ZoomToSource',
  init: function(data) {
    var map = data.map;

    function getLayersFromObject(object) {
      var layersInside = new ol.Collection();

      object.getLayers().forEach(function(layer) {
        if (layer instanceof ol.layer.Group) {
          layersInside.extend(getLayersFromObject(layer).getArray());
        } else {
          if (typeof layer.getSource === 'function') {
            if (layer.getSource() !== 'null' && layer.getSource() instanceof ol.source.Vector) {
              layersInside.push(layer);
            }
          }
        }
      });

      return layersInside;
    }

    var calculateMaxExtent = function() {
      var maxExtent = ol.extent.createEmpty();

      layers.forEach(function (layer) {
        var source = layer.getSource();
        if (typeof source.getFeatures === 'function') {
          if (source.getFeatures().length !== 0) {
            ol.extent.extend(maxExtent, source.getExtent());
          }
        }
      });

      return maxExtent;
    };

    var zoomToSource = function(source) {
      if (!data.opt.process_once || !data.opt.processed_once) {
        data.opt.processed_once = true;
        if (data.opt.enableAnimations === 1) {     
          if (ol.hasOwnProperty('animation')) {
            //  Deprecated in v3.20.0 - map.beforeRender() and ol.animation functions
            var animationPan = ol.animation.pan({
              duration: data.opt.animations.pan,
              source: map.getView().getCenter()
            });
            var animationZoom = ol.animation.zoom({
              duration: data.opt.animations.zoom,
              resolution: map.getView().getResolution()
            });
            map.beforeRender(animationPan, animationZoom);
            
            var maxExtent = calculateMaxExtent();

            if (!ol.extent.isEmpty(maxExtent)) {
              //  Introduced in v4.0.0 - number of parameters has changed to getView().fit function.
              if (map.getView().fit.length > 2) {
                map.getView().fit(maxExtent, map.getSize());
              } else {
                map.getView().fit(maxExtent);
              }
            }

            var zoom = Math.floor(map.getView().getZoom());
            map.getView().setZoom(zoom);
          } else {
            //  Introduced in v3.20.0 - view.animate() instead of map.beforeRender() and ol.animation functions
            var maxExtent = calculateMaxExtent();

            if (!ol.extent.isEmpty(maxExtent)) {
              //  Introduced in v4.0.0 - number of parameters has changed to getView().fit function.
              if (map.getView().fit.length > 2) {
                map.getView().fit(maxExtent, map.getSize());
              } else {
                map.getView().fit(maxExtent);
              }
            }

            var zoom = Math.floor(map.getView().getZoom());
            map.getView().animate({
              zoom: zoom,
              center: ol.extent.getCenter(maxExtent),
              duration: data.opt.animations.zoom
            });
          }
        } else {
          var maxExtent = calculateMaxExtent();

          if (!ol.extent.isEmpty(maxExtent)) {
            //  Introduced in v4.0.0 - number of parameters has changed to getView().fit function.
            if (map.getView().fit.length > 2) {
              map.getView().fit(maxExtent, map.getSize());
            } else {
              map.getView().fit(maxExtent);
            }
          }

          if (data.opt.zoom !== 'disabled') {
            if (data.opt.zoom !== 'auto') {
              map.getView().setZoom(data.opt.zoom);
            } else {
              var zoom = Math.floor(map.getView().getZoom());
              if (data.opt.max_zoom !== undefined && data.opt.max_zoom > 0 && zoom > data.opt.max_zoom) {
                zoom = data.opt.max_zoom;
              }
              map.getView().setZoom(zoom);
            }
          }        
        }
      }
    };

    var layers = getLayersFromObject(map);
    layers.forEach(function (layer) {
      var source = layer.getSource();

      // Only zoom to a source if it's in the configured list of sources.
      if (typeof data.opt.source[source.mn] !== 'undefined') {
        source.on('change', zoomToSource, source);
        if (typeof source.getFeatures === 'function') {
          if (source.getFeatures().length !== 0) {
            zoomToSource.call(source);
          }
        }
      }
    });
  }
});
