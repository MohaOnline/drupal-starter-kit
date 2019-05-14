module.exports = function(grunt) {
  grunt.initConfig({
    pkg: grunt.file.readJSON('package.json'),
    sass: {
      dist: {
        options:{
          style:'compressed'
        },
        files: {
          'css/glazed.css' : 'sass/glazed.scss',
          'css/glazed.admin.css' : 'sass/glazed.admin.scss',
          'css/glazed.admin.themesettings.css' : 'sass/glazed.admin.themesettings.scss',
        }
      }
    },
    postcss: {
        options: {
            processors: require('autoprefixer')({browsers: ['last 2 versions', 'ie 9']}),
        },
        dist: {
            src: 'css/*.css',
        },
    },
    watch: {
      css: {
        files: ['sass/*.scss', 'sass/**/*.scss'],
        tasks: ['sass', 'postcss']
      }
    }
  });
  grunt.loadNpmTasks('grunt-contrib-sass');
  grunt.loadNpmTasks('grunt-contrib-watch');
  grunt.loadNpmTasks('grunt-postcss');
  grunt.registerTask('default',['watch']);
}
