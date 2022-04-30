Drupal.openlayers.pluginManager.register({
  fs: 'openlayers.Component:BootstrapjsPopup',
  init: function(data) {
    jQuery("body").append("<div id='popup'></div>");

    var popup = new ol.Overlay({
      element: document.getElementById('popup'),
      positioning: 'bottom-center',
      stopEvent: false
    });
    data.map.addOverlay(popup);

    data.map.on('click', function(evt) {

      if ('getFeaturesAtPixel' in map) {
        //  Introduced in v4.3.0 - new map.getFeaturesAtPixel() method.
        var features = data.map.getFeaturesAtPixel(evt.pixel);
      } else {
        //  Replaced in v4.3.0 - forEachFeatureAtPixel() method replaced.
        features = [];        
        data.map.forEachFeatureAtPixel(evt.pixel, function(feature) {
          features.push(feature);
        });
      }

      var feature = undefined;
      for (item of features) {
        feature = item;
      }
      
      var element = popup.getElement();
      jQuery(element).popover('destroy');

      if (feature) {
        var geometry = feature.getGeometry();
        var coord = geometry.getCoordinates();

        jQuery(element).popover('destroy');

        jQuery(element).popover({
          'placement': 'top',
          'html': true,
          'title': feature.get('name'),
          'content': feature.get('description')
        });

        popup.setPosition(coord);
        jQuery(element).popover('show');
      }
    });
  }
});
