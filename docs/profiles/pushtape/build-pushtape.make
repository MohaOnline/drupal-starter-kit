; Include the definition of how to build Drupal core directly, including patches.
includes[] = "drupal-org-core.make"

; Download the Pushtape install profile and recursively build all its dependencies.
projects[pushtape][type] = "profile"
projects[pushtape][download][type] = "git"
projects[pushtape][download][url] = "http://git.drupal.org/project/pushtape.git"
projects[pushtape][download][branch] = "7.x-1.x"