module.exports = function (url) {
  var slices = url.match(/.+\?(.+)=(.+)/)
  var queryString = slices && slices[2] || ''

  var values = []
  if (!queryString) {
    for (let i = 1; i <= 300; i++) {
      values.push({
        value: 'node/' + i,
        label: 'Some Node title (' + i + ')'
      })
    }
  } else {
    for (let i = 1; i < 20 / queryString.length; i++) {
      values.push({
        value: 'node/' + i,
        label: 'Some Node title containing ' + queryString + ' (' + i + ')'
      })
    }
  }

  return {values}
}
