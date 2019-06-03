var merge = require('webpack-merge')
var prodEnv = require('./prod.env')

module.exports = merge(prodEnv, {
  NODE_ENV: '"development"',
  E2T_API_TOKEN: JSON.stringify(process.env.E2T_API_TOKEN)
})
