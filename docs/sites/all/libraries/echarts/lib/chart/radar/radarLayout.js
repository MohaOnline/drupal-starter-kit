function _default(ecModel) {
  ecModel.eachSeriesByType('radar', function (seriesModel) {
    var data = seriesModel.getData();
    var points = [];
    var coordSys = seriesModel.coordinateSystem;

    if (!coordSys) {
      return;
    }

    function pointsConverter(val, idx) {
      points[idx] = points[idx] || [];
      points[idx][i] = coordSys.dataToPoint(val, i);
    }

    var axes = coordSys.getIndicatorAxes();

    for (var i = 0; i < coordSys.getIndicatorAxes().length; i++) {
      data.each(data.mapDimension(axes[i].dim), pointsConverter);
    }

    data.each(function (idx) {
      // Close polygon
      points[idx][0] && points[idx].push(points[idx][0].slice());
      data.setItemLayout(idx, points[idx]);
    });
  });
}

module.exports = _default;