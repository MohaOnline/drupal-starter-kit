Drupal.openlayers.pluginManager.register({
  fs: 'openlayers.Component:Autopopup',
  init: function(data) {
    var map = data.map;
    var random = (new Date()).getTime();

    var container = jQuery('<div/>', {
      id: 'popup-' + random,
      'class': 'ol-popup'
    }).appendTo('body');
    var content = jQuery('<div/>', {
      id: 'popup-content-' + random
    }).appendTo('#popup-' + random);

    var container = document.getElementById('popup-' + random);
    var content = document.getElementById('popup-content-' + random);

    if (data.opt.closer !== undefined && data.opt.closer !== 0) {
      var closer = jQuery('<a/>', {
        href: '#',
        id: 'popup-closer-' + random,
        'class': 'ol-popup-closer'
      }).appendTo('#popup-' + random);

      var closer = document.getElementById('popup-closer-' + random);

      /**
       * Add a click handler to hide the popup.
       * @return {boolean} Don't follow the href.
       */
      closer.onclick = function() {
        container.style.display = 'none';
        closer.blur();
        return false;
      };
    }

    /**
     * Create an overlay to anchor the popup to the map.
     */
    var overlay = new ol.Overlay({
      element: container,
      positioning: data.opt.positioning
    });

    map.addOverlay(overlay);

    map.getLayers().forEach(function(layer) {
      var source = layer.getSource();
      if (source.mn === data.opt.source) {
        source.on('change', function(evt) {
          var feature = source.getFeatures()[0];
          var coordinates = feature.getGeometry().getFirstCoordinate();

          if (feature) {
            var featureProperties = feature.getProperties();
            var popupContent = featureProperties.popup_content;

            for (key in featureProperties) {        
              if (key != 'popup_content') {
                oldPopupContent = popupContent + '.';
                while (oldPopupContent != popupContent) {
                  oldPopupContent = popupContent;
                  popupContent = popupContent.replace('${' + key + '}', featureProperties[key]);
                }
              }
            }
        
            var name = feature.get('name') || '';
            var description = feature.get('description') || '';

            if (popupContent != '') {
              content.innerHTML = '<div class="ol-popup-content">' + popupContent + '</div>';
              container.style.display = 'block';
              overlay.setPosition(coordinates);
            }

            if (data.opt.zoom !== 'disabled') {
              var dataExtent = feature.getGeometry().getExtent();

              if (!ol.extent.isEmpty(maxExtent)) {
                //  Introduced in v4.0.0 - number of parameters has changed to getView().fit function.
                if (map.getView().fit.length > 2) {
                  map.getView().fit(maxExtent, map.getSize());
                } else {
                  map.getView().fit(maxExtent);
                }
              }

              if (data.opt.zoom != 'auto') {
                var zoom = data.opt.zoom;
              } else {
                var zoom = map.getView().getZoom() - 1;
              }
              
              if (data.opt.enableAnimations == 1) {
                if (ol.hasOwnProperty('animation')) {
                  //  Deprecated in v3.20.0 - map.beforeRender() and ol.animation functions
                  var pan = ol.animation.pan({duration: data.opt.animations.pan, source: map.getView().getCenter()});
                  var zoomAnimation = ol.animation.zoom({duration: data.opt.animations.zoom, resolution: map.getView().getResolution()});
                  map.beforeRender(pan, zoomAnimation);
                  map.getView().setZoom(zoom);
                } else {
                  //  Introduced in v3.20.0 - view.animate() instead of map.beforeRender() and ol.animation functions
                  map.getView().animate({
                    zoom: zoom,
                    duration: data.opt.animations.zoom
                  });
                }
              } else {
                  map.getView().setZoom(zoom);                
              }
            }
          }
        }, source);
        source.changed();
      }
    });
  }
});
