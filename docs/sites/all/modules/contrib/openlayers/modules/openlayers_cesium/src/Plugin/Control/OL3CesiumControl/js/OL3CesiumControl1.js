/**
 * OL3CesiumControl control
 */
ol.control.OL3CesiumControl = function(opt_options, map) {
    var options = opt_options || {};
    var className = options.className || 'ol-ol3cesiumcontrol';

    var this_ = this;
    var handleClick_ = function(e) {
        this_.handleClick_(e);
    };

    var button = document.createElement('button');
    button.innerHTML = options.ol3cesiumcontrolLabel || '3D';
    button.title = options.ol3cesiumcontrolTipLabel || 'Toggle';
    button.className = className + '-ol3cesiumcontrol';
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
    this.button = button;
};

if (ol.hasOwnProperty('inherits')) {
  //  Deprecated in v6.0.0 - ol.inherits function.
  ol.inherits(ol.control.OL3CesiumControl, ol.control.Control);
} else {
  //  Introduced in v6.0.0 - replace with ECMAScript classes.
  ol.control.OL3CesiumControl.prototype = Object.create(ol.control.Control.prototype);
  ol.control.OL3CesiumControl.prototype.constructor = ol.control.OL3CesiumControl;
}

/**
 * @param {event} event Browser event.
 */
ol.control.OL3CesiumControl.prototype.handleClick_ = function(event) {
    if (this.getMap().get('ol3d') !== undefined) {
        this.ol3d = this.getMap().get('ol3d');
    }

    if (this.ol3d !== undefined) {
        if (this.ol3d.getEnabled()) {
            this.button.innerHTML = "3D";
            this.ol3d.setEnabled(false);
        } else {
            this.button.innerHTML = "2D";
            this.ol3d.setEnabled(true);
        }
    } else {
        this.button.innerHTML = "2D";
        this.ol3d = new olcs.OLCesium({map: this.getMap()});
        this.ol3d.setEnabled(true);
    }
};