// This is the webpack config used for unit tests.

var path = require('path')
var utils = require('./utils')
var webpack = require('webpack')
var merge = require('webpack-merge')
var baseConfig = require('./webpack.base.conf')

var webpackConfig = merge(baseConfig, {
  // use inline sourcemap for karma-sourcemap-loader
  module: {
    rules: utils.styleLoaders()
  },
  resolve: {
    alias: {
      'vue$': 'vue/dist/vue.esm.js'
    }
  },
  devtool: '#inline-source-map',
  resolveLoader: {
    alias: {
      // necessary to to make lang="scss" work in test when using vue-loader's ?inject option
      // see discussion at https://github.com/vuejs/vue-loader/issues/724
      'scss-loader': 'sass-loader'
    }
  },
  plugins: [
    new webpack.ProvidePlugin({
      'Drupal': [path.resolve(__dirname, './drupal-fixture'), 'default']
    }),
    new webpack.DefinePlugin({
      'process.env': require('../config/test.env')
    }),
    // element-ui: replace default Chinese strings with English strings.
    new webpack.NormalModuleReplacementPlugin(
      /element-ui[\/\\]lib[\/\\]locale[\/\\]lang[\/\\]zh-CN/,
      'element-ui/lib/locale/lang/en'
    )
  ]
})

// no need for app entry during tests
delete webpackConfig.entry

module.exports = webpackConfig
