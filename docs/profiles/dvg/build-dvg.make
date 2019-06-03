api = 2
core = 7.x
; Include the definition for how to build Drupal core directly, including patches:
includes[] = drupal-org-core.make

; Download the install profile and recursively build all its dependencies:
projects[dvg][type] = profile
projects[dvg][download][type] = "git"
projects[dvg][download][branch] = 7.x-1.x
