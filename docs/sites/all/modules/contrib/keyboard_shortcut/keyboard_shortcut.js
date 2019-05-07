var shortcut_debug = false;

shortcuts_parent_path = '';
shortcutsPress = new Array();
shortcutsDown = new Array();
shortcut_found = false;


jQuery(document).ready(function(){
	// Register event handlers
	jQuery(document).keydown(shortcut_keydown_handler);
	jQuery(document).keypress(shortcut_keypress_handler);
});

function shortcut_keydown_handler(e) {
	shortcut_found = false;
	code = e.keyCode?e.keyCode:e.charCode;
	code_char = String.fromCharCode(code);
	for(i=0; i<shortcutsDown.length; i++){
		if(shortcutsDown[i].char_code == code 
			&& shortcutsDown[i].alt == e.altKey
			&& shortcutsDown[i].ctrl == e.ctrlKey
			&& shortcutsDown[i].shift == e.shiftKey){
				e.stopPropagation();
				debug_message('Down: '+shortcutsDown[i].func_name+'("'+shortcutsDown[i].param+'")');
				if (shortcutsDown[i].param == "") {
                    eval(shortcutsDown[i].func_name);
				}
				else {
                    eval(shortcutsDown[i].func_name+'("'+shortcutsDown[i].param+'")');
				}
				shortcut_found = true;
        return false;
		}
	}
}

function shortcut_keypress_handler(e) {
	if(shortcut_found) return false;
	code = e.keyCode?e.keyCode:e.charCode;
	code_char = String.fromCharCode(code);
	for(i=0; i<shortcutsPress.length; i++){
		if(shortcutsPress[i].char_text == code_char 
			&& shortcutsPress[i].alt == e.altKey
			&& shortcutsPress[i].ctrl == e.ctrlKey
			&& shortcutsPress[i].shift == e.shiftKey){
				e.stopPropagation();
				debug_message('Press: '+shortcutsPress[i].func_name+'("'+shortcutsPress[i].param+'")');
				if (shortcutsPress[i].param == "") {
                    eval(shortcutsPress[i].func_name);
				}
				else {
                    eval(shortcutsPress[i].func_name+'("'+shortcutsPress[i].param+'")');
        }
        return false;
		}
	}
}

function shortcut_call_absolute_path(path){
 document.location = path;
}

var debug_iniciated = false;
var debug_window;
function init_debug(){
	if (!debug_iniciated) {
		debug_window = window.open('', 'debug_window');
		debug_window.document.write('<html><body><h1>Debug</h1>');
		debug_iniciated = true;
	}
}

function debug_message(msg){
	if (shortcut_debug) {
		init_debug();
		debug_window.document.write('<li>');
		debug_window.document.write(msg);
		debug_window.document.write('</li>');
	}
}
