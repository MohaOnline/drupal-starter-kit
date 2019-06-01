; WetKit Search Makefile

api = 2
core = 7.x

; Apache Solr

projects[apachesolr][version] = 1.8
projects[apachesolr][subdir] = contrib

projects[apachesolr_autocomplete][version] = 1.5
projects[apachesolr_autocomplete][subdir] = contrib

projects[apachesolr_multilingual][version] = 1.3
projects[apachesolr_multilingual][subdir] = contrib
projects[apachesolr_multilingual][patch][1969268] = http://drupal.org/files/issues/1969268-17.patch

projects[apachesolr_sort][version] = 1.0
projects[apachesolr_sort][subdir] = contrib
projects[apachesolr_sort][patch][1765678] = http://drupal.org/files/apachesolr_sort-1765678-6.patch

projects[apachesolr_views][version] = 1.1-beta1
projects[apachesolr_views][subdir] = contrib
projects[apachesolr_views][patch][1823230] = http://drupal.org/files/check_for_empty_value_in_add_where_function-1823230-1.patch
projects[apachesolr_views][patch][2382091] = http://drupal.org/files/issues/handle_aborted_query-2382091-3.patch
projects[apachesolr_views][patch][2423231] = http://drupal.org/files/issues/apachesolr_views-exposed-filters-2423231-5.patch
projects[apachesolr_views][patch][2497391] = http://drupal.org/files/issues/apachesolr_views-date-handler-array-fix-2.patch
