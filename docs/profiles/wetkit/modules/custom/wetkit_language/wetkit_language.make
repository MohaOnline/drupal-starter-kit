; WetKit Language Makefile

api = 2
core = 7.x

; Contrib

projects[entity_translation][version] = 1.0
projects[entity_translation][subdir] = contrib
projects[entity_translation][patch][2557429] = http://drupal.org/files/issues/static_cache_for-2557429-17.patch
projects[entity_translation][patch][2734295] = http://drupal.org/files/issues/entity_translation-2734295-4.patch

projects[features_translations][version] = 2.0
projects[features_translations][subdir] = contrib

projects[i18n][version] = 1.26
projects[i18n][subdir] = contrib

projects[i18nviews][version] = 3.0-alpha1
projects[i18nviews][subdir] = contrib
projects[i18nviews][patch][1788832] = http://drupal.org/files/issues/transformed-contextual-filter-fix-178832-10.patch

projects[language_switch][version] = 1.0-alpha2
projects[language_switch][subdir] = contrib

projects[l10n_client][version] = 1.3
projects[l10n_client][subdir] = contrib
projects[l10n_client][patch][2191771] = http://drupal.org/files/issues/l10n_client-browser_is-2191771-17.patch

projects[l10n_update][version] = 1.1
projects[l10n_update][subdir] = contrib

projects[potx][version] = 1.0
projects[potx][subdir] = contrib

projects[stringoverrides][version] = 1.8
projects[stringoverrides][subdir] = contrib

projects[title][version] = 1.0-alpha9
projects[title][subdir] = contrib

projects[variable][version] = 2.5
projects[variable][subdir] = contrib

projects[webform_localization][version] = 4.14
projects[webform_localization][subdir] = contrib
