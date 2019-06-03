(function ($) {
  Drupal.behaviors.advancedTextFormatterTokenTree = {
    attach: function (context, settings) {
      $('table.token-tree', context).once('token-tree', function () {
        $(this).treeTable();
      });
    }
  };

  Drupal.behaviors.advancedTextFormatterTokenDialog = {
    attach: function (context, settings) {
      $('a.token-dialog', context).once('token-dialog').click(function() {
        var url = $(this).attr('href');
        var dialog = $('<div style="display: none" class="loading">' + Drupal.t('Loading token browser...') + '</div>').appendTo('body');

        // Emulate the AJAX data sent normally so that we get the same theme.
        var data = {};
        data['ajax_page_state[theme]'] = Drupal.settings.ajaxPageState.theme;
        data['ajax_page_state[theme_token]'] = Drupal.settings.ajaxPageState.theme_token;

        dialog.dialog({
          title: $(this).attr('title') || Drupal.t('Available tokens'),
          width: 700,
          close: function(event, ui) {
            dialog.remove();
          }
        });
        // Load the token tree using AJAX.
        dialog.load(
          url,
          data,
          function (responseText, textStatus, XMLHttpRequest) {
            dialog.removeClass('loading');
          }
        );
        // Prevent browser from following the link.
        return false;
      });
    }
  };

  Drupal.behaviors.advancedTextFormatterToken = {
    attach: function (context, settings) {
      $('body', context).once('atf', function () {
        // Keep track of which ckeditor was last selected/focused.
        if (typeof CKEDITOR !== 'undefined') {
          var ckeditorVersion = CKEDITOR.version.split('.');
          var ckeditorMajorVersion = ckeditorVersion[0] | 0;
          var ckeditorMinorVersion = ckeditorVersion[1] | 0;

          if (ckeditorMajorVersion > 3 || (ckeditorMajorVersion == 3 && ckeditorMinorVersion > 4)) {
            CKEDITOR.on('instanceCreated', function(e) {
              var editor = new Drupal.advancedTextFormatterTokenCKEditor(e.editor.id, e.editor);
              Drupal.advancedTextFormatterTokenField.instances[e.editor.id] = editor;
              delete Drupal.settings.tokenFocusedField;
            });

            CKEDITOR.on('instanceDestroyed', function (e) {
              delete Drupal.advancedTextFormatterTokenField.instances[e.editor.id];
              delete Drupal.settings.tokenFocusedField;
            });

          }
        }

        if (typeof tinyMCE !== 'undefined') {
          if (tinyMCE.majorVersion == 3) {
            tinyMCE.onAddEditor.add(function(mgr, tinyMceEditor) {
              var editor = new Drupal.advancedTextFormatterTokenTinyMceEditor(tinyMceEditor.id, tinyMceEditor);
              Drupal.advancedTextFormatterTokenField.instances[tinyMceEditor.id] = editor;
              delete Drupal.settings.tokenFocusedField;
            });

            tinyMCE.onRemoveEditor.add(function(mgr, editor) {
              delete Drupal.advancedTextFormatterTokenField.instances[editor.id];
              delete Drupal.settings.tokenFocusedField;
            });
          }
        }

        $(this).delegate('.token-click-insert .token-key a', 'click', function() {
          if (typeof Drupal.settings.tokenFocusedField === 'undefined') {
            alert(Drupal.t('First click a text field to insert your tokens into.'));
            return false;
          }

          var focusedField = Drupal.settings.tokenFocusedField;
          var token = $(this).text();
          var deltaOffset = $('#toolbar, #admin-menu').outerHeight() + 30;

          focusedField.insertToken(token);
          $('html, body').animate({scrollTop: focusedField.getElement().offset().top - deltaOffset}, 500);

          return false;
        });
      });

      // Keep track of which textfield was last selected/focused.
      $('textarea, input[type="text"]', context).once('atf', function() {
        var base = $(this).attr('id');
        var field = new Drupal.advancedTextFormatterTokenFormField(base, this);
        Drupal.advancedTextFormatterTokenField.instances[base] = field;
      });

      $('.token-click-insert .token-key', context).once('token-click-insert', function() {
        var html = '<a href="javascript:void(0);" title="' + Drupal.t('Insert this token into your form') + '">' + $(this).html() + '</a>';
        $(this).html(html);
      });
    }
  };

  Drupal.advancedTextFormatterTokenField = {
    instances: {},
    focus: function () {
      Drupal.settings.tokenFocusedField = this;
    },
    toString: function() {
      return '[Editor "' + this.id + '"]';
    },
    getElement: function () {
      return $([]);
    },
    insertToken: function () {
    }
  };

  // CKEditor
  Drupal.advancedTextFormatterTokenCKEditor = function (id, editor) {
    var self = this;
    self.id = id;
    self.editor = editor;

    editor.on('focus', function () {
      self.focus();
    });
  };

  Drupal.advancedTextFormatterTokenCKEditor.prototype = $.extend({}, Drupal.advancedTextFormatterTokenField);

  Drupal.advancedTextFormatterTokenCKEditor.prototype.toString = function(token) {
    return '[CKEditor "' + this.id + '"]';
  };

  Drupal.advancedTextFormatterTokenCKEditor.prototype.getElement = function(token) {
    return jQuery('.' + this.id);
  };

  Drupal.advancedTextFormatterTokenCKEditor.prototype.insertToken = function(token) {
    this.editor.insertText(token);
  };

  // TinyMceEditor
  Drupal.advancedTextFormatterTokenTinyMceEditor = function (id, editor) {
    var self = this;
    self.id = id;
    self.editor = editor;

    self.editor.onClick.add(function(ed) {
      self.focus();
    });
  };

  Drupal.advancedTextFormatterTokenTinyMceEditor.prototype = $.extend({}, Drupal.advancedTextFormatterTokenField);

  Drupal.advancedTextFormatterTokenTinyMceEditor.prototype.toString = function(token) {
    return '[TinyMceEditor "' + this.id + '"]';
  };

  Drupal.advancedTextFormatterTokenTinyMceEditor.prototype.getElement = function(token) {
    return jQuery('#' + this.id + '_parent');
  };

  Drupal.advancedTextFormatterTokenTinyMceEditor.prototype.insertToken = function(token) {
    this.editor.execCommand('mceInsertContent', false, token);
  };

  // Normal form element
  Drupal.advancedTextFormatterTokenFormField = function (id, field) {
    var self = this;
    self.id = id;
    self.field = $(field).get(0);

    $(self.field).focus(function () {
      self.focus();
    });
  };

  Drupal.advancedTextFormatterTokenFormField.prototype = $.extend({}, Drupal.advancedTextFormatterTokenField);

  Drupal.advancedTextFormatterTokenFormField.prototype.toString = function(token) {
    var nodeName = this.field.nodeName.toLowerCase();
    nodeName = nodeName[0].toUpperCase() + nodeName.slice(1);
    return '[' + nodeName + ' "' + this.id + '"]';
  };

  Drupal.advancedTextFormatterTokenFormField.prototype.getElement = function(token) {
    return $(this.field);
  };

  Drupal.advancedTextFormatterTokenFormField.prototype.insertToken = function(token) {
    // IE support.
    if (document.selection) {
      this.field.focus();
      sel = document.selection.createRange();
      sel.text = token;
    }
    // MOZILLA/NETSCAPE support.
    else if (this.field.selectionStart || this.field.selectionStart == '0') {
      var startPos = this.field.selectionStart;
      var endPos = this.field.selectionEnd;
      this.field.value = this.field.value.substring(0, startPos)
                          + token
                          + this.field.value.substring(endPos, this.field.value.length);
    }
    else {
      this.field.value += token;
    }
  };
}) (jQuery);
