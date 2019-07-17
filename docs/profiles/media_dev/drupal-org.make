core = 7.x
api = 2

; -----------------------------------------------------------------------------
; Contrib modules
; -----------------------------------------------------------------------------

; Optional contrib modules
projects[addressfield][type] = module
projects[addressfield][version] = 1.x-dev
projects[addressfield][subdir] = contrib

projects[admin_menu][type] = module
projects[admin_menu][download][type] = git
projects[admin_menu][download][url] = "http://git.drupal.org/project/admin_menu.git"
projects[admin_menu][download][revision] = 25255ccfa480
projects[admin_menu][download][branch] = 7.x-3.x
projects[admin_menu][subdir] = contrib

projects[admin_select][download][type] = git
projects[admin_select][download][url] = "http://git.drupal.org/project/admin_select.git"
projects[admin_select][download][revision] = 7a76159c3
projects[admin_select][download][branch] = 7.x-1.x
projects[admin_select][patch][1936550] = https://www.drupal.org/files/issues/2018-09-16/admin_select-user_action-1936550-4.patch
projects[admin_select][subdir] = contrib

projects[admin_views][type] = module
projects[admin_views][version] = 1.6
projects[admin_views][subdir] = contrib
projects[admin_views][patch][2545650] = https://www.drupal.org/files/issues/2545750-7.patch

projects[advanced_help][type] = module
projects[advanced_help][version] = 1.5
projects[advanced_help][subdir] = contrib

projects[ckeditor][type] = module
projects[ckeditor][download][type] = git
projects[ckeditor][download][url] = "http://git.drupal.org/project/ckeditor.git"
projects[ckeditor][download][revision] = 9ea879baf
projects[ckeditor][download][branch] = 7.x-1.x
projects[ckeditor][subdir] = contrib

projects[ctools][type] = module
projects[ctools][version] = 1.15
projects[ctools][subdir] = contrib

projects[devel][type] = module
projects[devel][download][type] = git
projects[devel][download][url] = "http://git.drupal.org/project/devel.git"
projects[devel][download][revision] = b2d076623
projects[devel][download][branch] = 7.x-1.x
projects[devel][subdir] = contrib

projects[entity][type] = module
projects[entity][version] = 1.9
projects[entity][subdir] = contrib
projects[entity][patch][2020325] = https://www.drupal.org/files/issues/entity-ctools-content-type-from-context-2020325-38.patch

projects[entity_translation][type] = module
projects[entity_translation][version] = 1.0
projects[entity_translation][patch][2908096] = "https://www.drupal.org/files/issues/entity_translation-translation_status_confusing-2908096-5.patch"
projects[entity_translation][patch][2734295] = "https://www.drupal.org/files/issues/entity_translation-2734295-4.patch"
projects[entity_translation][patch][2557429] = "https://www.drupal.org/files/issues/static_cache_for-2557429-17.patch"
projects[entity_translation][patch][2536292] = "https://www.drupal.org/files/issues/2018-08-25/entity_translation-instantiate_menu_arguments-2536292-4.patch"
projects[entity_translation][subdir] = contrib

projects[entity_translation_unified_form][type] = module
projects[entity_translation_unified_form][version] = 1.0
projects[entity_translation_unified_form][subdir] = contrib

projects[entity_embed][type] = module
projects[entity_embed][download][type] = git
projects[entity_embed][download][url] = "http://git.drupal.org/project/entity_embed.git"
projects[entity_embed][download][branch] = 7.x-1.x
projects[entity_embed][subdir] = contrib

projects[entityreference][type] = module
projects[entityreference][version] = 1.5
projects[entityreference][subdir] = contrib

projects[entityreference_prepopulate][type] = module
projects[entityreference_prepopulate][version] = 1.7
projects[entityreference_prepopulate][subdir] = contrib

projects[diff][type] = module
projects[diff][version] = 3.4
projects[diff][subdir] = contrib

projects[features_diff][type] = module
projects[features_diff][version] = 1.0-beta2
projects[features_diff][subdir] = contrib

projects[features][type] = module
projects[features][version] = 2.11
projects[features][subdir] = contrib

projects[fontawesome][type] = module
projects[fontawesome][version] = 3.11
projects[fontawesome][subdir] = contrib

projects[homebox][type] = module
projects[homebox][version] = "2.0"
projects[homebox][subdir] = contrib

projects[icon][type] = module
projects[icon][version] = 1.0
projects[icon][subdir] = contrib
projects[icon][patch][2978408] = https://www.drupal.org/files/issues/2018-06-14/2978408-12.patch

projects[imagick][type] = module
projects[imagick][version] = 1.x-dev
projects[imagick][subdir] = contrib

projects[getid3][type] = module
projects[getid3][version] = 2.x-dev
projects[getid3][subdir] = contrib

projects[geocoder][type] = module
projects[geocoder][version] = 1.4
projects[geocoder][subdir] = contrib
projects[geocoder][patch][2664294] = https://www.drupal.org/files/issues/2018-09-16/geocoder-admin_url_links-2664294-10.patch

projects[geophp][type] = module
projects[geophp][version] = 1.x-dev
projects[geophp][subdir] = contrib

projects[geofield][type] = module
projects[geofield][version] = 2.4
projects[geofield][subdir] = contrib
projects[geofield][patch][2214039] = https://www.drupal.org/files/issues/HTML_separated_in_markup-2214039-1.patch
projects[geofield][patch][1626716] = https://www.drupal.org/files/issues/geofield-sql_error-1626716-19.patch

projects[job_scheduler][type] = module
projects[job_scheduler][version] = 2.0
projects[job_scheduler][subdir] = contrib

projects[entity_view_mode][type] = module
projects[entity_view_mode][version] = 1.0-rc1
projects[entity_view_mode][subdir] = contrib
projects[entity_view_mode][patch][1702530] = https://www.drupal.org/files/issues/entity_view_mode-change_save_to_db-1702530-54_0.patch

projects[exif_custom][type] = module
projects[exif_custom][version] = 1.22
projects[exif_custom][subdir] = contrib

projects[feeds][type] = module
projects[feeds][download][type] = git
projects[feeds][download][url] = "http://git.drupal.org/project/feeds.git"
projects[feeds][download][branch] = 7.x-2.x
projects[feeds][subdir] = contrib

projects[file_entity][type] = module
projects[file_entity][version] = "3.x-dev"
projects[file_entity][patch][2000934] = https://www.drupal.org/files/issues/allow_selection_of-2000934-45.patch
projects[file_entity][subdir] = contrib

projects[flexslider][type] = module
projects[flexslider][version] = "2.0-rc2"
projects[flexslider][subdir] = contrib
projects[flexslider][patch][2941363] = https://www.drupal.org/files/issues/flexslider-2941363-2.patch

projects[i18n][type] = module
projects[i18n][version] = "1.26"
projects[i18n][subdir] = contrib

projects[jquery_update][type] = module
projects[jquery_update][version] = "3.0-alpha5"
projects[jquery_update][subdir] = contrib

projects[libraries][type] = module
projects[libraries][version] = "2.5"
projects[libraries][subdir] = contrib

projects[media][type] = module
projects[media][version] = "4.x-dev"
projects[media][subdir] = contrib

projects[mediaelement][type] = module
projects[mediaelement][download][type] = git
projects[mediaelement][download][url] = "http://git.drupal.org/project/mediaelement.git"
projects[mediaelement][download][branch] = 7.x-1.x
projects[mediaelement][subdir] = contrib

projects[media_archive][type] = module
projects[media_archive][download][type] = git
projects[media_archive][download][url] = "http://git.drupal.org/project/media_archive.git"
projects[media_archive][download][revision] = 4e1ec9f8
projects[media_archive][download][branch] = 7.x-1.x
projects[media_archive][subdir] = contrib

projects[media_bliptv][type] = module
projects[media_bliptv][download][type] = git
projects[media_bliptv][download][url] = "http://git.drupal.org/project/media_bliptv.git"
projects[media_bliptv][download][branch] = 7.x-1.x
projects[media_bliptv][subdir] = contrib

projects[media_ckeditor][type] = module
projects[media_ckeditor][download][type] = git
projects[media_ckeditor][download][url] = "http://git.drupal.org/project/media_ckeditor.git"
projects[media_ckeditor][download][branch] = 7.x-2.x
projects[media_ckeditor][subdir] = contrib

projects[media_gallery][type] = module
projects[media_gallery][download][type] = git
projects[media_gallery][download][url] = "http://git.drupal.org/project/media_gallery.git"
projects[media_gallery][download][branch] = 7.x-2.x
projects[media_gallery][subdir] = contrib

projects[media_feeds][type] = module
projects[media_feeds][download][type] = git
projects[media_feeds][download][url] = "http://git.drupal.org/project/media_feeds.git"
projects[media_feeds][download][branch] = 7.x-2.x
projects[media_feeds][subdir] = contrib

projects[media_flickr][type] = module
projects[media_flickr][download][type] = git
projects[media_flickr][download][url] = "http://git.drupal.org/project/media_flickr.git"
projects[media_flickr][download][branch] = 7.x-2.x
projects[media_flickr][subdir] = contrib

projects[media_vimeo][type] = module
projects[media_vimeo][download][type] = git
projects[media_vimeo][download][url] = "http://git.drupal.org/project/media_vimeo.git"
projects[media_vimeo][download][branch] = 7.x-2.x
projects[media_vimeo][subdir] = contrib

projects[media_youtube][type] = module
projects[media_youtube][version] = "3.8"
projects[media_youtube][subdir] = contrib

projects[media_unique][type] = module
projects[media_unique][version] = "1.x-dev"
projects[media_unique][subdir] = contrib

projects[module_filter][type] = module
projects[module_filter][download][type] = git
projects[module_filter][download][url] = "http://git.drupal.org/project/module_filter.git"
projects[module_filter][download][branch] = 7.x-2.x
projects[module_filter][subdir] = contrib

projects[multiform][type] = module
projects[multiform][version] = "1.6"
projects[multiform][subdir] = contrib

projects[navbar][version] = 1.7
projects[navbar][patch][2377149] = https://www.drupal.org/files/issues/navbar_modernizr-2377149-1.patch
projects[navbar][patch][2644930] = https://www.drupal.org/files/issues/navbar_link_language-2644930-3.patch
projects[navbar][patch][2481207] = https://www.drupal.org/files/issues/dropdown_for_navbar-2481207-22.patch
projects[navbar][subdir] = contrib

projects[oembed][type] = "module"
projects[oembed][download][type] = "git"
projects[oembed][download][url] = "http://git.drupal.org/project/oembed.git"
projects[oembed][download][branch] = "7.x-1.x"
projects[oembed][subdir] = contrib

projects[og][type] = module
projects[og][version] = "2.10"
projects[og][subdir] = contrib
projects[og][patch][1673472] = https://www.drupal.org/files/issues/1673472.5.og_.exception-on-deleted-group-entity.patch 
projects[og][patch][2123109] = https://www.drupal.org/files/issues/2018-07-24/og-breadcrumbs-taxonomy-term-2123109-6.patch
projects[og][patch][2245235] = https://www.drupal.org/files/issues/2018-08-28/og-node_access_create_fix-2245235-4.patch 
projects[og][patch][2299353] = https://www.drupal.org/files/issues/og_rules-get-members-with-a-role-2299353-19.patch  
projects[og][patch][2583303] = https://www.drupal.org/files/issues/2018-08-06/2583303-og_membership-access_check-14.patch
projects[og][patch][2724643] = https://www.drupal.org/files/issues/2724643-6-og_forced_context.patch
projects[og][patch][2998637] = https://www.drupal.org/files/issues/2018-09-16/og-check_for_getBundle_before_usage-2998637-10.patch

projects[media_oembed][type] = "module"
projects[media_oembed][download][type] = "git"
projects[media_oembed][download][url] = "http://git.drupal.org/project/media_oembed.git"
projects[media_oembed][download][branch] = "7.x-2.x"
projects[media_oembed][subdir] = contrib

projects[panels][type] = module
projects[panels][version] = "3.9"
projects[panels][subdir] = contrib
projects[panels][patch][2713697] = https://www.drupal.org/files/issues/performance_optimization-2713697-10.patch

projects[pathauto][type] = module
projects[pathauto][version] = "1.3"
projects[pathauto][subdir] = contrib

projects[pathauto_i18n][type] = module
projects[pathauto_i18n][version] = "1.5"
projects[pathauto_i18n][subdir] = contrib

projects[plupload][type] = module
projects[plupload][download][type] = git
projects[plupload][download][url] = "http://git.drupal.org/project/plupload.git"
projects[plupload][download][branch] = 7.x-1.x
projects[plupload][subdir] = contrib

projects[restws][type] = module
projects[restws][version] = "2.8"
projects[restws][subdir] = contrib

projects[rules][type] = module
projects[rules][version] = "2.x-dev"
projects[rules][subdir] = contrib

projects[title][type] = module
projects[title][version] = "1.x-dev"
projects[title][subdir] = contrib

projects[tinypng][type] = module
projects[tinypng][version] = "1.6"
projects[tinypng][subdir] = contrib

projects[tinypng_on_upload][type] = module
projects[tinypng_on_upload][version] = "1.0"
projects[tinypng_on_upload][subdir] = contrib
projects[tinypng_on_upload][patch][2867184] = https://www.drupal.org/files/issues/2867184-add_jpeg_support.patch

projects[token][type] = module
projects[token][download][type] = git
projects[token][download][url] = "http://git.drupal.org/project/token.git"
projects[token][download][branch] = 7.x-1.x
projects[token][subdir] = contrib
projects[token][patch][975116] = https://www.drupal.org/files/issues/languagetoken-975116-32.patch
projects[token][patch][2023423] = https://www.drupal.org/files/issues/token-2023423-11.patch

projects[smart_ip][type] = module
projects[smart_ip][version] = "2.x-dev"
projects[smart_ip][subdir] = contrib

projects[strongarm][type] = module
projects[strongarm][version] = "2.0"
projects[strongarm][subdir] = contrib
projects[strongarm][patch][1484530] = https://www.drupal.org/files/strongarm-issue-1484530-2_4.patch
projects[strongarm][patch][1612400] = https://www.drupal.org/files/invalid_arg-1612400-2.patch

projects[transliteration][type] = module
projects[transliteration][download][type] = git
projects[transliteration][download][url] = "http://git.drupal.org/project/transliteration.git"
projects[transliteration][download][branch] = 7.x-3.x
projects[transliteration][subdir] = contrib

projects[variable][type] = module
projects[variable][version] = "2.5"
projects[variable][subdir] = contrib

projects[views][type] = module
projects[views][version] = "3.23"
projects[views][subdir] = contrib

projects[views_bulk_operations][type] = module
projects[views_bulk_operations][download][type] = git
projects[views_bulk_operations][download][url] = "http://git.drupal.org/project/views_bulk_operations.git"
projects[views_bulk_operations][download][branch] = 7.x-3.x
projects[views_bulk_operations][subdir] = contrib

projects[views_slideshow][type] = module
projects[views_slideshow][version] = "3.9"
projects[views_slideshow][subdir] = contrib

projects[views_slideshow_swiper][type] = module
projects[views_slideshow_swiper][version] = "4.0"
projects[views_slideshow_swiper][subdir] = contrib

projects[weather][type] = module
projects[weather][version] = "2.14"
projects[weather][subdir] = contrib

projects[wysiwyg][type] = module
projects[wysiwyg][download][type] = git
projects[wysiwyg][download][url] = "http://git.drupal.org/project/wysiwyg.git"
projects[wysiwyg][download][branch] = 7.x-2.x
projects[wysiwyg][patch][961522] = https://www.drupal.org/files/issues/961522-wysiwyg-disable-on-mobile-26.patch
projects[wysiwyg][subdir] = contrib

projects[mobile_detect][type] = module
projects[mobile_detect][download][type] = git
projects[mobile_detect][download][url] = "http://git.drupal.org/project/mobile_detect.git"
projects[mobile_detect][download][revision] = 936682d2
projects[mobile_detect][download][branch] = 7.x-1.x
projects[mobile_detect][subdir] = contrib

; -----------------------------------------------------------------------------
; Theme
; -----------------------------------------------------------------------------
projects[adminimal_theme][type] = theme
projects[adminimal_theme][version] = 1.26

projects[bootstrap][type] = theme
projects[bootstrap][version] = 3.25

projects[startupgrowth_lite][type] = theme
projects[startupgrowth_lite][version] = 1.0

; -----------------------------------------------------------------------------
; Libraries
; -----------------------------------------------------------------------------

; CKEditor
libraries[ckeditor][download][type] = "get"
libraries[ckeditor][download][url] = "http://download.cksource.com/CKEditor/CKEditor/CKEditor%204.6.2/ckeditor_4.6.2_full.zip"
libraries[ckeditor][directory_name] = "ckeditor"
libraries[ckeditor][destination] = "libraries"

;image2 plugin (ckeditor):
libraries[widget][type] = library
libraries[widget][download][type] = "get"
libraries[widget][download][url] = "http://download.ckeditor.com/image2/releases/image2_4.6.2.zip"
libraries[widget][directory_name] = "ckeditor/plugins/image2"
libraries[widget][destination] = "libraries"

;lineutils plugin (ckeditor):
libraries[lineutils][type] = library
libraries[lineutils][download][type] = "get"
libraries[lineutils][download][url] = "http://download.ckeditor.com/lineutils/releases/lineutils_4.6.2.zip"
libraries[lineutils][directory_name] = "ckeditor/plugins/lineutils"
libraries[lineutils][destination] = "libraries"

;mobile detect library (mobile_detect)
libraries[mobile_detect][type] = library
libraries[mobile_detect][download][type] = "get"
libraries[mobile_detect][download][url] = "https://github.com/serbanghita/Mobile-Detect/archive/master.zip"
libraries[mobile_detect][directory_name] = "Mobile_Detect"
libraries[mobile_detect][destination] = "libraries"

;widget plugin (ckeditor):
libraries[widget][type] = library
libraries[widget][download][type] = "get"
libraries[widget][download][url] = "http://download.ckeditor.com/widget/releases/widget_4.6.2.zip"
libraries[widget][directory_name] = "ckeditor/plugins/widget"
libraries[widget][destination] = "libraries"

;widgetselection plugin (ckeditor):
libraries[widgetselection][type] = library
libraries[widgetselection][download][type] = "get"
libraries[widgetselection][download][url] = "http://download.ckeditor.com/widgetselection/releases/widgetselection_4.6.2.zip"
libraries[widgetselection][directory_name] = "ckeditor/plugins/widgetselection"
libraries[widgetselection][destination] = "libraries"

;moono skin (ckeditor):
libraries[moono][type] = library
libraries[moono][download][type] = "get"
libraries[moono][download][url] = "http://download.ckeditor.com/moono/releases/moono_4.6.2.zip"
libraries[moono][directory_name] = "ckeditor/skins/moono"
libraries[moono][destination] = "libraries"

; Plupload library
libraries[plupload][download][type] = "get"
libraries[plupload][download][url] = "https://github.com/moxiecode/plupload/archive/v1.5.8.zip"
libraries[plupload][directory_name] = "plupload"
libraries[plupload][destination] = "libraries"
libraries[plupload][patch][1903850] = https://www.drupal.org/files/issues/plupload-1_5_8-rm_examples-text-1903850-26.patch

; MediaElement library
libraries[mediaelement][directory_name] = "mediaelement"
libraries[mediaelement][type] = library
libraries[mediaelement][download][type] = "get"
libraries[mediaelement][download][url] = "https://github.com/johndyer/mediaelement/archive/master.zip"
libraries[mediaelement][destination] = "libraries"

; underscore (navbar)
libraries[underscore][directory_name] = underscore
libraries[underscore][type] = library
libraries[underscore][destination] = libraries
libraries[underscore][download][type] = get
libraries[underscore][download][url] = https://github.com/jashkenas/underscore/archive/1.5.2.zip

; modernizr (navbar)
libraries[modernizr][directory_name] = modernizr
libraries[modernizr][type] = library
libraries[modernizr][destination] = libraries
libraries[modernizr][download][type] = get
libraries[modernizr][download][url] = https://github.com/Modernizr/Modernizr/archive/v2.7.1.zip

; backbone (navbar)
libraries[backbone][directory_name] = backbone
libraries[backbone][type] = library
libraries[backbone][destination] = libraries
libraries[backbone][download][type] = get
libraries[backbone][download][url] = https://github.com/jashkenas/backbone/archive/1.1.0.zip
libraries[backbone][patch][2315315] = http://drupal.org/files/issues/backbone_source_map_distro-2315315-05.patch

; flexslider library (for the flexslider module)
libraries[flexslider][directory_name] = flexslider
libraries[flexslider][type] = library
libraries[flexslider][destination] = libraries
libraries[flexslider][download][type] = get
libraries[flexslider][download][url] = https://github.com/woothemes/FlexSlider/archive/2.7.1.zip

; Swiper library (for the views_slideshow_swiper module)
; I created a request to whitelist the swiper library: https://www.drupal.org/project/drupalorg_whitelist/issues/3006903
libraries[swiper][directory_name] = Swiper
libraries[swiper][type] = library
libraries[swiper][destination] = libraries
libraries[swiper][download][type] = get
libraries[swiper][download][url] = https://github.com/nolimits4web/swiper/archive/v4.4.2.tar.gz

; getid3 library for reading information from mp3 and media formats. This is not yet whitelisted, have to manually install it.
;libraries[getid3][directory_name] = getid3
;libraries[getid3][type] = library
;libraries[getid3][destination] = libraries
;libraries[getid3][download][type] = get
;libraries[getid3][download][url] = http://downloads.sourceforge.net/project/getid3/getID3%28%29%201.x/1.7.9/getid3-1.7.9.zip

