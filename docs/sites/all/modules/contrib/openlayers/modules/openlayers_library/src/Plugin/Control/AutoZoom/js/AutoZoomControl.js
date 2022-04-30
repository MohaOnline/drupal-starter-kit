ol.control.AutoZoom = function(opt_options) {
  var options = opt_options || {};
  var className = options.className || 'ol-autozoom';

  var this_ = this;
  var handleClick_ = function(e) {
    this_.handleClick_(e);
  };

  var autozoomLabel = options.autozoomLabel || 'A';
  var autozoomTipLabel = options.autozoomTipLabel || 'Autozoom';

  var button = document.createElement('button');
  button.innerHTML = autozoomLabel;
  button.title = autozoomTipLabel;
  button.className = className + '-autozoom';
  button.type = 'button';

  var element = document.createElement('div');
  element.className = className + ' ol-unselectable ol-control';
  element.appendChild(button);

  ol.control.Control.call(this, {
    element: element,
    target: options.target
  });

  button.addEventListener('click', handleClick_, false);
  this.options = options;
};

if (ol.hasOwnProperty('inherits')) {
  //  Deprecated in v6.0.0 - ol.inherits function.
  ol.inherits(ol.control.AutoZoom, ol.control.Control);
} else {
  //  Introduced in v6.0.0 - replace with ECMAScript classes.
  ol.control.AutoZoom.prototype = Object.create(ol.control.Control.prototype);
  ol.control.AutoZoom.prototype.constructor = ol.control.AutoZoom;
}

/**
 * @param {event} event Browser event.
 */
ol.control.AutoZoom.prototype.handleClick_ = function(event) {
  event.preventDefault();
  event.target.blur();

  var map = this.getMap();
  var options = this.options;

  function getLayersFromObject(object) {
    var layersInside = new ol.Collection();

    object.getLayers().forEach(function (layer) {
      if (layer instanceof ol.layer.Group) {
        layersInside.extend(getLayersFromObject(layer).getArray());
      }
      else {
        if (typeof layer.getSource === 'function') {
          layersInside.push(layer);
        }
      }
    });

    return layersInside;
  }

  var calculateMaxExtent = function() {
    var maxExtent = ol.extent.createEmpty();

    getLayersFromObject(map).forEach(function (layer) {
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
    if (!options.process_once || !options.processed_once) {
      options.processed_once = true;

      if (options.enableAnimations === 1) {
        if (ol.hasOwnProperty('animation')) {
          //  Deprecated in v3.20.0 - map.beforeRender() and ol.animation functions
          var animationPan = ol.animation.pan({
            duration: options.animations.pan,
            source: map.getView().getCenter()
          });
          var animationZoom = ol.animation.zoom({
            duration: options.animations.zoom,
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
          
          //  @TODO - find a way to to determine zoom without fitting theextend into the view, thereby ruining the animation
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
            duration: options.animations.zoom
          });
        }
      } else {
        //  No animation
        var maxExtent = calculateMaxExtent();
        if (!ol.extent.isEmpty(maxExtent)) {
            //  Introduced in v4.0.0 - number of parameters has changed to getView().fit function.
          if (map.getView().fit.length > 2) {
            map.getView().fit(maxExtent, map.getSize());
          } else {
            map.getView().fit(maxExtent);
          }
        }

        if (options.zoom !== 'disabled') {
          if (options.zoom !== 'auto') {
            map.getView().setZoom(options.zoom);
          } else {
            var zoom = Math.floor(map.getView().getZoom());
            if (options.max_zoom !== undefined && options.max_zoom > 0 && zoom > options.max_zoom) {
              zoom = options.max_zoom;
            }
            map.getView().setZoom(zoom);
          }
        }       
      }
    }
  };
  zoomToSource.call();
};
