var fs = require('fs');
var requireDir = require('require-dir');
var locale = requireDir('node_modules/element-ui/lib/locale/lang');

var outputPath = '../locale/';

for (let lang in locale) {
  if (locale.hasOwnProperty(lang)) {
    const filename = outputPath + lang + '.json';

    fs.writeFile(filename, JSON.stringify(locale[lang].default), (err) => {
      if (err) {
        console.log(err);
      }
      console.log('Extracted ' + filename);
    });
  }
}
