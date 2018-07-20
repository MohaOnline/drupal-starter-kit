/**
 * @file
 * GeoJSON is used to create Baidu Maps API v2 vectors from GeoJSON objects.
 * This file was greatly inspired and adapted from the Geofield Map module
 * GeoJSON.js file used for the integration with Google Map API.
 *
 * @link https://github.com/JasonSanford/GeoJSON-to-Google-Maps
 * @link http://drupalcode.org/project/geofield.git/blob/refs/heads/7.x-2.x:/modules/geofield_map/js/GeoJSON.js
 * @link http://developer.baidu.com/map/reference/
 */

var BaiduMapGeoJSON = function(geojson, options) {
var _geometryToBaiduMaps = function(geojsonGeometry, opts, geojsonProperties) {

    var baiduObj;

    switch (geojsonGeometry.type) {
      case "Point":
        opts.position = new BMap.Point(geojsonGeometry.coordinates[0], geojsonGeometry.coordinates[1]);
        var bounds = new BMap.Bounds();
        bounds.extend(opts.position);
        baiduObj = new BMap.Marker(opts.position, opts);
        baiduObj.bounds = bounds;
        if (geojsonProperties) {
          baiduObj.geojsonProperties = geojsonProperties;
        }
        break;

      case "MultiPoint":
        baiduObj = [];
        var bounds = new BMap.Bounds();
        for (var i = 0; i < geojsonGeometry.coordinates.length; i++){
          opts.position = new BMap.Point(geojsonGeometry.coordinates[i][0], geojsonGeometry.coordinates[i][1]);
          bounds.extend(opts.position);
          baiduObj.push(new BMap.Marker(opts.position, opts));
          baiduObj[i].bounds = bounds;
        }
        if (geojsonProperties) {
          for (var k = 0; k < baiduObj.length; k++){
            baiduObj[k].geojsonProperties = geojsonProperties;
          }
        }
        break;

      case "LineString":
        var path = [];
        var bounds = new BMap.Bounds();
        for (var i = 0; i < geojsonGeometry.coordinates.length; i++){
          var coord = geojsonGeometry.coordinates[i];
          var ll = new BMap.Point(coord[0], coord[1]);
          bounds.extend(ll);
          path.push(ll);
        }
        opts.path = path;
        baiduObj = new BMap.Polyline(opts.path, opts);
        baiduObj.bounds = bounds;
        if (geojsonProperties) {
          baiduObj.geojsonProperties = geojsonProperties;
        }
        break;

      case "MultiLineString":
        baiduObj = [];
        var bounds = new BMap.Bounds();
        for (var i = 0; i < geojsonGeometry.coordinates.length; i++){
          var path = [];
          for (var j = 0; j < geojsonGeometry.coordinates[i].length; j++){
            var coord = geojsonGeometry.coordinates[i][j];
            var ll = new BMap.Point(coord[0], coord[1]);
            bounds.extend(ll);
            path.push(ll);
          }
          opts.path = path;
          baiduObj.push(new BMap.Polyline(opts.path, opts));
          baiduObj[i].bounds = bounds;
        }
        if (geojsonProperties) {
          for (var k = 0; k < baiduObj.length; k++){
            baiduObj[k].geojsonProperties = geojsonProperties;
          }
        }
        break;

      case "Polygon":
        // Baidu handles each polygon as a separate path.
        baiduObj = [];
        var bounds = new BMap.Bounds();
        for (var i = 0; i < geojsonGeometry.coordinates.length; i++){
          var path = [];
          for (var j = 0; j < geojsonGeometry.coordinates[i].length; j++){
            var ll = new BMap.Point(geojsonGeometry.coordinates[i][j][0], geojsonGeometry.coordinates[i][j][1]);
            bounds.extend(ll);
            path.push(ll)
          }
          opts.paths = path;
          // Each Polygon needs to be rendered separately.
          baiduObj.push(new BMap.Polygon(opts.paths, opts));
          baiduObj[i].bounds = bounds;
        }
        if (geojsonProperties) {
          for (var k = 0; k < baiduObj.length; k++){
            baiduObj[k].geojsonProperties = geojsonProperties;
          }
        }
        break;

      case "MultiPolygon":
        baiduObj = [];
        var bounds = new BMap.Bounds();
        for (var i = 0; i < geojsonGeometry.coordinates.length; i++){
          for (var j = 0; j < geojsonGeometry.coordinates[i].length; j++){
            var path = [];
            for (var k = 0; k < geojsonGeometry.coordinates[i][j].length; k++){
              var ll = new BMap.Point(geojsonGeometry.coordinates[i][j][k][0], geojsonGeometry.coordinates[i][j][k][1]);
              bounds.extend(ll);
              path.push(ll);
            }
            // Aggregate each Polygon to be rendered as a separate Overlay.
            opts.paths = path;
            baiduObj.push(new BMap.Polygon(opts.paths, opts));
            baiduObj[i + j].bounds = bounds;
          }
        }
        if (geojsonProperties) {
          for (var k = 0; k < baiduObj.length; k++){
            baiduObj[k].geojsonProperties = geojsonProperties;
          }
        }
        break;

      case "GeometryCollection":
        baiduObj = [];
        if (!geojsonGeometry.geometries){
          baiduObj = _error("Invalid GeoJSON object: GeometryCollection object missing \"geometries\" member.");
        }else{
          for (var i = 0; i < geojsonGeometry.geometries.length; i++){
            baiduObj.push(_geometryToBaiduMaps(geojsonGeometry.geometries[i], opts, geojsonProperties || null));
          }
        }
        break;

      default:
        baiduObj = _error("Invalid GeoJSON object: Geometry object must be one of \"Point\", \"LineString\", \"Polygon\" or \"MultiPolygon\".");
    }

    return baiduObj;

  };

  var _error = function(message) {

    return {
      type: "Error",
      message: message
    };

  };

  var obj;

  var opts = options || {};

  switch (geojson.type) {

    case "FeatureCollection":
      if (!geojson.features){
        obj = _error("Invalid GeoJSON object: FeatureCollection object missing \"features\" member.");
      } else {
        obj = [];
        for (var i = 0; i < geojson.features.length; i++){
          obj.push(_geometryToBaiduMaps(geojson.features[i].geometry, opts, geojson.features[i].properties));
        }
      }
      break;

    case "GeometryCollection":
      if (!geojson.geometries){
        obj = _error("Invalid GeoJSON object: GeometryCollection object missing \"geometries\" member.");
      }else{
        obj = [];
        for (var i = 0; i < geojson.geometries.length; i++){
          obj.push(_geometryToBaiduMaps(geojson.geometries[i], opts, geojson.geometries[i].properties));
        }
      }
      break;

    case "Feature":
      if (!(geojson.properties && geojson.geometry)) {
        obj = _error("Invalid GeoJSON object: Feature object missing \"properties\" or \"geometry\" member.");
      }else{
        obj = _geometryToBaiduMaps(geojson.geometry, opts, geojson.properties);
      }
      break;

    case "Point": case "MultiPoint": case "LineString": case "MultiLineString": case "Polygon": case "MultiPolygon":
      obj = geojson.coordinates
        ? obj = _geometryToBaiduMaps(geojson, opts, geojson.properties)
        : _error("Invalid GeoJSON object: Geometry object missing \"coordinates\" member.");
      break;

    default:
      obj = _error("Invalid GeoJSON object: GeoJSON object must be one of \"Point\", \"LineString\", \"Polygon\", \"MultiPolygon\", \"Feature\", \"FeatureCollection\" or \"GeometryCollection\".");

  }

  return obj;
};
