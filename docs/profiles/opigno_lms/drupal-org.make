api = 2
core = 7.67

; Opigno modules ===============================================================

projects[opigno][type]               = module
projects[opigno][subdir]             = "opigno"
projects[opigno][version]            = 1.21

projects[opigno_calendar_app][type]    = module
projects[opigno_calendar_app][subdir]  = "opigno"
projects[opigno_calendar_app][version] = 1.0

projects[opigno_certificate_app][type]               = module
projects[opigno_certificate_app][subdir]             = "opigno"
projects[opigno_certificate_app][version]            = 1.2

projects[opigno_forum_app][type]               = module
projects[opigno_forum_app][subdir]             = "opigno"
projects[opigno_forum_app][version]            = 1.1

projects[opigno_messaging_app][type]               = module
projects[opigno_messaging_app][subdir]             = "opigno"
projects[opigno_messaging_app][version]            = 1.1

projects[opigno_notifications_app][type]               = module
projects[opigno_notifications_app][subdir]             = "opigno"
projects[opigno_notifications_app][version]            = 1.0

projects[opigno_poll_app][type]    = module
projects[opigno_poll_app][subdir]  = "opigno"
projects[opigno_poll_app][version] = 1.0

projects[opigno_quiz_import_app][type]    = module
projects[opigno_quiz_import_app][subdir]  = "opigno"
projects[opigno_quiz_import_app][version] = 1.3

projects[opigno_class_app][type]               = module
projects[opigno_class_app][subdir]             = "opigno"
projects[opigno_class_app][version]            = 1.6

projects[opigno_quiz_app][type]               = module
projects[opigno_quiz_app][subdir]             = "opigno"
projects[opigno_quiz_app][version]            = 1.18

projects[opigno_wt_app][type]               = module
projects[opigno_wt_app][subdir]             = "opigno"
projects[opigno_wt_app][version]            = 1.1

projects[opigno_course_categories_app][type]               = module
projects[opigno_course_categories_app][subdir]             = "opigno"
projects[opigno_course_categories_app][version]            = 1.2

projects[tft][type]        = module
projects[tft][subdir]      = "opigno"
projects[tft][version]     = 1.1

projects[opigno_statistics_app][type]               = module
projects[opigno_statistics_app][subdir]             = "opigno"
projects[opigno_statistics_app][version]            = 1.5

projects[opigno_moxtra_app][type]               = module
projects[opigno_moxtra_app][subdir]             = "opigno"
projects[opigno_moxtra_app][version]            = 1.5

projects[opigno_tincan_api][type]     = module
projects[opigno_tincan_api][subdir]   = "opigno"
projects[opigno_tincan_api][version]  = 1.3

projects[opigno_tincan_question_type][type]     = module
projects[opigno_tincan_question_type][subdir]   = "opigno"
projects[opigno_tincan_question_type][version]  = 1.2

projects[og_quiz][type]               = module
projects[og_quiz][subdir]             = "contrib"
projects[og_quiz][version]            = 1.5

; Opigno themes ================================================================

projects[platon][type]               = theme
projects[platon][version]            = 3.20

; Third-party modules that need to be patched ==================================

; Quiz
projects[quiz][version]        = 4.0-rc11
projects[quiz][subdir]         = "contrib"
projects[quiz][patch][937430]  = "http://drupal.org/files/add_plural_quiz_name-937430-8.patch"
projects[quiz][patch][2101063] = "http://drupal.org/files/issues/quiz_modify-quiz-to-lesson-in-ui-strings-2101063_4.patch"
projects[quiz][patch][2185205] = "http://drupal.org/files/issues/quiz-questiontostep-2185205-1.patch"
projects[quiz][patch][2212789] = "http://drupal.org/files/issues/quiz-fixed_settings_merge-2212789-13.patch"
projects[quiz][patch][2360523] = "http://drupal.org/files/issues/quiz-long_answer_max_score0-2360523-4.patch"
projects[quiz][patch][2384955] = "http://drupal.org/files/issues/quiz_feedback-after-question_2384955.patch"
projects[quiz][patch][2394759] = "http://drupal.org/files/issues/quiz_laq-0score_2394759.patch"
projects[quiz][patch][2394843] = "http://drupal.org/files/issues/quiz_poll-conflict_2394843.patch"
projects[quiz][patch][2401779] = "http://drupal.org/files/issues/quiz-browser_per_quiz_type-2401779-2.patch"
projects[quiz][patch][2582987] = "http://drupal.org/files/issues/quiz_ddlines_backgroundimage-2582987-1.patch"
projects[quiz][patch][2895276] = "http://drupal.org/files/issues/quiz_ajax_questions_browser-2895276-2.patch"
projects[quiz][patch][3003123] = "http://drupal.org/files/issues/2018-10-04/quiz-php7_timezone_warnings-3003123-4-D74.patch"
projects[quiz][patch][2915509] = "http://drupal.org/files/issues/2018-10-04/php-matching-php71-2915509-12-d74.patch"

; OG
projects[og][version]        = 2.9
projects[og][subdir]         = "contrib"
projects[og][patch][2330777] = "http://drupal.org/files/issues/og_2330777.patch"
projects[og][patch][2052067] = "http://drupal.org/files/issues/2018-03-16/og-delete-action-no-behavior_2052067_2.patch"

; OG Create Permissions
projects[og_create_perms][version]        = 1.0
projects[og_create_perms][subdir]         = "contrib"
projects[og_create_perms][patch][2077031] = "http://drupal.org/files/update_to_og2.x_api-2077031-2.patch"

; OG forum
projects[og_forum_D7][version]        = 2.0-alpha1
projects[og_forum_D7][subdir]         = "contrib"
projects[og_forum_D7][type]           = module
projects[og_forum_D7][patch][1802208] = "http://drupal.org/files/og_forum_D7-change-group_audience_to_gid-1802208.patch"
projects[og_forum_D7][patch][1844104] = "http://drupal.org/files/fix-forum-access-1844104-2.patch"
projects[og_forum_D7][patch][2206711] = "http://drupal.org/files/issues/og_forum_2206711.patch"

; Rules
projects[rules][subdir]         = "contrib"
projects[rules][version]        = 2.11
projects[rules][patch][1966426] = "http://drupal.org/files/system.rules_.inc_.patch"

; Apps
projects[apps][subdir]         = "contrib"
projects[apps][version]        = 1.1
projects[apps][patch][2357093] = "http://drupal.org/files/issues/apps_module-2357093_3.patch"

; Quiz cloze
projects[cloze][type]               = module
projects[cloze][subdir]             = "contrib"
projects[cloze][download][type]     = git
projects[cloze][download][branch]   = "7.x-1.x"
projects[cloze][download][url]      = "http://git.drupal.org/project/cloze.git"
projects[cloze][download][revision] = e3bb806823e46870e8e0d6dafce2d0b261c024c5
projects[cloze][patch][2249881]     = "http://drupal.org/files/issues/cloze_change_question_type_name-2249881-4.patch"

; Quiz drag drop
projects[quiz_drag_drop][subdir]  = "contrib"
projects[quiz_drag_drop][version] = 1.4
projects[quiz_drag_drop][patch][2249971] = "http://drupal.org/files/issues/drag_and_drop-forgivingbox-2249971-1.patch"
projects[quiz_drag_drop][patch][2364215] = "http://drupal.org/files/issues/quiz_drag_drop_2364215.patch"

; User Import
projects[user_import][subdir]         = "contrib"
projects[user_import][version]        = 3.2
projects[user_import][patch][2220193] = "http://drupal.org/files/issues/creationdate_2220193_1.patch"

; r403 2 Login
projects[r4032login][subdir]         = "contrib"
projects[r4032login][version]        = 1.8
projects[r4032login][patch][2362997] = "http://drupal.org/files/issues/r4032login-exclude_homepage-2362997.patch"

; Third-party modules ==========================================================

projects[login_history][subdir]  = "contrib"
projects[login_history][version] = 1.1

projects[quizfileupload][subdir]   = "contrib"
projects[quizfileupload][version]  = 4.1

projects[l10n_update][subdir]  = "contrib"
projects[l10n_update][version] = 2.2

projects[date][subdir]  = "contrib"
projects[date][version] = 2.10

projects[i18n][subdir]  = "contrib"
projects[i18n][version] = 1.22

projects[certificate][subdir]  = "contrib"
projects[certificate][version] = 2.3

projects[calendar][subdir]  = "contrib"
projects[calendar][version] = 3.5

projects[h5p][subdir] = "contrib"
projects[h5p][version] = 1.39
projects[h5p][patch][3024279] = "http://drupal.org/files/issues/2019-02-26/opigno_h5p_title_metatag-3024279-4.patch"

projects[admin_menu][subdir] = "contrib"
projects[admin_menu][version] = 3.0-rc5

projects[ctools][subdir] = "contrib"
projects[ctools][version] = 1.14

projects[views][subdir] = "contrib"
projects[views][version] = 3.21

projects[defaultconfig][subdir] = "contrib"
projects[defaultconfig][version] = 1.0-alpha11
projects[defaultconfig][patch][1900574] = "http://drupal.org/files/issues/1900574.defaultconfig.undefinedindex_20.patch"

projects[module_filter][subdir]  = "contrib"
projects[module_filter][version] = 2.2

projects[entity][subdir]  = "contrib"
projects[entity][version] = 1.9

projects[entityreference][subdir]  = "contrib"
projects[entityreference][version] = 1.5

projects[entityreference_prepopulate][subdir]  = "contrib"
projects[entityreference_prepopulate][version] = 1.7

projects[token][subdir]  = "contrib"
projects[token][version] = 1.7

projects[multiselect][subdir]  = "contrib"
projects[multiselect][version] = 1.13

projects[crumbs][subdir]  = "contrib"
projects[crumbs][version] = 2.6

projects[variable][subdir]  = "contrib"
projects[variable][version] = 2.5

projects[rules_conditional][subdir]  = "contrib"
projects[rules_conditional][version] = 1.0-beta2
projects[rules_conditional][patch][3024279] = "http://drupal.org/files/issues/2018-04-08/php7.2_incompatibility-2959426-4.patch"

projects[features][subdir]  = "contrib"
projects[features][version] = 2.11

projects[og_massadd][subdir]  = "contrib"
projects[og_massadd][version] = 1.0-beta2

projects[wysiwyg][subdir]  = "contrib"
projects[wysiwyg][version] = 2.5

projects[wysiwyg_filter][subdir]  = "contrib"
projects[wysiwyg_filter][version] = 1.6-rc9

projects[imce][subdir]  = "contrib"
projects[imce][version] = 1.11

projects[imce_wysiwyg][subdir]  = "contrib"
projects[imce_wysiwyg][version] = 1.0

projects[field_group][subdir]  = "contrib"
projects[field_group][version] = 1.6

projects[menu_attributes][subdir]  = "contrib"
projects[menu_attributes][version] = 1.0

projects[print][subdir]  = "contrib"
projects[print][version] = 2.1

projects[advanced_forum][subdir]  = "contrib"
projects[advanced_forum][version] = 2.7-rc0

projects[date_popup_authored][subdir]  = "contrib"
projects[date_popup_authored][version] = 1.2

projects[privatemsg][subdir]  = "contrib"
projects[privatemsg][version] = 1.4

projects[phpexcel][subdir]  = "contrib"
projects[phpexcel][version] = 3.11

projects[login_redirect][subdir]  = "contrib"
projects[login_redirect][version] = 1.2

projects[views_bulk_operations][subdir]  = "contrib"
projects[views_bulk_operations][version] = 3.4

projects[libraries][subdir]  = "contrib"
projects[libraries][version] = 2.3

projects[pathauto][subdir]  = "contrib"
projects[pathauto][version] = 1.3

projects[strongarm][subdir]  = "contrib"
projects[strongarm][version] = 2.0

projects[jquery_countdown][subdir]  = "contrib"
projects[jquery_countdown][version] = 1.1

projects[content_access][subdir]  = "contrib"
projects[content_access][version] = 1.2-beta2

projects[better_exposed_filters][subdir] = "contrib"
projects[better_exposed_filters][version] = 3.5

projects[pdf][subdir]  = "contrib"
projects[pdf][version] = 1.9

projects[wysiwyg_template][subdir]  = "contrib"
projects[wysiwyg_template][version] = 2.12

projects[homebox][subdir]  = "contrib"
projects[homebox][version] = 2.0

; Third-patry libraries ========================================================

libraries[CKEditor][download][type] = get
libraries[CKEditor][download][url]  = "http://download.cksource.com/CKEditor/CKEditor/CKEditor%204.9.2/ckeditor_4.9.2_full.zip"
libraries[CKEditor][destination]    = "libraries"
libraries[CKEditor][directory_name] = "ckeditor"

libraries[DOMPDF][download][type] = get
libraries[DOMPDF][download][url]  = "https://github.com/dompdf/dompdf/releases/download/v0.6.2/dompdf-0.6.2.zip"
libraries[DOMPDF][destination]    = "libraries"
libraries[DOMPDF][directory_name] = "dompdf"

libraries[PHPExcel][download][type] = "get"
libraries[PHPExcel][download][url]  = "https://github.com/PHPOffice/PHPExcel/archive/1.8.1.tar.gz"
libraries[PHPExcel][destination]    = "libraries"
libraries[PHPExcel][directory_name] = "PHPExcel"
