Drupal.openlayers.pluginManager.register({
  fs: 'openlayers.Style:InlineJS',
  init: function(data) {
    return new Function('feature', 'resolution', data.opt.javascript)  ;
  }
});
