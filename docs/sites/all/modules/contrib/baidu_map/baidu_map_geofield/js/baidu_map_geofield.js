/**
 * @file
 * Behaviors for the Baidu Map Geofield module based on Baidu Map JS API V2.
 *
 * @link http://developer.baidu.com/map/reference/
 */

(function($) {

  /**
   * Instantiate all Baidu Maps with configured display settings.
   */
  Drupal.behaviors.geofieldBaiduMap = {
    attach: function(context, settings) {
      // Integrate with the defined geoField JS object.
      Drupal.geoField = Drupal.geoField || {};
      Drupal.geoField.maps = Drupal.geoField.maps || {};

      // For each map with the class geofieldBaiduMap, intantiate once.
      $('.geofieldBaiduMap', context).once('geofield-processed', function(index, element) {
        var data = undefined;
        var map_settings = [];
        // Get the Baidu Map HTML ID.
        var elemID = $(element).attr('id');

        if (settings.geofieldBaiduMap[elemID]) {
          // Get markers data from the geofieldBaiduMap settings for the map.
          data = settings.geofieldBaiduMap[elemID].data;
          // Get map display configurations from the geofieldBaiduMap settings.
          map_settings = settings.geofieldBaiduMap[elemID].map_settings;
        }

        // Checking to see if google variable exists. We need this b/c views
        // breaks this sometimes. Probably an AJAX/external javascript bug in
        // core or something.
        if (typeof BMap != 'undefined' && data != undefined) {
          var features = BaiduMapGeoJSON(data);

          // Map type defaults to "NORMAL".
          var maptype = {
            'mapType': BMAP_NORMAL_MAP
          }
          switch (map_settings.maptype) {
            case 'perspective':
              maptype.mapType = BMAP_PERSPECTIVE_MAP;
              break;

            case 'satellite':
              maptype.mapType = BMAP_SATELLITE_MAP;
              break;

            case 'hybrid':
              // Currently, only supported for Beijing, Shanghai and Guangzhou.
              maptype.mapType = BMAP_HYBRID_MAP;
              break;
          }

          // Instantiate Baidu Map.
          var map = new BMap.Map(elemID, maptype);

          // Set the map style.
          var mapStyle = {
            features: ["road", "building", "water", "land", "point"],
            style: map_settings.mapstyle
          }
          map.setMapStyle(mapStyle);

          // Enable Zoom in or out with mouse wheel, disabled by default.
          if (map_settings.scrollwheel) {
            map.enableScrollWheelZoom();
          }

          // Disable Dragging behavior for the map, enabled by default.
          if (!map_settings.draggable) {
            map.disableDragging();
          }

          // Show traffic, disabled by default.
          if (map_settings.showtraffic) {
            var traffic = new BMap.TrafficLayer();
            map.addTileLayer(traffic);
          }

          // Map scale hidden by default.
          if (map_settings.scalecontrol) {
            map.addControl(new BMap.ScaleControl());
          }

          // Navigation controls hidden by default.
          if (map_settings.navigationcontrol) {

            // Navigation controls defaults to "BMAP_NAVIGATION_CONTROL_LARGE".
            var opts = {}
            switch (map_settings.navigationcontrol) {
              case 'large':
                opts.type = BMAP_NAVIGATION_CONTROL_LARGE;
                break;

              case 'pan':
                opts.type = BMAP_NAVIGATION_CONTROL_PAN;
                break;

              case 'small':
                opts.type = BMAP_NAVIGATION_CONTROL_SMALL;
                break;

              case 'zoom':
                opts.type = BMAP_NAVIGATION_CONTROL_ZOOM;
                break;
            }
            // Add Navigation Controls to the map.
            map.addControl(new BMap.NavigationControl(opts));
          }

          // Map type control hidden by default.
          if (map_settings.maptypecontrol) {
            map.addControl(new BMap.MapTypeControl({mapTypes: [BMAP_NORMAL_MAP, BMAP_HYBRID_MAP, BMAP_PERSPECTIVE_MAP]}));
          }

          // The Bounds of the geometries are required for centering the map.
          var range = new BMap.Bounds();
          // Instantiate an empty InfoWindow to be attached to Markers.
          var infowindow = new BMap.InfoWindow({
            content: ''
          });

          // Store all points to be displayed with automatic zoom and center.
          var all_points = [];
          var markers = [];
          var infos = [];
          // Attach all geometries to the Baidu Map instance.
          if (features.getMap) {
            // Currently, there is no support for better handling of the zoom.
            placeFeature(features, map, range);
          } else {
            for (var i in features) {
              if (features[i].getMap) {
                placeFeature(features[i], map, range);
              } else {
                for (var j in features[i]) {
                  // Baidu Map handles each path as a separate Overlay.
                  if (features[i][j].getMap) {
                    placeFeature(features[i][j], map, range);
                  }
                  else {
                    for (var k in features[i][j]) {
                      if (features[i][j][k].getMap) {
                        placeFeature(features[i][j][k], map, range);
                      }
                    }
                  }
                }
              }
            }
          }
          if (map_settings.zoom == 'auto') {
            // Automatically zoom and center on all the points.
            map.setViewport(all_points);
          }
          else {
            // Set the default center and zoom value.
            map.centerAndZoom(range.getCenter(), new Number(map_settings.zoom));
          }
          // Store a reference to the map object to allow further interactions.
          Drupal.geoField.maps[elemID] = {
              'map':map,
              'markers': markers,
              'infos': infos,
          };
        }

        /**
         * Helper function to add a Point or a path to a Baidu Map Overlay.
         */
        function placeFeature(feature, map, range) {
          var properties = feature.geojsonProperties;
          // Only supported by Markers: set the title property.
          if (feature.setTitle && properties && properties.title) {
            feature.setTitle(properties.title);
          }
          // Add the feature to the map in an Overlay.
          map.addOverlay(feature);
          if (feature.getPosition) {
            // Extend bounds/range for each Point.
            range.extend(feature.getPosition());
            all_points.push(feature.getPosition());
          } else {
            // Extend bounds/range for each path.
            var path = feature.getPath();
            path.forEach(function(element) {
              range.extend(element);
              all_points.push(element);
            });
          }
          // Attach InfoWindow to Markers if there is any content to display.
          if (properties && properties.description) {
            var bounds = feature.bounds;
            // Only supported by Markers: attach InfoWindow on click event.
            if (feature.openInfoWindow) {
              feature.addEventListener('click', function() {
                // Centering is automatic for InfoWindow.
                infowindow.setContent(properties.description);
                this.openInfoWindow(infowindow, map.getCenter());
              });
            }
          }
          infos.push(properties.description);
          markers.push(feature);
        }
      });
    }
  }
})(jQuery)
