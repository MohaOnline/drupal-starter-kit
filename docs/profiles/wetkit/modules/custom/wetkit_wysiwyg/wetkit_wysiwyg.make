; WetKit WYSIWYG Makefile

api = 2
core = 7.x

; Contrib

projects[linkit][version] = 3.5
projects[linkit][subdir] = contrib
projects[linkit][patch][2381549] = http://drupal.org/files/issues/linkit_title_and_uuid-2381549-10.patch

projects[wysiwyg][version] = 2.5
projects[wysiwyg][subdir] = contrib
projects[wysiwyg][patch][1786732] = http://drupal.org/files/wysiwyg-arbitrary_image_paths_markitup-1786732-3.patch

; Include our Editors

libraries[ckeditor][download][type] = get
libraries[ckeditor][download][url] = http://download.cksource.com/CKEditor/CKEditor/CKEditor%204.9.2/ckeditor_4.9.2_standard.zip

libraries[markitup][download][type] = get
libraries[markitup][download][url] = https://github.com/markitup/1.x/archive/1.1.15.tar.gz
libraries[markitup][patch][1715642] = http://drupal.org/files/1715642-adding-html-set-markitup-editor.patch
