/**
 * @file
 * Example of Baidu Map JS API V2 implementation to extend default behavior.
 * 
 * This file provides a few examples based on the Baidu Map JS API, of how map
 * objects could interact with the rest of the page DOM and specific JS logic
 * triggered by other objects or events on the page. To use and test these
 * examples, this file should be loaded after module's other JS files. Then,
 * modify the map ID below to match with the map to be tested.
 * API: http://developer.baidu.com/map/jshome.htm.
 * Library: http://developer.baidu.com/map/library.htm.
 */

jQuery(function($) {
  (function($) {
    // Retrieve the BaiduMapAPI object instantiated and stored in page DOM.
    var BaiduMapAPI = Drupal.geoField.maps;
    // Example of an HTML ID for a Node nid 6 and field_map.
    // elemID should be replaced before testing.
    var elemID = 'geofield-baidu-map-entity-node-12-field-baidumap';
    if ((typeof BaiduMapAPI !== 'undefined') && BaiduMapAPI[elemID]) {
      // Get the data, map settings and map objects for the map ID.
      var data = Drupal.settings.geofieldBaiduMap[elemID].data,
        map_settings = Drupal.geoField.maps[elemID].map_settings,
        map = BaiduMapAPI[elemID].map,
        markers = BaiduMapAPI[elemID].markers;

      /**
       * Example 1: Execute a local search when #header is clicked.
       * Example 2: Execute a local search inbounds when #page-title is clicked.
       * http://developer.baidu.com/map/jsdevelop-8.htm.
       */
      var local = new BMap.LocalSearch(map, {
        renderOptions: {map: map}
      });

      // Execute a local search.
      $("body").bind("click", function() {
         local.search("上海");
      });
      // Execute a local search restricted to map bounds.
      $("#page-title").bind("click", function() {
        local.searchInBounds("银行", map.getBounds());
      })

      /**
       * Example 3: Removing features:
       *   删除一个marker. Remove a single specific feature.
       *   map.removeOverlay(markers[0]);
       *
       *   删除所有的marker. Remove all features/clear the map.
       *   map.clearOverlays();
       */

      /**
       * Example 4: Resetting Marker's infoWindow, 重新定义infowindow.
       */
      // Override the addMarker function.
      var addMarker = function(point, description, key) {
        var marker = new BMap.Marker(point);
        // Add marker to pointer stack for later retrieval.
        markers[key] = marker;

        // Add infoWindow to be displayed when marker is clicked.
        marker.addEventListener('click', function() {
          // Declare Baidu JS Geocoder to be used for reverse geocoding.
          var myGeo = new BMap.Geocoder();
          // 根据坐标得到地址描述.
          var self = this;
          // Reverse geocode from Baidu Map JS API to bet more information.
          myGeo.getLocation(new BMap.Point(point.lng, point.lat), function(result) {
            var opts = {
              // Set message title.
              title: description,
              // Allow send message.
              enableMessage: true,
            }
            // Get additional address information to display in the infoWindow.
            if (result) {
              var infoWindow = new BMap.InfoWindow(result.address, opts);
            } else {
              var infoWindow = new BMap.InfoWindow(description);
            }
            self.openInfoWindow(infoWindow, map.getCenter());
          });
        });

        map.addOverlay(marker);
        // If several markers would need to be displayed on the map, by
        // default, center and zoom on the first one.
        if (key == 0) {
          map.centerAndZoom(point, new Number(map_settings.zoom));
          // To display 3D map, current city would need to be set.
          map.setCurrentCity(description);
        }
      }

      // Add all map markers.
      for (var i = 0; i < data.length; i++) {
        // Create a Point based on geo-coordinates data information.
        var point = new BMap.Point(data[i].coordinates[0], data[i].coordinates[1]);
        // Add a marker on the map.
        addMarker(point, data[i].description, i);
      }
      // END Example 4 / END 重新定义Infowindow.

      /**
       * Example 5: map控件, Interacting with map controls.
       * http://developer.baidu.com/map/jsdevelop-3.htm
       * http://developer.baidu.com/map/reference/index.php?title=Class:%E6%80%BB%E7%B1%BB/%E6%A0%B8%E5%BF%83%E7%B1%BB
       * */
//      map.disableDragging();
//      map.enableScrollWheelZoom();
      var opts = {type: BMAP_NAVIGATION_CONTROL_LARGE}
      map.addControl(new BMap.NavigationControl(opts));
      
      // get marker and info(window info).
      var map = BaiduMapAPI[elemID].map,
          markers = BaiduMapAPI[elemID].markers;
          infos = BaiduMapAPI[elemID].infos;

      // pop window info.
      var infoWindow = new BMap.InfoWindow('test');
      markers[0].openInfoWindow(infoWindow, map.getCenter());
    }
  })($);
});
