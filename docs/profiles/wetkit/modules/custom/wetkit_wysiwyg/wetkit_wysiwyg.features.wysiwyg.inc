<?php
/**
 * @file
 * wetkit_wysiwyg.features.wysiwyg.inc
 */

/**
 * Implements hook_wysiwyg_default_profiles().
 */
function wetkit_wysiwyg_wysiwyg_default_profiles() {
  $profiles = array();

  // Exported profile: wetkit_html_text.
  $profiles['wetkit_html_text'] = array(
    'format' => 'wetkit_html_text',
    'editor' => 'markitup',
    'settings' => array(
      'theme' => 'advanced',
      'language' => 'en',
      'buttons' => array(
        'default' => array(
          'bold' => 1,
          'italic' => 1,
          'stroke' => 1,
          'link' => 1,
        ),
      ),
      'toolbar_loc' => 'top',
      'toolbar_align' => 'left',
      'path_loc' => 'bottom',
      'resizing' => 1,
      'verify_html' => 1,
      'preformatted' => 1,
      'convert_fonts_to_spans' => 1,
      'remove_linebreaks' => 0,
      'apply_source_formatting' => 1,
      'paste_auto_cleanup_on_paste' => 1,
      'block_formats' => 'h1,h2,h3,h4,h5,h6',
      'css_setting' => 'theme',
      'css_path' => '',
      'css_classes' => '',
    ),
    'preferences' => array(
      'add_to_summaries' => FALSE,
      'default' => NULL,
      'show_toggle' => NULL,
      'user_choose' => NULL,
      'version' => '1.1.14',
    ),
    'name' => 'formatwetkit_html_text',
  );

  // Exported profile: wetkit_wysiwyg_text.
  $profiles['wetkit_wysiwyg_text'] = array(
    'format' => 'wetkit_wysiwyg_text',
    'editor' => 'ckeditor',
    'settings' => array(
      'buttons' => array(
        'default' => array(
          'Bold' => 1,
          'Italic' => 1,
          'Underline' => 1,
          'Strike' => 1,
          'JustifyLeft' => 1,
          'JustifyCenter' => 1,
          'JustifyRight' => 1,
          'JustifyBlock' => 1,
          'BulletedList' => 1,
          'NumberedList' => 1,
          'Outdent' => 1,
          'Indent' => 1,
          'Undo' => 1,
          'Redo' => 1,
          'Unlink' => 1,
          'Anchor' => 1,
          'Superscript' => 1,
          'Subscript' => 1,
          'Blockquote' => 1,
          'Source' => 1,
          'HorizontalRule' => 1,
          'Cut' => 1,
          'Copy' => 1,
          'Paste' => 1,
          'PasteText' => 1,
          'PasteFromWord' => 1,
          'ShowBlocks' => 1,
          'Format' => 1,
          'FontSize' => 1,
          'Styles' => 1,
          'Table' => 1,
          'SelectAll' => 1,
          'Find' => 1,
          'Replace' => 1,
          'CreateDiv' => 1,
          'Maximize' => 1,
          'SpellChecker' => 1,
          'Scayt' => 1,
        ),
        'linkit' => array(
          'linkit' => 1,
        ),
        'codemirror' => array(
          'codemirror' => 1,
        ),
        'drupal' => array(
          'media' => 1,
        ),
      ),
      'theme' => 'advanced',
      'language' => 'en',
      'toolbarLocation' => 'top',
      'resize_enabled' => 1,
      'default_toolbar_grouping' => 1,
      'simple_source_formatting' => 1,
      'acf_mode' => 0,
      'acf_allowed_content' => '',
      'css_setting' => 'self',
      'css_theme' => 'wetkit_bootstrap',
      'css_path' => '',
      'stylesSet' => 'alert-info=div.alert alert-info
alert-success=div.alert alert-success
alert-warning=div.alert alert-warning
alert-danger=div.alert alert-danger
badge=span.badge',
      'block_formats' => 'h1,h2,h3,h4,h5,h6',
      'forcePasteAsPlainText' => 0,
      'pasteFromWordPromptCleanup' => 0,
      'pasteFromWordRemoveFontStyles' => 0,
    ),
    'preferences' => array(
      'default' => 1,
      'user_choose' => 0,
      'show_toggle' => 0,
      'add_to_summaries' => 0,
      'version' => '4.9.2.95e5d83',
    ),
    'name' => 'formatwetkit_wysiwyg_text',
  );

  return $profiles;
}
