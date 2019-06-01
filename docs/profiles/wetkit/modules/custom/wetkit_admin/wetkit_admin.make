; WetKit Admin Makefile

api = 2
core = 7.x

; Contrib

projects[admin_menu][version] = 3.0-rc6
projects[admin_menu][subdir] = contrib

projects[admin_views][version] = 1.6
projects[admin_views][subdir] = contrib

projects[admin_select][version] = 1.5
projects[admin_select][subdir] = contrib

projects[backports][version] = 1.0-alpha1
projects[backports][subdir] = contrib

projects[date_popup_authored][version] = 1.2
projects[date_popup_authored][subdir] = contrib

projects[jquery_update][version] = 2.7
projects[jquery_update][subdir] = contrib
projects[jquery_update][patch][1969244] = http://drupal.org/files/issues/jquery_update-theme-specific-version-1969244-37.patch

projects[module_filter][version] = 2.2
projects[module_filter][subdir] = contrib
projects[module_filter][patch][2445133] = http://drupal.org/files/issues/clear-link-placement-problem-on-modules-admin-page-2445133-1_0.patch

; Contrib for WetKit NavBar

projects[navbar][version] = 1.7
projects[navbar][subdir] = contrib
projects[navbar][patch][2377149] = http://drupal.org/files/issues/navbar_modernizr-2377149-1.patch
projects[navbar][patch][2644930] = http://drupal.org/files/issues/navbar_link_language-2644930-3.patch

projects[responsive_preview][version] = 1.1
projects[responsive_preview][subdir] = contrib
projects[responsive_preview][patch][2175453] = http://drupal.org/files/issues/responsive.patch
projects[responsive_preview][patch][2256131] = http://drupal.org/files/issues/responsive_preview-subdir_site_suport-2256131-1.patch
projects[responsive_preview][patch][2263209] = http://drupal.org/files/issues/responsive_preview-item_list-new.patch
projects[responsive_preview][patch][2434913] = http://drupal.org/files/issues/unable_to_scroll_down-2434913-1.patch
