var path = require('path')
var utils = require('./utils')
var webpack = require('webpack')
var config = require('../config')
var merge = require('webpack-merge')
var baseWebpackConfig = require('./webpack.base.conf')
var HtmlWebpackPlugin = require('html-webpack-plugin')
var FriendlyErrorsPlugin = require('friendly-errors-webpack-plugin')

// add hot-reload related code to entry chunks
Object.keys(baseWebpackConfig.entry).forEach(function (name) {
  baseWebpackConfig.entry[name] = ['./build/dev-client'].concat(baseWebpackConfig.entry[name])
})

module.exports = merge(baseWebpackConfig, {
  module: {
    rules: utils.styleLoaders({ sourceMap: config.dev.cssSourceMap })
  },
  resolve: {
    alias: {
      'vue$': 'vue/dist/vue.esm.js'
    }
  },
  // cheap-module-eval-source-map is faster for development
  devtool: '#cheap-module-eval-source-map',
  plugins: [
    new webpack.ProvidePlugin({
      'Drupal': [path.resolve(__dirname, './drupal-fixture'), 'default']
    }),
    new webpack.DefinePlugin({
      'process.env': Object.assign({}, config.dev.env, {
        E2T_API_URL: process.env.E2T_API_TOKEN ? '"https://e2t-api.more-onion.com/v2"' : '"/api"',
        E2T_API_TOKEN: process.env.E2T_API_TOKEN ? '"' + process.env.E2T_API_TOKEN + '"' : '"xxx"'
      })
    }),
    // element-ui: replace default Chinese strings with English strings.
    new webpack.NormalModuleReplacementPlugin(
      /element-ui[\/\\]lib[\/\\]locale[\/\\]lang[\/\\]zh-CN/,
      'element-ui/lib/locale/lang/en'
    ),
    // https://github.com/glenjamin/webpack-hot-middleware#installation--usage
    new webpack.HotModuleReplacementPlugin(),
    new webpack.NoEmitOnErrorsPlugin(),
    // https://github.com/ampedandwired/html-webpack-plugin
    new HtmlWebpackPlugin({
      filename: 'index.html',
      template: 'index.html',
      inject: true
    }),
    new HtmlWebpackPlugin({
      filename: 'dataset-manager.html',
      template: 'dataset-manager.html',
      inject: true
    }),
    new FriendlyErrorsPlugin()
  ]
})
