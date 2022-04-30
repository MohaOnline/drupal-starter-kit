Drupal.openlayers.pluginManager.register({
    fs: 'openlayers.Control:OL3CesiumControl',
    init: function(data) {
        return new ol.control.OL3CesiumControl(data.opt, data.map);
    }
});
