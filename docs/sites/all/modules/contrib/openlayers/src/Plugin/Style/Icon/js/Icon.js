Drupal.openlayers.pluginManager.register({
  fs: 'openlayers.Style:Icon',
  init: function(data) {
    return function (feature, resolution) {
      var srcContent = data.opt.path;
      var colorContent = data.opt.color || '';
      if (feature) {
        var featureProperties = feature.getProperties();
        for (key in featureProperties) {

          //  Replace tokens in src field
          if (key != 'popup_content' && key != 'tooltip_content') {
            oldSrcContent = srcContent + '.';
            while (oldSrcContent != srcContent) {
              oldSrcContent = srcContent;
              srcContent = srcContent.replace('${' + key + '}', featureProperties[key]);
            }
          }

          //  Replace tokens in color field
          if (key != 'popup_content' && key != 'tooltip_content') {
            oldColorContent = colorContent + '.';
            while (oldColorContent != colorContent) {
              oldColorContent = colorContent;
              colorContent = colorContent.replace('${' + key + '}', featureProperties[key]);
            }
          }
        }
      }  

      //  Strip any remaining html from src field (TODO - why doesn't the Views setting work here?)
      srcContent = srcContent.replaceAll('<div>', '');
      srcContent = srcContent.replaceAll('</div>', '');
      srcContent = srcContent.trim();
      if (srcContent == '') {
        srcContent = 'unknown';
      }

      //  Strip any remaining html from src field (TODO - why doesn't the Views setting work here?)
      colorContent = colorContent.replaceAll('<div>', '');
      colorContent = colorContent.replaceAll('</div>', '');
      colorContent = colorContent.trim();
      if (colorContent == '') {
        colorContent = undefined;
      } else {
        colorContent = 'rgba(' + colorContent + ')';
      }
      
      return new ol.style.Style({
        image: new ol.style.Icon(({
          scale: data.opt.scale,
          anchor: data.opt.anchor,
          anchorXUnits: 'fraction',
          anchorYUnits: 'fraction',
          src: srcContent,
          color: colorContent,
        }))
      });
    };
  }
});
