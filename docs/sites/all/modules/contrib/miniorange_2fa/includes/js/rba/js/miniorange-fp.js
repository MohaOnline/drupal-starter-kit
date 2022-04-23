var geoip2 = function () {
  "use strict";

  function Lookup(successCallback, errorCallback, options, type) {
    this.successCallback = successCallback, this.errorCallback = errorCallback, this.geolocation = options && options.hasOwnProperty("geolocation") ? options.geolocation : navigator.geolocation, this.type = type
  }

  var exports = {};
  Lookup.prototype.returnSuccess = function (location) {
    this.successCallback && "function" == typeof this.successCallback && this.successCallback(this.fillInObject(this.objectFromJSON(location)))
  }, Lookup.prototype.returnError = function (error) {
    this.errorCallback && "function" == typeof this.errorCallback && (error || (error = {error: "Unknown error"}), this.errorCallback(error))
  }, Lookup.prototype.objectFromJSON = function (json) {
    return "undefined" != typeof window.JSON && "function" == typeof window.JSON.parse ? window.JSON.parse(json) : eval("(" + json + ")")
  };
  var fillIns = {
    country: [["continent", "Object", "names", "Object"], ["country", "Object", "names", "Object"], ["registered_country", "Object", "names", "Object"], ["represented_country", "Object", "names", "Object"], ["traits", "Object"]],
    city: [["city", "Object", "names", "Object"], ["continent", "Object", "names", "Object"], ["country", "Object", "names", "Object"], ["location", "Object"], ["postal", "Object"], ["registered_country", "Object", "names", "Object"], ["represented_country", "Object", "names", "Object"], ["subdivisions", "Array", 0, "Object", "names", "Object"], ["traits", "Object"]]
  };
  return Lookup.prototype.fillInObject = function (obj) {
    for (var fill = "country" === this.type ? fillIns.country : fillIns.city, i = 0; i < fill.length; i++) for (var path = fill[i], o = obj, j = 0; j < path.length; j += 2) {
      var key = path[j];
      o[key] || (o[key] = "Object" === path[j + 1] ? {} : []), o = o[key]
    }
    try {
      Object.defineProperty(obj.continent, "continent_code", {
        enumerable: !1, get: function () {
          return this.code
        }, set: function (value) {
          this.code = value
        }
      })
    } catch (e) {
      obj.continent.code && (obj.continent.continent_code = obj.continent.code)
    }
    if ("country" !== this.type) try {
      Object.defineProperty(obj, "most_specific_subdivision", {
        enumerable: !1, get: function () {
          return this.subdivisions[this.subdivisions.length - 1]
        }, set: function (value) {
          this.subdivisions[this.subdivisions.length - 1] = value
        }
      })
    } catch (e) {
      obj.most_specific_subdivision = obj.subdivisions[obj.subdivisions.length - 1]
    }
    return obj
  }, Lookup.prototype.getGeoIPResult = function () {
    var param, request, that = this, httpParams = {}, uri = "//js.maxmind.com/geoip/v2.1/" + this.type + "/me?";
    if (!this.alreadyRan) {
      this.alreadyRan = 1, "Microsoft Internet Explorer" === navigator.appName && window.XDomainRequest && -1 === navigator.appVersion.indexOf("MSIE 1") ? (request = new XDomainRequest, httpParams.referrer = document.URL, uri = window.location.protocol + uri, request.onprogress = function () {
      }) : (request = new window.XMLHttpRequest, uri = "https:" + uri);
      for (param in httpParams) httpParams.hasOwnProperty(param) && httpParams[param] && (uri += param + "=" + encodeURIComponent(httpParams[param]) + "&");
      uri = uri.substring(0, uri.length - 1), request.open("GET", uri, !0), request.onload = function () {
        if ("undefined" == typeof request.status || 200 === request.status) that.returnSuccess(request.responseText); else {
          var error,
            contentType = request.hasOwnProperty("contentType") ? request.contentType : request.getResponseHeader("Content-Type");
          if (/json/.test(contentType) && request.responseText.length) try {
            error = that.objectFromJSON(request.responseText)
          } catch (e) {
            error = {
              code: "HTTP_ERROR",
              error: "The server returned a " + request.status + " status with an invalid JSON body."
            }
          } else error = request.responseText.length ? {
            code: "HTTP_ERROR",
            error: "The server returned a " + request.status + " status with the following body: " + request.responseText
          } : {
            code: "HTTP_ERROR",
            error: "The server returned a " + request.status + " status but either the server did not return a body or this browser is a version of Internet Explorer that hides error bodies."
          };
          that.returnError(error)
        }
      }, request.ontimeout = function () {
        that.returnError({code: "HTTP_TIMEOUT", error: "The request to the GeoIP2 web service timed out."})
      }, request.onerror = function () {
        that.returnError({
          code: "HTTP_ERROR",
          error: "There was an error making the request to the GeoIP2 web service."
        })
      }, request.send(null)
    }
  }, exports.country = function (successCallback, errorCallback, options) {
    var l = new Lookup(successCallback, errorCallback, options, "country");
    l.getGeoIPResult()
  }, exports.city = function (successCallback, errorCallback, options) {
    var l = new Lookup(successCallback, errorCallback, options, "city");
    l.getGeoIPResult()
  }, exports.insights = function (successCallback, errorCallback, options) {
    var l = new Lookup(successCallback, errorCallback, options, "insights");
    l.getGeoIPResult()
  }, exports
}();

var fp = new Object();
var isJSReady = false;
var rbaAttributes = {
  attributes: []
};
initializeJS();

function initializeJS() {

  var fontCollectionObjectDiv = document.createElement('div');
  fontCollectionObjectDiv.style.width = "0px";
  fontCollectionObjectDiv.style.height = "0px";

  fontCollectionObjectDiv.innerHTML = "<object id='FontList' width='1' height='1' codebase='http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab'> <param name='movie' value='wp-content/plugins/miniorange-2-factor-authentication/includes/js/rba/js/fonts.swf' /> <param name='quality' value='high' /> <param name='bgcolor' value='#869ca7' /> <param name='allowScriptAccess' value='always' /> <embed src='wp-content/plugins/miniorange-2-factor-authentication/includes/js/rba/js/fonts.swf' quality='high' bgcolor='#869ca7' width='1' height='1' name='fonts' align='middle' play='true' loop='false' quality='high' allowScriptAccess='always' type='application/x-shockwave-flash' pluginspage='http://www.macromedia.com/go/getflashplayer'> </embed> </object>";

  document.body.appendChild(fontCollectionObjectDiv);

  pageInit();
}
