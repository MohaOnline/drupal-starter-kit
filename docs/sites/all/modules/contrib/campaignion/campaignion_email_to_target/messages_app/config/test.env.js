var merge = require('webpack-merge')
var devEnv = require('./dev.env')

module.exports = merge(devEnv, {
  NODE_ENV: '"testing"',
  E2T_API_TOKEN: JSON.stringify(process.env.E2T_API_TOKEN)
})
