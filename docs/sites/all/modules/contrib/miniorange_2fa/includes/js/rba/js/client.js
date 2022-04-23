//
// ClientJS.  An easy to use, simple, and flexible client information library written in JavaScript.
//
//      Version: 0.07
//
//      Jack Spirou
//      5 Nov 2013

// ClientJS.  Return a JavaScript object containing information collected about a client.
//            Return browser/device fingerprint as a 32 bit integer hash ID.

// BUILT UPON:
//      - https://github.com/Valve/fingerprintjs
//      - http://darkwavetech.com/device_fingerprint.html
//      - detectmobilebrowsers.com JavaScript Mobile Detection Script

// Dependencies Include:
//      - ua-parser.js
//      - fontdetect.js
//      - swfobject.js
//      - murmurhash3.js

// BROWSER FINGERPRINT DATA POINTS
//      - userAgent
//      - screenPrint
//          - colordepth
//          - currentResolution
//          - availableResolution
//          - deviceXDPI
//          - deviceYDPI
//      - plugin list
//      - font list
//      - localStorage
//      - sessionStorage
//      - timezone
//      - language
//      - systemLanguage
//      - cookies
//      - canvasPrint

// METHOD Naming CONVENTION
//      is[MethodName]  = return boolean
//      get[MethodName] = return int|string|object

// METHODS
//
//      var client = new ClientJS();
//
//      client.getSoftwareVersion();
//      client.getBrowserData();
//      client.getFingerPrint();
//
//      client.getUserAgent();
//      client.getUserAgentLowerCase();
//
//      client.getBrowser();
//      client.getBrowserVersion();
//      client.getBrowserMajorVersion();
//      client.isIE();
//      client.isChrome();
//      client.isFirefox();
//      client.isSafari();
//      client.isOpera();
//
//      client.getEngine();
//      client.getEngineVersion();
//
//      client.getOS();
//      client.getOSVersion();
//      client.isWindows();
//      client.isMac();
//      client.isLinux();
//      client.isUbuntu();
//      client.isSolaris();
//
//      client.getDevice();
//      client.getDeviceType();
//      client.getDeviceVendor();
//
//      client.getCPU();
//
//      client.isMobile();
//      client.isMobileMajor();
//      client.isMobileAndroid();
//      client.isMobileOpera();
//      client.isMobileWindows();
//      client.isMobileBlackBerry();
//
//      client.isMobileIOS();
//      client.isIphone();
//      client.isIpad();
//      client.isIpod();
//
//      client.getScreenPrint();
//      client.getColorDepth();
//      client.getCurrentResolution();
//      client.getAvailableResolution();
//      client.getDeviceXDPI();
//      client.getDeviceYDPI();
//
//      client.getPlugins();
//      client.isJava();
//      client.getJavaVersion();
//      client.isFlash();
//      client.getFlashVersion();
//      client.isSilverlight();
//      client.getSilverlightVersion();
//
//      client.getMimeTypes();
//      client.isMimeTypes();
//
//      client.isFont();
//      client.getFonts();
//
//      client.isLocalStorage();
//      client.isSessionStorage();
//      client.isCookie();
//
//      client.getTimeZone();
//
//      client.getLanguage();
//      client.getSystemLanguage();
//
//      client.isCanvas();
//      client.getCanvasPrint();

// Anonymous auto JavaScript function execution.
(function (scope) {
  'use strict';

  // Global user agent browser object.
  var browserData;

  // ClientJS constructor which sets the browserData object and returs the client object.
  var ClientJS = function () {
    var parser = new UAParser();
    browserData = parser.getResult();
    return this;
  };

  // ClientJS prototype which contains all methods.
  ClientJS.prototype = {

    //
    // MAIN METHODS
    //

    // Get Software Version.  Return a string containing this software version number.
    getSoftwareVersion: function () {
      var version = "ClientJS 0.05";
      return version;
    },

    // Get Browser Data.  Return an object containing browser user agent.
    getBrowserData: function () {
      return browserData;
    },

    // Get Fingerprint.  Return a 32-bit integer representing the browsers fingerprint.
    getFingerprint: function () {
      var bar             = '|';

      var userAgent       = browserData.ua;
      var screenPrint     = this.getScreenPrint();
      //var screenPrint 		= '';
      var pluginList      = this.getPlugins();
      var fontList        = this.getFonts();
      //var fontList 			= '';
      var localStorage    = this.isLocalStorage();
      var sessionStorage  = this.isSessionStorage();
      var timeZone        = this.getTimeZone();
      var language        = this.getLanguage();
      //var language 			= '';
      var systemLanguage  = this.getSystemLanguage();
      var cookies         = this.isCookie();
      var canvasPrint     = this.getCanvasPrint
      //var canvasPrint		= '';

      var key = userAgent+bar+screenPrint+bar+pluginList+bar+fontList+bar+localStorage+bar+sessionStorage+bar+timeZone+bar+language+bar+systemLanguage+bar+cookies+bar+canvasPrint;

      // alert("key:" + key);
      var seed = 256;

      return murmurhash3_32_gc(key, seed);
    },

    getCustomFingerprint: function () {
      var bar             = '|';

      var browser = this.getBrowserName(browserData.ua);
      var device = this.getDeviceType();
      var OS = this.getOS();
      var pluginList = this.getPlugins();
      var localStorage = this.isLocalStorage();
      var sessionStorage = this.isSessionStorage();
      var cookies = this.isCookie();
      var timezone = this.getTimeZone();

      var key = browser+bar+device+bar+OS+bar+pluginList+bar+localStorage+bar+sessionStorage+bar+cookies+bar+timezone;
      var seed = 256;

      return murmurhash3_32_gc(key, seed);
    },


    getBrowserName: function (userAgent) {
      if ((userAgent.indexOf('OPR')) != -1) {
        return "Opera";
      } else if (userAgent.indexOf("Edg") > -1) {
        return "Edge";
      } else if (userAgent.indexOf("Chrome") != -1) {
        return "Chrome";
      } else if (userAgent.indexOf("Safari") != -1) {
        return "Safari";
      } else if (userAgent.indexOf("Firefox") != -1) {
        return "Firefox";
      } else if ((userAgent.indexOf("MSIE") != -1) || (!!document.documentMode == true) || !!userAgent.match(/Trident.*rv\:11\./))
      { return "IE";
      } else {
        return "OtherBrowser";
      }
    },
    //
    // USER AGENT METHODS
    //

    // Get User Agent.  Return a string containing unparsed user agent.
    getUserAgent: function () {
      return browserData.ua;
    },

    // Get User Agent Lower Case.  Return a lowercase string containing the user agent.
    getUserAgentLowerCase: function () {
      return browserData.ua.toLowerCase();
    },

    //
    // BROWSER METHODS
    //

    // Get Browser.  Return a string containing the browser name.
    getBrowser: function () {
      return browserData.browser.name;
    },

    // Get Browser Version.  Return a string containing the browser version.
    getBrowserVersion: function () {
      return browserData.browser.version;
    },

    // Get Browser Major Version.  Return a string containing the major browser version.
    getBrowserMajorVersion: function () {
      return browserData.browser.major;
    },

    // Is IE.  Check if the browser is IE.
    isIE: function () {
      return (/IE/i.test(browserData.browser.name));
    },

    // Is Chrome.  Check if the browser is Chrome.
    isChrome: function () {
      return (/Chrome/i.test(browserData.browser.name));
    },

    // Is Firefox.  Check if the browser is Firefox.
    isFirefox: function () {
      return (/Firefox/i.test(browserData.browser.name));
    },

    // Is Safari.  Check if the browser is Safari.
    isSafari: function () {
      return (/Safari/i.test(browserData.browser.name));
    },

    // Is Opera.  Check if the browser is Opera.
    isOpera: function () {
      return (/Opera/i.test(browserData.browser.name));
    },

    isEdge: function () {
      return (/Opera/i.test(browserData.browser.name));
    },

    //
    // ENGINE METHODS
    //

    // Get Engine.  Return a string containing the browser engine.
    getEngine: function () {
      return browserData.engine.name;
    },

    // Get Engine Version.  Return a string containing the browser engine version.
    getEngineVersion: function () {
      return browserData.engine.version;
    },

    //
    // OS METHODS
    //

    // Get OS.  Return a string containing the OS.
    getOS: function () {
      return browserData.os.name;
    },

    // Get OS Version.  Return a string containing the OS Version.
    getOSVersion: function () {
      return browserData.os.version;
    },

    // Is Windows.  Check if the OS is Windows.
    isWindows: function () {
      return (/Windows/i.test(browserData.os.name));
    },

    // Is Mac.  Check if the OS is Mac.
    isMac: function () {
      return (/Mac/i.test(browserData.os.name));
    },

    // Is Linux.  Check if the OS is Linux.
    isLinux: function () {
      return (/Linux/i.test(browserData.os.name));
    },

    // Is Ubuntu.  Check if the OS is Ubuntu.
    isUbuntu: function () {
      return (/Ubuntu/i.test(browserData.os.name));
    },

    // Is Solaris.  Check if the OS is Solaris.
    isSolaris: function () {
      return (/Solaris/i.test(browserData.os.name));
    },

    //
    // DEVICE METHODS
    //

    // Get Device.  Return a string containing the device.
    getDevice: function () {
      return browserData.device.model;
    },

    // Get Device Type.  Return a string containing the device type.
    getDeviceType: function () {
      return browserData.device.type;
    },

    // Get Device Vendor.  Return a string containing the device vendor.
    getDeviceVendor: function () {
      return browserData.device.vendor;
    },

    //
    // CPU METHODS
    //

    // Get CPU.  Return a string containing the CPU architecture.
    getCPU: function () {
      return browserData.cpu.architecture;
    },

    //
    // MOBILE METHODS
    //

    // Is Mobile.  Check if the browser is on a mobile device.
    isMobile: function () {
      // detectmobilebrowsers.com JavaScript Mobile Detection Script
      var dataString = browserData.ua||navigator.vendor||window.opera;
      return (/(android|bb\d+|meego).+mobile|avantgo|bada\/|blackberry|blazer|compal|elaine|fennec|hiptop|iemobile|ip(hone|od)|iris|kindle|lge |maemo|midp|mmp|mobile.+firefox|netfront|opera m(ob|in)i|palm( os)?|phone|p(ixi|re)\/|plucker|pocket|psp|series(4|6)0|symbian|treo|up\.(browser|link)|vodafone|wap|windows (ce|phone)|xda|xiino/i.test(dataString) || /1207|6310|6590|3gso|4thp|50[1-6]i|770s|802s|a wa|abac|ac(er|oo|s\-)|ai(ko|rn)|al(av|ca|co)|amoi|an(ex|ny|yw)|aptu|ar(ch|go)|as(te|us)|attw|au(di|\-m|r |s )|avan|be(ck|ll|nq)|bi(lb|rd)|bl(ac|az)|br(e|v)w|bumb|bw\-(n|u)|c55\/|capi|ccwa|cdm\-|cell|chtm|cldc|cmd\-|co(mp|nd)|craw|da(it|ll|ng)|dbte|dc\-s|devi|dica|dmob|do(c|p)o|ds(12|\-d)|el(49|ai)|em(l2|ul)|er(ic|k0)|esl8|ez([4-7]0|os|wa|ze)|fetc|fly(\-|_)|g1 u|g560|gene|gf\-5|g\-mo|go(\.w|od)|gr(ad|un)|haie|hcit|hd\-(m|p|t)|hei\-|hi(pt|ta)|hp( i|ip)|hs\-c|ht(c(\-| |_|a|g|p|s|t)|tp)|hu(aw|tc)|i\-(20|go|ma)|i230|iac( |\-|\/)|ibro|idea|ig01|ikom|im1k|inno|ipaq|iris|ja(t|v)a|jbro|jemu|jigs|kddi|keji|kgt( |\/)|klon|kpt |kwc\-|kyo(c|k)|le(no|xi)|lg( g|\/(k|l|u)|50|54|\-[a-w])|libw|lynx|m1\-w|m3ga|m50\/|ma(te|ui|xo)|mc(01|21|ca)|m\-cr|me(rc|ri)|mi(o8|oa|ts)|mmef|mo(01|02|bi|de|do|t(\-| |o|v)|zz)|mt(50|p1|v )|mwbp|mywa|n10[0-2]|n20[2-3]|n30(0|2)|n50(0|2|5)|n7(0(0|1)|10)|ne((c|m)\-|on|tf|wf|wg|wt)|nok(6|i)|nzph|o2im|op(ti|wv)|oran|owg1|p800|pan(a|d|t)|pdxg|pg(13|\-([1-8]|c))|phil|pire|pl(ay|uc)|pn\-2|po(ck|rt|se)|prox|psio|pt\-g|qa\-a|qc(07|12|21|32|60|\-[2-7]|i\-)|qtek|r380|r600|raks|rim9|ro(ve|zo)|s55\/|sa(ge|ma|mm|ms|ny|va)|sc(01|h\-|oo|p\-)|sdk\/|se(c(\-|0|1)|47|mc|nd|ri)|sgh\-|shar|sie(\-|m)|sk\-0|sl(45|id)|sm(al|ar|b3|it|t5)|so(ft|ny)|sp(01|h\-|v\-|v )|sy(01|mb)|t2(18|50)|t6(00|10|18)|ta(gt|lk)|tcl\-|tdg\-|tel(i|m)|tim\-|t\-mo|to(pl|sh)|ts(70|m\-|m3|m5)|tx\-9|up(\.b|g1|si)|utst|v400|v750|veri|vi(rg|te)|vk(40|5[0-3]|\-v)|vm40|voda|vulc|vx(52|53|60|61|70|80|81|83|85|98)|w3c(\-| )|webc|whit|wi(g |nc|nw)|wmlb|wonu|x700|yas\-|your|zeto|zte\-/i.test(dataString.substr(0, 4)));
    },

    // Is Mobile Major.  Check if the browser is on a major mobile device.
    isMobileMajor: function() {
      return (this.isMobileAndroid() || this.isMobileBlackBerry() || this.isMobileIOS() || this.isMobileOpera() || this.isMobileWindows());
    },

    // Is Mobile.  Check if the browser is on an android mobile device.
    isMobileAndroid: function() {
      if(browserData.ua.match(/Android/i)) {
        return true;
      }
      return false;
    },

    // Is Mobile Opera.  Check if the browser is on an opera mobile device.
    isMobileOpera: function() {
      if(browserData.ua.match(/Opera Mini/i)) {
        return true;
      }
      return false;
    },

    // Is Mobile Windows.  Check if the browser is on a windows mobile device.
    isMobileWindows: function() {
      if(browserData.ua.match(/IEMobile/i)) {
        return true;
      }
      return false;
    },

    // Is Mobile BlackBerry.  Check if the browser is on a blackberry mobile device.
    isMobileBlackBerry: function() {
      if(browserData.ua.match(/BlackBerry/i)) {
        return true;
      }
      return false;
    },

    //
    // MOBILE APPLE METHODS
    //

    // Is Mobile iOS.  Check if the browser is on an Apple iOS device.
    isMobileIOS: function() {
      if(browserData.ua.match(/iPhone|iPad|iPod/i)) {
        return true;
      }
      return false;
    },

    // Is Iphone.  Check if the browser is on an Apple iPhone.
    isIphone: function() {
      if(browserData.ua.match(/iPhone/i)) {
        return true;
      }
      return false;
    },

    // Is Ipad.  Check if the browser is on an Apple iPad.
    isIpad: function() {
      if(browserData.ua.match(/iPad/i)) {
        return true;
      }
      return false;
    },

    // Is Ipod.  Check if the browser is on an Apple iPod.
    isIpod: function() {
      if(browserData.ua.match(/iPod/i)) {
        return true;
      }
      return false;
    },

    //
    // SCREEN METHODS
    //

    // Get Screen Print.  Return a string containing screen information.
    getScreenPrint: function () {
      return "Current Resolution: " + this.getCurrentResolution() + ", Avaiable Resolution: " + this.getAvailableResolution() + ", Color Depth: " + this.getColorDepth() + ", Device XDPI: " + this.getDeviceXDPI() + ", Device YDPI: " + this.getDeviceYDPI();
    },

    // Get Color Depth.  Return a string containing the color depth.
    getColorDepth: function () {
      return screen.colorDepth;
    },

    // Get Current Resolution.  Return a string containing the current resolution.
    getCurrentResolution: function () {
      return screen.width + "x" + screen.height;
    },

    // Get Available Resolution.  Return a string containing the available resolution.
    getAvailableResolution: function () {
      return screen.availWidth + "x" + screen.availHeight;
    },

    // Get Device XPDI.  Return a string containing the device XPDI.
    getDeviceXDPI: function () {
      return screen.deviceXDPI;
    },

    // Get Device YDPI.  Return a string containing the device YDPI.
    getDeviceYDPI: function () {
      return screen.deviceYDPI;
    },

    //
    // PLUGIN METHODS
    //

    // Get Plugins.  Return a string containing a list of installed plugins.
    getPlugins: function () {
      var pluginsList = [];
      for (var i=0; i<navigator.plugins.length; i++) {
        pluginsList.push(navigator.plugins[i].name);
      }
      pluginsList.sort();

      var pluginsAttr = '';
      for (var i=0; i < pluginsList.length; i++) {
        if( i == pluginsList.length-1 ) {
          pluginsAttr += pluginsList[i];
        }else{
          pluginsAttr += pluginsList[i] + ", ";
        }
      }
      return pluginsAttr;
    },

    // Is Java.  Check if Java is installed.
    isJava: function () {
      return navigator.javaEnabled();
    },

    // Get Java Version.  Return a string containing the Java Version.
    getJavaVersion: function () {
      return deployJava.getJREs().toString();
    },

    // Is Java.  Check if Java is installed.
    isFlash: function () {
      objPlayerVersion = swfobject.getFlashPlayerVersion();
      strTemp = objPlayerVersion.major + "." + objPlayerVersion.minor + "." + objPlayerVersion.release;
      if (strTemp === "0.0.0") {
        return false;
      }
      return true;
    },

    // Get Flash Version.  Return a string containing the Flash Version.
    getFlashVersion: function () {
      objPlayerVersion = swfobject.getFlashPlayerVersion();
      return objPlayerVersion.major + "." + objPlayerVersion.minor + "." + objPlayerVersion.release;
    },

    // Is Silverlight.  Check if Silverlight is installed.
    isSilverlight: function () {
      var objPlugin = navigator.plugins["Silverlight Plug-In"];
      if (objPlugin) {
        return true;
      }
      return false;
    },

    // Get Silverlight Version.  Return a string containing the Silverlight Version.
    getSilverlightVersion: function () {
      var objPlugin = navigator.plugins["Silverlight Plug-In"];
      return objPlugin.description;
    },

    //
    // MIME TYPE METHODS
    //

    // Is Mime Types.  Check if a mime type is installed.
    isMimeTypes: function () {
      if(navigator.mimeTypes.length){
        return true;
      }
      return false;
    },

    // Get Mime Types.  Return a string containing a list of installed mime types.
    getMimeTypes: function () {
      var mimeTypeList = "";

      for (var i=0; i<navigator.mimeTypes.length; i++) {
        if( i == navigator.mimeTypes.length-1 ) {
          mimeTypeList += navigator.mimeTypes[i].description;
        }else{
          mimeTypeList += navigator.mimeTypes[i].description + ", ";
        }
      }
      return mimeTypeList;
    },

    //
    // FONT METHODS
    //

    // Is Font.  Check if a font is installed.
    isFont: function (font) {
      var detective = new Detector();
      return detective.detect(font);
    },

    // Get Fonts.  Return a string containing a list of installed fonts.
    getFonts: function () {


      var fontString = $('#systemFonts').text();
      //alert("get fonts:" + fontString);
      return fontString;
    },

    //
    // STORAGE METHODS
    //

    // Is Local Storage.  Check if local storage is enabled.
    isLocalStorage: function () {
      try {
        return !!scope.localStorage;
      } catch(e) {
        return true; // SecurityError when referencing it means it exists
      }
    },

    // Is Session Storage.  Check if session storage is enabled.
    isSessionStorage: function () {
      try {
        return !!scope.sessionStorage;
      } catch(e) {
        return true; // SecurityError when referencing it means it exists
      }
    },

    // Is Cookie.  Check if cookies are enabled.
    isCookie: function () {
      return navigator.cookieEnabled;
    },

    //
    // TIME METHODS
    //

    // Get Time Zone.  Return a string containing the time zone.
    getTimeZone: function () {
      var rightNow = new Date();
      return String(String(rightNow).split("(")[1]).split(")")[0];
    },

    //
    // LANGUAGE METHODS
    //

    // Get Language.  Return a string containing the user language.
    getLanguage: function () {
      return navigator.language;
    },

    // Get System Language.  Return a string containing the system language.
    getSystemLanguage: function () {
      return navigator.systemLanguage;
    },

    //
    // CANVAS METHODS
    //

    // Is Canvas.  Check if the canvas element is enabled.
    isCanvas: function () {
      var elem = document.createElement('canvas');
      return !!(elem.getContext && elem.getContext('2d'));
    },

    // Get Canvas Print.  Return a string containing the canvas URI data.
    getCanvasPrint: function () {
      var canvas = document.createElement('canvas');
      var ctx = canvas.getContext('2d');

      // https://www.browserleaks.com/canvas#how-does-it-work
      var txt = 'http://valve.github.io';
      ctx.textBaseline = "top";
      ctx.font = "14px 'Arial'";
      ctx.textBaseline = "alphabetic";
      ctx.fillStyle = "#f60";
      ctx.fillRect(125,1,62,20);
      ctx.fillStyle = "#069";
      ctx.fillText(txt, 2, 15);
      ctx.fillStyle = "rgba(102, 204, 0, 0.7)";
      ctx.fillText(txt, 4, 17);
      return canvas.toDataURL();
    }

  };

  if (typeof module === 'object' && typeof exports === 'object') {
    module.exports = ClientJS;
  }
  scope.ClientJS = ClientJS;
})(window);
