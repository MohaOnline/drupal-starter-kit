<?php
/**
 * @file
 * dvg_wysiwyg.features.ckeditor_profile.inc
 */

/**
 * Implements hook_ckeditor_profile_defaults().
 */
function dvg_wysiwyg_ckeditor_profile_defaults() {
  $data = array(
    'Advanced' => array(
      'name' => 'Advanced',
      'settings' => array(
        'ss' => 2,
        'toolbar' => '[
    [\'Styles\',\'Bold\',\'Language\',\'-\',\'NumberedList\',\'BulletedList\',\'-\',\'HorizontalRule\',\'-\',\'linkit\',\'Media\',\'NodeEmbed\',\'TokenInsert\',\'-\',\'Table\',\'Source\']
]',
        'expand' => 't',
        'default' => 't',
        'show_toggle' => 'f',
        'uicolor' => 'default',
        'uicolor_user' => 'default',
        'width' => '100%',
        'lang' => 'en',
        'auto_lang' => 't',
        'language_direction' => 'default',
        'allowed_content' => 'f',
        'extraAllowedContent' => '',
        'enter_mode' => 'p',
        'shift_enter_mode' => 'br',
        'font_format' => 'p;div;pre;address;h1;h2;h3;h4;h5;h6',
        'custom_formatting' => 'f',
        'formatting' => array(
          'custom_formatting_options' => array(
            'indent' => 'indent',
            'breakBeforeOpen' => 'breakBeforeOpen',
            'breakAfterOpen' => 'breakAfterOpen',
            'breakAfterClose' => 'breakAfterClose',
            'breakBeforeClose' => 0,
            'pre_indent' => 0,
          ),
        ),
        'css_mode' => 'self',
        'css_path' => '%hprofiles/dvg/themes/dvg/css/editor.css',
        'css_style' => 'self',
        'styles_path' => '%hprofiles/dvg/themes/dvg/scripts/ckeditor.styles.js',
        'filebrowser' => 'none',
        'filebrowser_image' => '',
        'filebrowser_flash' => '',
        'UserFilesPath' => '%b%f/',
        'UserFilesAbsolutePath' => '%d%b%f/',
        'forcePasteAsPlainText' => 't',
        'html_entities' => 'f',
        'scayt_autoStartup' => 'f',
        'theme_config_js' => 'f',
        'js_conf' => 'config.disableObjectResizing = true;
config.language_list = [\'de:Duits\', \'en:Engels\', \'fr:Frans\', \'es:Spaans\'];',
        'loadPlugins' => array(
          'NodeEmbed' => array(
            'name' => 'NodeEmbed',
            'desc' => 'Node Embed - embed nodes in content.',
            'path' => '%base_path%profiles/dvg/modules/contrib/node_embed/ckeditor/NodeEmbed/',
          ),
          'drupalbreaks' => array(
            'name' => 'drupalbreaks',
            'desc' => 'Plugin for inserting Drupal teaser and page breaks.',
            'path' => '%plugin_dir%drupalbreaks/',
            'buttons' => array(
              'DrupalBreak' => array(
                'label' => 'DrupalBreak',
                'icon' => 'images/drupalbreak.png',
              ),
            ),
            'default' => 't',
          ),
          'linkit' => array(
            'name' => 'linkit',
            'desc' => 'Support for Linkit module',
            'path' => '%base_path%profiles/dvg/modules/contrib/linkit/editors/ckeditor/',
            'buttons' => array(
              'linkit' => array(
                'label' => 'Linkit',
                'icon' => 'icons/linkit.png',
              ),
            ),
          ),
          'media' => array(
            'name' => 'media',
            'desc' => 'Plugin for embedding files using Media CKEditor',
            'path' => '%base_path%profiles/dvg/modules/contrib/media_ckeditor/js/plugins/media/',
            'buttons' => array(
              'Media' => array(
                'icon' => 'icons/media.png',
                'label' => 'Add media',
              ),
            ),
            'default' => 'f',
          ),
          'token_insert_ckeditor' => array(
            'name' => 'token_insert_ckeditor',
            'desc' => 'Token insert',
            'path' => '%base_path%profiles/dvg/modules/contrib/token_insert/token_insert_ckeditor/plugins/token_insert_ckeditor/',
            'buttons' => array(
              'TokenInsert' => array(
                'icon' => 'images/insert.png',
                'label' => 'Token insert',
              ),
            ),
          ),
        ),
      ),
      'input_formats' => array(
        'filtered_html' => 'Filtered HTML',
      ),
    ),
    'CKEditor Global Profile' => array(
      'name' => 'CKEditor Global Profile',
      'settings' => array(
        'ckeditor_path' => '%l/ckeditor',
      ),
      'input_formats' => array(),
    ),
    'Email' => array(
      'name' => 'Email',
      'settings' => array(
        'ss' => 2,
        'toolbar' => '[
    [\'Source\',\'Bold\',\'Italic\',\'BulletedList\',\'NumberedList\',\'Link\',\'Unlink\',\'HorizontalRule\',\'Table\',\'TokenInsert\']
]',
        'expand' => 't',
        'default' => 't',
        'show_toggle' => 't',
        'uicolor' => 'default',
        'uicolor_user' => 'default',
        'width' => '100%',
        'lang' => 'en',
        'auto_lang' => 't',
        'language_direction' => 'default',
        'allowed_content' => 't',
        'extraAllowedContent' => '',
        'enter_mode' => 'p',
        'shift_enter_mode' => 'br',
        'font_format' => 'p;div;pre;address;h1;h2;h3;h4;h5;h6',
        'custom_formatting' => 'f',
        'formatting' => array(
          'custom_formatting_options' => array(
            'indent' => 'indent',
            'breakBeforeOpen' => 'breakBeforeOpen',
            'breakAfterOpen' => 'breakAfterOpen',
            'breakAfterClose' => 'breakAfterClose',
            'breakBeforeClose' => 0,
            'pre_indent' => 0,
          ),
        ),
        'css_mode' => 'self',
        'css_path' => '%hprofiles/dvg/themes/dvg/css/editor.css',
        'css_style' => 'theme',
        'styles_path' => '',
        'filebrowser' => 'none',
        'filebrowser_image' => '',
        'filebrowser_flash' => '',
        'UserFilesPath' => '%b%f/',
        'UserFilesAbsolutePath' => '%d%b%f/',
        'forcePasteAsPlainText' => 'f',
        'html_entities' => 'f',
        'scayt_autoStartup' => 'f',
        'theme_config_js' => 'f',
        'js_conf' => 'config.disableObjectResizing = true;',
        'loadPlugins' => array(
          'token_insert_ckeditor' => array(
            'name' => 'token_insert_ckeditor',
            'desc' => 'Token insert',
            'path' => '%base_path%profiles/dvg/modules/contrib/token_insert/token_insert_ckeditor/plugins/token_insert_ckeditor/',
            'buttons' => array(
              'TokenInsert' => array(
                'icon' => 'images/insert.png',
                'label' => 'Token insert',
              ),
            ),
          ),
        ),
      ),
      'input_formats' => array(
        'email_html' => 'Email HTML',
      ),
    ),
  );
  return $data;
}
