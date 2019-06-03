var utils = require('./utils')
var config = require('../config')
var isProduction = process.env.NODE_ENV === 'production'

module.exports = {
  loaders: utils.cssLoaders({
    sourceMap: isProduction
      ? config.build.productionSourceMap
      : config.dev.cssSourceMap,
    extract: isProduction
  }),
  postcss: {
    plugins: [
      require('postcss-discard-overridden'),
      require('postcss-discard-duplicates'),
      require('postcss-normalize-whitespace'),
      require('postcss-prettify')
    ]
  }
}
