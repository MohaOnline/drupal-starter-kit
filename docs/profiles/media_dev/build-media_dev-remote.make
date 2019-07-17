; Download core
core = 7.x

; Specify Drush make's API version
api = 2

includes[] = "https://cgit.drupalcode.org/media_dev/plain/build-media_dev-remote.make?h=7.x-4.x"

; Download media_dev profile.

projects[media_dev][type] = profile
projects[media_dev][download][type] = git
projects[media_dev][download][url] = "http://git.drupal.org/project/media_dev.git"
projects[media_dev][download][branch] = 7.x-4.x
