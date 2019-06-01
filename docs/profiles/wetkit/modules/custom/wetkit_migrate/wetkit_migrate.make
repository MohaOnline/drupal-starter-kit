; WetKit Migrate Makefile

api = 2
core = 7.x

; Modules needed for Migration

projects[migrate][version] = 2.11
projects[migrate][type] = module
projects[migrate][subdir] = contrib
projects[migrate][patch][2290027] = http://drupal.org/files/issues/migrate_uuid_keep-2290027-02.patch

projects[migrate_extras][version] = 2.5
projects[migrate_extras][type] = module
projects[migrate_extras][subdir] = contrib
projects[migrate_extras][patch][2126725] = http://drupal.org/files/issues/migrate_media_attributes-2126725-01.patch

; Libraries

libraries[spyc][download][type] = get
libraries[spyc][download][url] = https://github.com/mustangostang/spyc/archive/0.5.1.tar.gz

libraries[querypath][download][type] = git
libraries[querypath][download][branch] = 3.x
libraries[querypath][download][revision] = eeb67cc
libraries[querypath][download][url] = https://github.com/technosophos/querypath.git
