/*
 * elFinder Integration
 *
 * Copyright (c) 2010-2020, Alexey Sukhotin. All rights reserved.
 */

(function($) {

  /**
   * @class  elFinder command "search"
   * Find files
   *
   * @author Dmitry (dio) Levashov
   **/
  elFinder.prototype.commands.search = function() {
    this.title = 'Find files';
    this.options = {ui: 'searchbutton'}
    this.alwaysEnabled = true;
    this.updateOnSelect = false;

    /**
     * Return command status.
     * Search does not support old api.
     *
     * @return Number
     **/
    this.getstate = function() {
      return 0;
    }

    /**
     * Send search request to backend.
     *
     * @param  String  search string
     * @return $.Deferred
     **/
    this.exec = function(q) {
      var fm = this.fm;

      if (typeof(q) == 'string' && q) {
        return fm.request({
          data: {cmd: 'search', elfinder_search_q: q},
          notify: {type: 'search', cnt: 1, hideCnt: true}
        });
      }
      fm.getUI('toolbar').find('.' + fm.res('class', 'searchbtn') + ' :text').focus();
      return $.Deferred().reject();
    }

  }

  elFinder.prototype.i18.en.messages['cmdchown'] = 'Take Ownership';

  elFinder.prototype.commands.chown = function() {
    this.alwaysEnabled = true;
    this.updateOnSelect = false;
    this.state = 0;

    this.getstate = function() {
      return 0;
    }

    this.exec = function(hashes, opts) {
      var fm = this.fm;
      var files = this.files(hashes),
          dfrd     = $.Deferred(),
          cnt      = files.length,
          makedir  = opts && opts.makedir ? 1 : 0,
          i, error,
          decision;
      var file = files[0];

      fm.confirm({
        title  : 'Take Ownership',
        text   : ['Are you sure to take ownership of "'+file.name+'"'],
        accept : {
          label : 'btnYes',
          callback : function (all) {
            fm.request({
              data: {
                cmd: 'owner',
                target: file.hash,
                owner: Drupal.settings.elfinder.uid
              },
              notify: {
                type: 'owner',
                cnt: 1
              }
            })
            .always(function() {
            });

          }
        },
        cancel : {
          label : 'btnNo',
          callback : function () {
            dfrd.resolve();
          }
        },
        all : ((i+1) < cnt)
      });

      return $.Deferred().reject();
    }


  }

  $().ready(function() {
    var uiopts = elFinder.prototype._options.uiOptions.toolbar;

    var newOpts = new Array();

    var disabledCommands = Drupal.settings.elfinder.disabledCommands;


    for (var i in uiopts) {
      var optsGroup = uiopts[i];
      var newOptsGroup = Array();


      for (var j in optsGroup) {
        var found = false;
        for (var k in disabledCommands) {
          if (disabledCommands[k] == optsGroup[j]) {
            found = true;
          }
        }

        if (found == false) {
          newOptsGroup.push(optsGroup[j]);
        }

      }

      if (i == 0) {
        newOptsGroup.push('up');
      }

      if (newOptsGroup.length >= 1) {
        newOpts.push(newOptsGroup);
      }
    }

    /*elFinder.prototype._options.contextmenu.files.push('|');
    elFinder.prototype._options.contextmenu.files.push('rename');  */

    var contextMenuCwd = elFinder.prototype._options.contextmenu.cwd;
    var contextMenuFiles = elFinder.prototype._options.contextmenu.files;
    var contextMenuNavbar = elFinder.prototype._options.contextmenu.navbar;

    var newContextMenuCwd = Array();
    var newContextMenuFiles = Array();
    var newContextMenuNavbar = Array();

    for (var i in contextMenuCwd) {
      var found = false;
      for (var k in disabledCommands) {
        if (disabledCommands[k] == contextMenuCwd[i]) {
          found = true;
        }
      }

      if (found == false && contextMenuCwd[i] != '|') {
        newContextMenuCwd.push(contextMenuCwd[i]);
      }
    }

    for (var i in contextMenuFiles) {
      var found = false;
      for (var k in disabledCommands) {
        if (disabledCommands[k] == contextMenuFiles[i]) {
          found = true;
        }
      }

      if (found == false && contextMenuFiles[i] != '|') {
        newContextMenuFiles.push(contextMenuFiles[i]);
      }
    }

    for (var i in contextMenuNavbar) {
      var found = false;
      for (var k in disabledCommands) {
        if (disabledCommands[k] == contextMenuNavbar[i]) {
          found = true;
        }
      }

      if (found == false && contextMenuNavbar[i] != '|') {
        newContextMenuNavbar.push(contextMenuNavbar[i]);
      }
    }
    elFinder.prototype._options.uiOptions.toolbar = newOpts;
    elFinder.prototype._options.contextmenu.cwd = newContextMenuCwd;
    elFinder.prototype._options.contextmenu.files = newContextMenuFiles;
    elFinder.prototype._options.contextmenu.navbar = newContextMenuNavbar;

    //elFinder.prototype._options.ui.push('mouseover');
  });


  $().ready(function() {
    if (Drupal.settings.elfinder) {
      var editorApp = Drupal.settings.elfinder.editorApp;

      var elfinderOpts = {
        url: Drupal.settings.elfinder.connectorUrl,
        lang: Drupal.settings.elfinder.langCode,
        rememberLastDir: Drupal.settings.elfinder.rememberLastDir,
        closeOnEditorCallback: false,
        customData: {token: Drupal.settings.elfinder.token},
        commandsOptions_: {}
      }

      // help tab rendering issues in admin theme
      if (Drupal.settings.elfinder.browserMode != 'default' && elFinder.prototype._options.commandsOptions.help) {
        final_options = [];

        for (var i in elFinder.prototype._options.commandsOptions.help.view) {
          var option = elFinder.prototype._options.commandsOptions.help.view[i];
          if (option != 'help') {
            final_options.push(option);
          }
        }

        elFinder.prototype._options.commandsOptions.help['view'] = final_options;
      }

      /* Pushing all settings to elFinder */
      $.extend(elfinderOpts, Drupal.settings.elfinder);

      if (editorApp && typeof window[Drupal.settings.elfinder.editorCallback] == 'function') {
        elfinderOpts.editorCallback = window[Drupal.settings.elfinder.editorCallback];
      }

      if (editorApp && typeof window[Drupal.settings.elfinder.editorCallback] == 'function') {
        elfinderOpts.getFileCallback = window[Drupal.settings.elfinder.editorCallback];
      }

      if (elfinderOpts.api21) {
        elfinderOpts['commandsOptions']['info'] = {
          custom: {}

        };
        
/*        elfinderOpts['commandsOptions']['quicklook'] = {
          officeOnlineMimes : ['application/msword', 'application/vnd.ms-word', 'application/vnd.ms-excel', 'application/vnd.ms-powerpoint', 'application/vnd.openxmlformats-officedocument.wordprocessingml.document', 'application/vnd.openxmlformats-officedocument.spreadsheetml.sheet'],
        }*/

        var disabledCommands = Drupal.settings.elfinder.disabledCommands,
          viewDesc = $.inArray('desc', disabledCommands) == -1 ? true : false,
          editDesc = $.inArray('editdesc', disabledCommands) == -1 ? true : false,
          viewOwner = $.inArray('owner', disabledCommands) == -1 ? true : false,
          changeOwner = $.inArray('chown', disabledCommands) == -1 ? true : false,
          viewDownloads = $.inArray('downloadcount', disabledCommands) == -1 ? true : false;

        if (changeOwner) {
          elFinder.prototype._options.commands.push('chown');
          elFinder.prototype._options.contextmenu.files.push('chown');
        }

        if (viewDesc || editDesc) {

          // Key is the same as your command name
          elfinderOpts['commandsOptions']['info']['custom']['desc'] = {
            // Field label
            label: Drupal.t('Description'),

            // HTML Template
            tpl: '<div class="elfinder-info-desc"><span class="elfinder-info-spinner"></span></div><div class="elfinder-info-save"></div>',

            // Action that sends the request to the server and get the description
            action: function(file, filemanager, dialog) {
              console.log('desc action');

              console.log('fm');
              console.log(file.mime);

              // Use the @filemanager object to issue a request
              filemanager.request({
                // Issuing the custom 'desc' command, targetting the selected file
                data: {cmd: 'desc', target: file.hash,},
                preventDefault: true,
              })
              // If the request fails, populate the field with 'Unknown'
                .fail(function() {
                  console.log('desc fail');
                  dialog.find('.elfinder-info-desc').html(filemanager.i18n('unknown'));
                })
                // When the request is successful, show the description
                .done(function(data) {
                  console.log('desc done');
                  dialog.find('.elfinder-info-desc').html(data.desc);

                  if (editDesc) {
                    //filemanager.lockfiles({files : [file.hash]})
                    dialog.find('.elfinder-info-desc').html('<textarea cols="20" rows="5" id="elfinder-fm-file-desc" class="ui-widget ui-widget-content">' + data.desc + '</textarea>');

                    $('.elfinder-info-save').append('<input type="button" id="elfinder-fm-file-desc-btn-save" class="ui-widget ui-button" value="' + filemanager.i18n('btnSave') + '" />');

                    var btnSave = $('#elfinder-fm-file-desc-btn-save', dialog).button();
                    console.log(btnSave);

                    btnSave.click(function() {
                      filemanager.lockfiles({files: [file.hash]});
                      filemanager.request({
                        data: {cmd: 'desc', target: file.hash, content: $('#elfinder-fm-file-desc').val()},
                        notify: {type: 'desc', cnt: 1}
                      })
                        .always(function() {
                          filemanager.unlockfiles({files: [file.hash]});
                        });
                    });

                  }


                });
            }
          };

        }

        if (viewDownloads) {
          elfinderOpts['commandsOptions']['info']['custom']['downloadcount'] = {
            // Field label
            label: Drupal.t('Downloads'),

            // HTML Template
            tpl: '<div class="elfinder-info-downloadcount"><span class="elfinder-info-spinner"></span></div>',

            // Action that sends the request to the server and get the description
            action: function(file, filemanager, dialog) {
              // Use the @filemanager object to issue a request
              filemanager.request({
                // Issuing the custom 'desc' command, targetting the selected file
                data: {cmd: 'downloadcount', target: file.hash,},
                preventDefault: true,
              })
              // If the request fails, populate the field with 'Unknown'
                .fail(function() {
                  dialog.find('.elfinder-info-downloadcount').html(0);
                })
                // When the request is successful, show the description
                .done(function(data) {
                  dialog.find('.elfinder-info-downloadcount').html(data.desc);
                });
            }
          };

        }

//        console.log(elfinderOpts);
      }

      var fm = $('#finder').elfinder(elfinderOpts);

      var instance = fm.elfinder('instance');

//    console.log(typeof instance == 'object' && typeof instance.toast == 'function');

      if (typeof instance == 'object' && typeof instance.toast == 'function') {
        instance.bind('load', function(event) {
          var messages = '#elfinder-messages .messages';
          $(messages).each(function(index, value) {
            var mode = 'info';
            if ($(value).hasClass('warning')) {
              mode = 'warning';
            } else if ($(value).hasClass('error')) {
              mode = 'error';
            }

            instance.toast({
              msg: $(value).html(),
              hideDuration: 500,
              showDuration: 300,
              timeOut: 1000,
              mode: mode
            });

          });
        });
      } else {
        $('#elfinder-messages').addClass('legacy');
      }
      //$('#finder').toast({msg: '123'});

      var h;

      if (elfinderOpts.browserMode != 'backend') {
        h = ($(window).height());
      } else {
        h = ($('#page').height());
      }

      $(window).resize(function() {
        if ($('#finder').height() != h) {
          $('#finder').height(h).resize();
        }
      });

      $(window).trigger('resize');
    }
  });

})(jQuery);
