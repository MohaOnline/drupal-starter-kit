/* eslint-disable require-jsdoc */
var fs = require('fs-extra')

function copy (source, dest) {
  fs.copy(source, dest, {overwrite: true}, function (err) {
    if (err) {
      console.error(err)
    } else {
      console.log('Copied ' + source + ' to ' + dest)
    }
  })
}

function move (source, dest) {
  fs.moveSync(source, dest, {overwrite: true})
}

copy('./dist/campaignion_vue.js', '../js/campaignion_vue.js')
copy('./dist/campaignion_vue.min.js', '../js/campaignion_vue.min.js')
if (fs.existsSync('./dist/css')) move('./dist/css', '../css')
if (fs.existsSync('./dist/images')) move('./dist/images', '../css/images')
if (fs.existsSync('./dist/fonts')) move('./dist/fonts', '../css/fonts')
