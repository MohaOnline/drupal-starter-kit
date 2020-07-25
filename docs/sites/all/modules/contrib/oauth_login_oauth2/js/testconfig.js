function testConfig(testUrl) {
	var myWindow = window.open(testUrl, "TEST OAuth Client", "scrollbars=1 width=800, height=600");
}
function resetConfig()
{
	var myWindow = window.open("configure_oauth?&action=delete", "TEST OAuth Client", "scrollbars=1 width=800, height=600");
}

function show_backup_form() {
	jQuery('#backup_import_form').show();
	jQuery('#clientdata').hide();
	jQuery('#tabhead').hide();
	jQuery('#mo_advertise').hide();
}

function hide_backup_form() {
	jQuery('#backup_import_form').hide();
	jQuery('#clientdata').show();
	jQuery('#tabhead').show();
	jQuery('#mo_advertise').show();
}