; WetKit Search API Makefile

api = 2
core = 7.x

; Search API

projects[search_api][version] = 1.15
projects[search_api][subdir] = contrib
projects[search_api][patch][2479453] = http://drupal.org/files/issues/2479453-6--drush_enable_servers.patch
projects[search_api][patch][2520684] = http://drupal.org/files/issues/2520684-1--fix_bundle_setting.patch

projects[search_api_solr][version] = 1.8
projects[search_api_solr][subdir] = contrib
projects[search_api_solr][patch][2466897] = https://www.drupal.org/files/issues/2466897-3--add_solr_5_option.patch
projects[search_api_solr][patch][2532812] = https://www.drupal.org/files/issues/2532812-1--remove_qalt_optimization.patch

projects[search_api_db][version] = 1.4
projects[search_api_db][subdir] = contrib
projects[search_api_db][patch][2343371] = http://drupal.org/files/issues/2343371-18--fix_problems_with_features.patch
projects[search_api_db][patch][2346459] = http://drupal.org/files/issues/search_api_db-tablealias-2346459-1-D7.patch
projects[search_api_db][patch][2428693] = http://drupal.org/files/issues/2428693-3--invoke_no_hooks_in_update_7104.patch

; Solr PHP Client Library

libraries[SolrPhpClient][download][type] = get
libraries[SolrPhpClient][download][url] = http://solr-php-client.googlecode.com/files/SolrPhpClient.r60.2011-05-04.zip
