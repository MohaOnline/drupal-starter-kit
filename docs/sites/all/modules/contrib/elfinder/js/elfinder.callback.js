// $Id: elfinder.callback.js 106 2011-02-26 08:19:56Z ph0enix $

(function($) {

/**
 * @class  elFinder command "search"
 * Find files
 *
 * @author Dmitry (dio) Levashov
 **/
elFinder.prototype.commands.search = function() {
	this.title          = 'Find files';
	this.options        = {ui : 'searchbutton'}
	this.alwaysEnabled  = true;
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
				data   : {cmd : 'search', elfinder_search_q : q},
				notify : {type : 'search', cnt : 1, hideCnt : true}
			});
		}
		fm.getUI('toolbar').find('.'+fm.res('class', 'searchbtn')+' :text').focus();
		return $.Deferred().reject();
	}

}

elFinder.prototype.commands.test134 = function() {
	this.title          = 'Test Command';
	//this.options        = {ui : 'uploadbutton'}
	this.alwaysEnabled  = true;
	this.updateOnSelect = false;
	this.state = 0;
	
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
    
    alert(Drupal.t('test command. arg=') + q);
		
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

});



$().ready(function() {
  if (Drupal.settings.elfinder) {
    var editorApp = Drupal.settings.elfinder.editorApp;

    var elfinderOpts = {
      url : Drupal.settings.elfinder.connectorUrl,
      lang : Drupal.settings.elfinder.langCode,
      rememberLastDir : Drupal.settings.elfinder.rememberLastDir,
      closeOnEditorCallback : false,
      customData : {token: Drupal.settings.elfinder.token}
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
        //alert('api21');
        console.log('2.1 api');
      elfinderOpts['commandsOptions'] = {
              info: {
        // Key is the same as your command name
      	desc : {
          // Field label
          label : 'Description1',
          
          // HTML Template
          tpl : '<div class="elfinder-info-desc"><span class="elfinder-info-spinner"></span></div>',
          
          // Action that sends the request to the server and get the description
          action : function(file, filemanager, dialog) {
            // Use the @filemanager object to issue a request
            filemanager.request({
              // Issuing the custom 'desc' command, targetting the selected file
              data : { cmd: 'desc', target: file.hash, },
              preventDefault: true,
            })
            // If the request fails, populate the field with 'Unknown'
            .fail(function() {
              dialog.find('.elfinder-info-desc').html(filemanager.i18n('unknown'));
            })
            // When the request is successful, show the description
            .done(function(data) {
              dialog.find('.elfinder-info-desc').html(data.desc);
            });
          },
        },
      }
      };

    }
      
    $('#finder').elfinder(elfinderOpts);

    if(elfinderOpts.browserMode != 'backend') {
      // If this is a popup, add an event so that elfinder fills the window.
      $(window).resize(function() {
        var h = ($(window).height());
        if($('#finder').height() != h) {
          $('#finder').height(h).resize();
        }
      });
    }
  }
});

})(jQuery);
