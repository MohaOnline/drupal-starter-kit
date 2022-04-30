Drupal.openlayers.pluginManager.register({
  fs: 'openlayers.Component:Tooltip',
  init: function(data) {
    var map = data.map;
    var container = jQuery('<div/>', {
      id: 'tooltip',
      'class': 'ol-tooltip'
    }).appendTo('body');
    var content = jQuery('<div/>', {
      id: 'tooltip-content'
    }).appendTo('#tooltip');

    var container = document.getElementById('tooltip');
    var content = document.getElementById('tooltip-content');

    /**
     * Create an overlay to anchor the popup to the map.
     */
    var overlay = new ol.Overlay({
      element: container,
      positioning: data.opt.positioning
    });

    map.addOverlay(overlay);

    jQuery(map.getViewport()).on('mousemove', function(evt) {
      var pixel = map.getEventPixel(evt.originalEvent);
      var coordinates = map.getEventCoordinate(evt.originalEvent);

      if ('getFeaturesAtPixel' in map) {
        //  Introduced in v4.3.0 - new map.getFeaturesAtPixel() method.
        var features = map.getFeaturesAtPixel(pixel);
      } else {
        //  Replaced in v4.3.0 - forEachFeatureAtPixel() method replaced.
        features = [];        
        map.forEachFeatureAtPixel(pixel, function(feature) {
          features.push(feature);
        });
      }

      var feature = undefined;
      
      if (features && features.length > 0) {
        for (item of features) {
          feature = item;
        }
      }

      if (feature) {
        var featureProperties = feature.getProperties();
        var tooltipContent = featureProperties.tooltip_content;
        for (key in featureProperties) {        
          if (key != 'tooltip_content') {
            oldTooltipContent = tooltipContent + '.';
            while (oldTooltipContent != tooltipContent) {
              oldTooltipContent = tooltipContent;
              tooltipContent = tooltipContent.replace('${' + key + '}', featureProperties[key]);
            }
          }
        }

        if (tooltipContent != '') {
          overlay.setPosition(coordinates);
          content.innerHTML = '<div class="ol-tooltip-content">' + tooltipContent + '</div>';
          container.style.display = 'block';
        }
      } else {
        container.style.display = 'none';
      }
    });
  }
});
