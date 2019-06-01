; WetKit Bean Makefile

api = 2
core = 7.x

; Modules for WetKit Bean

projects[bean][version] = 1.13
projects[bean][subdir] = contrib
projects[bean][patch][2295973] = https://www.drupal.org/files/issues/2019-01-08/bean-migrate-support-2295973-24.patch
projects[bean][patch][3044062] = https://www.drupal.org/files/issues/2019-03-29/3044062-revert-patch.diff
